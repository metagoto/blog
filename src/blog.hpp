#pragma once
#include <algorithm>

#include <fcgixx/policy/js_router.hpp>
#include <fcgixx/policy/simple_dispatcher.hpp>

#include <fcgixx/application.hpp>
#include <fcgixx/cache.hpp>
#include <fcgixx/templater.hpp>
#include <fcgixx/templater/filesys_loader.hpp>
#include <fcgixx/templater/v8_engine.hpp>
#include <fcgixx/templater/simple_cache.hpp>

#include <boost/thread.hpp>
#include <mongo/client/dbclient.h>

#include "model/model.hpp"


namespace runpac {


typedef fcgixx::v8_engine<fcgixx::compiler_t> engine_t;
typedef fcgixx::simple_cache<engine_t::compiled_type, engine_t::release> cache_t;
typedef fcgixx::templater<fcgixx::filesys_loader, engine_t, cache_t> js_template;

struct blog
    : public fcgixx::application<
        blog
      , fcgixx::policy::js_router
      , fcgixx::policy::simple_dispatcher
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


private:
    // utility
    void make_sidebar();
    void make_footer();

    // helpers
    bool on_reset();
    bool on_check();

    mongo::DBClientConnection db;

    runpac::model mod;

    //tmp
    static const int items_per_page = 4;

    typedef fcgixx::cache<std::string, std::string> cache_type;
    cache_type cache;

    js_template view;

};


} // ns


