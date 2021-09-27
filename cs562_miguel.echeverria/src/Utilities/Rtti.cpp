#include "pch.h"
#include "Rtti.h"


namespace cs460
{
	// Don't use this function. Meant for internal use in getTypeName overloads.
	std::string extractClassOrStruct(const char* name)
	{
		// remove class
		std::string out = name;
		std::size_t c = out.find("class ");
		while (c != std::string::npos)
		{
			auto s = out.begin() + c;
			auto e = s + strlen("class ");
			out.replace(s, e, "");
			c = out.find("class ");
		}
		// remove struct
		c = out.find("struct ");
		while (c != std::string::npos)
		{
			auto s = out.begin() + c;
			auto e = s + strlen("struct ");
			out.replace(s, e, "");
			c = out.find("struct ");
		}
		return out;
	}
}