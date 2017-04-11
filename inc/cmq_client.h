#ifndef CMQ_SDK_INCLUDE_CLIENT_H__
#define CMQ_SDK_INCLUDE_CLIENT_H__


#include <stdlib.h>      
#include <time.h> 
#include <string>
#include <map>
#include <iostream>

#include "cmq_exception.h"
#include "cmq_http.h"

using  std::string;
using  std::map;

namespace cmq
{

class CMQClient
{
	protected:
		string endpoint;
		string path;
		string secretId;
		string secretKey;
		string method;
		string signMethod;
		
		CMQHttp http;
		
	public:
		CMQClient();
		
		CMQClient(const string &endpoint, const string &path, const string &secretId, const string &secretKey, const string &method="POST");

		CMQClient& operator=(const CMQClient &r)
		{
			if(this != &r)
			{
				this->endpoint = r.endpoint;
				this->path = r.path;
				this->secretId = r.secretId;
				this->secretKey = r.secretKey;
				this->method = r.method;
				this->signMethod =r.signMethod;
			}
			return *this;
		}

                CMQClient(const CMQClient &r)
                {
                     this->endpoint = r.endpoint;
                     this->path = r.path;
                     this->secretId = r.secretId;
                     this->secretKey = r.secretKey;
                     this->method = r.method;
                     this->signMethod =r.signMethod;
                }

        void setSignMethod (string method)
        {
        	if (method != "sha1" && method != "sha256")
        	{
        		throw  CMQClientException("Only support sha1 or sha256 now");
        	}
        	else
        	{
        	    this->signMethod = method ;
        	}
        }
		void setEndpoint(string endpoint)
		{
			this->endpoint = endpoint;
		}
		void setPath(string path)
		{
			this->path = path;
		}
		
		void setSecretId(string secretId)
		{
			this->secretId = secretId;
		}
		
		void setSecretKey(string secretKey)
		{
			this->secretKey = secretKey;
		}
		
		void setMethod(string method)
		{
			this->method = method;
		}
		
		string call(const string &action, map<string,string> &param);
};

}

#endif

