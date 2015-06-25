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
#endif
#ifdef OS_IOS
    bRenderer().initRenderer(true);
#endif
    // Test second view
    //View v;
    //v.initView(200, 200, false);

	// start main loop 
	bRenderer().runRenderer();

	// test restart
//#ifdef OS_DESKTOP
//	bRenderer().initRenderer(1920, 1080, false);
//	bRenderer().runRenderer();
//#endif
}

/* This function is executed when initializing the renderer */
void ProjectMain::initFunction()
{
	bRenderer::log("my initialize function was started");

	// TEST: load material and shader before loading the model
	MaterialPtr caveMtl = bRenderer().loadMaterial("cave_start.mtl", "cave_start", "default", 4);
	ShaderPtr flameShader = bRenderer().loadShader("flame");
	// load models
	bRenderer().loadModel("cave_start.obj", true, true, caveMtl);
	bRenderer().loadModel("crystal.obj", false, true);
	bRenderer().loadModel("torch.obj", false, true, "torch");
	bRenderer().loadModel("flame.obj", false, true, flameShader);
	bRenderer().loadModel("sparks.obj", false, true);
	bRenderer().loadModel("bTitle.obj", false, true);

	//MatrixStack for the cave
	bRenderer().createMatrixStack("caveStartStack");
	//MatrixStack for torch
	bRenderer().createMatrixStack("torchStack");
	//MatrixStack for crystal
	bRenderer().createMatrixStack("crystalStack");	

	// initialize variables
	randomTime = 0.0f;

	// initialize free moving camera
	cameraForward = 0.0f;
	cameraRotationX = M_PI_F;
	cameraRotationY = 0.0f;
	bRenderer().createCamera("camera", vmml::vec3f(33.0, 0.0, 13.0), vmml::create_rotation(cameraRotationX, vmml::vec3f::UNIT_Y));

	// initialize static camera
	bRenderer().createCamera("static camera");

	// get shading language version
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// create lights
	bRenderer().createLight("mainLight", vmml::vec3f(80.0f, 0.0f, 0.0f), vmml::vec3f(0.5f, 0.5f, 1.0f), 100.0f, 0.3f);
	bRenderer().createLight("secondLight", vmml::vec3f(150.0f, 0.0f, 0.0f), vmml::vec3f(0.5f, 1.0f, 0.0f), 100.0f, 0.5f);
	bRenderer().createLight("thirdLight", vmml::vec3f(210.0f, 0.0f, 0.0f), vmml::vec3f(0.8f, 0.0f, 0.0f), 100.0f, 0.5f);
	bRenderer().createLight("torchLight", bRenderer().getCamera("camera")->getPosition(), vmml::vec3f(1.0f, 0.4f, -0.5f), 1200.0f, 0.7f);

	bRenderer().createLight("staticTorchLight", vmml::vec3f(0.0f, 0.0f, 0.0f), vmml::vec3f(1.0f, 0.4f, -0.5f), 1200.0f, 0.7f);

	// set ambient color
	bRenderer().setAmbientColor(vmml::vec3f(0.0f, 0.0f, 0.05f));

	/* Windows only: Mouse Movement */
#ifdef OS_DESKTOP
	glfwSetInputMode(bRenderer().getView()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(bRenderer().getView()->getWindow(), &mouseX, &mouseY);
#endif
}

/* Draw your scene here */
void ProjectMain::loopFunction(const double &deltaTime, const double &elapsedTime)
{
//	bRenderer::log("deltaTime: "+lexical_cast< std::string >(deltaTime)+", elapsedTime: "+lexical_cast< std::string >(elapsedTime));
	bRenderer::log("FPS: "+lexical_cast< std::string >(1/deltaTime));
	if (((int)elapsedTime % 3) >= 1)
	{
		/* Test something after 3 seconds*/

	}

	////// Movement ////
	/* Windows only: Mouse and Keyboard Movement */
#ifdef OS_DESKTOP
	double xpos, ypos;
	glfwGetCursorPos(bRenderer().getView()->getWindow(), &xpos, &ypos);
	double deltaCameraX = xpos - mouseX;
	mouseX = xpos;
	double deltaCameraY = ypos - mouseY;
	mouseY = ypos;

	if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 			cameraForward = 1.0;		
		else			cameraForward = 0.5;
	else if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 			cameraForward = -1.0;
		else			cameraForward = -0.5;
	else if (glfwGetKey(bRenderer().getView()->getWindow(), GLFW_KEY_F) == GLFW_PRESS)					bRenderer().getView()->setFullscreen(!bRenderer().getView()->isFullscreen());
	else
		cameraForward = 0.0;

#endif
	/* On iOS automatic movement (for now) */
#ifdef OS_IOS
	double deltaCameraX = -0.1 / deltaTime;
	double deltaCameraY = 0.0;
	cameraForward = 0.001 / deltaTime;
    
//    double deltaCameraX = 0.0;
//    double deltaCameraY = 0.0;
//    cameraForward = 0.0;
#endif
	cameraRotationX += deltaCameraX / 1000;
	cameraRotationY += deltaCameraY / 1000;

	////// Camera ////
	bRenderer().getCamera("camera")->moveCamera(cameraForward);
	bRenderer().getCamera("camera")->rotateCamera(deltaCameraY / 1000, vmml::vec3f::UNIT_Z);
	bRenderer().getCamera("camera")->rotateCamera(deltaCameraX / 1000, vmml::vec3f::UNIT_Y);

	///// Perspective ////
	// adjust aspect ratio
	bRenderer().getCamera("camera")->setAspectRatio(bRenderer().getView()->getAspectRatio());
	bRenderer().getCamera("static camera")->setAspectRatio(bRenderer().getView()->getAspectRatio());

	//// Torch Light ////
	if (deltaTime < 0.5){
		randomTime += deltaTime + randomNumber(0.0, 0.12);
	}
	float flickeringLight = 1.0 + (randomTime)* 2 * M_PI_F*(0.032);
	float flickeringLightPosX = bRenderer().getCamera("camera")->getPosition().x();
	float flickeringLightPosY = bRenderer().getCamera("camera")->getPosition().y();
	float flickeringLightPosZ = bRenderer().getCamera("camera")->getPosition().z();
	flickeringLightPosX += 2.5*sin(flickeringLightPosY * 4.0 + 3.0*flickeringLight);
	flickeringLightPosY += 2.5*sin(flickeringLightPosX * 4.0  + 3.0*flickeringLight);
	bRenderer().getLight("torchLight")->setPosition(vmml::vec3f(flickeringLightPosX, flickeringLightPosY, flickeringLightPosZ) + bRenderer().getCamera("camera")->getOrientation()*20.0);

	//// Draw Models ////

	/*** Cave Start ***/
	// translate and scale using the matrix stack
	bRenderer().getMatrixStack("caveStartStack")->pushTranslation(vmml::create_translation(vmml::vec3f(100.0, -80.0, 0.0)));
	bRenderer().getMatrixStack("caveStartStack")->pushScaling(vmml::create_scaling(vmml::vec3f(0.3f)));
	// draw cave using standard shader
	//bRenderer().drawModel("cave_start", "camera", "caveStartStack");
	// draw without static torch light
	bRenderer().drawModel("cave_start", "camera", "caveStartStack", std::vector<std::string>({ "mainLight", "torchLight", "secondLight", "thirdLight" }));
	bRenderer().getMatrixStack("caveStartStack")->clearMatrixStack();

	/*** Crystal (blue) ***/
	// translate and scale using the matrix stack
	bRenderer().getMatrixStack("crystalStack")->pushTranslation(vmml::create_translation(vmml::vec3f(780.0, -170.0, 55.0)));
	bRenderer().getMatrixStack("crystalStack")->pushScaling(vmml::create_scaling(vmml::vec3f(0.1f)));
	// draw without static torch light
	bRenderer().setAmbientColor(vmml::vec3f(0.2, 0.2, 0.8));
	bRenderer().drawModel("crystal", "camera", "crystalStack", std::vector<std::string>({ "mainLight", "torchLight", "secondLight", "thirdLight" }));
	bRenderer().getMatrixStack("crystalStack")->clearMatrixStack();
	bRenderer().setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR);

	/*** Crystal (green) ***/
	// translate and scale using the matrix stack
	bRenderer().getMatrixStack("crystalStack")->pushTranslation(vmml::create_translation(vmml::vec3f(1480.0, -170.0, 70.0)));
	bRenderer().getMatrixStack("crystalStack")->pushScaling(vmml::create_scaling(vmml::vec3f(0.1f)));
	// draw without static torch light
	bRenderer().setAmbientColor(vmml::vec3f(0.1, 0.45, 0.1));
	bRenderer().drawModel("crystal", "camera", "crystalStack", std::vector<std::string>({ "mainLight", "torchLight", "secondLight", "thirdLight" }));
	bRenderer().getMatrixStack("crystalStack")->clearMatrixStack();
	bRenderer().setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR);

	/*** Crystal (red) ***/
	// translate and scale using the matrix stack
	bRenderer().getMatrixStack("crystalStack")->pushTranslation(vmml::create_translation(vmml::vec3f(2180.0, -170.0, 40.0)));
	bRenderer().getMatrixStack("crystalStack")->pushScaling(vmml::create_scaling(vmml::vec3f(0.1f)));
	// draw without static torch light
	bRenderer().setAmbientColor(vmml::vec3f(0.6, 0.1, 0.1));
	bRenderer().drawModel("crystal", "camera", "crystalStack", std::vector<std::string>({ "mainLight", "torchLight", "secondLight", "thirdLight" }));
	bRenderer().getMatrixStack("crystalStack")->clearMatrixStack();
	bRenderer().setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR);

	/*** Torch ***/
	//// translate and scale using the matrix stack
	//bRenderer().getMatrixStack("torchStack")->pushScaling(vmml::create_scaling(vmml::vec3f(1.3f)));	
	//bRenderer().getMatrixStack("torchStack")->pushTranslation(vmml::create_translation(vmml::vec3f(-1.04, -1.2, 0.8)));
	////bRenderer().getMatrixStack("torchStack")->pushRotation(vmml::create_rotation(cameraRotationY, vmml::vec3f::UNIT_Z));
	////bRenderer().getMatrixStack("torchStack")->pushRotation(vmml::create_rotation(cameraRotationX, vmml::vec3f::UNIT_Y));		
	//bRenderer().getMatrixStack("torchStack")->pushRotation(bRenderer().getCamera("camera")->getRotation());
	//bRenderer().getMatrixStack("torchStack")->pushTranslation(vmml::create_translation(bRenderer().getCamera("camera")->getPosition()));
	//// draw torch using standard shader
	//bRenderer().drawModel("torch", "camera", "torchStack");

	Model::GroupMap &groupsTorch = bRenderer().getModel("torch")->getGroups();
	for (auto i = groupsTorch.begin(); i != groupsTorch.end(); ++i)
	{
		Geometry &geometry = i->second;
		MaterialPtr material = geometry.getMaterial();
		ShaderPtr shader = material->getShader();
		if (shader)
		{
			// translate and scale using the matrix stack
			bRenderer().getMatrixStack("torchStack")->pushScaling(vmml::create_scaling(vmml::vec3f(1.3f)));
			bRenderer().getMatrixStack("torchStack")->pushTranslation(vmml::create_translation(vmml::vec3f(-1.04, -1.2, 0.84)));

			//get model matrix from stack
			vmml::mat4f modelMatrix = bRenderer().getMatrixStack("torchStack")->getModelMatrix();

			//VIEW MATRIX
			vmml::mat4f viewMatrix = bRenderer().getCamera("static camera")->getViewMatrix();


			shader->setUniform("ProjectionMatrix", bRenderer().getCamera("static camera")->getProjectionMatrix());
			shader->setUniform("ViewMatrix", viewMatrix);
			shader->setUniform("ModelMatrix", modelMatrix);

			//LIGHT
			shader->setUniform("LightPos", bRenderer().getLight("staticTorchLight")->getPosition());
			shader->setUniform("LightColor", bRenderer().getLight("staticTorchLight")->getColor());
			shader->setUniform("lightIntensity", bRenderer().getLight("staticTorchLight")->getIntensity());
			shader->setUniform("attenuation", bRenderer().getLight("staticTorchLight")->getAttenuation());
			shader->setUniform("flickeringLight", flickeringLight*8.0);

			shader->setUniform("Id", vmml::vec3f::ONE);
			shader->setUniform("Ia", vmml::vec3f::ONE);
		}
		else
		{
			bRenderer::log("No shader available.", bRenderer::LM_WARNING);
		}
		geometry.draw();
	}
	bRenderer().getMatrixStack("torchStack")->clearMatrixStack();
    
    /*** Flame ***/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	Model::GroupMap &groupsParticle = bRenderer().getModel("flame")->getGroups();
    for (auto i = groupsParticle.begin(); i != groupsParticle.end(); ++i)
    {
        Geometry &geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader)
        {
            for (float z = 0.0; z < 3.0; z++) {
                
                if(z==1.0)
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                
				vmml::mat4f translation = vmml::create_translation(vmml::vec3f(0.65 / bRenderer().getView()->getAspectRatio(), 0.6 + (0.08*z), (-z / 100.0 - 0.50)));
                
                float rot = 0.0;
                if(fmod(z, 2.0) == 0){
                    rot = 0;
                }else{
                    rot = M_PI_F;
                }
                
                vmml::mat4f rotation = vmml::create_rotation(rot, vmml::vec3f::UNIT_Z);
                
                float ParticleScale = 2.45-(0.46*z);                
				vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(ParticleScale / bRenderer().getView()->getAspectRatio(), ParticleScale, ParticleScale));
                
                vmml::vec3f eyePos(0, 0, 0.25);
                vmml::vec3f eyeUp = vmml::vec3f::UP;
				vmml::mat4f viewMatrix = bRenderer().lookAt(eyePos, vmml::vec3f::ZERO, eyeUp);
                
                vmml::mat4f modelMatrix(translation * scaling * rotation);

                //wave effect
                float uniform_offset = (randomTime+0.3*z)*2*M_PI_F*(0.75+0.5*z);
                float transparency = 1.0;
                if(z==0.0)transparency = 0.8;
                
                shader->setUniform("ProjectionMatrix", vmml::mat4f::IDENTITY);
                shader->setUniform("ViewMatrix", viewMatrix);
                shader->setUniform("ModelMatrix", modelMatrix);
                shader->setUniform("offset", uniform_offset);
                shader->setUniform("transparency", transparency);
                
                geometry.draw();
            }
        }
        else
        {
			bRenderer::log("No shader available.", bRenderer::LM_WARNING);
        }
    }
    
    
    /*** Sparks ***/
	Model::GroupMap &groupsSparks = bRenderer().getModel("sparks")->getGroups();
    for (auto i = groupsSparks.begin(); i != groupsSparks.end(); ++i)
    {
        Geometry &geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader)
        {
            for (float z = 1.0; z < 2.0; z++) 
			{                
				vmml::mat4f translation = vmml::create_translation(vmml::vec3f(0.65 / bRenderer().getView()->getAspectRatio(), 0.65, (-z / 100.0 - 0.58)));
                
                float rot;
                rot = randomNumber(1.0, 1.1)*randomTime*(z+0.3)*M_PI_F;
                
                vmml::mat4f rotation = vmml::create_rotation(rot, vmml::vec3f::UNIT_Z);
                
                float ParticleScale = 1.1-(0.5*z);                
				vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(ParticleScale / bRenderer().getView()->getAspectRatio(), 4.0*ParticleScale, ParticleScale));
                
                vmml::vec3f eyePos(0, 0, 0.25);
                vmml::vec3f eyeUp = vmml::vec3f::UP;
				vmml::mat4f viewMatrix = bRenderer().lookAt(eyePos, vmml::vec3f::ZERO, eyeUp);
                
                vmml::mat4f modelMatrix(translation * scaling * rotation);
                                
                shader->setUniform("ProjectionMatrix", vmml::mat4f::IDENTITY);
                shader->setUniform("ViewMatrix", viewMatrix);
                shader->setUniform("ModelMatrix", modelMatrix);
                
                geometry.draw();
            }  
        }
        else
        {
			bRenderer::log("No shader available.", bRenderer::LM_WARNING);
        } 
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*** Title ***/
	Model::GroupMap &groupsTitle = bRenderer().getModel("bTitle")->getGroups();
	for (auto i = groupsTitle.begin(); i != groupsTitle.end(); ++i)
	{
		Geometry &geometry = i->second;
		MaterialPtr material = geometry.getMaterial();
		ShaderPtr shader = material->getShader();
		if (shader)
		{
			vmml::mat4f translation = vmml::create_translation(vmml::vec3f(-0.4, 0, -0.65));

			float titleScale = 0.4;
			vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(titleScale / bRenderer().getView()->getAspectRatio(), titleScale, titleScale));

			vmml::vec3f eyePos(0, 0, 0.25);
			vmml::vec3f eyeUp = vmml::vec3f::UP;
			vmml::mat4f viewMatrix = bRenderer().lookAt(eyePos, vmml::vec3f::ZERO, eyeUp);

			vmml::mat4f modelMatrix(translation * scaling);

			shader->setUniform("ProjectionMatrix", vmml::mat4f::IDENTITY);
			shader->setUniform("ViewMatrix", viewMatrix);
			shader->setUniform("ModelMatrix", modelMatrix);
		}
		else
		{
			bRenderer::log("No shader available.", bRenderer::LM_WARNING);
		}
		geometry.draw();
	}

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
//        //////////TEST!!!!!
//        bRenderer().terminateRenderer();
	}
}

/* For iOS only: Handle app coming back from background */
void ProjectMain::appDidBecomeActive()
{
	if (bRenderer().isInitialized()){
		// run the renderer as soon as the app is active
		bRenderer().runRenderer();
	}
//    else{
//        bRenderer().initRenderer(true);
//        bRenderer().runRenderer();
//    }
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