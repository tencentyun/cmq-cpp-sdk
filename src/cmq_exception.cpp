#include "cmq_exception.h"

using namespace cmq;

const char* CMQClientException::what() const throw()
{
    return this->msg.c_str();
}