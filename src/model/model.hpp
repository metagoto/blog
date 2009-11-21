#pragma once

#include <functional>
#include <boost/algorithm/string.hpp>
#include <boost/tr1/regex.hpp>
#include <mongo/client/dbclient.h>
#include <fcgixx/util/file_loader.hpp>

//#include <boost/xpressive/xpressive_static.hpp>
//#include <boost/xpressive/xpressive_dynamic.hpp>
//#include <regex>


namespace runpac {


static const char* ns_db   = "blogx";
static const char* ns_post = "blogx.post";
static const char* ns_cat  = "blogx.cat";
static const char* ns_tag  = "blogx.tag";
static const char* ns_user = "blogx.user";
static const char* ns_rcom = "blogx.recCom";
static const char* ns_sysjs= "blogx.system.js";


struct model
{

    typedef mongo::BSONElement entity;

    model()
        : db(true)
    {
        db.connect("localhost");

        db.ensureIndex(ns_post, BSON("date" << 1));
        db.ensureIndex(ns_post, BSON("_cats" << 1));
        db.ensureIndex(ns_post, BSON("_tags" << 1));
        db.ensureIndex(ns_post, BSON("coms.date" << 1));

        register_mongo_func("fillPostMeta");
        register_mongo_func("getPosts");
        register_mongo_func("getPost");
        register_mongo_func("getPostsForCat");
        register_mongo_func("getPostsForTag");
        register_mongo_func("getTagCloud");
        register_mongo_func("getCatCloud");
        register_mongo_func("getRecentPosts");
        register_mongo_func("getRecentComments");
        register_mongo_func("getPostRaw");
    }


    int count_posts()
    {
        return db.count(ns_post);
    }

    int count_posts_for_cat(const std::string& id)
    {
        return db.count(ns_post, BSON("_cats" << id));
    }

    int count_posts_for_tag(const std::string& id)
    {
        return db.count(ns_post, BSON("_tags" << id));
    }

    bool post_exists(const std::string& id)
    {
        return db.count(ns_post, BSON("_id" << id)) == 1;
    }

