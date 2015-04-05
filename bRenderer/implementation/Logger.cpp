#include "../headers/Logger.h"

namespace bRenderer
{
	void log(const std::string &msg, LogMode mode)
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
		
		std::cout << tag << ": " << msg << std::endl;
	}

	void log(const GLubyte *arg, LogMode mode)
	{
		log("", arg, mode);
	}

	void log(const std::string &msg, const GLubyte *arg, LogMode mode)
	{
		const int argumentLength = (sizeof(arg) / sizeof(*arg));		
		std::string message = msg;

		for (int i = 0; i < argumentLength; i++)
		{
			message += arg[i];
		}

		log(message, mode);
	}

} // namespace bRenderer