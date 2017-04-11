#ifndef CMQ_SDK_INCLUDE_QUEUE_H__
#define CMQ_SDK_INCLUDE_QUEUE_H__


#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include "cmq_client.h"
#include "Message.h"
using std::string;
using std::map;
using std::vector;


namespace cmq
{
	const unsigned int DEFAULT_POLLING_WAIT_SECONDS = 0; 			//缺省消息接收长轮询等待时间
	const unsigned int DEFAULT_VISIBILITY_TIMEOUT = 30; 			//缺省消息可见性超时
	const unsigned int DEFAULT_MAX_MSG_SIZE = 65536;				//缺省消息最大长度
	const unsigned int  DEFAULT_MSG_RETENTION_SECONDS = 345600;	//缺省消息保留周期
class QueueMeta
{
	public:
		QueueMeta();
		//最大堆积消息数
		int maxMsgHeapNum;
		//消息接收长轮询等待时间
		int pollingWaitSeconds;
		//消息可见性超时
		int visibilityTimeout;
		//消息最大长度
		int maxMsgSize;
		//消息保留周期
		int msgRetentionSeconds;
		//队列创建时间
		int createTime;
		//队列属性最后修改时间
		int lastModifyTime;
		//队列处于Active状态的消息总数
		int activeMsgNum;
		//队列处于Inactive状态的消息总数
		int inactiveMsgNum;
		// 队列设定的最大回溯时间
		unsigned int rewindSeconds ;
		// 已删除，但是还在回溯保留时间内的消息数量
		int rewindmsgNum ;
		// 消息最小未消费时间，单位为秒
		int minMsgTime;
		// 延时消息数量
		int delayMsgNum;
};

/**Queue类对象非线程安全，如果多线程使用，需要每个线程单独初始化Queue类对象*/
class Queue
{
	public:
		Queue(const string &queueName, const CMQClient &client);
	
		/**
		 * @brief             设置队列属性
		 * @param meta        队列属性参数
		**/
		void setQueueAttributes(const QueueMeta &meta);
		/**
		 * @brief             获取队列属性
		 * @param meta        返回的队列属性参数
		**/
		void getQueueAttributes(QueueMeta &meta);
		/**
		 * @brief             发送消息
		 * @param msgBody     消息内容
		 * @param msgId       服务器返回的消息唯一标识
		**/
		void sendMessage(const string &msgBody, string &msgId, const unsigned int delayTime =0);
		/**
		 * @brief             批量发送消息
		 * @param vtMsgBody   消息列表
		 * @param vtMsgId     服务器返回的消息唯一标识列表
		**/
		void batchSendMessage(const vector<string> &vtMsgBody, vector<string> &vtMsgId, const unsigned int delayTime = 0 );
		/**
		 * @brief             获取消息
		 * @param pollingWaitSeconds     请求最长的Polling等待时间
		 * @param msg         服务器返回消息
		**/
		void receiveMessage(int pollingWaitSeconds, Message &msg);
		/**
		 * @brief             批量获取消息
		 * @param numOfMsg    准备获取消息数
		 * @param pollingWaitSeconds     请求最长的Polling等待时间
		 * @param msgList     服务器返回消息列表
		**/
		void batchReceiveMessage(int numOfMsg, int pollingWaitSeconds, vector<Message> &msgList);
		/**
		 * @brief             删除消息
		 * @param receiptHandle     消息句柄，获取消息时由服务器返回
		**/
		void deleteMessage(const string &receiptHandle);
		/**
		 * @brief             批量删除消息
		 * @param receiptHandle     消息句柄列表，获取消息时由服务器返回
		**/
		void batchDeleteMessage(const vector<string> &vtReceiptHandle);

		/*
		 *
		 * @brief rewind queue 
		 *
		 */
		void rewindQueue(const unsigned long);
	protected:
		string queueName;
		CMQClient client;
};

}

#endif

