#pragma once

#include <string>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_confix.hpp>

namespace runpac {

struct simple_snip
{
    struct m_grammar : public boost::spirit::classic::grammar<m_grammar>
    {

        typedef boost::function<void (const char *, const char *)> action_f;

        action_f& code_action;
        action_f& code_complete_action;
        action_f& para_action;

        m_grammar(action_f& code_action, action_f& code_complete_action, action_f& para_action)
            : code_action( code_action )
            , code_complete_action(code_complete_action)
            , para_action( para_action )
        {
        }

        template<typename ScannerT> struct definition
        {
            definition(m_grammar const& self)
            {
                using namespace boost::spirit::classic;

                doc =
                    *(code | para);

                code =
                        str_p("---")
                     >> +eol_p
                     >> (*(anychar_p - (eol_p >> str_p("---")) ))[self.code_action]
                     >> eol_p
                     >> str_p("---")
                     >> eol_p[self.code_complete_action];

                para =
                        (*(anychar_p - eol_p))[self.para_action]
                     >> +eol_p;

            }

            boost::spirit::classic::rule<ScannerT>
                doc, code, para
                ;

            boost::spirit::classic::rule<ScannerT> const&  start() const { return doc; }
        };

    };


    struct m_compiler
    {
        std::string code_buf;
        std::string buf;

        void do_code(char const* s, char const* e)
        {
            code_buf.clear();
            code_buf = "<pre>" + std::string(s, e) + "</pre>";
        }

        void do_code_complete(char const* s, char const* e)
        {
            buf += code_buf;
        }

        void do_para(char const* s, char const* e)
        {
            if (s != e) {
                buf += "<p>" + std::string(s, e) + "</p>";
            }
        }
    };


    std::string operator()(std::string::iterator s, std::string::iterator e) const
    {

        m_compiler compiler;

        m_grammar::action_f code_f (boost::bind(&m_compiler::do_code,  &compiler, _1,_2));
        m_grammar::action_f code_complete_f (boost::bind(&m_compiler::do_code_complete,  &compiler, _1,_2));
        m_grammar::action_f para_f (boost::bind(&m_compiler::do_para,  &compiler, _1,_2));

        m_grammar parser(code_f, code_complete_f, para_f);

        std::string source(s, e);
        boost::spirit::classic::parse_info<> info =
            boost::spirit::classic::parse(source.c_str(), parser);

        if (info.full) {
            return compiler.buf;
        }
        return std::string(); //
    }

};

} // ns
