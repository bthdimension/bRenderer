#include "ProjectMain.h"

/* Initialize the Project */
void ProjectMain::init()
{
	bRenderer::loadConfigFile("config.json");
	// let the renderer create an OpenGL context and the main window
	if(Input::isTouchDevice())
		bRenderer().initRenderer(true);										// fullscreen on iOS
	else
		bRenderer().initRenderer(1920, 1080, false, "The Cave - Demo");		// windowed mode on desktop
		//bRenderer().initRenderer(View::getScreenWidth(), View::getScreenHeight(), true);		// fullscreen using full width and height of the screen

	// start main loop 
	bRenderer().runRenderer();
}

/* This function is executed when initializing the renderer */
void ProjectMain::initFunction()
{
	// initialize variables
	_offset = 0.0f;
	_randomOffset = 0.0f;
	_cameraSpeed = 40.0f;
	_running = false; _lastStateSpaceKey = bRenderer::INPUT_UNDEFINED;
	_viewMatrixHUD = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);

	// set shader versions (optional)
	bRenderer().getObjects()->setShaderVersionDesktop("#version 120");
	bRenderer().getObjects()->setShaderVersionES("#version 100");

	// demo: load material and shader before loading the model
	ShaderPtr customShader = bRenderer().getObjects()->generateShader("customShader", { 2, true, true, true, true, true, true, true, true, true, false, false, false });	// create custom shader with a maximum of 2 lights
	ShaderPtr flameShader = bRenderer().getObjects()->loadShaderFile("flame", 0, false, true, true, false, false);				// load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
	MaterialPtr flameMaterial = bRenderer().getObjects()->loadObjMaterial("flame.mtl", "flame", flameShader);				// load material from file using the shader created above

	// create additional properties for a model
	PropertiesPtr flameProperties = bRenderer().getObjects()->createProperties("flameProperties");
	PropertiesPtr streamProperties = bRenderer().getObjects()->createProperties("streamProperties");

	// load models
	bRenderer().getObjects()->loadObjModel("cave.obj", true, true, false, 4, true, false);								// create custom shader with a maximum of 4 lights (number of lights may vary between 0 and 4 during rendering without performance loss)
	bRenderer().getObjects()->loadObjModel("cave_stream.obj", true, true, true, 4, false, false, streamProperties);		// automatically loads shader files according to the name of the material
	bRenderer().getObjects()->loadObjModel("crystal.obj", false, true, customShader);										// the custom shader created above is used
	bRenderer().getObjects()->loadObjModel("torch.obj", false, true, false, 1, false, true);								// create custom shader with a maximum of 1 light

	// create sprites
	bRenderer().getObjects()->createSprite("flame", flameMaterial, false, flameProperties);				// create a sprite using the material created above, to pass additional properties a Properties object is used
	bRenderer().getObjects()->createSprite("sparks", "sparks.png");										// create a sprite displaying sparks as a texture
	bRenderer().getObjects()->createSprite("bTitle", "basicTitle_light.png");								// create a sprite displaying the title as a texture

	// create text sprites
	FontPtr font = bRenderer().getObjects()->loadFont("KozGoPro-ExtraLight.otf", 50);
	if (Input::isTouchDevice())
		bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.f, 1.f), "Double tap to start", font);
	else
		bRenderer().getObjects()->createTextSprite("instructions", vmml::Vector3f(1.f, 1.f, 1.f), "Press space to start", font);

	//////////////////////////////////TEXTTEST
	FontPtr fontTest = bRenderer().getObjects()->loadFont("arial.ttf", 128);
	ShaderPtr testi = bRenderer().getObjects()->generateShader("testi", { 2, true, true, true, true, true, true, true, true, true, false, false, true });
	MaterialPtr testMaterial = bRenderer().getObjects()->loadObjMaterial("testi.mtl", "testi", testi);
	TextSpritePtr testSprite = bRenderer().getObjects()->createTextSprite("test_text", testMaterial, "Benjamin's Test", fontTest);
	/////////////////////////////////

	// create camera
	bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(-33.0, 0.f, -13.0), vmml::Vector3f(0.f, -M_PI_F / 2, 0.f));

	// create lights
	bRenderer().getObjects()->createLight("firstLight", vmml::Vector3f(78.0f, -3.0f, 0.0f), vmml::Vector3f(0.5f, 0.5f, 1.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.4f, 100.0f);
	bRenderer().getObjects()->createLight("secondLight", vmml::Vector3f(148.0f, -3.0f, 15.0f), vmml::Vector3f(0.3f, 1.0f, 0.3f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.8f, 100.0f);
	bRenderer().getObjects()->createLight("thirdLight", vmml::Vector3f(218.0f, -3.0f, 0.0f), vmml::Vector3f(0.8f, 0.2f, 0.2f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.8f, 100.0f);
	bRenderer().getObjects()->createLight("torchLight", -bRenderer().getObjects()->getCamera("camera")->getPosition(), vmml::Vector3f(1.0f, 0.45f, -0.4f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 1400.0f, 0.9f, 280.0f);

	// postprocessing
	bRenderer().getObjects()->createFramebuffer("fbo");																				// create framebuffer object
	bRenderer().getObjects()->createTexture("fbo_texture1", bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());	// create texture to bind to the fbo
	bRenderer().getObjects()->createTexture("fbo_texture2", bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());	// create texture to bind to the fbo
	ShaderPtr blurShader = bRenderer().getObjects()->loadShaderFile("blurShader", 0, false, false, false, false, false);				// load shader that blurs the texture
	MaterialPtr blurMaterial = bRenderer().getObjects()->createMaterial("blurMaterial", blurShader);									// create an empty material to assign either texture1 or texture2 to
	bRenderer().getObjects()->createSprite("blurSprite", blurMaterial);																// create a sprite using the material created above

	//////////////////////////////TESTreflections
	bRenderer().getObjects()->loadObjModel("sphere.obj", true, true, true, 4, true, true);											// create custom shader with a maximum of 4 lights 
	bRenderer().getObjects()->createCamera("reflectionCamera", 90.0, 16.f / 9.f, -1.0, 1000.0);																	// camera to capture reflecions
	FramebufferPtr fboRefl = bRenderer().getObjects()->createFramebuffer("fboRefl");																				// create framebuffer object
    fboRefl->resize(512, 512, false);
    CubeMapPtr reflCubeMap = bRenderer().getObjects()->createCubeMap("reflMap", 512);			// create cube map to bind to the fbo
	//pass cube map to sphere
	bRenderer().getObjects()->getMaterial("sphere")->setTexture(bRenderer::DEFAULT_SHADER_UNIFORM_CUBE_MAP(), reflCubeMap);
	//////////////////////////////

	//////////////////////////////TEST Depth Map
	/*FramebufferPtr fboDepth = bRenderer().getObjects()->createFramebuffer("fboDepth");
	DepthMapPtr depthMap = bRenderer().getObjects()->createDepthMap("depthMap", 0, 0);

	ShaderPtr depthShader = bRenderer().getObjects()->loadShaderFile("depthShader", 0, false, false, false, false, false);
	MaterialPtr depthMaterial = bRenderer().getObjects()->createMaterial("depthMaterial", depthShader);
	bRenderer().getObjects()->createSprite("depthSprite", depthMaterial);

	depthMaterial->setTexture(bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP(), depthMap);*/
	//////////////////////////////

	// get shading language version
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//////////////////////////////TESTreflections
	// get reflections for cube map
	updateReflections(fboRefl, reflCubeMap, "reflectionCamera", vmml::Vector3f(-148.0, -5.0, -40.0));
	//////////////////////////////

	// Update render queue
	updateRenderQueue("camera", 0.0f);
}

/* Draw your scene here */
void ProjectMain::loopFunction(const double &deltaTime, const double &elapsedTime)
{
	//bRenderer::log("deltaTime: "+std::to_string(deltaTime)+", elapsedTime: "+std::to_string(elapsedTime));
	//bRenderer::log("FPS: " + std::to_string(1 / deltaTime));

	//// Draw Scene and do postprocessing ////
	/*
	*	Performance is a bit better if we draw first and then do all the cpu-heavy updating
	*	This way we can keep the cpu busy while the graphics card draws the scene (only works on highend graphics cards)
	*/

	/// Begin postprocessing ///
	GLint defaultFBO;
	if (!_running){
		bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth() / 5, bRenderer().getView()->getHeight() / 5);		// reduce vieport size
		defaultFBO = Framebuffer::getCurrentFramebuffer();	// get current fbo to bind it again after drawing the scene
		bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture("fbo_texture1"), false);	// bind the fbo
	}
	//////////////////////////////TEST Depth Map
	//else{
	//	defaultFBO = Framebuffer::getCurrentFramebuffer();	// get current fbo to bind it again after drawing the scene
	//	bRenderer().getObjects()->getFramebuffer("fboDepth")->bindDepthMap(bRenderer().getObjects()->getDepthMap("depthMap"), false); // bind the fbo
	//}
	//////////////////////////////

	/// Draw scene ///	
	
	bRenderer().drawQueue(/*GL_LINES*/);
	bRenderer().clearQueue();
	
	if (!_running){
		/// End postprocessing ///		
        /*** Blur ***/
		// translate
		vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, -0.5));
		// blur vertically and horizontally
		bool b = true;		int numberOfBlurSteps = 2;
		for (int i = 0; i < numberOfBlurSteps; i++) {
			if (i == numberOfBlurSteps - 1){
				bRenderer().getObjects()->getFramebuffer("fbo")->unbind(defaultFBO); //unbind (original fbo will be bound)
				bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());								// reset vieport size
			}
			else
				bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture(b ? "fbo_texture2" : "fbo_texture1"), false);
			bRenderer().getObjects()->getMaterial("blurMaterial")->setTexture("fbo_texture", bRenderer().getObjects()->getTexture(b ? "fbo_texture1" : "fbo_texture2"));
			bRenderer().getObjects()->getMaterial("blurMaterial")->setScalar("isVertical", static_cast<GLfloat>(b));
			// draw
			bRenderer().drawModel(bRenderer().getObjects()->getModel("blurSprite"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
			b = !b;
		}
	
        /*** Title ***/
        // translate and scale 
        GLfloat titleScale = 0.5f;
        vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
		modelMatrix = vmml::create_translation(vmml::Vector3f(-0.4f, 0.0f, -0.65f)) * scaling;
        // draw
		bRenderer().drawModel(bRenderer().getObjects()->getModel("bTitle"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false);

		/*** Instructions ***/
		titleScale = 0.1f;
		scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
		modelMatrix = vmml::create_translation(vmml::Vector3f(-0.45f / bRenderer().getView()->getAspectRatio(), -0.6f, -0.65f)) * scaling;
		bRenderer().drawModel(bRenderer().getObjects()->getTextSprite("instructions"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
    }
	//////////////////////////////TEST Depth Map
	//else{
	//	bRenderer().getObjects()->getFramebuffer("fbo")->unbind(defaultFBO); //unbind (original fbo will be bound)

	//	// translate
	//	vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, -0.5));
	//	// draw
	//	bRenderer().drawModel(bRenderer().getObjects()->getModel("depthSprite"), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);

	//}
	//////////////////////////////


	//// Camera Movement ////
	updateCamera("camera", deltaTime);
	
	//// Torch Light ////
	if (_running){
		if (deltaTime > 0.0f){
			_offset += 5 * deltaTime;
			_randomOffset += (randomNumber(10.0f, 20.0f)) * deltaTime;
		}
		// let the light follow the camera
		GLfloat flickeringLightPosX = -bRenderer().getObjects()->getCamera("camera")->getPosition().x();
		GLfloat flickeringLightPosY = -bRenderer().getObjects()->getCamera("camera")->getPosition().y();
		GLfloat flickeringLightPosZ = -bRenderer().getObjects()->getCamera("camera")->getPosition().z();
		// let the light flicker
		flickeringLightPosX += 2*sin(flickeringLightPosY + 0.5f*_randomOffset);
		flickeringLightPosY += 2*sin(flickeringLightPosX + 0.5f*_randomOffset);
		bRenderer().getObjects()->getLight("torchLight")->setPosition(vmml::Vector3f(flickeringLightPosX, flickeringLightPosY, flickeringLightPosZ) - bRenderer().getObjects()->getCamera("camera")->getForward()*10.0f);
	}
	else{
		// set the light to be at the camera position
		bRenderer().getObjects()->getLight("torchLight")->setPosition(-bRenderer().getObjects()->getCamera("camera")->getPosition() - bRenderer().getObjects()->getCamera("camera")->getForward()*10.0f);
	}

	/// Update render queue ///
	updateRenderQueue("camera", deltaTime);
	
}

/* This function is executed when terminating the renderer */
void ProjectMain::terminateFunction()
{
	bRenderer::log("I totally terminated this Renderer :-)");
}

/* Update render queue */
void ProjectMain::updateRenderQueue(const std::string &camera, const double &deltaTime)
{
	/*** Cave ***/
	// translate and scale 
	vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f(30.f, -24.0, 0.0)) * vmml::create_scaling(vmml::Vector3f(0.3f));
	// submit to render queue
	bRenderer().queueModelInstance("cave", "cave_instance", camera, modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }), true, true);
	
	/*** Cave stream ***/
	bRenderer().getObjects()->getProperties("streamProperties")->setScalar("offset", _offset);		// pass offset for wave effect
	// submit to render queue
	bRenderer().queueModelInstance("cave_stream", "cave_stream_instance", camera, modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }), true, false, true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 1.0f);
	
	//////////////////////////////TESTreflections
	/*** Sphere ***/
	// translate and scale
	modelMatrix = vmml::create_translation(vmml::Vector3f(148.0f, 5.0f, 40.0f)) * vmml::create_scaling(vmml::Vector3f(0.1f));
	// submit to render queue
	bRenderer().queueModelInstance("sphere", "sphere_instance", camera, modelMatrix, std::vector<std::string>({ /*"torchLight",*/ "firstLight", "secondLight", "thirdLight" }), true, false, true);
	bRenderer().getObjects()->getModel("sphere")->getInstanceProperties("sphere_instance")->at(bRenderer().getObjects()->getShader("sphere"))->setMatrix(bRenderer::DEFAULT_SHADER_UNIFORM_INVERSE_VIEW_MATRIX(), vmml::Matrix3f(bRenderer().getObjects()->getCamera(camera)->getInverseViewMatrix()));
	//////////////////////////////

	/*** Crystal (blue) ***/
	// translate and scale
	modelMatrix = vmml::create_translation(vmml::Vector3f(78.0f, -17.0f, 5.5f)) * vmml::create_scaling(vmml::Vector3f(0.1f));
	// submit to render queue
	bRenderer().getObjects()->setAmbientColor(vmml::Vector3f(0.2f, 0.2f, 1.0f));
	bRenderer().queueModelInstance("crystal", "crystal_blue", camera, modelMatrix, std::vector<std::string>({ "torchLight", "firstLight" }), true, false, true);

	/*** Crystal (green) ***/
	// translate and scale 
	modelMatrix = vmml::create_translation(vmml::Vector3f(148.0f, -17.0f, 15.0f)) * vmml::create_scaling(vmml::Vector3f(0.1f));
	// submit to render queue
	bRenderer().getObjects()->setAmbientColor(vmml::Vector3f(0.2f, 0.7f, 0.2f));
	bRenderer().queueModelInstance("crystal", "crystal_green", camera, modelMatrix, std::vector<std::string>({ "torchLight", "secondLight" }), true, false, true);

	/*** Crystal (red) ***/
	// translate and scale 
	modelMatrix = vmml::create_translation(vmml::Vector3f(218.0f, -17.0f, 4.0f)) * vmml::create_scaling(vmml::Vector3f(0.1f));
	// submit to render queue
	bRenderer().getObjects()->setAmbientColor(vmml::Vector3f(0.8f, 0.2f, 0.2f));
	bRenderer().queueModelInstance("crystal", "crystal_red", camera, modelMatrix, std::vector<std::string>({ "torchLight", "thirdLight" }), true, false, true);
	bRenderer().getObjects()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());

	///*** Torch ***/
	// Position the torch relative to the camera
	modelMatrix = bRenderer().getObjects()->getCamera(camera)->getInverseViewMatrix();		// position and orient to match camera
	modelMatrix *= vmml::create_translation(vmml::Vector3f(0.75f, -1.1f, 0.8f)) * vmml::create_scaling(vmml::Vector3f(1.2f)) * vmml::create_rotation(1.64f, vmml::Vector3f::UNIT_Y); // now position it relative to the camera
	// submit to render queue
	bRenderer().queueModelInstance("torch", "torch_instance", camera, modelMatrix, std::vector<std::string>({ "torchLight" }));

	/*** Flame ***/
	// pass additional properties to the shader
	bRenderer().getObjects()->getProperties("flameProperties")->setScalar("offset", _randomOffset);		// pass offset for wave effect
	// create three flames
	for (GLfloat z = 0.0f; z < 3.0f; z++)
	{
		// translate
		vmml::Matrix4f translation = vmml::create_translation(vmml::Vector3f(0.65f / bRenderer().getView()->getAspectRatio(), 0.6f + (0.08f*z), (-z / 100.0f - 0.50f)));
		// rotate
		GLfloat rot = 0.0f;
		if (fmod(z, 2.0f) != 0.0f)
			rot = M_PI_F;
			
		vmml::Matrix4f rotation = vmml::create_rotation(rot, vmml::Vector3f::UNIT_Z);
		// scale
		GLfloat ParticleScale = 1.225f - (0.23f*z);
		vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(ParticleScale / bRenderer().getView()->getAspectRatio(), ParticleScale, ParticleScale));
		// model matrix
		modelMatrix = translation * scaling * rotation;
		// submit to render queue
		bRenderer().queueModelInstance(bRenderer().getObjects()->getModel("flame"), ("flame_instance" + std::to_string(z)), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false, true, GL_SRC_ALPHA, GL_ONE, (-1.0f - 0.01f*z));  // negative distance because always in foreground
	}

	/*** Sparks ***/
	for (GLfloat z = 1.0f; z < 2.0f; z++)
	{
		// translate
		vmml::Matrix4f translation = vmml::create_translation(vmml::Vector3f(0.65f / bRenderer().getView()->getAspectRatio(), 0.65f, (-z / 100.0f - 0.58f)));
		// rotate
		GLfloat rot = 1.0f;
		if (_running)
			rot = randomNumber(1.0f, 1.1f)*_randomOffset*z;
		vmml::Matrix4f rotation = vmml::create_rotation(rot, vmml::Vector3f::UNIT_Z);
		// scale
		GLfloat ParticleScale = 0.55f - (0.25f*z);
		vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(ParticleScale / bRenderer().getView()->getAspectRatio(), 4.0f*ParticleScale, ParticleScale));
		// model matrix
		modelMatrix = translation * scaling * rotation;

		// submit to render queue
		bRenderer().queueModelInstance(bRenderer().getObjects()->getModel("sparks"), ("sparks_instance" + std::to_string(z)), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false, true, GL_SRC_ALPHA, GL_ONE, (-2.0f - 0.01f*z)); // negative distance because always in foreground
	}

	//////////////////////////////////TEXTTEST
	//bRenderer().getObjects()->getFont("arial")->setPixelSize(static_cast<int>(1 / deltaTime));

	GLfloat titleScale = 6.f;
	vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale / bRenderer().getView()->getAspectRatio(), titleScale));
	modelMatrix = vmml::create_translation(vmml::Vector3f(78.f, 0.f, 10.f)) * bRenderer().getObjects()->getCamera("camera")->getInverseRotation() * scaling;
	bRenderer().getObjects()->getTextSprite("test_text")->setText("FPS: " + std::to_string(static_cast<int>(1 / deltaTime)) + " \nthe cave - demo");
	bRenderer().queueTextInstance("test_text", "textInstance_Test", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight" }));
	//////////////////////////////////
}

