#pragma once


namespace Xaloc
{

struct CmpTag
{
	std::string sTag;

	operator std::string& () { return sTag; }
	operator const std::string& () const { return sTag; }
};

}