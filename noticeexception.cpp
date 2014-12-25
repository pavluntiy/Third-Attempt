
#include "noticeexception.h"


	NoticeException::NoticeException (std::string error, Type type = Type::DEFAULT):
	MyException(error, type)
	{
	}
