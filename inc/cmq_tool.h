#ifndef CMQ_SDK_INCLUDE_TOOL_H__
#define CMQ_SDK_INCLUDE_TOOL_H__

#include <string>

using std::string;

namespace cmq
{	
        typedef unsigned char byte;	

		 string int2str(int n);

		 string url_encode(const string &src);

		 string base64_encode(const byte *src, int length);


		 string sign(const string &src, const string &key,const string & method ="sha1");


}

#endif

