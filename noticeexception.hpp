#ifndef NOTICEEXCEPTION
#define NOTICEEXCEPTION

#include "myexception.hpp"
#include <string>

class NoticeException : public MyException {
public:
	NoticeException (std::string error);
};

#endif