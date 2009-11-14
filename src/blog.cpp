
#define BOOST_SPIRIT_THREADSAFE

#include <fcgixx/file_loader.hpp>
#include <fcgixx/conversion.hpp>

#include <fcgixx/templater/modifier/html_esc.hpp>

#include "blog.hpp"
#include "paginator.hpp"
#include "modifier/simple_snip.hpp"

#include <fcgixx/mongo_v8.hpp>


namespace runpac {

using namespace std;
using namespace runpac::fcgixx;


blog::blog()
    : db(true) // auto reconnect
    , mod(db)
{

    dispatcher::bind("__500__",   &blog::on_error500); // special
    dispatcher::bind("error",     &blog::on_error404);
    dispatcher::bind("about",     &blog::on_about);
    dispatcher::bind("help",      &blog::on_help);
    dispatcher::bind("index",     &blog::on_index);
    dispatcher::bind("reset",     &blog::on_reset);
    dispatcher::bind("check",     &blog::on_check);
    dispatcher::bind("post",      &blog::on_post);
    dispatcher::bind("category",  &blog::on_category);
    dispatcher::bind("tag",       &blog::on_tag);
    dispatcher::bind("post_post", &blog::on_post_post);
    dispatcher::bind("preview",   &blog::on_com_preview);

    router::set_js_source(file_loader::load("js/router.js"));

    view.set_format("template/%1%.html");
    view.register_modifier("h", modifier::html_esc());
    view.register_modifier("m", simple_snip());
}

blog::~blog()
{

}


bool blog::on_error404()
{
    response.clear();
    if (!cache.has("error")) {
        view.assign("content", view.render("error.content"));
        view.assign("title", "404 - Page not found");
        make_sidebar();
        make_footer();
        cache.add("error", view.render("main"));
    }
    response << header("Content-type", "text/html; charset=utf-8");
    response << cache.get("error");
    return true;
}


bool blog::on_error500()
{
    response.clear();
    response << "error 500 (something went wrong..)";
    return true;
}


bool blog::on_index()
{
    const string& key = string("index") + request.get_param<string>("page", "1");
    if (!cache.has(key)) {
        //view.clear_context();
        if (!cache.has("index")) cache.add("index", "");

        int num_posts = mod.count_posts();
        if (!num_posts) {
            return on_error404();
        }
        int page = request.get_param<int>("page", 1);
        paginator pagi(page, num_posts, items_per_page);
        if (pagi.error) {
            return on_error404();
        }
        view.assign("prevPageNum", pagi.prev_page_num);
        view.assign("nextPageNum", pagi.next_page_num);

        view.assign("posts", mod.get_posts(pagi.offset, items_per_page));
        view.assign("content", view.render("index.content"));

        make_sidebar();
        make_footer();

        view.assign("title", "blog.runpac.com");
        cache.add("index", key, view.render("main"));
    }

    response << header("Content-type", "text/html; charset=utf-8");
    response << cache.get(key);
    return true;
}



bool blog::on_post()
{
    const string& id = request.get_param<string>("id");
    const string& key = string("post") + id;
    if (!cache.has(key)) {
        if (!cache.has("posts")) cache.add("posts", "");

        const model::entity& post = mod.get_post(id);
        if (mod.valid(post)) {
            return on_error404();
        }
        view.assign("post", post);
        view.assign("content", view.render("post.content"));

        make_sidebar();
        make_footer();

        view.assign("title", mod.get_field(post, "title"));
        cache.add("posts", key, view.render("main"));
    }
    response << header("Content-type", "text/html; charset=utf-8");
    response << cache.get(key);
    return true;
}


bool blog::on_post_post()
{
    const string& id = request.get_param<string>("id");
    const string& key = string("post") + id;

    if (!mod.post_exists(id)) {
        return on_error404();
    }

    if (mod.add_new_comment(id, request.get_post_params())) {
        cache.del("index");
        cache.del("footer");
        cache.del(key); // rajouter un delete sur la category et les tags du post!!!
        return on_post();
    }
    response << "Invalid form";
    return true;
}

bool blog::on_category()
{
    const string& id = request.get_param<string>("id");
    const string& parent_key = string("cat") + id;
    const string& key = parent_key + "-" + request.get_param<string>("page", "1"); // cat<id>-<page>
    if (!cache.has(key)) {

        int num_posts = mod.count_posts_for_cat(id);
        if (!num_posts) {
            return on_error404();
        }
        int page = request.get_param<int>("page", 1);
        paginator pagi(page, num_posts, items_per_page);
        if (pagi.error) {
            return on_error404();
        }
        view.assign("prevPageNum", pagi.prev_page_num);
        view.assign("nextPageNum", pagi.next_page_num);

        const string& cat_name = mod.get_cat_name(id);
        view.assign("catName", cat_name);
        view.assign("pageUri", string("/category/") + id);
        view.assign("posts", mod.get_posts_for_cat(id, pagi.offset, items_per_page));

        view.assign("content", view.render("category.content"));

        make_sidebar();
        make_footer();

        view.assign("title", string("Category ") + cat_name);

        if (!cache.has("cats")) cache.add("cats", "");
        if (!cache.has(parent_key)) cache.add("cats", parent_key, ""); // checker pertinence du test. voir pour auto add ?
        cache.add(parent_key, key, view.render("main"));
    }

    response << header("Content-type", "text/html; charset=utf-8");
    response << cache.get(key);
    return true;
}


bool blog::on_tag()
{
    const string& id = request.get_param<string>("id");
    const string& parent_key = string("tag") + id;
    const string& key = parent_key + "-" + request.get_param<string>("page", "1"); // tag<id>-<page>
    if (!cache.has(key)) {

        int num_posts = mod.count_posts_for_tag(id);
        if (!num_posts) {
            return on_error404();
        }
        int page = request.get_param<int>("page", 1);
        paginator pagi(page, num_posts, items_per_page);
        if (pagi.error) {
            return on_error404();
        }
        view.assign("prevPageNum", pagi.prev_page_num);
        view.assign("nextPageNum", pagi.next_page_num);

        const string& tag_name = mod.get_tag_name(id);
        view.assign("tagName", tag_name);
        view.assign("pageUri", string("/tag/") + id);
        view.assign("posts", mod.get_posts_for_tag(id, pagi.offset, items_per_page));

        view.assign("content", view.render("tag.content"));

        make_sidebar();
        make_footer();

        view.assign("title", string("Tag ") + tag_name);

        if (!cache.has("tags")) cache.add("tags", "");
        if (!cache.has(parent_key)) cache.add("tags", parent_key, ""); // checker pertinence du test. voir pour auto add ?
        cache.add(parent_key, key, view.render("main"));
    }

    response << header("Content-type", "text/html; charset=utf-8");
    response << cache.get(key);
    return true;
}


bool blog::on_about()
{
    if (!cache.has("about")) {
        view.assign("content", view.render("about.content"));
        make_sidebar();
        make_footer();
        view.assign("title", "About");
        cache.add("about", view.render("main"));
    }
    response << header("Content-type", "text/html; charset=utf-8");
    response << cache.get("about");
    return true;
}


bool blog::on_help()
{
    if (!cache.has("help")) {
        view.assign("content", view.render("help.content"));
        make_sidebar();
        make_footer();
        view.assign("title", "Help");
        cache.add("help", view.render("main"));
    }
    response << header("Content-type", "text/html; charset=utf-8");
    response << cache.get("help");
    return true;
}

bool blog::on_com_preview()
{
    view.assign("user", mod.format_comment_user(request.get_post_param("email")));
    view.assign("site", mod.format_comment_site(request.get_post_param("site")));
    view.assign("content", mod.format_comment_content(request.get_post_param("com_con")));

    response << header("Content-type", "text/html; charset=utf-8");
    response << view.render("preview");
    return true;
}

bool blog::on_reset()
{
    response << cache.check();
    response << cache.check_list();
    response << "now clearing...<br/>\n";
    view.clear();
    cache.clear();
    return on_index();
}

bool blog::on_check()
{
    response << cache.check();
    response << cache.check_list();
    return true;
}



void blog::make_sidebar()
{
    if (!cache.has("sidebar")) {
        view.assign("cats", mod.get_cats());
        view.assign("tags", mod.get_tags());
        cache.add("sidebar", view.render("sidebar"));
    }
    view.assign("sidebar", cache.get("sidebar"));
}


void blog::make_footer()
{
    if (!cache.has("footer")) {

        view.assign("recentPosts", mod.get_recent_posts());
        view.assign("recentComs", mod.get_recent_comments());

        cache.add("footer", view.render("footer"));

    }
    view.assign("footer", cache.get("footer"));
}



} // ns