/* Camera movement */
void ProjectMain::updateCamera(const std::string &camera, const double &deltaTime)
{
	//// Adjust aspect ratio ////
	bRenderer().getObjects()->getCamera(camera)->setAspectRatio(bRenderer().getView()->getAspectRatio());

	double deltaCameraY = 0.0;
	double deltaCameraX = 0.0;
	double cameraForward = 0.0;

	/* iOS: control movement using touch screen */
	if (Input::isTouchDevice()){

		// pause using double tap
		if (bRenderer().getInput()->doubleTapRecognized()){
			_running = !_running;
		}

		if (_running){
			// control using touch
			TouchMap touchMap = bRenderer().getInput()->getTouches();
			int i = 0;
			for (auto t = touchMap.begin(); t != touchMap.end(); ++t)
			{
				Touch touch = t->second;
				// If touch is in left half of the view: move around
				if (touch.startPositionX < bRenderer().getView()->getWidth() / 2){
					cameraForward = -(touch.currentPositionY - touch.startPositionY) / 100;
				}
				// if touch is in right half of the view: look around
				else
				{
					deltaCameraY = (touch.currentPositionX - touch.startPositionX) / 2000;
					deltaCameraX = (touch.currentPositionY - touch.startPositionY) / 2000;
				}
				if (++i > 2)
					break;
			}
		}

	}
	/* Windows: control movement using mouse and keyboard */
	else{
		// use space to pause and unpause
		GLint currentStateSpaceKey = bRenderer().getInput()->getKeyState(bRenderer::KEY_SPACE);
		if (currentStateSpaceKey != _lastStateSpaceKey)
		{
			_lastStateSpaceKey = currentStateSpaceKey;
			if (currentStateSpaceKey == bRenderer::INPUT_PRESS){
				_running = !_running;
				bRenderer().getInput()->setCursorEnabled(!_running);
			}
		}

		// mouse look
		double xpos, ypos; bool hasCursor = false;
		bRenderer().getInput()->getCursorPosition(&xpos, &ypos, &hasCursor);

		deltaCameraY = (xpos - _mouseX) / 1000;
		_mouseX = xpos;
		deltaCameraX = (ypos - _mouseY) / 1000;
		_mouseY = ypos;

		if (_running){
			// movement using wasd keys
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_W) == bRenderer::INPUT_PRESS)
				if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = 2.0;
				else			cameraForward = 1.0;
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_S) == bRenderer::INPUT_PRESS)
				if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = -2.0;
				else			cameraForward = -1.0;
			else
				cameraForward = 0.0;

			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_A) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->moveCameraSideward(-_cameraSpeed*deltaTime);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_D) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->moveCameraSideward(_cameraSpeed*deltaTime);
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_UP) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->moveCameraUpward(_cameraSpeed*deltaTime);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_DOWN) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->moveCameraUpward(-_cameraSpeed*deltaTime);
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->rotateCamera(0.0f, 0.0f, 0.03f*_cameraSpeed*deltaTime);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_RIGHT) == bRenderer::INPUT_PRESS)
				bRenderer().getObjects()->getCamera(camera)->rotateCamera(0.0f, 0.0f, -0.03f*_cameraSpeed*deltaTime);
		}
	}

	//// Update camera ////
	if (_running){
		bRenderer().getObjects()->getCamera(camera)->moveCameraForward(cameraForward*_cameraSpeed*deltaTime);
		bRenderer().getObjects()->getCamera(camera)->rotateCamera(deltaCameraX, deltaCameraY, 0.0f);
	}	
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

