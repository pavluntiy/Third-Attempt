#ifndef NOTICE_EXCEPTION
#define NOTICE_EXCEPTION

#include "my_exception.h"
#include <string>

class NoticeException : public MyException {
public:
	NoticeException (std::string error, Type type = Type::DEFAULT):
	MyException(error, type)
	{
	}
};

#endif