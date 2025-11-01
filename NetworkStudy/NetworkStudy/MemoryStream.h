#pragma once
#include "Type.h"
#include <memory>

class OutputMemoryStream
{
public:
	OutputMemoryStream();
	~OutputMemoryStream();

	const char* GetBufferPtr() const { return mBuffer; }
	uint32 GetLength() const { return mHead; }

	void Write(const void* inData, uint64 inByteCount);
	void Write(uint32 inData) { Write(&inData, sizeof(inData)); }
	void Write(int32 inData) { Write(&inData, sizeof(inData)); }
	template<typename T> 
	void Write(const T& inData)
	{
		// 원시 자료형 여부 컴파일 검사
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value
			, "Generic Write only supports primitive data types!");

		Write(inData, sizeof(inData));
	}

private:
	void ReallocBuffer(uint32 inNewLength);
	void Free();

	char* mBuffer;
	uint32 mHead;
	uint32 mCapacity;
};

