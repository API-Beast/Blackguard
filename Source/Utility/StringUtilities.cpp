#include "StringUtilities.h"

inline bool Contains(std::string input, std::string search)
{
	if (std::string::npos != input.find(search))
		return true;
	return false;
}

inline bool HasFileExtension(std::string input, std::string ext)
{
	if(Contains(input,"."))
	{
		if(input.substr(input.find_last_of(".")) == ext) {
			return true;
		}
	}
	return false;
}

inline bool StringStartsWith(std::string input, std::string search)
{
	if (input.compare(0, search.length(), search) == 0)
		return true;
	else
		return false;
}

inline bool StringEndsWith(std::string input, std::string search)
{
	if (input.length() >= search.length())
		return (0 == input.compare (input.length() - search.length(), search.length(), search));
	else
		return false;
}

inline std::string StringDeleteLastCharacter(std::string input)
{
	return input.substr(0, input.size()-1);
}