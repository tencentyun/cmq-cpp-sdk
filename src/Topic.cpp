#include "TopicMeta.h"
#include "Topic.h"
#include "./json/json.h"
#include "cmq_exception.h"
#include "cmq_tool.h"

using namespace cmq;


namespace  cmq
{

Topic::Topic(const string &topicName, const CMQClient &client):
topicName(topicName),
client(client)
{
}
Topic::~Topic()
{

}
void Topic::SetTopicAttributes(const unsigned int maxMsgSize)
{
	if ( maxMsgSize > 65536)
	{
		throw CMQClientException("invalid argument maxMsgSize > 65536");
		return ;
	}
	map<string, string> param;
	param["topicName"] = this->topicName;
	param["maxMsgSize"] = cmq::int2str(maxMsgSize);

	string result = this->client.call("SetTopicAttributes", param);
	Json::Reader reader;
	Json::Value value;
	if(!reader.parse(result, value))
		throw CMQClientException("Json parse failed");

	int code = value["code"].asInt();
	if(code != 0)
		throw CMQServerException(code,value["message"].asString(),value["requestId"].asString());
}

void Topic::GetTopicAttributes(TopicMeta &meta)
{
	map<string,string> param;
	param["topicName"] = this->topicName;
	string result = this->client.call("GetTopicAttributes", param);
	Json::Reader reader;
	Json::Value value;
	if(!reader.parse(result, value))
	{
		throw CMQClientException("Json parse failed");
		return;
	}

	int code = value["code"].asInt();
	if(code != 0)
	{
		throw CMQServerException(code,value["message"].asString(),value["requestId"].asString());
		return;
	}

    meta.m_msgCount=value["msgCount"].asInt();
    meta.m_maxMsgSize=value["maxMsgSize"].asInt();
    meta.m_msgRetentionSeconds=value["msgRetentionSeconds"].asInt();
    meta.m_loggingEnabled=value["LoggingEnabled"].asInt();
    meta.m_createTime=value["createTime"].asInt();
    meta.m_lastModifyTime=value["lastModifyTime"].asInt();
	meta.m_filterType = value["filterType"].asInt();
}

void Topic::PublishMessage(const string &msgBody, string &msgId, const string & routingKey)
{
	vector<string> vmsgTag;
	PublishMessage(msgBody, msgId, vmsgTag, routingKey);
}

void Topic::PublishMessage(const string &msgBody, string &msgId, const vector<string > & msgTag, const string & routingKey)
{
	map<string,string> param;
	param["topicName"] = this->topicName;
	param["msgBody"] = msgBody;
	param["routingKey"] = routingKey;
	if(! msgTag.empty())
	{
		for (unsigned int i = 0 ; i < msgTag.size() ; ++i)
		{
			string k = "msgTag." + cmq::int2str(i+1);
			param[k] = msgTag[i];
		}
	}
	string result = this->client.call("PublishMessage", param);

	Json::Reader reader;
	Json::Value value;
	if(!reader.parse(result, value))
	{
		throw CMQClientException("Json parse failed");
		return;
	}

	int code = value["code"].asInt();
	if(code != 0)
	{
		throw CMQServerException(code,value["message"].asString(),value["requestId"].asString());
		return;
	}

	msgId = value["msgId"].asString();
}

void Topic::BatchPublishMessage(const vector<string> &vtMsgBody, vector<string> & vtMsgId,const string & routingKey)
{

	vector<string > vtMsgTag;
	BatchPublishMessage(vtMsgBody, vtMsgId, vtMsgTag, routingKey);

}

void Topic::BatchPublishMessage(const vector<string> &vtMsgBody, vector<string> &vtMsgId, const vector<string> & vmsgTag, const string & routingKey)
{
	if(vtMsgBody.empty())
		return;
	map<string,string> param;
	param["topicName"] = this->topicName;
	param["routingkey"] = routingKey;
	for(size_t i=0;i<vtMsgBody.size();i++)
	{
		string k = "msgBody." + cmq::int2str(i+1);
		param[k] = vtMsgBody[i];
	}
	if(! vmsgTag.empty())
	{
		for(unsigned int i = 0 ; i < vmsgTag.size() ; ++i )
		{
			string k = "msgTag." + cmq::int2str(i +1);
            param[k] = vmsgTag[i];
		}
	}
	string result = this->client.call("BatchPublishMessage", param);
	Json::Reader reader;
	Json::Value value;
	if(!reader.parse(result, value))
	{
		throw CMQClientException("Json parse failed");
		return;
	}

	int code = value["code"].asInt();
	if(code != 0)
	{
		throw CMQServerException(code,value["message"].asString(),value["requestId"].asString());
		return;
	}

        vtMsgId.clear();
	Json::Value valMsgList = value["msgList"];
	for(unsigned int i=0;i<valMsgList.size();i++)
	{
		string msgId = valMsgList[i]["msgId"].asString();
		vtMsgId.push_back(msgId);
	}
}


	void Topic::listSubscription(int & totalCount, vector< vector<string> > & vSubscriptionList, const string  & searchWord, const int offset, const int limit )
	{
		map<string, string> param;
		param["topicName"] = this->topicName;
		if (searchWord != "")
			param["searchWord"] = searchWord;
		if (offset >= 0)
			param["offset"] = cmq::int2str(offset);
		if (limit > 0)
			param["limit"] = cmq::int2str(limit);

		string result = this->client.call("ListSubscriptionByTopic", param);
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
		Json::Value valSubList = value["subscriptionList"];
		// clean vector
		vSubscriptionList.clear();
		for (unsigned int i = 0; i < valSubList.size(); i++)
		{
			vector<string> Subscribe;
			Subscribe.push_back(valSubList[i]["subscriptionName"].asString());
			Subscribe.push_back(valSubList[i]["subscriptionId"].asString());
			Subscribe.push_back(valSubList[i]["endpoint"].asString());
			Subscribe.push_back(valSubList[i]["protocol"].asString());
			vSubscriptionList.push_back(Subscribe);
		}
	}

}
