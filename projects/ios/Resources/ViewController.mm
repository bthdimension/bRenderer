
#import "ViewController.h"
//#import "BView.h"
#include "ProjectMain.h"


@interface ViewController () {
@private
    ProjectMain p;
//    BView *bView;
}

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
//    CGRect frame = [[UIScreen mainScreen] applicationFrame];
//    bView = [[BView alloc] initWithFrame:frame];
//    [[[[UIApplication sharedApplication] windows] objectAtIndex:0] addSubview:bView];
    
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
