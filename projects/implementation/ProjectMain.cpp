#include "ProjectMain.h"


/* Initialize the Project */
void ProjectMain::init()
{
	// set this instance of RenderProject to be used for function calls
	bRenderer.setRenderProject(this);

	// let the renderer create an OpenGL context and the main window
	bRenderer.initRenderer(1024, 768, false);

    // Test second view
    //View v;
    //v.initView(200, 200, false);

	// start main loop 
	bRenderer.runRenderer();
}

/* This function is executed when initializing the renderer */
void ProjectMain::initFunction()
{
	bRenderer::log("my initialize function was started");

	// load models
	bRenderer.loadModel("cave_start.obj", true, true);
	bRenderer.loadModel("torch.obj", false, true);
	bRenderer.loadModel("flame.obj", false, true);
	bRenderer.loadModel("sparks.obj", false, true);
	bRenderer.loadModel("menu.obj", false, true);

	//MatrixStack for the cave
	bRenderer.createMatrixStack("caveStartStack");

	//MatrixStack for torch
	bRenderer.createMatrixStack("torchStack");

	// initialize variables
	randomTime = 0.0f;

	// initialize free moving camera
	cameraForward = 0.0f;
	cameraRotation = M_PI_F;
	bRenderer.createCamera("camera", vmml::vec3f(0, 0, 0), vmml::vec3f(vmml::create_rotation(-cameraRotation, vmml::vec3f::UNIT_Y)));

	// initialize static camera
	bRenderer.createCamera("static camera", vmml::vec3f(0, 0, 0), vmml::vec3f(vmml::create_rotation(-cameraRotation, vmml::vec3f::UNIT_Y)));

	// get shading language version
	bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

/* Draw your scene here */
void ProjectMain::loopFunction(const double deltaTime, const double elapsedTime)
{

	//if (((int)elapsedTime % 3) >= 1)
	//{
	//	/* Test something after 3 seconds*/
	//
	//}

	//// Camera ////
	cameraForward = 0.01;
	cameraRotation += 0.001;
	bRenderer.getCamera("camera")->moveCamera(cameraForward);
	bRenderer.getCamera("camera")->rotateCamera(vmml::vec3f::UNIT_Y, cameraRotation);

	///// Perspective ////
	// adjust aspect ratio
	bRenderer.getCamera("camera")->setAspectRatio(bRenderer.getView()->getAspectRatio());
	// get projection matrix
	vmml::mat4f projectionMatrix = bRenderer.getCamera("camera")->getProjectionMatrix();

	//// Camera Light (Torch) ////
	if (deltaTime < 0.5){
		randomTime += deltaTime + randomNumber(0.0, 0.12);
	}
	vmml::vec4f lightPos = vmml::vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	vmml::vec4f lightColor = vmml::vec4f(0.5f, 0.5f, 1.0f, 1.0f);
	float lightIntensity = 400.0f;
	float attenuation = 1.0f;
	//Flickering Light
	float flickeringLight = (randomTime)* 2 * M_PI_F*(0.032);

	//// Draw Models ////

	/*** CAVE Start ***/
	Model::GroupMap &groupsCaveStart = bRenderer.getModel("cave_start")->getGroups();
	for (auto i = groupsCaveStart.begin(); i != groupsCaveStart.end(); ++i)
	{
		Geometry &geometry = i->second;
		MaterialPtr material = geometry.getMaterial();
		ShaderPtr shader = material->getShader();
		if (shader)
		{
			// translate and scale using the matrix stack
			bRenderer.getMatrixStack("caveStartStack")->pushTranslation(vmml::create_translation(vmml::vec3f(100.0, -80.0, 0.0)));
			bRenderer.getMatrixStack("caveStartStack")->pushScaling(vmml::create_scaling(vmml::vec3f(0.25f)));

			//get model matrix from stack
			vmml::mat4f modelMatrix = bRenderer.getMatrixStack("caveStartStack")->getModelMatrix();
			
			//VIEW MATRIX
			vmml::mat4f viewMatrix = bRenderer.getCamera("camera")->getViewMatrix();
			
			shader->setUniform("ProjectionMatrix", projectionMatrix);
			shader->setUniform("ViewMatrix", viewMatrix);
			shader->setUniform("ModelMatrix", modelMatrix);

			//LIGHT
			shader->setUniform("LightPos", lightPos);
			shader->setUniform("LightColor", lightColor);
			shader->setUniform("lightIntensity", lightIntensity);
			shader->setUniform("attenuation", attenuation);
			shader->setUniform("flickeringLight", flickeringLight);

			shader->setUniform("Id", vmml::vec3f::ONE);
			shader->setUniform("Ia", vmml::vec3f::ONE);
		}
		else
		{
			bRenderer::log("No shader available.", bRenderer::LM_WARNING);
		}
		geometry.draw();
	}

	/*** Torch ***/
	Model::GroupMap &groupsTorch = bRenderer.getModel("torch")->getGroups();
	for (auto i = groupsTorch.begin(); i != groupsTorch.end(); ++i)
	{
		Geometry &geometry = i->second;
		MaterialPtr material = geometry.getMaterial();
		ShaderPtr shader = material->getShader();
		if (shader)
		{
			// translate and scale using the matrix stack
			bRenderer.getMatrixStack("torchStack")->pushTranslation(vmml::create_translation(vmml::vec3f(0.5, -1.05, -0.87)));
			bRenderer.getMatrixStack("torchStack")->pushScaling(vmml::create_scaling(vmml::vec3f(2.4f)));

			//get model matrix from stack
			vmml::mat4f modelMatrix = bRenderer.getMatrixStack("torchStack")->getModelMatrix();
			//get normal matrix from stack
			vmml::mat4f normalMatrix = bRenderer.getMatrixStack("torchStack")->getNormalMatrix();

			//VIEW MATRIX
			vmml::mat4f viewMatrix = bRenderer.getCamera("static camera")->getViewMatrix();


			shader->setUniform("ProjectionMatrix", projectionMatrix);
			shader->setUniform("ViewMatrix", viewMatrix);
			shader->setUniform("ModelMatrix", modelMatrix);

			//LIGHT
			shader->setUniform("LightPos", lightPos);
			shader->setUniform("LightColor", lightColor);
			shader->setUniform("lightIntensity", lightIntensity);
			shader->setUniform("attenuation", attenuation);
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
    
    /*** Flame ***/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	Model::GroupMap &groupsParticle = bRenderer.getModel("flame")->getGroups();
    for (auto i = groupsParticle.begin(); i != groupsParticle.end(); ++i)
    {
        Geometry &geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader)
        {
            //for-loop if I decide that more than one flame would look better and the performance wouldn't suffer too much
            for (float z = 0.0; z < 3.0; z++) {
                
                if(z==1.0)
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                
				vmml::mat4f translation = vmml::create_translation(vmml::vec3f(0.65 / bRenderer.getView()->getAspectRatio(), 0.6 + (0.08*z), (-z / 100.0 - 0.50)));
                
                float rot = 0.0;
                if(fmod(z, 2.0) == 0){
                    rot = 0;
                }else{
                    rot = M_PI_F;
                }
                
                vmml::mat4f rotation = vmml::create_rotation(rot, vmml::vec3f::UNIT_Z);
                
                float ParticleScale = 2.45-(0.46*z);
                
				vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(ParticleScale / bRenderer.getView()->getAspectRatio(), ParticleScale, ParticleScale));
                
                vmml::vec3f eyePos(0, 0, 0.25);
                vmml::vec3f eyeUp = vmml::vec3f::UP;
				vmml::mat4f viewMatrix = bRenderer.lookAt(eyePos, vmml::vec3f::ZERO, eyeUp);
                
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
	Model::GroupMap &groupsSparks = bRenderer.getModel("sparks")->getGroups();
    for (auto i = groupsSparks.begin(); i != groupsSparks.end(); ++i)
    {
        Geometry &geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader)
        {
            //for-loop if I decide that more than one flame would look better and the performance wouldn't suffer too much
            for (float z = 1.0; z < 2.0; z++) {
                
				vmml::mat4f translation = vmml::create_translation(vmml::vec3f(0.65 / bRenderer.getView()->getAspectRatio(), 0.65, (-z / 100.0 - 0.58)));
                
                float rot;
                rot = randomNumber(1.0, 1.1)*randomTime*(z+0.3)*M_PI_F;
                
                vmml::mat4f rotation = vmml::create_rotation(rot, vmml::vec3f::UNIT_Z);
                
                float ParticleScale = 1.1-(0.5*z);
                
				vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(ParticleScale / bRenderer.getView()->getAspectRatio(), 4.0*ParticleScale, ParticleScale));
                
                vmml::vec3f eyePos(0, 0, 0.25);
                vmml::vec3f eyeUp = vmml::vec3f::UP;
				vmml::mat4f viewMatrix = bRenderer.lookAt(eyePos, vmml::vec3f::ZERO, eyeUp);
                
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

	/*** MENU ***/
	Model::GroupMap &groupsMenu = bRenderer.getModel("menu")->getGroups();
	for (auto i = groupsMenu.begin(); i != groupsMenu.end(); ++i)
	{
		Geometry &geometry = i->second;
		MaterialPtr material = geometry.getMaterial();
		ShaderPtr shader = material->getShader();
		if (shader)
		{

			vmml::mat4f translation = vmml::create_translation(vmml::vec3f(0, 0, -0.65));

			float menuScale = 0.00132;
			vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(menuScale, bRenderer.getView()->getAspectRatio()*menuScale, menuScale));

			vmml::vec3f eyePos(0, 0, 0.25);
			vmml::vec3f eyeUp = vmml::vec3f::UP;
			vmml::mat4f viewMatrix = bRenderer.lookAt(eyePos, vmml::vec3f::ZERO, eyeUp);

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
    // set view to fullscreen after device rotation
	bRenderer.getView()->setFullscreen(true);
	bRenderer::log("Device rotated");
}

/* For iOS only: Handle app going into background */
void ProjectMain::appWillResignActive()
{
    // stop the renderer when the app isn't active
	bRenderer.stopRenderer();
}

/* For iOS only: Handle app coming back from background */
void ProjectMain::appDidBecomeActive()
{
    // run the renderer as soon as the app is active
	bRenderer.runRenderer();
}

/* For iOS only: Handle app being terminated */
void ProjectMain::appWillTerminate()
{
    // terminate renderer before the app is closed
	bRenderer.terminateRenderer();
}

/* Helper functions */
float ProjectMain::randomNumber(float min, float max){
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}