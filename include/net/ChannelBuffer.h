#ifndef CHANNEL_BUFFER_H__
#define CHANNEL_BUFFER_H__

#include <stdint.h>
#include <vector>
#include <assert.h>
#include <string.h>

namespace Net
{
    const uint8_t kBufferCheapPrepend = 8;
    const uint64_t kBufferInitialSize = 1024;
	class ChannelBuffer
	{
	public:
		ChannelBuffer() 
            : buffer_(kBufferCheapPrepend + kBufferInitialSize)
			, writeIndex_(kBufferCheapPrepend)
			, readIndex_(kBufferCheapPrepend)
		{

		}
		const char* GetReadPointer() { return Begin() + readIndex_; }

        char* GetWriteablePointer() { return Begin() + writeIndex_; }

		uint64_t ReadableBytes(){ return writeIndex_ - readIndex_; }

		uint64_t WriteableBytes() { return Size() - writeIndex_; }

		uint64_t PrependbaleBytes(){ return readIndex_; }
		
		void Append(const std::string& buf){ Append(buf.c_str(), buf.size()); }

		std::string ReadAllAsString() { return ReadBytesAsString(ReadableBytes()); }

		std::string ReadBytesAsString(uint64_t len);

        std::string ReadContentsUntilLastEOF()
        {
            return ReadBytesAsString(FindLastEndOfLine() - GetReadPointer());
        }

        const char* FindLastEndOfLine();

        int64_t ReadFromSocketFD(uint32_t sockFd);
		

		template <class T>
		void Append(T* dataPtr, uint64_t len)
		{
			if (len > WriteableBytes())
			{
				AssureEnoughSpace(len);
			}
			assert(WriteableBytes() >= len);
			const char* tempPtr = static_cast<const char*>(dataPtr);
			std::copy(tempPtr, tempPtr + len, Begin() + writeIndex_);
            writeIndex_ += len;
		}


	private:
		char* Begin() { return &*buffer_.begin(); }
		char* End() { return &*buffer_.end(); }
		uint64_t Size() { return buffer_.size(); }
		void AssureEnoughSpace(uint64_t len);
		void Retrieve(uint64_t len);


	private:
		std::vector<char> buffer_;
		uint64_t writeIndex_;
		uint64_t readIndex_;
	};
}

#endif