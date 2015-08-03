#include "ProjectMain.h"
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

/* Initialize the Project */
void ProjectMain::init()
{
	// let the renderer create an OpenGL context and the main window
	if(bRenderer().getInput()->isTouchDevice())
		bRenderer().initRenderer(true);										// fullscreen on iOS
	else
		//bRenderer().initRenderer(1920, 1080, false, "The Cave - Demo");		// windowed mode on desktop
		bRenderer().initRenderer(View::getScreenWidth(), View::getScreenHeight(), true);		// fullscreen using full width and height of the screen

	// set shader versions (optional)
	bRenderer().getAssets()->setShaderVersionDesktop("#version 120");
	bRenderer().getAssets()->setShaderVersionES("#version 100");

	// start main loop 
	bRenderer().runRenderer();
}

/* This function is executed when initializing the renderer */
void ProjectMain::initFunction()
{
	// initialize variables
	_randomTime = 0.0f;
	_running = false; _lastStateSpaceKey = bRenderer::INPUT_UNDEFINED;
	_viewMatrixHUD = Camera::lookAt(vmml::Vector3f(0.0f, 0.0f, 0.25f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);

	// demo: load material and shader before loading the model
	ShaderPtr customShader = bRenderer().getAssets()->generateShader("customShader", 2, true, true, true, true, true, true, true, true, true, false, false);	// create custom shader with a maximum of 2 lights
	ShaderPtr flameShader = bRenderer().getAssets()->loadShaderFile("flame", 0, false, true, true, false);				// load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
	MaterialPtr flameMaterial = bRenderer().getAssets()->loadObjMaterial("flame.mtl", "flame", flameShader);				// load material from file using the shader created above

	// create additional properties for a model
	PropertiesPtr flameProperties = bRenderer().getAssets()->createProperties("flameProperties");		

	// load models
	bRenderer().getAssets()->loadObjModel("cave.obj", true, true, false, 4, true, true);				// create custom shader with a maximum of 4 lights (since nothing else was specified, number of lights may vary between 0 and 4 during rendering without performance loss)
	bRenderer().getAssets()->loadObjModel("sphere.obj", true, true, false, 4, true, true);				// create custom shader with a maximum of 4 lights 
	bRenderer().getAssets()->loadObjModel("crystal.obj", false, true, customShader);					// the custom shader created above is used
	bRenderer().getAssets()->loadObjModel("torch.obj", false, true, false, 1, false, true);				// create custom shader with a maximum of 1 light

	// create sprites
	bRenderer().getAssets()->createSprite("flame", flameMaterial, flameProperties);						// create a sprite using the material created above, to pass additional properties a Properties object is used
	bRenderer().getAssets()->createSprite("sparks", "sparks.png");										// create a sprite displaying sparks as a texture
	bRenderer().getAssets()->createSprite("bTitle", "bTitle.png");										// create a sprite displaying the title as a texture

	// create camera
	bRenderer().getAssets()->createCamera("camera", vmml::Vector3f(-33.0, 0.0, -13.0), vmml::Vector3f(0.0, -M_PI_F / 2, 0.0));

	// create lights
	bRenderer().getAssets()->createLight("firstLight", vmml::Vector3f(80.0f, 0.0f, 0.0f), vmml::Vector3f(0.5f, 0.5f, 1.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.3f, 120.0f);
	bRenderer().getAssets()->createLight("secondLight", vmml::Vector3f(150.0f, 0.0f, 0.0f), vmml::Vector3f(0.5f, 1.0f, 0.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.5f, 120.0f);
	bRenderer().getAssets()->createLight("thirdLight", vmml::Vector3f(210.0f, 0.0f, 0.0f), vmml::Vector3f(0.8f, 0.0f, 0.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.5f, 120.0f);
	bRenderer().getAssets()->createLight("torchLight", -bRenderer().getAssets()->getCamera("camera")->getPosition(), vmml::Vector3f(1.0f, 0.4f, -0.5f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 1200.0f, 0.7f, 600.0f);

	// postprocessing
	bRenderer().getAssets()->createFramebuffer("fbo");																				// create framebuffer object
	bRenderer().getAssets()->createTexture("fbo_texture1", bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());	// create texture to bind to the fbo
	bRenderer().getAssets()->createTexture("fbo_texture2", bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());	// create texture to bind to the fbo
	ShaderPtr blurShader = bRenderer().getAssets()->loadShaderFile("blurShader", 0, false, false, false, false);					// load shader that blurs the texture
	MaterialPtr blurMaterial = bRenderer().getAssets()->createMaterial("blurMaterial", blurShader);									// create an empty material to assign either texture1 or texture2 to
	bRenderer().getAssets()->createSprite("blurSprite", blurMaterial);																// create a sprite using the material created above

	// set ambient color
	bRenderer().getAssets()->setAmbientColor(vmml::Vector3f(0.0f, 0.0f, 0.05f));

	// get shading language version
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Update render queue
	updateRenderQueue();
}

/* Draw your scene here */
void ProjectMain::loopFunction(const double &deltaTime, const double &elapsedTime)
{
	//bRenderer::log("deltaTime: "+lexical_cast< std::string >(deltaTime)+", elapsedTime: "+lexical_cast< std::string >(elapsedTime));
	bRenderer::log("FPS: "+lexical_cast< std::string >(1/deltaTime));

	//// Draw Scene and do postprocessing ////
	/*
	*	Performance is a bit better if we draw first and then do all the cpu-heavy updating
	*	This way we can keep the cpu busy while the graphics card draws the scene
	*/

	/// Begin postprocessing ///
	GLint defaultFBO;
	if (!_running){
		defaultFBO = Framebuffer::getCurrentFramebuffer();	// get current fbo to bind it again after drawing the scene
		bRenderer().getAssets()->getFramebuffer("fbo")->bind(bRenderer().getAssets()->getTexture("fbo_texture1"), false); // bind the fbo
	}

	/// Draw scene ///
	bRenderer().getRenderQueue()->draw();	
	
	if (!_running){
        
		/// End postprocessing ///		
        /*** Blur ***/
		// translate
		vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, -0.5));
		// blur vertically and horizontally
		bool b = true;		int numberOfBlurSteps = 2;
		for (int i = 0; i < numberOfBlurSteps; i++) {
			if (i == numberOfBlurSteps - 1)
				bRenderer().getAssets()->getFramebuffer("fbo")->unbind(defaultFBO); //unbind (original fbo will be bound)
			else
				bRenderer().getAssets()->getFramebuffer("fbo")->bind(bRenderer().getAssets()->getTexture(b ? "fbo_texture2" : "fbo_texture1"), false);            
			bRenderer().getAssets()->getMaterial("blurMaterial")->setTexture("fbo_texture", bRenderer().getAssets()->getTexture(b ? "fbo_texture1" : "fbo_texture2"));
			bRenderer().getAssets()->getMaterial("blurMaterial")->setScalar("isVertical", (GLfloat)b);
			// draw
			bRenderer().drawModel("blurSprite", modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false);
			b = !b;
		}
	
        /*** Title ***/
        // translate and scale 
        GLfloat titleScale = 0.4f;
        vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
		modelMatrix = vmml::create_translation(vmml::Vector3f(-0.4f, 0.0f, -0.65f)) * scaling;
        // draw
		bRenderer().drawModel("bTitle", modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false);
    }

	//// Camera Movement ////
	updateCamera(deltaTime);
	
	//// Torch Light ////
	if (_running){
		if (deltaTime < 0.5f){
			_randomTime += (deltaTime + randomNumber(0.0f, 0.12f))*0.5;
		}
		GLfloat flickeringLight = 1.0f + (_randomTime)* 2.0f * M_PI_F*(0.032f);
		// let the light follow the camera
		GLfloat flickeringLightPosX = -bRenderer().getAssets()->getCamera("camera")->getPosition().x();
		GLfloat flickeringLightPosY = -bRenderer().getAssets()->getCamera("camera")->getPosition().y();
		GLfloat flickeringLightPosZ = -bRenderer().getAssets()->getCamera("camera")->getPosition().z();
		// let the light flicker
		flickeringLightPosX += 2.5f*sin(flickeringLightPosY * 4.0f + 3.0f*flickeringLight);
		flickeringLightPosY += 2.5f*sin(flickeringLightPosX * 4.0f + 3.0f*flickeringLight);
		bRenderer().getAssets()->getLight("torchLight")->setPosition(vmml::Vector3f(flickeringLightPosX, flickeringLightPosY, flickeringLightPosZ) - bRenderer().getAssets()->getCamera("camera")->getForward()*10.0f);
	}
	else{
		// set the light to be at the camera position
		bRenderer().getAssets()->getLight("torchLight")->setPosition(-bRenderer().getAssets()->getCamera("camera")->getPosition() - bRenderer().getAssets()->getCamera("camera")->getForward()*10.0f);
	}

	/// Update render queue ///
	updateRenderQueue();
	
}

/* This function is executed when terminating the renderer */
void ProjectMain::terminateFunction()
{
	bRenderer::log("I totally terminated this Renderer :-)");
}


/* Camera movement */
void ProjectMain::updateCamera(const double &deltaTime)
{
	//// Adjust aspect ratio ////
	bRenderer().getAssets()->getCamera("camera")->setAspectRatio(bRenderer().getView()->getAspectRatio());

	double deltaCameraY = 0.0;
	double deltaCameraX = 0.0;
    double cameraForward = 0.0;

	/* iOS: control movement using touch screen */
	if (bRenderer().getInput()->isTouchDevice()){
        
        // pause using double tap
        if (bRenderer().getInput()->doubleTapRecognized()){
            _running = !_running;
        }
        
        if(_running){
            // control using touch
            TouchMap touchMap = bRenderer().getInput()->getTouches();
            int i = 0;
            for(auto t = touchMap.begin(); t != touchMap.end(); ++t)
            {
                Touch touch = t->second;
                // If touch is in left half of the view: move around
                if(touch.startPositionX < bRenderer().getView()->getWidth()/2){
                    cameraForward = -(touch.currentPositionY - touch.startPositionY)/100;
                }
                // if touch is in right half of the view: look around
                else
                {
                    deltaCameraY = (touch.currentPositionX - touch.startPositionX)/2000;
                    deltaCameraX = (touch.currentPositionY - touch.startPositionY)/2000;
                }
                if(++i > 2)
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

		deltaCameraY = (xpos - _mouseX)/1000;
		_mouseX = xpos;
		deltaCameraX = (ypos - _mouseY)/1000;
		_mouseY = ypos;

		if (_running){
			// movement using wasd keys
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_W) == bRenderer::INPUT_PRESS)
				if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = 1.0;
				else			cameraForward = 0.5;
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_S) == bRenderer::INPUT_PRESS)
				if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT_SHIFT) == bRenderer::INPUT_PRESS) 			cameraForward = -1.0;
				else			cameraForward = -0.5;
			else
				cameraForward = 0.0;

			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_A) == bRenderer::INPUT_PRESS)
				bRenderer().getAssets()->getCamera("camera")->moveCameraSideward(-0.5);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_D) == bRenderer::INPUT_PRESS)
				bRenderer().getAssets()->getCamera("camera")->moveCameraSideward(0.5);
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_UP) == bRenderer::INPUT_PRESS)
				bRenderer().getAssets()->getCamera("camera")->moveCameraUpward(0.5);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_DOWN) == bRenderer::INPUT_PRESS)
				bRenderer().getAssets()->getCamera("camera")->moveCameraUpward(-0.5);
			if (bRenderer().getInput()->getKeyState(bRenderer::KEY_LEFT) == bRenderer::INPUT_PRESS)
				bRenderer().getAssets()->getCamera("camera")->rotateCamera(0.0f, 0.0f, 0.03f);
			else if (bRenderer().getInput()->getKeyState(bRenderer::KEY_RIGHT) == bRenderer::INPUT_PRESS)
				bRenderer().getAssets()->getCamera("camera")->rotateCamera(0.0f, 0.0f, -0.03f);
		}
	}

	//// Update camera ////
	if (_running){
		bRenderer().getAssets()->getCamera("camera")->moveCameraForward(cameraForward);
		bRenderer().getAssets()->getCamera("camera")->rotateCamera(deltaCameraX, deltaCameraY, 0.0f);
	}
}

