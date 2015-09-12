#include "bRenderer.h"
#include "RenderProject.h"

int main(void)
{
	bRenderer::setStandardFilePath("project/data");

	// Create and initialize the RenderProject
	RenderProject *project = new RenderProject();
	project->init();
	delete project;

	std::cout << "press ENTER to quit" << std::endl;
	std::cin.ignore();

	return 0;
}