
#include "ProjectMain.h"


using namespace std;
using namespace bRenderer;

/* variables */
float randomTime;
float menuSliderPosX;
float menuSliderPosY;
float maxMenuScrollX;
float minMenuScrollX;
float maxMenuScrollY;
double timePastSinceMenuManipulated;
bool sliderYisReset;
Camera staticCamera;


/* helper functions */
float randomNumber(float min, float max){
    float range = max - min;
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

vmml::mat4f lookAt(vmml::vec3f eye, vmml::vec3f target, vmml::vec3f up)
{
    vmml::vec3f zaxis = vmml::normalize(eye - target);
    vmml::vec3f xaxis = vmml::normalize(vmml::cross<3>(up, zaxis));
    vmml::vec3f yaxis = vmml::cross<3>(zaxis, xaxis);
    
    vmml::mat4f view;
    view.set_row(0, vmml::vec4f(xaxis.x(), xaxis.y(), xaxis.z(), -vmml::dot(xaxis, eye)));
    view.set_row(1, vmml::vec4f(yaxis.x(), yaxis.y(), yaxis.z(), -vmml::dot(yaxis, eye)));
    view.set_row(2, vmml::vec4f(zaxis.x(), zaxis.y(), zaxis.z(), -vmml::dot(zaxis, eye)));
    view.set_row(3, vmml::vec4f(0, 0, 0, 1.0));
    
    return view;
}

/* This function is executed when initializing the renderer */
void bInitialize()
{
    log("my initialize function was started");
    
    // load models
    loadModel("torch.obj", true, true);
    loadModel("menu.obj", true, true);
    
    // initialize variables
    randomTime = 0.0f;
    // menu
    menuSliderPosX = 0.0f;
    menuSliderPosY = 0.0f;
    maxMenuScrollX = 0.04f;
    minMenuScrollX = 0.005f;
    maxMenuScrollY = -0.05f;
    timePastSinceMenuManipulated = 0.0;
    sliderYisReset = true;
    
    // initialize camera
    staticCamera.moveCamera(0.0f);
    staticCamera.rotateCamera(vmml::vec3f::UNIT_Y, M_PI_F);
    
    // OpenGL stuff
    // clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    // for Alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
}

/* Draw your scene here */
void bLoop(const double deltaTime, const double elapsedTime)
{
    if (((int)elapsedTime % 2) >= 1)
    {
        /* Test something after 2 seconds*/
        glClearColor(0.0, 1.0, 0.0, 1.0);
    }
    else
    {
        glClearColor(1.0, 0.0, 0.0, 1.0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Projection Matrix, Perspective
    vmml::mat4f projectionMatrix = createPerspective(90.0f, getAspectRatio(), -1.0f, 1280.0f);
    
    
    //// Camera Light (Torch)
    if (deltaTime < 0.5){
        randomTime += deltaTime + randomNumber(0.0, 0.12);
    }
    vmml::vec4f lightPos = vmml::vec4f(0.0f, 0.0f, 0.0f, 1.0f);
    vmml::vec4f lightColor = vmml::vec4f(0.5f, 0.5f, 1.0f, 1.0f);
    float lightIntensity = 400.0f;
    float attenuation = 1.0f;
    //Flickering Light
    float flickeringLight = (randomTime)* 2 * M_PI_F*(0.032);
    
    /*** Torch ***/
    Model::GroupMap &groupsTorch = getModel("torch")->getGroups();
    for (auto i = groupsTorch.begin(); i != groupsTorch.end(); ++i)
    {
        Geometry &geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader)
        {
            
            vmml::mat4f modelMatrix = vmml::mat4f::IDENTITY;
            vmml::mat4f normalMatrix = vmml::mat4f::IDENTITY;
            
            //MatrixStack
            MatrixStack TorchStack;
            TorchStack.pushTranslation(vmml::create_translation(vmml::vec3f(0.5, -1.05, -0.87)));
            TorchStack.pushScaling(vmml::create_scaling(vmml::vec3f(2.4f)));
            
            //get Model Matrix from Stack
            modelMatrix = TorchStack.getModelMatrix();
            //get Normal Matrix from Stack
            normalMatrix = TorchStack.getNormalMatrix();
            
            //VIEW MATRIX
            vmml::mat4f viewMatrix = staticCamera.getViewMatrix();
            
            
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
            log("No shader available.", LM_WARNING);
        }
        geometry.draw();
    }
    
    /*** MENU ***/
    Model::GroupMap &groupsMenu = getModel("menu")->getGroups();
    for (auto i = groupsMenu.begin(); i != groupsMenu.end(); ++i)
    {
        Geometry &geometry = i->second;
        MaterialPtr material = geometry.getMaterial();
        ShaderPtr shader = material->getShader();
        if (shader)
        {
            
            menuSliderPosX = 0;

            
            vmml::mat4f translation = vmml::create_translation(vmml::vec3f(menuSliderPosX, menuSliderPosY, -0.65));
            //snap down menu
            if(menuSliderPosX >= -0.8)
                translation = vmml::create_translation(vmml::vec3f(0.0, menuSliderPosY, -0.65));
            
            float menuScale = 0.00132;
            vmml::mat4f scaling = vmml::create_scaling(vmml::vec3f(menuScale, getAspectRatio()*menuScale, menuScale));
            
            vmml::vec3f eyePos(0, 0, 0.25);
            vmml::vec3f eyeUp = vmml::vec3f::UP;
            vmml::mat4f viewMatrix = lookAt(eyePos, vmml::vec3f::ZERO, eyeUp);
            
            vmml::mat4f modelMatrix(translation * scaling);
            
            
            shader->setUniform("ProjectionMatrix", vmml::mat4f::IDENTITY);
            shader->setUniform("ViewMatrix", viewMatrix);
            shader->setUniform("ModelMatrix", modelMatrix);
        }
        else
        {
            log("No shader available.", LM_WARNING);
        }
        geometry.draw();
    }
}

/* This function is executed when terminating the renderer */
void bTerminate()
{
    log("I totally terminated this Renderer :-)");
}


ProjectMain::ProjectMain()
{
    // define the functions to be called by the renderer
    defineInitFunction(bInitialize);
    defineLoopFunction(bLoop);
    defineTerminateFunction(bTerminate);
    
    // let the renderer create an OpenGL context
    initRenderer();
}


ProjectMain::~ProjectMain()
{

}