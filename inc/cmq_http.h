#ifndef CMQ_SDK_INCLUDE_HTTP_H__
#define CMQ_SDK_INCLUDE_HTTP_H__

#include <curl/curl.h>
#include <string>

using std::string;

namespace cmq
{
	
class CMQHttp
{	
	public:
		CMQHttp(int timout=10/*sec*/,bool isKeepAlive=true);
		~CMQHttp();
		
		void request(const string &method, const string &url, const string &req, string &rsp, const int timeout = 0);
	
	protected:
		CURL *curl;
		int timeout;
		bool isKeepAlive;
};

}

#endif

