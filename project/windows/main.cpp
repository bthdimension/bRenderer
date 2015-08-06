
#include "bRenderer.h"
#include "ProjectMain.h"

int main(void)
{
	bRenderer::setStandardFilePath("../data");

	ProjectMain project;

	project.init();

	std::cout << "press ENTER to quit" << std::endl;
	std::cin.ignore();

	return 0;
}