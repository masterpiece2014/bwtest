/*
 *  bwtest
 *  Copyright  Bowen Cai
 *  MIT lecense
 */

#ifndef NULLOSTREAM_H
#define NULLOSTREAM_H

#include "commen.h"

namespace bwtest {

namespace BWTestInternal
{

///////////////////////////////////////////////////////////////////////////////////////
/////////////  two classes below are adopted from stackoverflow
/////////////   http://stackoverflow.com/questions/760301/implementing-a-no-op-stdostream
/////////////////////////////////////////////////////////////////////////////////////////////
class basicNullBuf: public std::basic_streambuf<char, TYPENAME_OUT std::char_traits<char> > {
    TYPENAME_OUT std::char_traits<char>::int_type overflow(TYPENAME_OUT std::char_traits<char>::int_type c) {
        typedef std::char_traits<char>  nameTmp;
        return nameTmp::not_eof(c);
    }
};
class NullOStream: public std::basic_ostream<char, typename std::char_traits<char> > {
    public:
        NullOStream():
        std::basic_ios<char, typename std::char_traits<char> >(&basicNullOSBuf_),
        std::basic_ostream<char, typename std::char_traits<char> >(&basicNullOSBuf_) {
            std::basic_ostream<char, typename std::char_traits<char> >::init(&basicNullOSBuf_);
        }

    private:
        basicNullBuf basicNullOSBuf_;
};

}
}
#endif // NULLOSTREAM_H
