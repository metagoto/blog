#pragma once

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <string>
#include <vector>


namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template<typename Iterator, typename Writer>
struct tweet_parser : qi::grammar<Iterator, std::vector<std::string>()>
{


    tweet_parser(Writer& w)
        : tweet_parser::base_type(start)
        , writer(w)
    {

        using qi::lit;
        using qi::raw;
        using qi::byte_;
        using ascii::char_;
        using ascii::alnum;
        using boost::bind;


        plain =
                raw[
                    +(byte_ - ((lit("http://") | '@' | '#') >> alnum))
                ][bind(&Writer::plain, &writer, _1)];

        at =
                lit("@")
                >> raw[
                       +alnum
                   ][bind(&Writer::at, &writer, _1)];

        hash =
                lit("#")
                >> raw[
                       +alnum
                   ][bind(&Writer::hash, &writer, _1)];

        url =
                lit("http://")
                >> raw[
                        +(alnum | '/' | '#' | '?' | '&') % '.'
                   ][bind(&Writer::url, &writer, _1)];

        start =
                +( at
                 | hash
                 | url
                 | plain
                 );
    }

    qi::rule<Iterator, std::string()> plain;
    qi::rule<Iterator, std::string()> at;
    qi::rule<Iterator, std::string()> hash;
    qi::rule<Iterator, std::string()> url;
    qi::rule<Iterator, std::vector<std::string>()> start;

    Writer& writer;
};
