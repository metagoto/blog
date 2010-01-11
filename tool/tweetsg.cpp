
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <exception>
#include <iostream>
//#include <fcgixx/tpl/modifier/html_esc.hpp>

int gen(const std::string& read_file, const std::string& gen_file)
{

    using boost::property_tree::ptree;
    //using runpac::fcgixx::tpl::modifier::html_esc;

    ptree pt;
    read_xml(read_file, pt);

    std::ofstream out(gen_file.c_str());
    if (!out.is_open()) {
        return 1;
    }
    out << "<h3>Tweets</h3>\n";
    out << "<ul class=\"tweets\">\n";

    int err = 1;
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("rss.channel"))
    {
        const std::string& guid = v.second.get<std::string>("guid", "");
        if (!guid.empty()) {
            const std::string& title = v.second.get<std::string>("title", "");
            if (!title.empty()) {
                const std::string& date = v.second.get<std::string>("pubDate", "");
                out << "<li>";
                out << title ;
                //out << html_esc()(title.begin(), title.end()) << "\n""; // not const
                out << " - <a href=\"" << guid << "\">" << date.substr(5, 11) << "</a>";
                out << "</li>\n";
                err = 0;
            }
        }
    }
    out << "<li><a href=\"http://twitter.com/metagoto\">More tweets..</a></li>";
    out << "</ul>\n";
    out.close();
    return err;
}

// file_to_read file_to_gen
int main(int argc, char* argv[])
{
    if (argc != 3) return 1;
    try
    {
        return gen(argv[1], argv[2]);
    }
    catch (std::exception& e)
    {
        //std::cout << "error: " << e.what() << "\n";
    }
    return 1;
}