void ProjectMain::updateReflections(FramebufferPtr fbo, CubeMapPtr cubeMap, const std::string &camera, const vmml::Vector3f &position)
{
	GLint defaultFBO = Framebuffer::getCurrentFramebuffer();				// get current fbo to bind it again after drawing the scene
	bRenderer().getObjects()->getCamera(camera)->setPosition(position);

	for (GLuint i = 0; i < 6; i++)
	{
		if (i == 0)
			bRenderer().getObjects()->getCamera(camera)->setRotation(vmml::Vector3f(0.f, -M_PI_F / 2, 0.f));
		if (i == 1)
			bRenderer().getObjects()->getCamera(camera)->setRotation(vmml::Vector3f(0.f, M_PI_F / 2, 0.f));
		if (i == 2)
			bRenderer().getObjects()->getCamera(camera)->setRotation(vmml::Vector3f(-M_PI_F / 2, 0.f, 0.f));
		if (i == 3)
			bRenderer().getObjects()->getCamera(camera)->setRotation(vmml::Vector3f(M_PI_F / 2, 0.f, 0.f));
		if (i == 4)
			bRenderer().getObjects()->getCamera(camera)->setRotation(vmml::Vector3f(0.f, 0.f, 0.f));
		if (i == 5)
			bRenderer().getObjects()->getCamera(camera)->setRotation(vmml::Vector3f(0.f, 0.f, M_PI_F));
		
		// Bind buffer with cube map
		fbo->bindCubeMap(cubeMap, i, false);
		// Update scene
		updateRenderQueue(camera, 0.0f);
		// Draw scene into buffer
		bRenderer().drawQueue(/*GL_LINES*/);
	}

	fbo->unbind(defaultFBO);
}

/* Helper functions */
GLfloat ProjectMain::randomNumber(GLfloat min, GLfloat max){
	return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}