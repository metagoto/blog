#pragma once

#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>


template< template<typename,typename> class parser_type
        , typename writer_type
        >
struct generator
{

    writer_type& writer;

    generator(writer_type& writer)
        : writer(writer)
    {
    }

    template<typename T>
    T gen(const T& read_file, bool guid_link = true)
    {
        typedef typename T::const_iterator iterator_type;

        using boost::property_tree::ptree;
        using boost::spirit::qi::parse;

        ptree pt;
        read_xml(read_file, pt);

        parser_type<iterator_type, writer_type> parser(writer);

        std::ostringstream out;

        BOOST_FOREACH(ptree::value_type &v, pt.get_child("rss.channel"))
        {
            const T& guid = v.second.get<T>("guid", "");
            if (!guid.empty()) {
                const T& title = v.second.get<T>("title", "");
                if (!title.empty()) {
                    const T& date = v.second.get<T>("pubDate", "");

                    const T& data = title.substr(title.find_first_of(":")+2);
                    iterator_type it = data.begin();
                    iterator_type end = data.end();

                    writer.clear();
                    if (parse(it, end, parser) && it == end) {
                        out << "<li>";
                        out << writer.str();
                        if (guid_link) {
                            out << " - <a class=\"guid\" href=\"" << guid << "\">" << date.substr(5, 11) << "</a>";
                        }
                        else {
                            out << " - <span class=\"guid\">" << date.substr(5, 11) << "</span>";
                        }
                        out << "</li>\n";
                    }
                }
            }
        }

        return out.str();
    }

};
