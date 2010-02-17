#pragma once

#include <string>
#include <sstream>
#include <boost/range/iterator_range.hpp>

struct tweet_twitter_writer
{
    typedef boost::iterator_range<std::string::const_iterator> range;

    void plain(range const& s)
    {
        os << std::string(boost::begin(s), boost::end(s));
    }

    void at(range const& s)
    {
        std::string name(boost::begin(s), boost::end(s));
        os << "<a href=\"http://twitter.com/" << name << "\">@" << name << "</a>";
    }

    void hash(range const& s)
    {
        std::string name(boost::begin(s), boost::end(s));
        os << "<a href=\"http://twitter.com/search?q=%23" << name << "\">#" << name << "</a>";
    }

    void url(range const& s)
    {
        std::string name(boost::begin(s), boost::end(s));
        os << "<a href=\"http://" << name << "\">http://" << name << "</a>";
    }

    std::string str()
    {
        return os.str();
    }

    void clear()
    {
        os.str("");
    }

    std::ostringstream os;
};
