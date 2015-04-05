#pragma once

#include "bRenderer_GL.h"
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

	/**	@brief Write a string to the console
	 *	@param[in] msg The message to be displayed
	 *	@param[in] mode Defines the mode of the logger (adds a tag to the string)
	 */
	void log(const std::string &msg, LogMode mode = LM_INFO);

	/**	@brief Write a GLEW error string to the console
	 *	@param[in] arg The error to be displayed
	 *	@param[in] mode Defines the mode of the logger (adds a tag to the string)
	 */
	void log(const GLubyte *arg, LogMode mode = LM_INFO);

	/**	@brief Write a message and a GLEW error string to the console
	 *	@param[in] msg The message to be displayed
	 *	@param[in] arg The error to be displayed
	 *	@param[in] mode Defines the mode of the logger (adds a tag to the string)
	 */
	void log(const std::string &msg, const GLubyte *arg, LogMode mode = LM_INFO);

} // namespace bRenderer

