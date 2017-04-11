#ifndef CMQ_SDK_SRC_TOPICMETA__
#define CMQ_SDK_SRC_TOPICMETA__
namespace cmq
{
	class TopicMeta
	{

	 public:
		unsigned int m_msgCount;
		unsigned int m_maxMsgSize;
	    unsigned int m_msgRetentionSeconds;
		unsigned int m_createTime;
		unsigned int m_lastModifyTime;
		unsigned int m_loggingEnabled;
        unsigned int m_filterType;
		
	 public:
		TopicMeta();

		~TopicMeta();

	};
}
#endif
