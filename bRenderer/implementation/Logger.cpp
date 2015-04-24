#include "../headers/Logger.h"

namespace bRenderer
{
	std::string getTag(LogMode mode)
	{
		std::string tag;
		switch (mode)
		{
		case LM_INFO:
			tag = "INFO";
			break;
		case LM_WARNING:
			tag = "WARNING";
			break;
		case LM_ERROR:
			tag = "ERROR";
			break;
		case LM_SYS:
			tag = "SYSTEM";
			break;
		default:
			break;
		}

		return tag;
	}

	void log(const std::string &msg, LogMode mode)
	{
		std::cout << getTag(mode) << ": " << msg << std::endl;
	}

	void log(const GLubyte *arg, LogMode mode)
	{
		log("", arg, mode);
	}

	void log(const std::string &msg, const GLubyte *arg, LogMode mode)
	{	
		std::cout << getTag(mode) << ": " << msg << arg << std::endl;
	}

} // namespace bRenderer