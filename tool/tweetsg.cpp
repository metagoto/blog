
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <exception>
#include <iostream>
//#include <fcgixx/tpl/modifier/html_esc.hpp>

#include "tweet_twitter_writer.hpp"
#include "tweet_parser.hpp"

int gen(const std::string& read_file, const std::string& gen_file)
{

    using boost::property_tree::ptree;
    using boost::spirit::qi::parse;
    //using runpac::fcgixx::tpl::modifier::html_esc;

    typedef std::string::const_iterator iterator_type;

    ptree pt;
    read_xml(read_file, pt);

    std::ofstream out(gen_file.c_str());
    if (!out.is_open()) {
        return 1;
    }

    tweet_twitter_writer writer;
    parser::tweet_parser<iterator_type, tweet_twitter_writer> g(writer);
    std::vector<std::string> vec_str;

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

                const std::string& data = title.substr(10);
                iterator_type it = data.begin();
                iterator_type end = data.end();

                writer.clear();
                vec_str.clear();
                bool r = parse(it, end, g, vec_str);

                if (r && it == end) {
                    out << "<li>";
                    out << writer.str();
                    //out << html_esc()(title.begin(), title.end()) << "\n""; // not const
                    out << " - <a href=\"" << guid << "\">" << date.substr(5, 11) << "</a>";
                    out << "</li>\n";
                    err = 0;
                }
            }
        }
    }
    out << "<li><a href=\"http://twitter.com/metagoto\">More tweets..</a></li>";
    out << "</ul>\n";
    out.close();
    return err;
}


// file_to_read file_to_gen
//g++ -O3 -o tweetsg -I/Users/runpac/dev/boost_44/include tweetsg.cpp
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

