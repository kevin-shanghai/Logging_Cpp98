#include <algorithm>
#include "Common/Logger/Log/include/base/StringUtil.h"


namespace Logger
{
	namespace Util
	{
		static const char digits[] = "9876543210123456789";
		static const char* zero = digits + 9;
		static const char digitsHex[] = "0123456789ABCDEF";

		// Efficient Integer to String Conversions, by Matthew Wilson.
		template<typename T>
		size_t ConvertDigitToString(char buf[], T value)
		{
			T i = value;
			char* p = buf;

			do
			{
				int lsd = static_cast<int>(i % 10);
				i /= 10;
				*p++ = zero[lsd];
			} while (i != 0);

			if (value < 0)
			{
				*p++ = '-';
			}
			*p = '\0';
			std::reverse(buf, p);

			return p - buf;
		}

		size_t ConvertToHexString(char buf[], uintptr_t value)
		{
			uintptr_t i = value;
			char* p = buf;

			do
			{
				int lsd = i % 16;
				i /= 16;
				*p++ = digitsHex[lsd];
			} while (i != 0);

			*p = '\0';
			std::reverse(buf, p);
			return p - buf;
		}
	}
	
}

