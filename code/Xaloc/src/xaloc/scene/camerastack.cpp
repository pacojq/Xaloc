#include "xapch.h"
#include "camerastack.h"

namespace Xaloc
{

// TODO: camera stack dunctionality

const SSceneCamera& ZCameraStack::ReadCamera(int32 nCamera) const
{
	return m_MainCamera;
}

SSceneCamera& ZCameraStack::WriteCamera(int32 nCamera)
{
	return m_MainCamera;
}

}