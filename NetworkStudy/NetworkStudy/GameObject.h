#pragma once
#include <iostream>
#include <string>

class OutputMemoryStream;
class InputMemoryStream;

class GameObject
{
public:
	GameObject() : mName(), mCount(0) {}
	GameObject(std::string inName) : mName(inName), mCount(0) {}

	std::string GetName() { return mName; }

	void AddCount() 
	{
		std::cout << mName << ": AddCount(), Result : " << mCount << '\n';
		mCount++; 
	}
	void SayName() { std::cout << mName << '\n'; }
	void SayCount() { std::cout << mCount << '\n'; }

	void SayInfo() 
	{
		std::cout << "Name : " << mName << ", Count : " << mCount << '\n';
	}

	void Write(OutputMemoryStream& inStream) const;
	void Read(InputMemoryStream& inStream);

private:
	std::string mName;
	int mCount;
};