    entity get_posts(int skip = 0, int limit = 10)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << skip << "1" << limit);
        db.eval(ns_db, "function (skip, limit) { return getPosts(skip, limit); }", info, ret, &args); //bool ok
        return ret;
    }

    entity get_recent_posts(int limit = 10)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << limit);
        db.eval(ns_db, "function (limit) { return getRecentPosts(limit); }", info, ret, &args); //bool ok
        return ret;
    }

    entity get_recent_comments(int limit = 10)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << limit);
        db.eval(ns_db, "function (limit) { return getRecentComments(limit); }", info, ret, &args); //bool ok
        return ret;
    }

    entity get_post(const std::string& id)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << id);
        db.eval(ns_db, "function (id) { return getPost(id); }", info, ret, &args);
        return ret;
    }

    entity get_post_raw(const std::string& id)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << id);
        db.eval(ns_db, "function (id) { return getPostRaw(id); }", info, ret, &args);
        return ret;
    }

    entity get_posts_for_cat(const std::string& id, int skip = 0, int limit = 10)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << id << "1" << skip << "2" << limit);
        db.eval(ns_db, "function (id, skip, limit) { return getPostsForCat(id, skip, limit); }", info, ret, &args);
        return ret;
    }

    entity get_posts_for_tag(const std::string& id, int skip = 0, int limit = 10)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << id << "1" << skip << "2" << limit);
        db.eval(ns_db, "function (id, skip, limit) { return getPostsForTag(id, skip, limit); }", info, ret, &args);
        return ret;
    }

    entity get_tags()
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args;
        db.eval(ns_db, "function () { return getTagCloud(); }", info, ret, &args);
        return ret;
    }

    entity get_cats()
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args;
        db.eval(ns_db, "function () { return getCatCloud(); }", info, ret, &args);
        return ret;
    }

    std::string get_cat_name(const std::string& id)
    {
        const mongo::BSONObj& obj = db.findOne(ns_cat, BSON("_id" << id));
        return obj.getField("name").valuestr();
    }

    std::string get_tag_name(const std::string& id)
    {
        const mongo::BSONObj& obj = db.findOne(ns_tag, BSON("_id" << id));
        return obj.getField("name").valuestr();
    }


    entity get_field(const mongo::BSONObj& obj, const std::string& field)
    {
        return obj.getField(field);
    }

    entity get_field(const entity& elem, const std::string& field)
    {
        return elem.embeddedObject().getField(field);
    }

    bool valid(const entity& e)
    {
        return e.isNull();
    }


    std::string format_comment_content(std::string str, bool* valid = 0)
    {
        boost::trim(str);
        if (!str.size() || str.size() > 16000) {
            if (valid) *valid = false;
            return std::string();
        }
        if (valid) *valid = true;
        return str + '\n';
    }
    std::string format_comment_user(std::string str, bool* valid = 0)
    {
        boost::trim(str);
        if (!str.size() || str.size() > 40) {
            if (valid) *valid = false;
            return std::string();
        }
        if (valid) *valid = true;
        return str;
    }
    std::string format_comment_email(std::string str, bool* valid = 0)
    {
        static const std::tr1::regex reg("[a-zA-Z0-9._%+-]+@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,4}");
        boost::trim(str);
        if (!str.size() || str.size() > 80 || !regex_match(str, reg)) {
            if (valid) *valid = false;
            return std::string();
        }
        if (valid) *valid = true;
        return str;
    }
    std::string format_comment_site(std::string str, bool* valid = 0)
    {
        static const std::tr1::regex reg("^https?://.+");
        boost::trim(str);
        if (str.size() < 4 || str.size() > 100) {
            if (valid) *valid = false;
            return std::string();
        }
        if (!regex_match(str, reg)) {
            str = "http://" + str;
        }
        if (valid) *valid = true;
        return str;
    }

    template<typename T>
    bool add_new_comment(const std::string& id, const T& params)
    {
        using std::string;

        bool valid = false;
        const string& content = format_comment_content(params.at("com_con"), &valid);
        if (!valid) return false;

        const string& user = format_comment_user(params.at("email"), &valid);
        if (!valid) return false;

        const string& email = format_comment_email(params.at("user"), &valid);
        if (!valid) return false;

        const string& site = format_comment_site(params.at("site"));

        db.update(ns_post, BSON("_id" << id),
            BSON("$push" << BSON( "coms" <<
                BSON("user" << user <<
                     "email"<< email <<
                     "site" << site <<
                     "date" << mongo::DATENOW <<
                     "content" << content
                )
            )
        ));

        db.insert(ns_rcom, BSON("_id" << id));

        return true;
    }

    // tmp. need to move to conv ns
    std::vector<std::string> to_array(const std::string& tokens)
    {
        typedef std::vector<std::string> vec_t;
        vec_t v;
        boost::algorithm::split(v, tokens, boost::algorithm::is_any_of(", ")
                               ,boost::algorithm::token_compress_on);
        return v;
    }

    // tmp
    template<typename T>
    bool update_post(const std::string& id, const T& params)
    {
        using std::string;

        const string& title = params.at("title");
        const string& subtitle = params.at("subtitle");
        const string& cats = params.at("cats");
        const string& tags = params.at("tags");
        const string& content = params.at("cont");

        mongo::BSONObjBuilder o;
        o << "title" << title
          << "subtitle" << subtitle
          << "content" << content;
        o.append("_cats", to_array(cats));
        o.append("_tags", to_array(tags));

        db.update(ns_post, BSON("_id" << id),
            BSON( "$set" << o.done() )
        );
        return true;
    }

    // tmp
    template<typename T>
    bool new_post(const T& params)
    {
        using std::string;

        const string& id = params.at("id");
        const string& title = params.at("title");
        const string& subtitle = params.at("subtitle");
        const string& cats = params.at("cats");
        const string& tags = params.at("tags");
        const string& content = params.at("cont");

        mongo::BSONObjBuilder o;
        o << "_id" << id
          << "title" << title
          << "subtitle" << subtitle
          << "content" << content
          << "date" << mongo::DATENOW;
        o.append("_cats", to_array(cats));
        o.append("_tags", to_array(tags));
        mongo::BSONArray a;
        o.appendArray("coms", a);

        db.insert(ns_post, o.done() );
        return true;
    }


    bool can_login(const std::string& user, const std::string& pass)
    {
        if (!user.size() || user.size() > 40) return false;
        if (!pass.size() || pass.size() > 40) return false;
        return db.count(ns_user, BSON("_id" << user << "pass" << pass)) == 1;
    }



private:

    void register_mongo_func(const char* name)
    {
        const std::string& code =
            fcgixx::util::file_loader::load(std::string("js/mongo/") + name + ".js");
        mongo::BSONObj query = BSON("_id" << name);
        mongo::BSONObjBuilder o;
        o.append("_id", name);
        o.appendCode("value", code.c_str());
        db.update(ns_sysjs, query, o.obj(), 1);
    }


    mongo::DBClientConnection db;

};



} // ns
