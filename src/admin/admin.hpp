#pragma once


namespace runpac {

//struct blog;

template<typename T>
struct admin
{



    admin(T* owner)
        : owner(*owner)
    {

    }

    bool on_index()
    {
        using namespace fcgixx::http;

        if (!logged()) return owner.on_error404();

        owner.view.assign("posts", owner.mod.get_posts(0, owner.mod.count_posts()));

        //owner.response << header("Content-type", "text/html; charset=utf-8");
        //owner.response << owner.view.render("admin/index.content");
        //return true;
        return owner.send(owner.view.render("admin/index.content"));
    }

    bool on_edit()
    {
        return true;
    }


    bool logged()
    {
        return owner.sess.template get<bool>("logged");
    }


private:

    T& owner;


};






} // ns
