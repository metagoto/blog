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
struct tweet_parser : qi::grammar<Iterator>
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


        token =
                +(alnum | '_');

        plain =
                raw[
                    +(byte_ - ((lit("http://") | '@' | '#') >> token))
                ][bind(&Writer::plain, &writer, _1)];

        at =
                lit("@")
                >> raw[
                       +token
                   ][bind(&Writer::at, &writer, _1)];

        hash =
                lit("#")
                >> raw[
                       token
                   ][bind(&Writer::hash, &writer, _1)];

        url =
                lit("http://")
                >> raw[
                        +(token | '/' | '-' | '#' | '?' | '&') % '.'
                   ][bind(&Writer::url, &writer, _1)];

        start =
                +( at
                 | hash
                 | url
                 | plain
                 );
    }

    qi::rule<Iterator> token;
    qi::rule<Iterator> plain;
    qi::rule<Iterator> at;
    qi::rule<Iterator> hash;
    qi::rule<Iterator> url;
    qi::rule<Iterator> start;

    Writer& writer;
};