/* Update render queue */
void ProjectMain::updateRenderQueue()
{
	/*** Cave ***/
	// translate and scale 
	vmml::Matrix4f modelMatrix = vmml::create_scaling(vmml::Vector3f(0.3f)) * vmml::create_translation(vmml::Vector3f(100.0, -80.0, 0.0));
	// draw
	bRenderer().queueModelInstance("cave", "cave_instance", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }), true, true);

	/*** Sphere ***/
	// translate and scale
	modelMatrix = vmml::create_scaling(vmml::Vector3f(0.1f)) * vmml::create_translation(vmml::Vector3f(1480.0, 50.0, 300.0));
	// draw 
	bRenderer().queueModelInstance("sphere", "sphere_instance", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }), true, false, true);

	/*** Crystal (blue) ***/
	// translate and scale
	modelMatrix = vmml::create_scaling(vmml::Vector3f(0.1f)) * vmml::create_translation(vmml::Vector3f(780.0, -170.0, 55.0));
	// draw
	bRenderer().getAssets()->setAmbientColor(vmml::Vector3f(0.2f, 0.2f, 0.8f));
	bRenderer().queueModelInstance("crystal", "crystal_blue", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight" }), true, false, true);
	bRenderer().getAssets()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());

	/*** Crystal (green) ***/
	// translate and scale 
	modelMatrix = vmml::create_scaling(vmml::Vector3f(0.1f)) * vmml::create_translation(vmml::Vector3f(1480.0f, -170.0f, 70.0f));
	// draw
	bRenderer().getAssets()->setAmbientColor(vmml::Vector3f(0.1f, 0.45f, 0.1f));
	bRenderer().queueModelInstance("crystal", "crystal_green", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "secondLight" }), true, false, true);
	bRenderer().getAssets()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());

	/*** Crystal (red) ***/
	// translate and scale 
	modelMatrix = vmml::create_scaling(vmml::Vector3f(0.1f)) * vmml::create_translation(vmml::Vector3f(2180.0f, -170.0f, 40.0f));
	// draw
	bRenderer().getAssets()->setAmbientColor(vmml::Vector3f(0.6f, 0.1f, 0.1f));
	bRenderer().queueModelInstance("crystal", "crystal_red", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "thirdLight" }), true, false, true);
	bRenderer().getAssets()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());

	///*** Torch ***/
	// Position the torch relative to the camera
	modelMatrix = bRenderer().getAssets()->getCamera("camera")->getInverseViewMatrix();		// position and orient to match camera
	modelMatrix *= vmml::create_translation(vmml::Vector3f(0.75f, -1.1f, 0.8f)) * vmml::create_scaling(vmml::Vector3f(1.2f)) * vmml::create_rotation(1.64f, vmml::Vector3f::UNIT_Y); // now position it relative to the camera
	// draw
	bRenderer().queueModelInstance("torch", "torch_instance", "camera", modelMatrix, std::vector<std::string>({ "torchLight" }));

	/*** Flame ***/
	
	// create three flames
	for (GLfloat z = 0.0f; z < 3.0f; z++)
	{
		// translate
		vmml::Matrix4f translation = vmml::create_translation(vmml::Vector3f(0.65f / bRenderer().getView()->getAspectRatio(), 0.6f + (0.08f*z), (-z / 100.0f - 0.50f)));
		// rotate
		GLfloat rot = 0.0f;
		if (fmod(z, 2.0f) == 0.0f)
			rot = 0.0f;
		else
			rot = M_PI_F;
		vmml::Matrix4f rotation = vmml::create_rotation(rot, vmml::Vector3f::UNIT_Z);
		// scale
		GLfloat ParticleScale = 1.225f - (0.23f*z);
		vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(ParticleScale / bRenderer().getView()->getAspectRatio(), ParticleScale, ParticleScale));
		// model matrix
		modelMatrix = translation * scaling * rotation;
		// wave effect
		GLfloat offset = (_randomTime + 0.3f*z) * 2 * M_PI_F*(0.75f + 0.5f*z);
		GLfloat transparency = 1.0f;
		if (z == 0.0f)transparency = 0.8f;
		// pass additional properties to the shader
		bRenderer().getAssets()->getProperties("flameProperties")->setScalar("offset", offset);		// pass offset for wave effect
		bRenderer().getAssets()->getProperties("flameProperties")->setScalar(bRenderer::DEFAULT_SHADER_UNIFORM_TRANSPARENCY(), transparency); // pass transparency value
		// draw
		bRenderer().queueModelInstance("flame", ("flame_instance" + lexical_cast< std::string >(z)), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false, true, GL_SRC_ALPHA, GL_ONE, (-1.0f - 0.01*z));  // negative distance because always in foreground
	}

	/*** Sparks ***/
	for (GLfloat z = 1.0f; z < 2.0f; z++)
	{
		// translate
		vmml::Matrix4f translation = vmml::create_translation(vmml::Vector3f(0.65f / bRenderer().getView()->getAspectRatio(), 0.65f, (-z / 100.0f - 0.58f)));
		// rotate
		GLfloat rot = 1.0f;
		if (_running)
			rot = randomNumber(1.0f, 1.1f)*_randomTime*(z + 0.3f)*M_PI_F;
		vmml::Matrix4f rotation = vmml::create_rotation(rot, vmml::Vector3f::UNIT_Z);
		// scale
		GLfloat ParticleScale = 0.55f - (0.25f*z);
		vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(ParticleScale / bRenderer().getView()->getAspectRatio(), 4.0f*ParticleScale, ParticleScale));
		// model matrix
		modelMatrix = translation * scaling * rotation;

		// draw
		bRenderer().queueModelInstance("sparks", ("sparks_instance" + lexical_cast< std::string >(z)), modelMatrix, _viewMatrixHUD, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}), false, false, true, GL_SRC_ALPHA, GL_ONE, (-2.0f - 0.01*z)); // negative distance because always in foreground
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

/* Helper functions */
GLfloat ProjectMain::randomNumber(GLfloat min, GLfloat max){
	return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}