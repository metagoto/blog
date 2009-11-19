#pragma once

#include <fcgixx/app.hpp>
#include <fcgixx/request/http_simple.hpp>
#include <fcgixx/response/http_simple.hpp>
#include <fcgixx/router/js.hpp>
#include <fcgixx/dispatcher/simple.hpp>
#include <fcgixx/tpl/v8_tpl.hpp>
#include <fcgixx/cache/hierarchic.hpp>
#include <fcgixx/session/mem_cook.hpp>

#include "model/model.hpp"

#include "admin/admin.hpp"


namespace runpac {


struct blog
    : public fcgixx::app<
        blog
      , fcgixx::request::http_simple
      , fcgixx::response::http_simple
      , fcgixx::router::js
      , fcgixx::dispatcher::simple
    >
{

    blog();
    ~blog();

    bool on_error404();
    bool on_error500();
    bool on_index();
    bool on_about();
    bool on_help();

    bool on_post();
    bool on_category();
    bool on_tag();

    bool on_post_post();

    bool on_com_preview();

    bool on_login();
    bool on_try_login();

    bool on_blank();


private:
    void make_sidebar();
    void make_footer();

    bool send(const std::string&);

    // tmp
    bool on_reset();
    bool on_check();

    runpac::model mod;

    //tmp
    static const int items_per_page = 4;

    fcgixx::cache::hierarchic<> cache;

    fcgixx::tpl::v8_tpl view;

    fcgixx::session::mem_cook<request_type, response_type>::type sess;

    typedef admin<blog> admin_type;
    friend class admin<blog>;
    admin_type adm;
};


} // ns


