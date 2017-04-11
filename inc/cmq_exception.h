#ifndef __CMQ_SDK_INCLUDE_EXCEPTION_H__
#define __CMQ_SDK_INCLUDE_EXCEPTION_H__

#include <exception>
#include <string>


using std::exception;
using std::string;

namespace cmq
{

class CMQClientException : public exception
{
public:
    CMQClientException(const string &msg) throw()
    {
        this->msg = msg;
    }
	
    virtual ~CMQClientException() throw(){}
	
	virtual const char* what() const throw();
	
	string msg;
};

class CMQServerNetworkException : public exception
{
public:
    CMQServerNetworkException(int status) throw()
    {
        this->status = status;
    }
	
    virtual ~CMQServerNetworkException() throw(){}

    int getStatus() const
    {
        return this->status;
    }
	
	//http status
	int status;
};

class CMQServerException : public exception
{
public:
    CMQServerException(int code, const string &message, const string &requestId) throw()
    {
        this->code = code;
		this->message = message;
		this->requestId = requestId;
    }
	
    ~CMQServerException() throw(){}

    string getRequestId() const
    {
        return this->requestId;
    }
    int getCode() const
    {
        return this->code;
    }
    string getMessage() const
    {
        return this->message;
    }
	
	/**
	 *常见错误类型:
	 *: 4000       参数不合法
     *: 4100       鉴权失败:密钥不存在/失效
     *: 4300       账户欠费了
     *: 4400       消息大小超过队列属性设置的最大值
     *: 4410       已达到队列最大的消息堆积数
     *: 4420       qps限流
     *: 4430       删除消息的句柄不合法或者过期了
     *: 4440       队列不存在
     *: 4450       队列个数超过限制
     *: 4460       队列已经存在
     *: 6000       服务器内部错误
     *: 6010       批量删除消息失败（具体原因还要看每个消息删除失败的错误码）
     *: 7000       空消息，即队列当前没有可用消息
	**/
	int code;
	//返回的详细错误信息
	string message;
	//服务器生成的请求Id，便于追踪问题
	string requestId;
};

}

#endif
