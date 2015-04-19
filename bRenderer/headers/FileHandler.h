#pragma once

#include "OSdetect.h"
#include "bRenderer_GL.h"
#include <string>

namespace bRenderer
{

#ifdef OS_DESKTOP
	/* On desktop systems the standard lookup path for files can be specified  */

	// TODO: Add a search system that makes this function obsolete

	/**	@brief Specify the standard path to user files
	*	@param[in] path
	*/
	void setStandardFilePath(const std::string &path);

#endif

	/**	@brief Get the full path to a file
	*	@param[in] fileName
	*/
	std::string getFilePath(const std::string &fileName);

	/**	@brief Check if a file exists
	 *	@param[in] fileName
	 */
	bool fileExists(const std::string &fileName);

} // namespace bRenderer

