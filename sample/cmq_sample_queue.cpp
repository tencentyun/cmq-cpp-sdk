#include <iostream>
#include <vector>
#include <fstream>
#include "cmq_exception.h"
#include "account.h"
#include "queue.h"

using namespace std;
using namespace cmq;

int main()
{
    //从腾讯云官网查询的云API密钥信息
    string secretId="AKIDU65RoYl9Q4okMfnrTE9FVhmmuWYnZJXl";
    string secretKey="mWVemFhWOiQX0ZEErlizRDLXgOVyptH1";
    string endpoint="http://cmq-queue-sh.api.qcloud.com";

    try
    {
		Account account(endpoint, secretId, secretKey);

		//创建队列
		cout<<"---------------create queue ...---------------"<<endl;
		QueueMeta meta;
		meta.pollingWaitSeconds = 10;
		meta.visibilityTimeout = 10;
		meta.maxMsgSize = 65536;
		meta.msgRetentionSeconds = 345600;
		account.createQueue("queue-test10",meta);
		cout<<"queue-test10 created"<<endl;
		account.createQueue("queue-test11",meta);
		cout<<"queue-test11 created"<<endl;
		account.createQueue("queue-test12",meta);
		cout<<"queue-test12 created"<<endl;
		//account.createQueue("queue-test13",meta);
		//cout<<"queue-test13 created"<<endl;
		//account.createQueue("queue-test14",meta);
		//cout<<"queue-test14 created"<<endl;

		//列出当前帐号下所有队列名字
		cout<<"---------------list queue ...---------------"<<endl;
		vector<string> vtQueue;
		int totalCount = 0;
		account.listQueue(totalCount,vtQueue);
		cout<<"totalCount:"<<totalCount<<endl;
		for(size_t i=0;i<vtQueue.size();i++)
		{
			cout<<"queueName:"<<vtQueue[i]<<endl;
		}

		//删除队列
		cout<<"---------------delete queue ...---------------"<<endl;
		account.deleteQueue("queue-test11");
		cout<<"queue-test11 deleted"<<endl;
		account.deleteQueue("queue-test12");
		cout<<"queue-test12 deleted"<<endl;

		//获得队列实例
		cout<<"--------------- queue[queue-test10] ---------------"<<endl;
		Queue queue = account.getQueue("queue-test10");

		//设置队列属性
		cout<<"---------------set queue attributes ...---------------"<<endl;
		QueueMeta meta1;
		meta1.pollingWaitSeconds = 20;
		queue.setQueueAttributes(meta1);
		cout<<"pollingWaitSeconds=20 set"<<endl;

		//获取队列属性
		cout<<"---------------get queue attributes ...---------------"<<endl;
		QueueMeta meta2;
		queue.getQueueAttributes(meta2);
		cout<<"maxMsgHeapNum:"<<meta2.maxMsgHeapNum<<endl;
		cout<<"pollingWaitSeconds:"<<meta2.pollingWaitSeconds<<endl;
		cout<<"visibilityTimeout:"<<meta2.visibilityTimeout<<endl;
		cout<<"maxMsgSize:"<<meta2.maxMsgSize<<endl;
		cout<<"createTime:"<<meta2.createTime<<endl;
		cout<<"lastModifyTime:"<<meta2.lastModifyTime<<endl;
		cout<<"activeMsgNum:"<<meta2.activeMsgNum<<endl;
		cout<<"inactiveMsgNum:"<<meta2.inactiveMsgNum<<endl;

		//发送消息
		cout<<"---------------send message ...---------------"<<endl;
		string msgId;
		queue.sendMessage("hello world,this is cmq sdk for c++",msgId,10);
		cout<<"[hello world,this is cmq sdk for c++] sent"<<endl;

		//接收消息
		cout<<"---------------recv message ...---------------"<<endl;
		Message msg;
		queue.receiveMessage(10,msg);
		cout<<"msgId:"<<msg.msgId<<endl;
		cout<<"receiptHandle:"<<msg.receiptHandle<<endl;
		cout<<"msgBody:"<<msg.msgBody<<endl;
		cout<<"enqueueTime:"<<msg.enqueueTime<<endl;
		cout<<"nextVisibleTime:"<<msg.nextVisibleTime<<endl;
		cout<<"firstDequeueTime:"<<msg.firstDequeueTime<<endl;
		cout<<"dequeueCount:"<<msg.dequeueCount<<endl;

		//删除消息
		cout<<"---------------delete message ...---------------"<<endl;
		queue.deleteMessage(msg.receiptHandle);
		cout<<"receiptHandle:"<<msg.receiptHandle<<" deleted"<<endl;

		//批量操作
		//批量发送消息
		cout<<"---------------batch send message ...---------------"<<endl;
		vector<string> vtMsgId;
		vector<string> vtMsgBody;
		string msgBody = "hello world,this is cmq sdk for c++ 1";
		vtMsgBody.push_back(msgBody);
		msgBody = "hello world,this is cmq sdk for c++ 2";
		vtMsgBody.push_back(msgBody);
		msgBody = "hello world,this is cmq sdk for c++ 3";
		vtMsgBody.push_back(msgBody);
		queue.batchSendMessage(vtMsgBody,vtMsgId);
		for(size_t i=0;i<vtMsgBody.size();i++)
			cout<<"["<<vtMsgBody[i]<<"] sent"<<endl;
		for(size_t i=0;i<vtMsgId.size();i++)
			cout<<"msgId:"<<vtMsgId[i]<<endl;

		//批量接收消息
		vector<string> vtReceiptHandle; //保存服务器返回的消息句柄，用于删除消息
		cout<<"---------------batch recv message ...---------------"<<endl;
		vector<Message> msgList;
		queue.batchReceiveMessage(10,10,msgList);
		cout<<"recv msg count:"<<msgList.size()<<endl;
		for(size_t i=0;i<msgList.size();i++)
		{
			Message &msg1 = msgList[i];
			cout<<"msgId:"<<msg1.msgId<<endl;
			cout<<"receiptHandle:"<<msg1.receiptHandle<<endl;
			cout<<"msgBody:"<<msg1.msgBody<<endl;
			cout<<"enqueueTime:"<<msg1.enqueueTime<<endl;
			cout<<"nextVisibleTime:"<<msg.nextVisibleTime<<endl;
			cout<<"firstDequeueTime:"<<msg.firstDequeueTime<<endl;
			cout<<"dequeueCount:"<<msg.dequeueCount<<endl;

			vtReceiptHandle.push_back(msg1.receiptHandle);
		}

		//批量删除消息
		cout<<"---------------batch delete message ...---------------"<<endl;
		queue.batchDeleteMessage(vtReceiptHandle);
		for(size_t i=0;i<vtReceiptHandle.size();i++)
			cout<<"receiptHandle:"<<vtReceiptHandle[i]<<" deleted"<<endl;

    }
	catch(CMQServerNetworkException &e)
    {
        cout<<"Server Network exception,http status: "<<e.getStatus()<< endl;
        return -1;
    }
    catch(CMQServerException &e)
    {
        cout<<"Server Action failed,code:"<< e.getCode()<<",message:"<<e.getMessage()<<",requestId:"<<e.getRequestId()<< endl;
        return -1;
    }
    catch(CMQClientException &e)
    {
        cout<<"CMQ Client Exceptrion:"<<e.what()<< endl;
        return -2;
    }
}
