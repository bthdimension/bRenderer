#ifndef B_BVIEW_H
#define B_BVIEW_H

#import <UIKit/UIKit.h>

#include "../../headers/Renderer_GL.h"

@class EAGLContext;

/* This is a UIView specifically designed for bRenderer and needs to be added to a window in your project */
@interface BView : UIView
{
	@private
    GLint width;
	GLint height;

	// the time when the app was started
	double initialTime, stopTime;
    bool wasStopped;

	GLuint defaultFramebuffer, colorRenderbuffer, depthRenderbuffer;

	// the display link is used to create a render loop
	CADisplayLink *displayLink;
	CAEAGLLayer *eaglLayer;
    
    // OpenGL context
    EAGLContext *context;
}

typedef BView* BViewRef;

/* Control the renderer from within Objective-C code using these functions */
-(void)runRenderer;
-(void)stopRenderer;

-(bool)isRunning;

-(int)getViewWidth;
-(int)getViewHeight;
-(int)getViewPositionX;
-(int)getViewPositionY;
-(double)getTime;

-(void)setFullscreen;
-(void)setViewWidth:(GLint)w setViewHeight : (GLint)h;
-(void)setViewPositionX:(GLint)x setViewPositionY : (GLint)y;
-(bool)setContextCurrent;

//-(void)setLink:(BViewLink)link;
@end

#endif /* defined(B_BVIEW_H) */