#include "xapch.h"
#include "xaloc/core/assets/util/parsing.h"
#include "xaloc/debugging/assert.h"


namespace Xaloc
{

std::vector<float32> ParseVector(const std::string& sVector)
{
	// TODO: find a less crappy way of parsing this

	std::vector<float> output;
	std::string sVectorTemp = std::string(sVector);
	char separator = ',';

	sVectorTemp.erase(0, 1); // Remove first "["
	sVectorTemp.erase(sVector.length() - 1, 1); // Remove last "]"

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = sVectorTemp.find(separator, pos)) != std::string::npos)
	{
		std::string substring(sVectorTemp.substr(prev_pos, pos - prev_pos));

		output.push_back(std::stof(substring));

		prev_pos = ++pos;
	}

	output.push_back(std::stof(sVectorTemp.substr(prev_pos, pos - prev_pos))); // Last word

	return output;
}

uint64 ParseUint64(const std::string& sValue)
{
	errno = 0;
	unsigned long long result = strtoull(sValue.c_str(), NULL, 16);

	if (errno == EINVAL)
	{
		XA_CORE_ASSERT(false, "Not a valid number!");
		return 0;
	}
	else if (errno == ERANGE)
	{
		XA_CORE_ASSERT(false, "Does not fit in an unsigned long long!");
		return 0;
	}

	return result;
}

std::string Uint64ToString(uint64 nValue)
{
	char sBuff[20 + 1]; // number of chars needed to represent uint64 max value
	snprintf(sBuff, sizeof(sBuff), "%llx", nValue);
	std::string sResult = sBuff;
	return sResult;
}

}