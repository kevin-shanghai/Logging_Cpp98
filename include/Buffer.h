#ifndef BUFFER_H__
#define BUFFER_H__
#include <stdint.h>
#include <string.h>
#include <string>

namespace Logger
{
	const int64_t kSmallBuffer = 400;
	const int64_t kLargeBuffer = 4000;
	template <uint64_t BUFFER_SIZE = 1000>
	class Buffer 
	{
	public:
		Buffer() :cur_(buffer_)
		{
			::memset(buffer_, 0, BUFFER_SIZE);
		}

		~Buffer()
		{
		}

		Buffer(const Buffer& other) : cur_(buffer_) 
		{
			::memcpy(this->buffer_, other.buffer_, static_cast<size_t>(other.Length()));
			this->cur_ += other.Length();
		}

		Buffer& operator =(const Buffer& other)
		{
			::memcpy(this->buffer_, other.buffer_, other.Length());
			cur_ += other.Length();
			return *this;
		}

		bool Append(const char* msg, int64_t msg_len)
		{
			bool ret = (memcpy(cur_, msg, static_cast<size_t>(msg_len)) != NULL);
			cur_ += msg_len;
			return ret;
		}

        bool Append(const std::string& msg)
        {
            return Append(msg.c_str(), msg.size());
        }

		uint64_t Length() const
		{
			return cur_ - buffer_;
		}

		uint64_t AvailableSize()
		{
			return  End() - cur_;
		}

	    const char* GetBufferData() const
		{
			return buffer_;
		}

		 void Clear()
		{
			cur_ = buffer_;
			memset(buffer_, 0, BUFFER_SIZE);
		}

		 bool Empty() const
		 {
			 return cur_ == buffer_;
		 }

		 char* Current() const  { return cur_; }

	private:
		const char* End()
		{
			return buffer_ + BUFFER_SIZE;
		}

	private:
		char buffer_[BUFFER_SIZE];
		char* cur_;
	};
}



#endif
