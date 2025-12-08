#pragma once
#include "xaloc/types.h"
#include <string>
#include <vector>


namespace Xaloc
{

std::vector<float32> ParseVector(
	const std::string& sVector);

uint64 ParseUint64(
	const std::string& sValue);

std::string Uint64ToString(
	uint64 nValue);

}