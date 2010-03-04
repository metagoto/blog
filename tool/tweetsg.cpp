
#include "tweet_twitter_writer.hpp"
#include "tweet_parser.hpp"
#include "generator.hpp"


// file_to_read file_to_gen
//g++ -O3 -o tweetsg -I/Users/runpac/dev/boost_44/include tweetsg.cpp
int main(int argc, char* argv[])
{
    if (argc != 3) return 1;
    try
    {
        tweet_twitter_writer writer;
        generator<tweet_parser, tweet_twitter_writer> g(writer);

        const std::string& res = g.gen(std::string(argv[1]));

        if (res.empty()) {
            return 1;
        }

        std::ofstream out(argv[2]);
        if (!out.is_open()) {
            return 1;
        }

        out << "<h3>Tweets</h3>\n";
        out << "<ul class=\"tweets\">\n";
        out << res;
        out << "<li><a href=\"http://twitter.com/metagoto\">More tweets..</a></li>";
        out << "</ul>\n";

        out.close();
        return 0;
    }
    catch (...)
    {
    }
    return 1;
}
