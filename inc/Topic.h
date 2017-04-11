#ifndef CMQ_SDK_SRC_TOPIC_H__
#define CMQ_SDK_SRC_TOPIC_H__

#include<string>
#include<vector>
#include"cmq_client.h"
#include"TopicMeta.h"
#include"Message.h"
#include"Subscription.h"

using std::string;
using std::vector;
using cmq::CMQClient;
using cmq::TopicMeta;
using cmq::Message;


namespace cmq
{
	class Topic
	{
	protected:
		string topicName;
		CMQClient client;
	public:

		Topic(const string &, const CMQClient &);
                ~Topic();
                

		void SetTopicAttributes(const unsigned int maxMsgSize) ;
		void GetTopicAttributes(TopicMeta &);

		void PublishMessage(const string & msgBody , string & msgId ,const string & routingKey="");
		void PublishMessage(const string & msgBody , string & msgId, const  vector<string> & vmsgTag, const string & routingKey ="");

		void BatchPublishMessage(const vector< string > &, vector< string > & vmsgId, const vector< string > & vmsgTag , const string & routingKey = "");
		void BatchPublishMessage(const vector< string > &, vector< string > & vmsgId, const string & routingKey ="");
			
		/**
		 * @brief             列出Topic的订阅列表
		 * @param topicName   主题名称
		 * @param totalCount  用户帐号下的订阅数
		 * @param searchWord  查询关键字
		 * @param offset      
		 * @param limit  
		 * #param vSubscriptionList 返回订阅列表
		**/	void listSubscription(int & totalCount,vector< vector<string> > & , const string  & searchWord="", const int offset = -1 , const int limit = -1 );
	};
}

#endif

