//for hmacsha1 or hmacsha256
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include <string>
#include <string.h>
#include <sstream>
#include "cmq_tool.h"


using  std::string;
using  std::stringstream;

namespace cmq
{

	string int2str(int n)
	{
		stringstream ss;
		ss << n;
		return ss.str();
	}

	string url_encode(const string &src)
	{
		static const char *hex = "0123456789ABCDEF";

		string dst;
		for (size_t i = 0; i < src.size(); i++)
		{
			byte c = (byte)src[i];
			if (isalnum(c) || (c == '-') || (c == '_') || (c == '.') || (c == '!') ||
				(c == '~') || (c == '*') || (c == '\'') || (c == '(') ||
				(c == ')') || (c == '/'))
			{
				dst += c;
			}
			else if (c == ' ')
			{
				dst += "%20";
			}
			else
			{
				dst += '%';
				dst += hex[c >> 4];
				dst += hex[c & 15];
			}
		}

		return dst;
	}

	string base64_encode(const byte *src, int length)
	{
		static const char *b64c = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		string dst;
		const byte *p = src;

		while (length)
		{
			dst += b64c[*p >> 2];
			if (!--length)
			{
				dst += b64c[(*p & 0x3) << 4];
				dst += "==";
				break;
			}
			dst += b64c[((*p & 0x3) << 4) | (*(p + 1) >> 4)];
			p++;
			if (!--length)
			{
				dst += b64c[(*p & 0xF) << 2];
				dst += '=';
				break;
			}
			dst += b64c[((*p & 0xF) << 2) | (*(p + 1) >> 6)];
			p++;

			dst += b64c[*p & 0x3F];
			p++, length--;
		}

		return dst;
	}


	string sign(const string &src, const string &key,const string &method)
	{
		unsigned char result[32];
		unsigned int len=0;
		HMAC_CTX ctx;
		HMAC_CTX_init(&ctx);

		if(method  == "sha1")
		{
			HMAC_Init_ex(&ctx,key.c_str(),key.size(),EVP_sha1(),NULL);
			len=20;
		}
		else
		{
			HMAC_Init_ex(&ctx,key.c_str(),key.size(),EVP_sha256(),NULL);
			len=32;
		}
		HMAC_Update(&ctx, (unsigned char*)src.c_str(), src.size());
		HMAC_Final(&ctx, result, &len);
		HMAC_CTX_cleanup(&ctx);

		return cmq::base64_encode(result,len);
	}

}
