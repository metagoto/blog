#pragma once

#include <string>
#include <mongo/client/dbclient.h>

namespace runpac {


template<typename T>
struct admin
{
    typedef T owner_type;
    typedef typename T::request_type request_type;
    typedef typename T::model_type model_type;


    admin(T* const owner)
        : owner(*owner)
    {
    }

    bool on_index()
    {
        if (!logged()) return owner.on_error404();
        view().assign("posts", mod().get_posts(0, mod().count_posts()));
        view().assign("content", view().render("admin/index.content"));
        owner.make_sidebar();
        return send(view().render("main"));
    }

    bool on_edit()
    {
        if (!logged()) return owner.on_error404();
        const std::string& id = request().template get_param<std::string>("id");
        const typename model_type::entity& post = mod().get_post_raw(id);
        if (!mod().valid(post)) {
            return owner.on_error404();
        }
        view().assign("post", post);
        view().assign("content", view().render("admin/edit.content"));
        owner.make_sidebar();
        return send(view().render("main"));
    }

    bool on_edit_post()
    {
        if (!logged()) return owner.on_error404();
        const std::string& id = request().template get_param<std::string>("id");
        const typename model_type::entity& post = mod().get_post_raw(id);
        if (!mod().valid(post)) {
            return owner.on_error404();
        }
        if (mod().update_post(id, request().get_post_params())) {
            cache().clear();
            return on_edit();
        }
        return send("Invalid form");
    }

    bool on_new()
    {
        if (!logged()) return owner.on_error404();
        owner.make_sidebar();
        view().assign("content", view().render("admin/new.content"));
        return send(view().render("main"));
    }

    bool on_new_post()
    {
        if (!logged()) return owner.on_error404();
        if (mod().new_post(request().get_post_params())) {
            cache().clear();
            return on_index();
        }
        return send("Invalid form");
    }

    bool on_preview()
    {
        if (!logged()) return owner.on_error404();
        mongo::BSONObjBuilder b;
        b << "title" << request().get_post_param("title")
          << "subtitle" << request().get_post_param("subtitle")
          << "content" << request().get_post_param("cont")
          << "date" << mongo::DATENOW
          << "numComs" << 0
          << "cats" << BSON_ARRAY( BSON("name" << "Vrac") )
          << "tags" << BSON_ARRAY( BSON("name" << "blog") )
          ;
        view().assign("post", b.obj());
        return send(view().render("admin/preview"));
    }

    bool on_genfeed()
    {
        if (!logged()) return owner.on_error404();
        cache().del("feed");
        return owner.on_feed();
    }

    bool on_genfeed_comments()
    {
        if (!logged()) return owner.on_error404();
        cache().del("feed_comments");
        return owner.on_feed_comments();
    }


    bool logged()
    {
        return owner.sess.template get<bool>("logged");
    }

    bool send(const std::string& s)
    {
        return owner.send(s);
    }

    typename owner_type::request_type& request()
    {
        return owner.request;
    }

    typename owner_type::view_type& view()
    {
        return owner.view;
    }

    typename owner_type::model_type& mod()
    {
        return owner.mod;
    }

    typename owner_type::cache_type& cache()
    {
        return owner.cache;
    }



private:

    T& owner;


};


} // ns
