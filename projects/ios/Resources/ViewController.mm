
#import "ViewController.h"
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
    
    // initialize user application
    p.init();
    
    //TEST adding BView to a UIView of our choosing
//    UIView *newView = [[UIView alloc] initWithFrame:CGRectMake(100,100,600,600)];
//    newView.backgroundColor=[UIColor blueColor];
//    [[[[UIApplication sharedApplication] windows] objectAtIndex:0] addSubview:newView];
//    Renderer::get().getView()->attachToUIView(newView);
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
