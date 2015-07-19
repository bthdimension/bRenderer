#include "ProjectMain.h"
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

/* Initialize the Project */
void ProjectMain::init()
{
	// set this instance of RenderProject to be used for function calls
	bRenderer().setRenderProject(this);

	int w, h;
	View::getScreenSize(&w, &h);
	bRenderer::log("Screen width: "+lexical_cast< std::string >(w)+", Screen height: "+lexical_cast< std::string >(h));

	// let the renderer create an OpenGL context and the main window
#ifdef B_OS_DESKTOP
	bRenderer().initRenderer(1920, 1080, false, "The Cave - Demo");
	//bRenderer().initRenderer(w, h, true);	// Fullscreen using full width and height of the screen
#endif
#ifdef B_OS_IOS
    bRenderer().initRenderer(true);
#endif

	// set shader versions (optional)
	bRenderer().getAssets()->setShaderVersionDesktop("#version 120");
	bRenderer().getAssets()->setShaderVersionES("#version 100");

	// start main loop 
	bRenderer().runRenderer();
}

/* This function is executed when initializing the renderer */
void ProjectMain::initFunction()
{
	bRenderer::log("my initialize function was started");

	// TEST: load material and shader before loading the model
	ShaderPtr customShader = bRenderer().getAssets()->generateShader("customShader", 2, true, true, true, true, true, true, true, true, true, false);		// create custom shader with a maximum of 2 lights
	ShaderPtr flameShader = bRenderer().getAssets()->loadShaderFile("flame", 0, false, true, true, false);												// load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
	MaterialPtr flameMaterial = bRenderer().getAssets()->loadMaterial("flame.mtl", "flame", flameShader);												// load material from file using the shader created above

	PropertiesPtr flameProperties = bRenderer().getAssets()->createProperties("flameProperties");	// Add additional properties to a model

	// load models
	bRenderer().getAssets()->loadModel("cave_start.obj", true, true, false, 4, true, true);			// create custom shader with a maximum of 4 lights (since nothing else was specified, number of lights may vary between 0 and 4 during rendering without performance loss)
	bRenderer().getAssets()->loadModel("sphere.obj", true, true, false, 4, true, true);				// create custom shader with a maximum of 4 lights 
	bRenderer().getAssets()->loadModel("crystal.obj", false, true, customShader);					// the custom shader created above is used
	bRenderer().getAssets()->loadModel("torch.obj", false, true, false, 1, false, true);				// create custom shader with a maximum of 1 light
	bRenderer().getAssets()->loadModel("flame.obj", false, true, flameMaterial, flameProperties);	// the flame material created above is used, to pass additional properties a Properties object is used
	bRenderer().getAssets()->loadModel("sparks.obj", false, true, true, 0, false, true);				// automatically loads shader files "sparks.vert" and "sparks.frag", we specify 0 lights since the shader doesn't consider lights
	bRenderer().getAssets()->loadModel("bTitle.obj", false, true, false, 0, false, false);			// create custom shader with 0 lights -> the title will always be fully lit

	// initialize variables
	_randomTime = 0.0f;
	_running = false; _lastStateSpaceKey = bRenderer::INPUT_UNDEFINED;

	// initialize free moving camera
	bRenderer().getAssets()->createCamera("camera", vmml::Vector3f(-33.0, 0.0, -13.0), vmml::Vector3f(0.0, -M_PI_F / 2, 0.0));

	// get shading language version
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// create lights
	bRenderer().getAssets()->createLight("firstLight", vmml::Vector3f(80.0f, 0.0f, 0.0f), vmml::Vector3f(0.5f, 0.5f, 1.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.3f);
	bRenderer().getAssets()->createLight("secondLight", vmml::Vector3f(150.0f, 0.0f, 0.0f), vmml::Vector3f(0.5f, 1.0f, 0.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.5f);
	bRenderer().getAssets()->createLight("thirdLight", vmml::Vector3f(210.0f, 0.0f, 0.0f), vmml::Vector3f(0.8f, 0.0f, 0.0f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 0.5f);
	bRenderer().getAssets()->createLight("torchLight", -bRenderer().getAssets()->getCamera("camera")->getPosition(), vmml::Vector3f(1.0f, 0.4f, -0.5f), vmml::Vector3f(1.0f, 1.0f, 1.0f), 1200.0f, 0.7f);

	// postprocessing
	bRenderer().getAssets()->createFramebuffer("fbo");
	bRenderer().getAssets()->createTexture("fbo_texture1", bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());
	bRenderer().getAssets()->createTexture("fbo_texture2", bRenderer().getView()->getWidth(), bRenderer().getView()->getHeight());
	ShaderPtr blurShader = bRenderer().getAssets()->loadShaderFile("blurShader", 0, false, false, false, false);
	bRenderer().getAssets()->loadModel("quad.obj", false, true, blurShader);

	// set ambient color
	bRenderer().getAssets()->setAmbientColor(vmml::Vector3f(0.0f, 0.0f, 0.05f));
}

/* Draw your scene here */
void ProjectMain::loopFunction(const double &deltaTime, const double &elapsedTime)
{
	//if ((int)elapsedTime >= 6 && (int)elapsedTime <= 7)
	//{
	//	/* Start animating after 6 seconds */
	//	if (!_running){ _running = true; bRenderer().getInput()->setCursorEnabled(!_running); }
	//}

	//bRenderer::log("deltaTime: "+lexical_cast< std::string >(deltaTime)+", elapsedTime: "+lexical_cast< std::string >(elapsedTime));
	//bRenderer::log("FPS: "+lexical_cast< std::string >(1/deltaTime));

	//// Camera Movement ////
	moveCamera(deltaTime);

	//// Adjust aspect ratio ////
	bRenderer().getAssets()->getCamera("camera")->setAspectRatio(bRenderer().getView()->getAspectRatio());

	//// Torch Light ////
	if (_running){
		if (deltaTime < 0.5f){
			_randomTime += deltaTime + randomNumber(0.0f, 0.12f);
		}
		GLfloat flickeringLight = 1.0f + (_randomTime)* 2.0f * M_PI_F*(0.032f);
		GLfloat flickeringLightPosX = -bRenderer().getAssets()->getCamera("camera")->getPosition().x();
		GLfloat flickeringLightPosY = -bRenderer().getAssets()->getCamera("camera")->getPosition().y();
		GLfloat flickeringLightPosZ = -bRenderer().getAssets()->getCamera("camera")->getPosition().z();
		flickeringLightPosX += 2.5f*sin(flickeringLightPosY * 4.0f + 3.0f*flickeringLight);
		flickeringLightPosY += 2.5f*sin(flickeringLightPosX * 4.0f + 3.0f*flickeringLight);
		bRenderer().getAssets()->getLight("torchLight")->setPosition(vmml::Vector3f(flickeringLightPosX, flickeringLightPosY, flickeringLightPosZ) - bRenderer().getAssets()->getCamera("camera")->getForward()*10.0f);
	}

	//// Draw Models ////

	/// Begin postprocessing ///
	GLint defaultFBO;
	if (!_running){
		defaultFBO = Framebuffer::getCurrentFramebuffer();
		bRenderer().getAssets()->getFramebuffer("fbo")->bind(bRenderer().getAssets()->getTexture("fbo_texture1"), false);
	}

	/*** Cave Start ***/
	// translate and scale 
	vmml::Matrix4f modelMatrix = vmml::create_scaling(vmml::Vector3f(0.3f)) * vmml::create_translation(vmml::Vector3f(100.0, -80.0, 0.0));
	// draw
	bRenderer().drawModel("cave_start", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }));

	/*** Sphere ***/
	// translate and scale
	modelMatrix = vmml::create_scaling(vmml::Vector3f(0.1f)) * vmml::create_translation(vmml::Vector3f(1480.0, 50.0, 300.0));
	// draw 
	bRenderer().drawModel("sphere", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight", "secondLight", "thirdLight" }));

	/*** Crystal (blue) ***/
	// translate and scale
	modelMatrix = vmml::create_scaling(vmml::Vector3f(0.1f)) * vmml::create_translation(vmml::Vector3f(780.0, -170.0, 55.0));
	// draw
	bRenderer().getAssets()->setAmbientColor(vmml::Vector3f(0.2f, 0.2f, 0.8f));
	bRenderer().drawModel("crystal", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "firstLight"}));
	bRenderer().getAssets()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());

	/*** Crystal (green) ***/
	// translate and scale 
	modelMatrix = vmml::create_scaling(vmml::Vector3f(0.1f)) * vmml::create_translation(vmml::Vector3f(1480.0f, -170.0f, 70.0f));
	// draw
	bRenderer().getAssets()->setAmbientColor(vmml::Vector3f(0.1f, 0.45f, 0.1f));
	bRenderer().drawModel("crystal", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "secondLight" }));
	bRenderer().getAssets()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());

	/*** Crystal (red) ***/
	// translate and scale 
	modelMatrix = vmml::create_scaling(vmml::Vector3f(0.1f)) * vmml::create_translation(vmml::Vector3f(2180.0f, -170.0f, 40.0f));
	// draw
	bRenderer().getAssets()->setAmbientColor(vmml::Vector3f(0.6f, 0.1f, 0.1f));
	bRenderer().drawModel("crystal", "camera", modelMatrix, std::vector<std::string>({ "torchLight", "thirdLight" }));
	bRenderer().getAssets()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());

	///*** Torch ***/
	// translate and scale 	
		// Position the torch relative to the camera
	modelMatrix = bRenderer().getAssets()->getCamera("camera")->getInverseViewMatrix();
	modelMatrix *= vmml::create_translation(vmml::Vector3f(0.75f, -1.1f, 0.8f)) * vmml::create_scaling(vmml::Vector3f(1.2f)) * vmml::create_rotation(1.64f, vmml::Vector3f::UNIT_Y);
	// draw
	bRenderer().drawModel("torch", "camera", modelMatrix, std::vector<std::string>({ "torchLight" }));
    
    /*** Flame ***/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	// view matrix
	vmml::Vector3f eyePos(0.0f, 0.0f, 0.25f);
	vmml::Matrix4f viewMatrix = Camera::lookAt(eyePos, vmml::Vector3f::ZERO, vmml::Vector3f::UP);
	// create three flames
	for (GLfloat z = 0.0f; z < 3.0f; z++) 
	{
		if (z == 1.0f)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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
		GLfloat ParticleScale = 2.45f - (0.46f*z);
		vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(ParticleScale / bRenderer().getView()->getAspectRatio(), ParticleScale, ParticleScale));
		// model matrix
		modelMatrix = translation * scaling * rotation;
		// wave effect
		GLfloat offset = (_randomTime + 0.3f*z) * 2 * M_PI_F*(0.75f + 0.5f*z);
		GLfloat transparency = 1.0f;
		if (z == 0.0f)transparency = 0.8f;
		// pass additional properties to the shader
		bRenderer().getAssets()->getProperties("flameProperties")->setScalar("offset", offset);
		bRenderer().getAssets()->getProperties("flameProperties")->setScalar("transparency", transparency);
		// draw
		bRenderer().drawModel("flame", modelMatrix, viewMatrix, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}));
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
		GLfloat ParticleScale = 1.1f - (0.5f*z);
		vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(ParticleScale / bRenderer().getView()->getAspectRatio(), 4.0f*ParticleScale, ParticleScale));
		// model matrix
		modelMatrix = translation * scaling * rotation;

		// draw
		bRenderer().drawModel("sparks", modelMatrix, viewMatrix, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}));
	}
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!_running){
        
    /// End postprocessing ///
		
        /*** Blur ***/
		// translate
		vmml::Matrix4f translation = vmml::create_translation(vmml::Vector3f(0.0f, 0.0f, -0.5));
		// model matrix
		modelMatrix = translation;
		// blur vertically and horizontally
		bool b = true;		int numberOfBlurSteps = 2;
		for (int i = 0; i < numberOfBlurSteps; i++) {
			if (i == numberOfBlurSteps - 1)
				bRenderer().getAssets()->getFramebuffer("fbo")->unbind(defaultFBO); //unbind (original fbo will be bound)
			else
				bRenderer().getAssets()->getFramebuffer("fbo")->bind(bRenderer().getAssets()->getTexture(b ? "fbo_texture2" : "fbo_texture1"), false);
            
			bRenderer().getAssets()->getMaterial("quad")->setTexture("fbo_texture", bRenderer().getAssets()->getTexture(b ? "fbo_texture1" : "fbo_texture2"));
			bRenderer().getAssets()->getMaterial("quad")->setScalar("isVertical", (GLfloat)b);
			// draw
			bRenderer().drawModel("quad", modelMatrix, viewMatrix, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}));
			b = !b;
		}
	
        /*** Title ***/
        // translate and scale 
        GLfloat titleScale = 0.4f;
        translation = vmml::create_translation(vmml::Vector3f(-0.4f, 0.0f, -0.65f));	
        vmml::Matrix4f scaling = vmml::create_scaling(vmml::Vector3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));
        modelMatrix = translation * scaling;
        // draw
        bRenderer().drawModel("bTitle", modelMatrix, viewMatrix, vmml::Matrix4f::IDENTITY, std::vector<std::string>({}));
    }
}

