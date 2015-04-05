
#import <QuartzCore/QuartzCore.h>

#import "BView.h"
#include "../../bRenderer.h"
#include "bLinkToView.h"


@interface BView (PrivateMethods)
/* Private methods */
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end


@implementation BView

/* Generate getter and setter for the context */
@synthesize context;

/* Set layer class to CAEAGLLayer */
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

/* This function is used to initialize the view when using a XIB */
- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    return [self bInit];
}

/* This function is used to initialize the view when setting the frame manually within code */
- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
    return [self bInit];
}

/* Initialize OpenGL */
- (id)bInit
{
    if (!self) {
        return nil;
    }
    
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    // set the surface to not be transparent
    eaglLayer.opaque = TRUE;
    
    // configure the properties of the canvas
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                    kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                    nil];
    
    // create an OpenGL ES 2.0 context
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    // quit if context creation failed
    if (!context || ![EAGLContext setCurrentContext:context]) {
        bRenderer::log("Could not create context!", bRenderer::LM_SYS);
        return nil;
    }
    
    
    //default values for our OpenGL buffers
    defaultFramebuffer = 0;
    colorRenderbuffer = 0;
    depthRenderbuffer = 0;
    
    // initial time is not set at this point because the renderer hasn't fully started yet
    initialTime = -1.0;
    
    return self;
}

/* Create frame and renderbuffer and to display in the view */
- (void)createFramebuffer
{
    // make sure the default framebuffer hasn't been declared yet
    assert(defaultFramebuffer == 0);
    
    // create default framebuffer object and bind it
    glGenFramebuffers(1, &defaultFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    
    // create color render buffer
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    // get the storage from iOS so it can be displayed in the view
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    // get the frame's width and height
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
    
    // attach the color buffer to the framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    // pass width and height of the view to the renderer
    bRenderer::setWindowSize(framebufferWidth, framebufferHeight);

    // create a depth renderbuffer
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    // create the storage for the buffer, optimized for depth values, same size as the colorRenderbuffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, framebufferWidth, framebufferHeight);
    // attach the depth buffer to the framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    // check that the configuration of the framebuffer is valid
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        bRenderer::log("Failed to make complete framebuffer object", bRenderer::LM_SYS);
}

/* Deletion of the framebuffer and all the buffers it contains */
- (void)deleteFramebuffer
{
    // to access any OpenGL methods a valid and current context is needed
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer) {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }

        if (colorRenderbuffer) {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
        
        if (depthRenderbuffer) {
            glDeleteRenderbuffers(1, &depthRenderbuffer);
            depthRenderbuffer = 0;
        }
    }
}

/* This methdod is called every time the view is redrawn (ideally 60 times a secod)*/
- (void)render
{
    // handle time
    if(initialTime < 0)
        initialTime = (int)[displayLink timestamp];
    
    bRenderer::setTime(([displayLink timestamp] - initialTime));

    if (context != nil)
    {
		// only render if the renderer is supposed to be running
		if(bRenderer::isRunning()){
			// set current context
			[EAGLContext setCurrentContext:context];
        
			if (!defaultFramebuffer)
				[self createFramebuffer];
        
			glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
			bRenderer::render();
        
			// display the color buffer to the screen
			glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
			[context presentRenderbuffer:GL_RENDERBUFFER];
		}
        else{
            [self stopRenderer];
        }
    }
    else
        bRenderer::log("Context not set!", bRenderer::LM_SYS);
}

- (void)runRenderer
{
    // check whether the loop is already running
    if(displayLink == nil)
    {
        // specify render method
        displayLink = [self.window.screen displayLinkWithTarget:self selector:@selector(render)];
        
        // add the display link to the run loop (will be called 60 times per second)
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        bRenderer::runRenderer();
    }
}

- (void)stopRenderer
{
    if (displayLink != nil) {
        // if the display link is present, it gets invalidated (loop stops)
        [displayLink invalidate];
        displayLink = nil;
        
        bRenderer::stopRenderer();
    }
}

- (void)terminateRenderer
{
    [self stopRenderer];
    bRenderer::terminateRenderer();
}

/* As soon as the view is resized or new subviews are added, this method is called,
 * apparently the framebuffers are invalid in this case so they are deleted and recreated 
 * when entering the render loop again */
- (void)layoutSubviews
{
    [self deleteFramebuffer];
}

- (void)dealloc
{
    [self deleteFramebuffer];
}


@end
