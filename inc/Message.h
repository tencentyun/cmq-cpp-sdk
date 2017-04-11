#ifndef CMQ_SDK_INCLUDE_MESSAGE_H_
#define CMQ_SDK_INCLUDE_MESSAGE_H_

#include"stdint.h"
#include<string>
#include<vector>
using std::vector;
using std::string;

namespace cmq
{
	class Message
	{
	public:
		Message();

		~Message();

		//服务器返回的消息ID
		string msgId;
		//每次消费唯一的消息句柄，用于删除等操作
		string receiptHandle;
		//消息标签（用于消息过滤）
		vector<string> msgTag;
		//消息体
		string msgBody;
		//消息发送到队列的时间，从 1970年1月1日 00:00:00 000 开始的毫秒数
		uint64_t enqueueTime;
		//消息下次可见的时间，从 1970年1月1日 00:00:00 000 开始的毫秒数
		uint64_t nextVisibleTime;
		//消息第一次出队列的时间，从 1970年1月1日 00:00:00 000 开始的毫秒数
		uint64_t firstDequeueTime;
		//出队列次数
		int dequeueCount;

	};
}
#endif
