#ifndef CMQ_SDK_INCLUDE_SUBSCRIPTION_H_
#define CMQ_SDK_INCLUDE_SUBSCRIPTION_H_

#include<string>
#include"cmq_client.h"
#include"SubscriptionMeta.h"

using std::string;

namespace cmq
{
	class Subscription
	{
	public:

		Subscription(const string & ,const string & ,const CMQClient &);
		~Subscription();
		void getSubscriptionAttributes(SubscriptionMeta & meta);
		void setSubscriptionAttributes(const SubscriptionMeta meta);
		void clearFilterTags();
	protected:
		string m_topicName;
		string m_subscriptionName;
		CMQClient m_client;



	};
}
#endif //


