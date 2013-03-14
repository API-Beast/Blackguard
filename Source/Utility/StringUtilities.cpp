#include "StringUtilities.h"

namespace Blackguard
{
	namespace Utility
	{
		bool Contains(const std::string& input, const std::string& search)
		{
			if (std::string::npos != input.find(search))
				return true;
			return false;
		}

		bool HasFileExtension(const std::string& input, const std::string& ext)
		{
			if(Contains(input,"."))
			{
				if(input.substr(input.find_last_of(".")) == ext) {
					return true;
				}
			}
			return false;
		}

		bool StringStartsWith(const std::string& input, const std::string& search)
		{
			if (input.compare(0, search.length(), search) == 0)
				return true;
			else
				return false;
		}

		bool StringEndsWith(const std::string& input, const std::string& search)
		{
			if (input.length() >= search.length())
				return (0 == input.compare (input.length() - search.length(), search.length(), search));
			else
				return false;
		}

		std::string StringDeleteLastCharacter(const std::string& input)
		{
			return input.substr(0, input.size()-1);
		}
	}
}