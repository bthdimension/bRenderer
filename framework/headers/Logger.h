#pragma once

#include "Framework_GL.h"
#include <iostream>
#include <string>

namespace bRenderer
{
	enum LogMode
	{
		LM_INFO,
		LM_WARNING,
		LM_ERROR,
		LM_SYS
	};

	void log(const std::string &msg, LogMode mode = LM_INFO);

	void log(const GLubyte *arg, LogMode mode = LM_INFO);

	void log(const std::string &msg, const GLubyte *arg, LogMode mode = LM_INFO);

};

