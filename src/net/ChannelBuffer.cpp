#include "Common/Logger/Log/include/net/ChannelBuffer.h"
#include "Common/Logger/Log/include/net/SocketApiWrapper.h"


namespace Net
{
    static const char kEOL = '\n';

    namespace details
    {
        const void* memrchar(const char* sourceBuf, int findChar, uint64_t iFindNum)
        {
#ifdef ON_LINUX
            return ::memrchr(sourceBuf, findChar, iFindNum);       
#else 
           ;
            while(iFindNum >= 0 && *(sourceBuf + iFindNum -1) != kEOL)
            {
                if (iFindNum-- == 0)
                {
                    return NULL;
                }
            }
            return sourceBuf + iFindNum; 
#endif
        }
    }

    void ChannelBuffer::AssureEnoughSpace(uint64_t len)
    {
        if (len > (WriteableBytes() + PrependbaleBytes() - kBufferCheapPrepend))
        {
            buffer_.resize(writeIndex_ + len);
        }
        else
        {
            //do not need to allocate new memory spaces, just move the data forward
            uint64_t iReadbleBytes = ReadableBytes();
            std::copy(Begin() + readIndex_, Begin() + writeIndex_, Begin() + kBufferCheapPrepend);
            readIndex_ = kBufferCheapPrepend;
            writeIndex_ = readIndex_ + iReadbleBytes;
        }
    }

    void ChannelBuffer::Retrieve(uint64_t len)
    {
        assert(len <= ReadableBytes());
        if (len < ReadableBytes())
        {
            readIndex_ += len;
        }
        else
        {
            writeIndex_ = readIndex_ = kBufferCheapPrepend;
        }
    }

    std::string ChannelBuffer::ReadBytesAsString(uint64_t len)
    {
        assert(len <= ReadableBytes());
        std::string result(GetReadPointer(), len);
        Retrieve(len);
        return result;
    }

    const char* ChannelBuffer::FindLastEndOfLine()
    {
        return static_cast<const char*>
            ( details::memrchar(GetReadPointer(), kEOL, ReadableBytes()) );
    }

    int64_t ChannelBuffer::ReadFromSocketFD(uint32_t sockFd)
    {
        uint32_t iWriteble = WriteableBytes();
        char buffer[1000] = {'\0'};
        uint64_t iReadBytes = SocketsApi::Receive(sockFd, buffer, 1000);
        Append(buffer, iReadBytes);
        return iReadBytes;
    }




}
