
#import "AppDelegate.h"
#import "ViewController.h"
#import "BView.h"

@implementation AppDelegate

@synthesize window;
@synthesize glView;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    ViewController *viewController = [[ViewController alloc] init];
    [self.window setRootViewController:viewController];
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // stop the renderer when the app isn't active
    [glView stopRenderer];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // run the renderer as soon as the app is active
    [glView runRenderer];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // terminate renderer before the app is closed
    [glView terminateRenderer];
}

@end
