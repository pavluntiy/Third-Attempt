
#include "noticeexception.hpp"


	NoticeException::NoticeException (std::string error, Position position):
	MyException(error, position)
	{
	}