/* This function is executed when terminating the renderer */
void ProjectMain::terminateFunction()
{
	bRenderer::log("I totally terminated this Renderer :-)");
}


/* Camera movement */
void ProjectMain::moveCamera(const double &deltaTime)
{
	double deltaCameraY = 0.0;
	double deltaCameraX = 0.0;
    double cameraForward = 0.0;

	/* On iOS automatic movement (for now) */
	if (bRenderer().getInput()->isTouchDevice()){
        
        // Pause using double tap
        if (bRenderer().getInput()->doubleTapRecognized()){
            _running = !_running;
        }
        
        // Control using gyroscope
//        deltaCameraY = bRenderer().getInput()->getGyroscopePitch() / 20;
//		cameraForward = -bRenderer().getInput()->getGyroscopeRoll();
        
        if(_running){
            // Control using touch
            TouchMap touchMap = bRenderer().getInput()->getTouches();
            int i = 0;
            for(auto t = touchMap.begin(); t != touchMap.end(); ++t)
            {
                Touch touch = t->second;
                // If touch is in left half of the view: move around
                if(touch.startPositionX < bRenderer().getView()->getWidth()/2){
                    cameraForward = -(touch.currentPositionY - touch.startPositionY)/100;
                }
                // If touch is in right half of the view: look around
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
	/* Windows only: Mouse and Keyboard Movement */
	else{
		GLint currentStateSpaceKey = bRenderer().getInput()->getKeyState(bRenderer::KEY_SPACE);
		if (currentStateSpaceKey != _lastStateSpaceKey)
		{
			_lastStateSpaceKey = currentStateSpaceKey;
			if (currentStateSpaceKey == bRenderer::INPUT_PRESS){
				_running = !_running;
				bRenderer().getInput()->setCursorEnabled(!_running);
			}
		}

		double xpos, ypos; bool hasCursor = false;
		bRenderer().getInput()->getCursorPosition(&xpos, &ypos, &hasCursor);

		deltaCameraY = (xpos - _mouseX)/1000;
		_mouseX = xpos;
		deltaCameraX = (ypos - _mouseY)/1000;
		_mouseY = ypos;

		if (_running){
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

	//// Camera ////
	if (_running){
		bRenderer().getAssets()->getCamera("camera")->moveCameraForward(cameraForward);
		bRenderer().getAssets()->getCamera("camera")->rotateCamera(deltaCameraX, deltaCameraY, 0.0f);
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