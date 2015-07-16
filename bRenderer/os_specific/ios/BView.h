#ifndef B_BVIEW_H
#define B_BVIEW_H

#import <UIKit/UIKit.h>

#include "../../headers/Renderer_GL.h"
#include "Touch.h"

@class EAGLContext;

/* This is a UIView specifically designed for bRenderer and needs to be added to a window in your project */
@interface BView : UIView
{
	@private
    GLint _width;
	GLint _height;

	// the time when the app was started
	double _initialTime, _stopTime;
    bool _wasStopped;

	GLuint _defaultFramebuffer, _colorRenderbuffer, _depthRenderbuffer;

	// the display link is used to create a render loop
	CADisplayLink *_displayLink;
	CAEAGLLayer *_eaglLayer;
    
    // OpenGL context
    EAGLContext *_context;
    
    // Touch handling
    bool _singleTapRecognized;
    bool _doubleTapRecognized;
    CGPoint _lastDoubleTapLocation;
    CGPoint _lastSingleTapLocation;
    TouchMap _touches;
    
}

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

-(TouchMap)getTouches;
-(bool)singleTapRecognized;
-(bool)doubleTapRecognized;
-(Touch)getLastSingleTapLocation;
-(Touch)getLastDoubleTapLocation;

//-(void)setLink:(BViewLink)link;
@end

#endif /* defined(B_BVIEW_H) */