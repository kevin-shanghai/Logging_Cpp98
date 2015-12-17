#ifndef SYNC_LOGGING_STRATEGY_H__
#define SYNC_LOGGING_STRATEGY_H__

#include <stdint.h>
#include "Common/Logger/Log/include/base/SmartPointer.h"
#include "Common/Logger/Log/include/base/Noncopyable.h"

namespace Logger
{
	class LoggingStrategy : private Noncopyable
	{
	public:
		virtual void Append(const char* msg, uint64_t len) = 0;

		virtual void Flush() = 0;


	protected:
		LoggingStrategy(){};
	};
}



#endif