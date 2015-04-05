
#import "ViewController.h"
#include "ProjectMain.h"


@interface ViewController () { }
@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    // initialize user application
    ProjectMain p;
}

-(BOOL) shouldAutorotate
{
    // app doesn't adjust view correctly if this function doesn't return "yes"
    return YES;
}

@end
