#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/tr1/regex.hpp>
#include <mongo/client/dbclient.h>

//#include <boost/xpressive/xpressive_static.hpp>
//#include <boost/xpressive/xpressive_dynamic.hpp>
//#include <regex>

namespace runpac {


struct model
{

    typedef mongo::BSONElement entity;

    model(mongo::DBClientConnection& db)
        : db(db)
        , db_ns  ("blogx")
        , post_ns("blogx.post")
        , cat_ns ("blogx.cat")
        , tag_ns ("blogx.tag")
        , system_js_ns("blogx.system.js")
    {
        db.connect("localhost");

        db.ensureIndex("blogx.post", BSON("date" << 1));
        db.ensureIndex("blogx.post", BSON("_cats" << 1));
        db.ensureIndex("blogx.post", BSON("_tags" << 1));
        db.ensureIndex("blogx.post", BSON("coms.date" << 1));

        register_mongo_func("fillPostMeta");
        register_mongo_func("getPosts");
        register_mongo_func("getPost");
        register_mongo_func("getPostsForCat");
        register_mongo_func("getPostsForTag");
        register_mongo_func("getTagCloud");
        register_mongo_func("getCatCloud");
        register_mongo_func("getRecentPosts");
        register_mongo_func("getRecentComments");
    }


    int count_posts()
    {
        return db.count(post_ns);
    }

    int count_posts_for_cat(const std::string& id)
    {
        return db.count(post_ns, BSON("_cats" << id));
    }

    int count_posts_for_tag(const std::string& id)
    {
        return db.count(post_ns, BSON("_tags" << id));
    }

    bool post_exists(const std::string& id)
    {
        return db.count(post_ns, BSON("_id" << id)) == 1;
    }

    entity get_posts(int skip = 0, int limit = 10)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << skip << "1" << limit);
        db.eval(db_ns, "function (skip, limit) { return getPosts(skip, limit); }", info, ret, &args); //bool ok
        return ret;
    }

    entity get_recent_posts(int limit = 10)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << limit);
        db.eval(db_ns, "function (limit) { return getRecentPosts(limit); }", info, ret, &args); //bool ok
        return ret;
    }

    entity get_recent_comments(int limit = 10)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << limit);
        db.eval(db_ns, "function (limit) { return getRecentComments(limit); }", info, ret, &args); //bool ok
        return ret;
    }

    entity get_post(const std::string& id)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << id);
        db.eval(db_ns, "function (id) { return getPost(id); }", info, ret, &args);
        return ret;
    }

    entity get_posts_for_cat(const std::string& id, int skip = 0, int limit = 10)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << id << "1" << skip << "2" << limit);
        db.eval(db_ns, "function (id, skip, limit) { return getPostsForCat(id, skip, limit); }", info, ret, &args);
        return ret;
    }

    entity get_posts_for_tag(const std::string& id, int skip = 0, int limit = 10)
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args = BSON("0" << id << "1" << skip << "2" << limit);
        db.eval(db_ns, "function (id, skip, limit) { return getPostsForTag(id, skip, limit); }", info, ret, &args);
        return ret;
    }

    entity get_tags()
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args;
        db.eval(db_ns, "function () { return getTagCloud(); }", info, ret, &args);
        return ret;
    }

    entity get_cats()
    {
        static mongo::BSONObj info;
        mongo::BSONElement ret;
        mongo::BSONObj args;
        db.eval(db_ns, "function () { return getCatCloud(); }", info, ret, &args);
        return ret;
    }

    std::string get_cat_name(const std::string& id)
    {
        const mongo::BSONObj& obj = db.findOne(cat_ns, BSON("_id" << id));
        return obj.getField("name").valuestr();
    }

    std::string get_tag_name(const std::string& id)
    {
        const mongo::BSONObj& obj = db.findOne(tag_ns, BSON("_id" << id));
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

        db.update(post_ns, BSON("_id" << id),
            BSON("$push" << BSON( "coms" <<
                BSON("user" << user <<
                     "email"<< email <<
                     "site" << site <<
                     "date" << mongo::DATENOW <<
                     "content" << content
                )
            )
        ));

        db.insert("blogx.recCom", BSON("_id" << id));

        return true;
    }



private:

    void register_mongo_func(const char* name)
    {
        const std::string& code = fcgixx::file_loader::load(std::string("js/mongo/") + name + ".js");
        mongo::BSONObj query = BSON("_id" << name);
        mongo::BSONObjBuilder o;
        o.append("_id", name);
        o.appendCode("value", code.c_str());
        db.update(system_js_ns, query, o.obj(), 1);
    }


    mongo::DBClientConnection& db;

    const std::string db_ns;
    const std::string post_ns;
    const std::string cat_ns;
    const std::string tag_ns;
    const std::string system_js_ns;


};



} // ns
