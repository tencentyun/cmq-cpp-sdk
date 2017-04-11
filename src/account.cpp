// for pair
#include <utility>
#include <vector>
#include "account.h"
#include "cmq_exception.h"
#include "json/json.h"
#include "cmq_tool.h"

#include <sstream>
using namespace cmq;
using std::pair;
using std::map;
using std::stringstream;
using std::string;
using std::vector;
namespace cmq {


	Account::Account(const string &endpoint, const string &secretId, const string &secretKey, const string &method)
		:client(endpoint, "/v2/index.php", secretId, secretKey, method)
	{
	}

	Account::Account(const string &endpoint, const string &secretId, const string &secretKey, const string &path, const string &method)
		: client(endpoint, path, secretId, secretKey, method)
	{
	}

	void Account::createQueue(const string &queueName, const QueueMeta &meta)
	{
		map<string, string> param;
		if (queueName == "")
		{
			throw CMQClientException("Invalid parameter:queueName is empty");
			return;
		}
		else
			param["queueName"] = queueName;

		if (meta.maxMsgHeapNum > 0)
			param["maxMsgHeapNum"] = cmq::int2str(meta.maxMsgHeapNum);
		if (meta.pollingWaitSeconds > 0)
			param["pollingWaitSeconds"] = cmq::int2str(meta.pollingWaitSeconds);
		if (meta.visibilityTimeout > 0)
			param["visibilityTimeout"] = cmq::int2str(meta.visibilityTimeout);
		if (meta.maxMsgSize > 0)
			param["maxMsgSize"] = cmq::int2str(meta.maxMsgSize);
		if (meta.msgRetentionSeconds > 0)
			param["msgRetentionSeconds"] = cmq::int2str(meta.msgRetentionSeconds);

        param["rewindSeconds"] = cmq::int2str(meta.rewindSeconds);
		string result = this->client.call("CreateQueue", param);
		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(result, value))
			throw CMQClientException("Json parse failed");

