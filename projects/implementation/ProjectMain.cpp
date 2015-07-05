#include "ProjectMain.h"
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

/* Initialize the Project */
void ProjectMain::init()
{
	// set this instance of RenderProject to be used for function calls
	bRenderer().setRenderProject(this);

	// set shader versions (optional)
	bRenderer().setShaderVersionDesktop("#version 120");
	bRenderer().setShaderVersionES("#version 100");

	int w, h;
	bRenderer().getView()->getScreenSize(&w, &h);
	bRenderer::log("Screen width: "+lexical_cast< std::string >(w)+", Screen height: "+lexical_cast< std::string >(h));
    
	// let the renderer create an OpenGL context and the main window
#ifdef OS_DESKTOP
	bRenderer().initRenderer(1920, 1080, false);
	//bRenderer().initRenderer(w, h, true);	// Fullscreen using full width and height of the screen
#endif
#ifdef OS_IOS
    bRenderer().initRenderer(true);
#endif
    // Test second view
    //View v;
    //v.initView(200, 200, false);

	// start main loop 
	bRenderer().runRenderer();
}

/* This function is executed when initializing the renderer */
void ProjectMain::initFunction()
{
	bRenderer::log("my initialize function was started");

	// TEST: load material and shader before loading the model
	ShaderPtr customShader = bRenderer().loadShader("customShader", 4, true, true, true, true, true, true);		// create custom shader with a maximum of 4 lights
	ShaderPtr flameShader = bRenderer().loadShaderFile("flame", 0, false);										// load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
	MaterialPtr flameMaterial = bRenderer().loadMaterial("flame.mtl", "flame", flameShader);					// load material from file using the shader created above

	PropertiesPtr flameProperties = bRenderer().createProperties("flameProperties");	// TODO: add additional properties to a model

	// load models
	bRenderer().loadModel("cave_start.obj", true, true, false, 4);						// create custom shader with a maximum of 4 lights (since nothing else was specified, number of lights may vary between 0 and 4 during rendering without performance loss)
	bRenderer().loadModel("sphere.obj", true, true, false, 4);							// create custom shader with a maximum of 4 lights 
	bRenderer().loadModel("crystal.obj", false, true, customShader);					// the custom shader created above is used
	bRenderer().loadModel("torch.obj", false, true, false, 4);							// create custom shader with a maximum of 4 lights 
	bRenderer().loadModel("flame.obj", false, true, flameMaterial, flameProperties);	// the flame material created above is used, to pass additional properties a Properties object is used
	bRenderer().loadModel("sparks.obj", false, true, true, 0, false);					// automatically loads shader files "sparks.vert" and "sparks.frag", we specify 0 lights since the shader doesn't consider lights
	bRenderer().loadModel("bTitle.obj", false, true, false, 0, false);					// create custom shader with 0 lights -> the title will always be fully lit

	// initialize variables
	_randomTime = 0.0f;
	_running = true; _lastStateSpaceKey = -1;

	// initialize free moving camera
	_cameraForward = 0.0f;
	bRenderer().createCamera("camera", vmml::vec3f(-33.0, 0.0, -13.0), vmml::vec3f(0.0, -M_PI_F/2, 0.0));

	// get shading language version
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// create lights
	bRenderer().createLight("firstLight", vmml::vec3f(80.0f, 0.0f, 0.0f), vmml::vec3f(0.5f, 0.5f, 1.0f), 100.0f, 0.3f);
	bRenderer().createLight("secondLight", vmml::vec3f(150.0f, 0.0f, 0.0f), vmml::vec3f(0.5f, 1.0f, 0.0f), 100.0f, 0.5f);
	bRenderer().createLight("thirdLight", vmml::vec3f(210.0f, 0.0f, 0.0f), vmml::vec3f(0.8f, 0.0f, 0.0f), 100.0f, 0.5f);
	bRenderer().createLight("torchLight", -bRenderer().getCamera("camera")->getPosition(), vmml::vec3f(1.0f, 0.4f, -0.5f), 1200.0f, 0.7f);

	// set ambient color
	bRenderer().setAmbientColor(vmml::vec3f(0.0f, 0.0f, 0.05f));

	/* Windows only: Mouse Movement */
#ifdef OS_DESKTOP
	glfwSetInputMode(bRenderer().getView()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(bRenderer().getView()->getWindow(), &_mouseX, &_mouseY);
#endif
}

/* Draw your scene here */
void ProjectMain::loopFunction(const double &deltaTime, const double &elapsedTime)
{
	//	bRenderer::log("deltaTime: "+lexical_cast< std::string >(deltaTime)+", elapsedTime: "+lexical_cast< std::string >(elapsedTime));
	//bRenderer::log("FPS: "+lexical_cast< std::string >(1/deltaTime));

	//// Movement ////
	
		/* Windows only: Mouse and Keyboard Movement */
#ifdef OS_DESKTOP
	int currentStateSpaceKey = glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_SPACE);
	if (currentStateSpaceKey != _lastStateSpaceKey)
	{
		_lastStateSpaceKey = currentStateSpaceKey;
		if (currentStateSpaceKey == GLFW_PRESS)
			_running = !_running;
	}

	double xpos, ypos;
	glfwGetCursorPos(bRenderer().getView()->getWindow(), &xpos, &ypos);
	double deltaCameraY = xpos - _mouseX;
	_mouseX = xpos;
	double deltaCameraX = ypos - _mouseY;
	_mouseY = ypos;
	
	if (_running){
		if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
			if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 			_cameraForward = 1.0;
			else			_cameraForward = 0.5;
		else if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
			if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 			_cameraForward = -1.0;
			else			_cameraForward = -0.5;
		else
			_cameraForward = 0.0;

		if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
			bRenderer().getCamera("camera")->moveCameraSideward(-0.5);
		else if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
			bRenderer().getCamera("camera")->moveCameraSideward(0.5);
		if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
			bRenderer().getCamera("camera")->moveCameraUpward(0.5);
		else if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
			bRenderer().getCamera("camera")->moveCameraUpward(-0.5);
		if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
			bRenderer().getCamera("camera")->rotateCamera(0.0f, 0.0f, 0.03f);
		else if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
			bRenderer().getCamera("camera")->rotateCamera(0.0f, 0.0f, -0.03f);
	}
#endif
		/* On iOS automatic movement (for now) */
#ifdef OS_IOS
	double deltaCameraY = -0.1 / deltaTime;
	double deltaCameraX = 0.0;
	_cameraForward = 0.001 / deltaTime;
#endif

	//// Camera ////
	if (_running){
		bRenderer().getCamera("camera")->moveCameraForward(_cameraForward);
		bRenderer().getCamera("camera")->rotateCamera(deltaCameraX / 1000, deltaCameraY / 1000, 0.0f);
	}
	//// Perspective ////
	// adjust aspect ratio
	bRenderer().getCamera("camera")->setAspectRatio(bRenderer().getView()->getAspectRatio());

	//// Torch Light ////
	if (_running){
		if (deltaTime < 0.5f){
			_randomTime += deltaTime + randomNumber(0.0f, 0.12f);
		}
		float flickeringLight = 1.0f + (_randomTime)* 2.0f * M_PI_F*(0.032f);
		float flickeringLightPosX = -bRenderer().getCamera("camera")->getPosition().x();
		float flickeringLightPosY = -bRenderer().getCamera("camera")->getPosition().y();
		float flickeringLightPosZ = -bRenderer().getCamera("camera")->getPosition().z();
		flickeringLightPosX += 2.5f*sin(flickeringLightPosY * 4.0f + 3.0f*flickeringLight);
		flickeringLightPosY += 2.5f*sin(flickeringLightPosX * 4.0f + 3.0f*flickeringLight);
		bRenderer().getLight("torchLight")->setPosition(vmml::vec3f(flickeringLightPosX, flickeringLightPosY, flickeringLightPosZ) - bRenderer().getCamera("camera")->getForward()*10.0f);
	}

	//// Draw Models ////

	/*** Cave Start ***/
	// translate and scale 
	vmml::mat4f modelMatrix = vmml::create_scaling(vmml::vec3f(0.3f)) * vmml::create_translation(vmml::vec3f(100.0, -80.0, 0.0));
	// draw
	bRenderer().drawModel("cave_start", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }));

	/*** Sphere ***/
	// translate and scale
	modelMatrix = vmml::create_scaling(vmml::vec3f(0.1f)) * vmml::create_translation(vmml::vec3f(1480.0, 50.0, 300.0));
	// draw 
	bRenderer().drawModel("sphere", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }));

	/*** Crystal (blue) ***/
	// translate and scale
	modelMatrix = vmml::create_scaling(vmml::vec3f(0.1f)) * vmml::create_translation(vmml::vec3f(780.0, -170.0, 55.0));
	// draw
	bRenderer().setAmbientColor(vmml::vec3f(0.2f, 0.2f, 0.8f));
	bRenderer().drawModel("crystal", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }));
	bRenderer().setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR);

	/*** Crystal (green) ***/
	// translate and scale 
	modelMatrix = vmml::create_scaling(vmml::vec3f(0.1f)) * vmml::create_translation(vmml::vec3f(1480.0f, -170.0f, 70.0f));
	// draw
	bRenderer().setAmbientColor(vmml::vec3f(0.1f, 0.45f, 0.1f));
	bRenderer().drawModel("crystal", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }));
	bRenderer().setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR);

	/*** Crystal (red) ***/
	// translate and scale 
	modelMatrix = vmml::create_scaling(vmml::vec3f(0.1f)) * vmml::create_translation(vmml::vec3f(2180.0f, -170.0f, 40.0f));
	// draw
	bRenderer().setAmbientColor(vmml::vec3f(0.6f, 0.1f, 0.1f));
	bRenderer().drawModel("crystal", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }));
	bRenderer().setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR);

	///*** Torch ***/
	// translate and scale 	
		// Position the torch relative to the camera
		modelMatrix = bRenderer().getCamera("camera")->getInverseViewMatrix();
	modelMatrix *= vmml::create_translation(vmml::vec3f(0.75f, -1.1f, 0.8f)) * vmml::create_scaling(vmml::vec3f(1.2f)) * vmml::create_rotation(1.64f, vmml::vec3f::UNIT_Y);
	// draw
	bRenderer().drawModel("torch", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }));
    
    /*** Flame ***/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	for (float z = 0.0f; z < 3.0f; z++) 
	{
		if (z == 1.0f)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		// translate
		vmml::mat4f translation = vmml::create_translation(vmml::vec3f(0.65f / bRenderer().getView()->getAspectRatio(), 0.6f + (0.08f*z), (-z / 100.0f - 0.50f)));
		// rotate
		float rot = 0.0f;
		if (fmod(z, 2.0f) == 0.0f)
			rot = 0.0f;
		else
			rot = M_PI_F;
		vmml::mat4f rotation = vmml::create_rotation(rot, vmml::vec3f::UNIT_Z);
		// scale
		float ParticleScale = 2.45f - (0.46f*z);
		vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(ParticleScale / bRenderer().getView()->getAspectRatio(), ParticleScale, ParticleScale));
		// model matrix
		modelMatrix = translation * scaling * rotation;
		// view matrix
		vmml::vec3f eyePos(0.0f, 0.0f, 0.25f);
		vmml::mat4f viewMatrix = bRenderer().lookAt(eyePos, vmml::vec3f::ZERO, vmml::vec3f::UP);
		// wave effect
		float offset = (_randomTime + 0.3f*z) * 2 * M_PI_F*(0.75f + 0.5f*z);
		float transparency = 1.0f;
		if (z == 0.0f)transparency = 0.8f;
		// pass additional properties to the shader
		bRenderer().getProperties("flameProperties")->setScalar("offset", offset);
		bRenderer().getProperties("flameProperties")->setScalar("transparency", transparency);
		// draw
		bRenderer().drawModel("flame", modelMatrix, viewMatrix, vmml::mat4f::IDENTITY, std::vector<std::string>({}));
	}    
    
    /*** Sparks ***/
	for (float z = 1.0f; z < 2.0f; z++)
	{
		// translate
		vmml::mat4f translation = vmml::create_translation(vmml::vec3f(0.65f / bRenderer().getView()->getAspectRatio(), 0.65f, (-z / 100.0f - 0.58f)));
		// rotate
		float rot = 1.0f;
		if (_running)
			rot = randomNumber(1.0f, 1.1f)*_randomTime*(z + 0.3f)*M_PI_F;
		vmml::mat4f rotation = vmml::create_rotation(rot, vmml::vec3f::UNIT_Z);
		// scale
		float ParticleScale = 1.1f - (0.5f*z);
		vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(ParticleScale / bRenderer().getView()->getAspectRatio(), 4.0f*ParticleScale, ParticleScale));
		// model matrix
		modelMatrix = translation * scaling * rotation;
		// view matrix
		vmml::vec3f eyePos(0.0f, 0.0f, 0.25f);
		vmml::mat4f viewMatrix = bRenderer().lookAt(eyePos, vmml::vec3f::ZERO, vmml::vec3f::UP);

		// draw
		bRenderer().drawModel("sparks", modelMatrix, viewMatrix, vmml::mat4f::IDENTITY, std::vector<std::string>({}));
	}
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*** Title ***/
	// translate and scale 
	float titleScale = 0.4f;
	vmml::mat4f translation = vmml::create_translation(vmml::vec3f(-0.4f, 0.0f, -0.65f));	
	vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
	modelMatrix = translation * scaling;
	// create view matrix
	vmml::vec3f eyePos(0.0f, 0.0f, 0.25f);
	vmml::vec3f eyeUp = vmml::vec3f::UP;
	vmml::mat4f viewMatrix = bRenderer().lookAt(eyePos, vmml::vec3f::ZERO, eyeUp);
	// draw
	bRenderer().drawModel("bTitle", modelMatrix, viewMatrix, vmml::mat4f::IDENTITY, std::vector<std::string>({}));

}

/* This function is executed when terminating the renderer */
void ProjectMain::terminateFunction()
{
	bRenderer::log("I totally terminated this Renderer :-)");
}

/* For iOS only: Handle device rotation */
void ProjectMain::deviceRotated()
{
	if (bRenderer().isInitialized()){
		// set view to fullscreen after device rotation
		bRenderer().getView()->setFullscreen(true);
		bRenderer::log("Device rotated");
	}
}

/* For iOS only: Handle app going into background */
void ProjectMain::appWillResignActive()
{
	if (bRenderer().isInitialized()){
		// stop the renderer when the app isn't active
		bRenderer().stopRenderer();
	}
}

/* For iOS only: Handle app coming back from background */
void ProjectMain::appDidBecomeActive()
{
	if (bRenderer().isInitialized()){
		// run the renderer as soon as the app is active
		bRenderer().runRenderer();
	}
}

/* For iOS only: Handle app being terminated */
void ProjectMain::appWillTerminate()
{
	if (bRenderer().isInitialized()){
		// terminate renderer before the app is closed
		bRenderer().terminateRenderer();
	}
}

/* Helper functions */
float ProjectMain::randomNumber(float min, float max){
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}