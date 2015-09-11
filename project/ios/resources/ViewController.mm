
#import "ViewController.h"
#include "RenderProject.h"


@interface ViewController () {
@private
    RenderProject p;
}

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // initialize user application
    p.init();
    
    // There are two ways to display the view of the renderer in your project:
//    [self.view addSubview:p.getProjectRenderer().getView()->getUIView()];    // method 1
    p.getProjectRenderer().getView()->attachToUIView(self.view);   // method 2
}


-(BOOL) shouldAutorotate
{
    // app doesn't adjust view correctly if this function doesn't return "yes"
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    p.deviceRotated();
}

- (void)appWillResignActive
{
    // pass event to c++ application
    p.appWillResignActive();
}

- (void)appDidBecomeActive
{
    // pass event to c++ application
    p.appDidBecomeActive();
}

- (void)appWillTerminate
{
    // pass event to c++ application
    p.appWillTerminate();
}

@end
