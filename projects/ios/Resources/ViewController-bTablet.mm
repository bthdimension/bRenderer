
#import "ViewController.h"
#include "ProjectMain.h"

@interface ViewController () {
@private
    ProjectMain p;
}

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // initialize user application
    p.init();
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
