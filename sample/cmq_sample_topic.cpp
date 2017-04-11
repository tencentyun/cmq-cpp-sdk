

// for cmq sdk
#include "cmq_common.h"

#include <iostream>
#include <vector>

#include <fstream>

using namespace std;
using namespace cmq;

int main()
{
    //从腾讯云官网查询的云API密钥信息
    string secretId= "AKIDU65RoYl9Q4okMfnrTE9FVhmmuWYnZJXl";
    string secretKey= "mWVemFhWOiQX0ZEErlizRDLXgOVyptH1";
    string endpoint= "http://cmq-topic-sh.api.qcloud.com";

    try
    {
        // create account and topic
        cout<<"Init account"<<endl;
        Account account(endpoint, secretId, secretKey);

        string topicName = "topic-test";
        account.createTopic(topicName, 65536);
        cout<< "create topic " <<endl;
        //get topic meta
        Topic topic = account.getTopic(topicName);
        //cout<< "get topic meta "<<endl;
        TopicMeta topicMeta;
        topic.GetTopicAttributes(topicMeta);
        cout <<"get topic meta "<<endl ;
        cout<< "topic maxMsgSize is " << topicMeta.m_maxMsgSize <<endl;

        cout<<"create subscription"<<endl;
        string subscriptionName="sub-test";
        // your endpoint and protocal
        string endpoint = "";
        string protocol = "";
        vector<string> vflterTag;
        account.createSubscribe(topicName,subscriptionName,endpoint,protocol,vflterTag);
        cout<<"create subscription"<<endl;

        string msg = "this is a test message";
        string msgId;

        topic.PublishMessage(msg,msgId);
        cout<<"Publish message" <<endl;



        // batch publish message
        vector< string > vmsg;
        for (unsigned int i = 0; i < 5; ++i)
        {
            string ss;
            ss = "this is a test message ";
            vmsg.push_back(ss);
        }
        vector<string> vmsgId;
        topic.BatchPublishMessage(vmsg, vmsgId);
        cout<< "batch publish message "<<endl;
        // list topic
        // create subscription
        vector< pair<string, string > >  vTopic;
        int total(0);
        account.listTopic(total, vTopic);
        Subscription sub = account.getSubscription(topicName, subscriptionName);

        //set and get subscription meta
        SubscriptionMeta subMeta;
        sub.getSubscriptionAttributes(subMeta);
        cout<<"get sub meta"<<endl ;
        sub.setSubscriptionAttributes(subMeta);

        cout<<"set sub meta"<<endl ;
        vector< vector<string> > vsublist;
        topic.listSubscription(total, vsublist);
        account.deleteSubscribe(topicName,subscriptionName);
        account.deleteTopic(topicName);
        cout<<"delete sub and topic"<<endl;
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
