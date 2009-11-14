
#define BOOST_SPIRIT_THREADSAFE

#include <fstream>

#include <fcgixx/web_app.hpp>

#include "blog.hpp"

int main (int, char**)
{
    std::ofstream cout_file("blogxx.log", std::ios_base::out | std::ios_base::app);

    std::cout.rdbuf(cout_file.rdbuf());

    try {
        runpac::fcgixx::web_app<runpac::blog> app;
        app.run();
    }
    catch(const std::exception& e)  {
        std::cout << "exception: " << e.what() << std::endl;
        return 1;
    }
    catch(...) {
        std::cout << "unknown exception" << std::endl;
        return 2;
    }

    return 0;
}
