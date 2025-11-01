#include "GameObject.h"
#include "MemoryStream.h"

void GameObject::Write(OutputMemoryStream& inStream) const
{
	//inStream.Write(mName.c_str());
	inStream.Write(mCount);
}

void GameObject::Read(InputMemoryStream& inStream)
{
	inStream.Read(mCount);
}
