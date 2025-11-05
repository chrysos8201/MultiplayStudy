#include "GameObject.h"
#include "MemoryStream.h"

void Player::Write(OutputMemoryStream& inStream) const
{
	inStream.Write(mY);
	inStream.Write(mX);
}