		int code = value["code"].asInt();
		if (code != 0)
			throw CMQServerException(code, value["message"].asString(), value["requestId"].asString());
	}

	void Account::deleteQueue(const string &queueName)
	{
		map<string, string> param;
		param["queueName"] = queueName;
		string result = this->client.call("DeleteQueue", param);
		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(result, value))
			throw CMQClientException("Json parse failed");

		int code = value["code"].asInt();
		if (code != 0)
			throw CMQServerException(code, value["message"].asString(), value["requestId"].asString());
	}

	void Account::listQueue(int &totalCount, vector<string> &vtQueue, const string &searchWord, int offset, int limit)
	{
		map<string, string> param;
		if (searchWord != "")
			param["searchWord"] = searchWord;
		if (offset >= 0)
            param["offset"] = cmq::int2str(offset);
		if (limit > 0)
			param["limit"] = cmq::int2str(limit);

		string result = this->client.call("ListQueue", param);
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

		totalCount = value["totalCount"].asInt();
		Json::Value valQueueList = value["queueList"];
		for (unsigned int i = 0; i < valQueueList.size(); i++)
		{
			vtQueue.push_back(valQueueList[i]["queueName"].asString());
		}
	}

	Queue Account::getQueue(const string &queueName)
	{
		return Queue(queueName, this->client);
	}


	void Account::createTopic(const string & topicName, const int maxMsgSize, const int filterType,const int LoggingEnabled)
	{
		map<string, string> param;
		// not empty
		if (topicName == "")
		{
			throw CMQClientException("Invalid parameter:topicName is empty");
			return;
		}
		// size < 64
		if (topicName.size() > 64){
			throw CMQClientException("Invalid parameter:topicName length  > 64");
			return;
		}

		if (maxMsgSize < 1 || maxMsgSize > 65536)
		{
			throw CMQClientException("Invalid parameter:maxMsgSize Invalid");
			return;
		}

		param["topicName"] = topicName;
		param["filterType"] = cmq::int2str(filterType);
		if (maxMsgSize > 0)
			param["maxMsgSize"] = cmq::int2str(maxMsgSize);

		string result = this->client.call("CreateTopic", param);
		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(result, value))
			throw CMQClientException("Json parse failed");

		int code = value["code"].asInt();

		if (code != 0)
			throw CMQServerException(code, value["message"].asString(), value["requestId"].asString());
	}

	void Account::deleteTopic(const string & topicName)
	{
		map<string, string>  param;
		param["topicName"] = topicName;

		string result = this->client.call("DeleteTopic", param);
		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(result, value))
			throw CMQClientException("Json parse failed");

		int code = value["code"].asInt();

		if (code != 0)
			throw CMQServerException(code, value["message"].asString(), value["requestId"].asString());
	}

	Topic Account::getTopic(const string &topicName)
	{
		return Topic(topicName, this->client);
	}

	void Account::listTopic(int & totalCount, vector< pair< string, string> > & vTopicList, const string & searchWord, const int offset, const int limit)
	{
		map<string, string> param;
		if (searchWord != "")
			param["searchWord"] = searchWord;
		if (offset >= 0)
			param["offset"] = cmq::int2str(offset);
		if (limit > 0)
			param["limit"] = cmq::int2str(limit);

		string result = this->client.call("ListTopic", param);
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

		totalCount = value["totalCount"].asInt();
		Json::Value valTopicList = value["topicList"];
		// clean vector
		vTopicList.clear();
		for (unsigned int i = 0; i < valTopicList.size(); i++)
		{
			pair<string, string> TopicItem(valTopicList[i]["topicId"].asString(), valTopicList[i]["topicName"].asString());
			vTopicList.push_back(TopicItem);
		}
	}
	void Account::createSubscribe(const string & topicName, const string & subscriptionName,const string & endpoint, const string & protocol, const vector<string> & bindingKey)
        {
		vector<string> vfilter ;
		this->createSubscribe(topicName,subscriptionName,endpoint,protocol,bindingKey, vfilter);
	}
	void Account::createSubscribe(const string & topicName,
           const  string & subscriptionName, const string & endpoint,
		const string & protocol, const vector<string> & bindingKey,const vector<string>  & vFilterTag,
		const string & notifyStrategy, const string & notifyContentFormat)
	{

		if (topicName.empty() || topicName.size() == 0)
		{
			throw CMQClientException("Invalid parameter:topicName is empty");
			return;
		}
		if (subscriptionName.empty() || subscriptionName.size() == 0)
		{
			throw CMQClientException("Invalid parameter:topicName is empty");
			return;
		}
		if (endpoint.empty() || endpoint.size() == 0)
		{
			throw CMQClientException("Invalia parameter:endpoint is empty");
			return;
		}
		if (protocol.empty() || protocol.size() == 0)
		{
			throw CMQClientException("Invalia parameter:protocol is empty");
			return;
		}

		map<string, string> param;
		param["topicName"] = topicName;
		param["subscriptionName"] = subscriptionName;
		param["endpoint"] = endpoint;
		param["protocol"] = protocol;
		param["notifyStrategy"] = notifyStrategy;
		param["notifyContentFormat"] = notifyContentFormat;

	if(! bindingKey.empty())
        {
#if __cplusplus < 201103L
		unsigned int n  = 1 ;
		for(unsigned i = 0 ; i < bindingKey.size() ; ++i)
		{
				stringstream ss;
				ss<<n;
                param["bindingKey."+ss.str()] =bindingKey[i];
				++n;

		}

#else
        unsigned int n = 1 ;
        for (auto tag : bindingKey)
        {
            stringstream ss;
            ss<<n;
            param["bindingKey,"+ss.str()] = tag ;

		}
#endif

	if(! vFilterTag.empty())
        {
#if __cplusplus < 201103L
		unsigned int n  = 1 ;
		for(unsigned i = 0 ; i < vFilterTag.size() ; ++i)
		{
				stringstream ss;
				ss<<n;
                param["filterTag."+ss.str()] =vFilterTag[i];
				++n;
		}

#else
        unsigned int n = 1 ;
        for (auto tag : vFilterTag)
        {
            stringstream ss;
            ss<<n;
            param["filterTag,"+ss.str()] = tag ;
        }
#endif
        }

        string result = this->client.call("Subscribe", param);
		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(result, value))
			throw CMQClientException("Json parse failed");

		int code = value["code"].asInt();
		if (code != 0)
			throw CMQServerException(code, value["message"].asString(), value["requestId"].asString());

		}

	}

	void Account::deleteSubscribe(const string & topicName, const string & subscriptionName)
	{
		if (topicName.empty() || topicName.size() == 0)
		{
			throw CMQClientException("Invalid parameter:topicName is empty");
			return;
		}

		if (subscriptionName.empty() || subscriptionName.size() == 0)
		{
			throw CMQClientException("Invalid parameter:subscriptionName is empty");
			return;
		}

		map<string, string> param;
		param["topicName"] = topicName;
		param["subscriptionName"] = subscriptionName;

		string result = this->client.call("Unsubscribe", param);
		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(result, value))
			throw CMQClientException("Json parse failed");

		int code = value["code"].asInt();
		if (code != 0)
			throw CMQServerException(code, value["message"].asString(), value["requestId"].asString());

	}


	Subscription Account::getSubscription(const string &topicName, const string & subscriptionName)
	{

		return Subscription(topicName, subscriptionName, this->client);
	}

}
