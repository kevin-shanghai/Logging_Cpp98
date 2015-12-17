#ifndef NONCOPYABLE_H__
#define NONCOPYABLE_H__
#include "Common/Logger/Log/include/PlatformDefine.h"

class EXPORTLOG Noncopyable
{
protected:
	Noncopyable(){}
private:
	Noncopyable(const Noncopyable&);
	Noncopyable& operator = (const Noncopyable&);
};


#endif