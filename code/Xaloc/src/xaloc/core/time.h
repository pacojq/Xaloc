#pragma once
#include "xaloc/types.h"


namespace Xaloc
{

class ZTimestep
{
public:
	ZTimestep(float32 fTime = 0.0f)
		: m_fTime(fTime)
	{
	}

	//Cast to float to obtain timestep in seconds
	operator float32() const { return m_fTime; }

	float32 GetSeconds() const { return m_fTime; }
	float32 GetMilliseconds() const { return m_fTime * 1000.0f; }

private:
	float32 m_fTime;

};

}