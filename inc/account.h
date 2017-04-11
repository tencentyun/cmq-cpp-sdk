#ifndef CMQ_SDK_INCLUDE_ACCOUNT_H__
#define CMQ_SDK_INCLUDE_ACCOUNT_H__



#include "stdint.h"
#include <utility>
#include <string>

#include "cmq_client.h"
#include "queue.h"
#include "Topic.h"
#include "Subscription.h"

using  std::string;
using  std::pair;

namespace cmq
{


/*Account类对象非线程安全，如果多线程使用，需要每个线程单独初始化Account类对象*/
class Account
{
	public:
		/**
		 * @brief
		 * @param secretId    从腾讯云官网查看云api的密钥ID
		 * @param secretKey   从腾讯云官网查看云api的密钥
		 * @param endpoint    消息服务api地址，例如：https://cmq-queue-gz.api.qcloud.com
		 * @param method	  请求方法，可使用GET或POST方法
		**/
		Account(const string &endpoint,const string &secretId, const string &secretKey, const string &method="POST");
		Account(const string &endpoint,const string &secretId, const string &secretKey, const string &path, const string &method);

        /**
         * @setSignMethod set the sign method and now we support sha1 and sha256
         * @ signMethod string  only set "sha1" or "sha256"
         */
		void setSignMethod(const string & signMethod = "sha1"){this->client.setSignMethod(signMethod);}
		/**
		 * @brief             创建队列
		 * @param queueName   队列名字
		 * @param meta        队列属性参数
		**/
		void createQueue(const string &queueName,const QueueMeta &meta);
		/**
		 * @brief             删除队列
		 * @param queueName   队列名字
		**/
		void deleteQueue(const string &queueName);
		/**
		 * @brief             列出Account的队列列表
		 * @param totalCount  返回用户帐号下总队列数
		 * @param vtQueue     返回队列列表
		 * @param searchWord  查询关键字
		 * @param offset
		 * @param limit
		**/
		void listQueue(int &totalCount,vector<string> &vtQueue, const string &searchWord="", int offset=-1, int limit=-1);
		/**
		 * @brief             获取Account的一个Queue对象
		 * @param queueName   Queue名称
		 * @return 		      返回一个Queue对象
		**/
		Queue getQueue(const string &queueName);

        /**
		 * @brief             获取Account的一个Topic对象
		 * @param topicName   Topic名称
		 * @return 		      返回一个Topic对象
		**/
		Topic getTopic(const string &topicName);
        /**
		 * @brief             创建主题
		 * @param topicName   主题名字
		 * @param maxMsgSize  队列消息最大长度
		 * @param filterType  过滤选项， 0 不过滤， 1 标签过滤，2 routing key 过滤
		**/
		void createTopic(const string & topicName, const int maxMsgSize = 654536 , const int filterType = 1 , const int LoggingEnabled = 0);
        /**
		 * @brief             删除主题
		 * @param topicName   主题名字
		**/
		void deleteTopic(const string & topicName);
        /**
		 * @brief             列出Account的主题列表
		 * @param totalCount  返回用户帐号下的主题数
		 * @param vtQueue     返回主题列表
		 * @param searchWord  查询关键字
		 * @param offset
		 * @param limit
		**/
		void listTopic(int & totalCount , vector< pair<string, string> > &, const string & searchWord="",
			const int offset=-1, const int limit= -1);

        /**
		 * @brief                     创建订阅
		 * @param topicName           主题名字
		 * @param subscriptionName    订阅名称
		 * @param endpoint            订阅终端
		 * @param protocol            订阅协议
		 * @param filterTag           过滤标签
		 * @param bindingKey
		 * @param notifyStrategy      推送失败重试策略
		 * @param notifyContentFormat 推送格式
		**/
		void createSubscribe(const string & topicName, const string & subscriptionName, const string & endpoint ,const  string  & protocol ,const vector<string> & bindingKey);
		void createSubscribe(const string & topicName, const string & subscriptionName, const  string & endpoint,
			const string & protocol, const vector<string> & bindingKey , const vector<string> & vFilterTag,
			const string & notifyStrategy="BACKOFF_RETRY", const string & notifyContentFormat="JSON");

		/**
		 * @brief                   删除订阅
		 * @param subscriptionName  订阅名称
		**/
		void deleteSubscribe(const string & topicName, const string & subscriptionName);


        /**
		 * @brief                    获取Account的一个订阅对象
		 * @param topicName          主题
		 * @param subscriptionName   订阅名称
		 * @return 		             返回一个Subscription对象
		**/
        Subscription getSubscription(const string & , const string & );


	protected:
		CMQClient client;
};





}

#endif

