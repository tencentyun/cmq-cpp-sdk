#include"Message.h"
namespace cmq
{
    Message::Message():
    msgId(""),
    receiptHandle(""),
    msgTag(NULL),
    msgBody(""),
    enqueueTime(0),
    nextVisibleTime(0),
    firstDequeueTime(0),
    dequeueCount(0)
    {

    }
    Message::~Message()
    {

    }
}
