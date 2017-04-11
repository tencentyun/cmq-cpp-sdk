#include "Subscription.h"
#include "cmq_exception.h"
#include "json/json.h"
#include "cmq_tool.h"

namespace cmq
{

	Subscription::Subscription(const string & topicName,const string & subscriptionName, const CMQClient & client):
        m_topicName(topicName),
        m_subscriptionName(subscriptionName),
        m_client(client)
	{
	}


	Subscription::~Subscription()
	{
	}
	void Subscription::clearFilterTags()
	{
		map<string, string> param;

		param["topicName"] = this->m_topicName;
		param["subscriptionName"] = this->m_subscriptionName;

		string result = this->m_client.call("ClearSubscriptionFilterTags", param);

		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(result, value))
		{
			throw CMQClientException("Json parse failed");
			return;
		}

		int code = value["code"].asInt();
		if (code != 0)
		{
			throw CMQServerException(code, value["message"].asString(), value["requestId"].asString());
			return;
		}
	}


	void Subscription::getSubscriptionAttributes(SubscriptionMeta & meta)
	{
		map<string, string> param;

		param["topicName"] = this->m_topicName;
		param["subscriptionName"] = this->m_subscriptionName;

		string result = this->m_client.call("GetSubscriptionAttributes", param);

		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(result, value))
		{
			throw CMQClientException("Json parse failed");
			return;
		}

		int code = value["code"].asInt();
		if (code != 0)
		{
			throw CMQServerException(code, value["message"].asString(), value["requestId"].asString());
			return;
		}

		meta.TopicOwner = value["topicOwner"].asString();
		meta.endpoint = value["endpoint"].asString();
		meta.protocol = value["protocal"].asString();
		meta.notifyStrategy = value["notifyStrategy"].asString();
		meta.notifyContentFormat = value["notifyContentFormat"].asString();
		meta.CreateTime = value["createTime"].asInt();
		meta.LastModifyTime = value["lastModifyTime"].asInt();
	    meta.msgCount = value["msgCount"].asInt();

		meta.filterTag.clear();
		Json::Value FilterTagList = value["filterTag"];
		for (unsigned int i = 0; i < FilterTagList.size(); ++i)
		{
			meta.filterTag.push_back(FilterTagList[i].asString());
		}
        
		//clear bindingKey
		meta.bindingKey.clear();
        Json::Value bindingKeyList = value["bindingKey"];
		for(unsigned int i = 0 ; i < bindingKeyList.size(); ++i)
		{
			meta.bindingKey.push_back(bindingKeyList[i].asString());
		}
	}

	void Subscription::setSubscriptionAttributes(const SubscriptionMeta meta)
	{
		map<string, string> param;
		param["topicName"] = this->m_topicName;
		param["subscriptionName"] = this->m_subscriptionName;
		param["notifyStrategy"] = meta.notifyStrategy;
        param["notifyContentFormat"] = meta.notifyContentFormat;

#if  __cplusplus < 201103L
		if(! meta.bindingKey.empty())
		{
			for(unsigned int i= 0 ; i < meta.bindingKey.size() ; ++i)

			{
				string k = "bindingKey." + cmq::int2str(i+1);
				param[k]=meta.bindingKey[i];
			}
		}
#else
		if (! meta.bindingKey.empty())
		{
			unsigned int n = 1; 
       			for(auto tag : meta.bindingKey)
        		{		
            			string k = "bindingKey." + cmq::int2str(n);
           	 		    param[k] = tag;
            			++n;
				}
		}

#endif

#if  __cplusplus < 201103L
		if(! meta.filterTag.empty())
		{
			for(unsigned int i= 0 ; i < meta.filterTag.size() ; ++i)

			{
				string k = "filterTag." + cmq::int2str(i+1);
				param[k]=meta.filterTag[i];
			}
		}
#else
		if (! meta.filterTag.empty())
		{
			unsigned int n = 1; 
       			for(auto tag : meta.filterTag)
        		{
            		string k = "filterTag." + cmq::int2str(n);
           	 		param[k] = tag;
            			++n;
				}
		}

#endif

		string result = this->m_client.call("SetSubscriptionAttributes", param);
		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(result, value))
		{
			throw CMQClientException("Json parse failed");
			return;
		}

		int code = value["code"].asInt();
		if (code != 0)
			throw CMQServerException(code, value["message"].asString(), value["requestId"].asString());
	}

}
