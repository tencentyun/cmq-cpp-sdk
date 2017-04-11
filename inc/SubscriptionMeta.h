#ifndef CMQ_SDK_INCLUDE_SUBSCRIPTIONMETA_H_
#define CMQ_SDK_INCLUDE_SUBSCRIPTIONMETA_H_


#include<string>
#include<vector>

using std::string;
using std::vector;
namespace cmq
{
	class SubscriptionMeta
	{
	public:
		SubscriptionMeta();
		~SubscriptionMeta();
		string TopicOwner;
		string endpoint;
		string protocol;
		string notifyStrategy;
		string notifyContentFormat;
		vector<string> filterTag;
		int CreateTime;
		int LastModifyTime;
		int msgCount;
		vector<string> bindingKey;

	};
}
#endif // !CMQ_SDK_INCLUDE_SUBSCRIPTIONMETA_H_



