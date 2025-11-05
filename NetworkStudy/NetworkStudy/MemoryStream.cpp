#include "MemoryStream.h"

OutputMemoryStream::OutputMemoryStream()
	: mBuffer(nullptr)
	, mHead(0)
	, mCapacity(0)
{
	ReallocBuffer(32);
}

OutputMemoryStream::~OutputMemoryStream()
{
	Free();
}

void OutputMemoryStream::Write(const void* inData, uint64 inByteCount)
{
	uint32 resultHead = mHead + (uint32)inByteCount;
	if (resultHead > mCapacity)
		ReallocBuffer((resultHead) * 1.5f);
	memcpy(mBuffer + mHead, inData, inByteCount);
	mHead = resultHead;
}

void OutputMemoryStream::ReallocBuffer(uint32 inNewLength)
{
	//char* newBuffer = new char[](inNewLength);
	char* newBuffer = static_cast<char*>(std::realloc(mBuffer, inNewLength));
	if (mBuffer)
	{
		memcpy(newBuffer, mBuffer, mCapacity);
		Free();
	}
	mBuffer = newBuffer;
	mCapacity = inNewLength;
}

void OutputMemoryStream::Free()
{
	if (mBuffer)
	{
		//delete[] mBuffer;
		std::free(mBuffer);
		mBuffer = nullptr;
	}
}

//---------------InputMemoryStream---------------//

InputMemoryStream::InputMemoryStream(char* inBuffer, uint32 inByteCount)
	: mBuffer(inBuffer), mCapacity(inByteCount), mHead(0)
{
}

InputMemoryStream::~InputMemoryStream()
{
	//if (mBuffer)
	//{
	//	delete[] mBuffer;
	//	mBuffer = nullptr;
	//}
}

void InputMemoryStream::Read(void* outData, uint32 inByteCount)
{
	memcpy(outData, mBuffer + mHead, inByteCount);
	mHead += inByteCount;
}
