#ifndef LOGSTREAM_H__
#define LOGSTREAM_H__
#include "Common/Logger/Log/include/Buffer.h"
#include "Common/Logger/Log/include/base/Noncopyable.h"
#include "Common/Logger/Log/include/base/StringUtil.h"
#include <string>	
#include <sstream>
#include "Common/Logger/Log/include/PlatformDefine.h"

namespace Logger
{
    class EXPORTLOG LogStream : private Noncopyable
    {
    public:
		typedef Buffer<kSmallBuffer> LogBuffer;

        LogStream& operator << (bool b);

        LogStream& operator << (char v);


        LogStream& operator << (const char* str);

        LogStream& operator << (const unsigned char* str);

        LogStream& operator << (const std::string& v);

        template <class T>
		LogStream& operator << (T value)
		{
			//static_assert(std::is_arithmetic<T>::value, "Arithmetic required.");
			FormatInteger(value);
			return *this;
		}

        template <class T>
		LogStream& operator << (T* ptr)
		{
			if (NULL != ptr)
			{
				char ptrStr[10] = { '\0' };
				sprintf(ptrStr, "0x%p", ptr);
				logBuffer_.Append(ptrStr, 10);
			}
			else
			{
				char ptrStr[7] = { '\0' };
				strncpy(ptrStr, "nullptr", 7);
				logBuffer_.Append(ptrStr, 7);
			}
			return *this;
		}

	 void Append(const char* data, int len) { logBuffer_.Append(data, len); }

	 const LogBuffer& GetLogBuffer() const { return logBuffer_; }


    private:
        template <class T>
		void FormatInteger(T value)
		{
			if (logBuffer_.AvailableSize() >= 32)
			{
				//logBuffer_.Append(std::to_string(value).c_str(), std::to_string(value).size());
                std::stringstream sstream;
                sstream << value;
                logBuffer_.Append(sstream.str());
			}
		}

    private:
        LogBuffer logBuffer_;
    };
}


#endif
