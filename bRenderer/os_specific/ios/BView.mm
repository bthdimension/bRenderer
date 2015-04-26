
#import <QuartzCore/QuartzCore.h>

#import "BView.h"
#include "bLinkToView.h"
#include "../../bRenderer.h"


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
    
//    bRenderer::view = self;
    
    eaglLayer = (CAEAGLLayer *)self.layer;
    
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
    if (!context || ![self setContextCurrent]) {
        bRenderer::log("Could not create context!", bRenderer::LM_SYS);
        return nil;
    }
    
    // set view size to fullscreen
    [self setFullscreen];
    
    // default values for our OpenGL buffers
    defaultFramebuffer = 0;
    colorRenderbuffer = 0;
    depthRenderbuffer = 0;
    
    // initial time is not set at this point because the renderer hasn't fully started yet
    initialTime = -1.0;
    
    // create buffers
    [self createFramebuffer];

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
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
    // attach the color buffer to the framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    // create a depth renderbuffer
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    // create the storage for the buffer, optimized for depth values, same size as the colorRenderbuffer
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
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
        [self setContextCurrent];
        
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
    
//    bRenderer::passTime(([displayLink timestamp] - initialTime));

    if (context != nil)
    {
//		// only render if the renderer is supposed to be running
//		if(bRenderer::isRunning()){
			   
			if (!defaultFramebuffer)
				[self createFramebuffer];
        
			glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
			bRenderer::draw();
        
            // set current context
            [self setContextCurrent];
            
			// display the color buffer to the screen
			glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
			[context presentRenderbuffer:GL_RENDERBUFFER];
//		}
//        else{
//            [self stopRenderer];
//        }
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
    }
}

- (void)stopRenderer
{
    if (displayLink != nil) {
        // if the display link is present, it gets invalidated (loop stops)
        [displayLink invalidate];
        displayLink = nil;
    }
}

-(bool)isRunning
{
    return displayLink != nil;
}

- (int) getViewWidth
{
    return self.frame.size.width;
}

- (int) getViewHeight
{
    return self.frame.size.height;
}

- (int) getViewPositionX
{
    return self.center.x - [self getViewWidth]*0.5;
}

- (int) getViewPositionY
{
    return self.center.y - [self getViewHeight]*0.5;
}

-(double)getTime
{
    return ([displayLink timestamp] - initialTime);
}


/* Set view to fullscreen */
- (void) setFullscreen
{
    // set width and height to fullscreen
    width = [[UIScreen mainScreen] bounds].size.width;
    height = [[UIScreen mainScreen] bounds].size.height;
    
    [self setViewPositionX: 0 setViewPositionY: 0];
    
    CGRect newFrame = self.frame;
    newFrame.size.width = width;
    newFrame.size.height = height;
    [self setFrame:newFrame];
    
    // set viewport
    glViewport(0, 0, width , height);
}

/* Set width and height of the view */
- (void) setViewWidth:(GLint)w setViewHeight:(GLint)h
{
    width = w;
    height = h;
    CGRect newFrame = self.frame;
    newFrame.size.width = w;
    newFrame.size.height = h;
    [self setFrame:newFrame];
    
    // set viewport
    glViewport(0, 0, w , h);
}

/* Set x and y position of the view */
- (void) setViewPositionX:(GLint)x setViewPositionY:(GLint)y
{
    self.frame = CGRectMake(x, y, self.frame.size.width, self.frame.size.height);
}

-(bool)setContextCurrent
{
    return [EAGLContext setCurrentContext:context];
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
