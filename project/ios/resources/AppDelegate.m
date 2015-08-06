
#import "AppDelegate.h"
#import "ViewController.h"

@interface AppDelegate () {
@private
    ViewController *viewController;
}

@end

@implementation AppDelegate

@synthesize window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // create window
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.backgroundColor = [UIColor blackColor];
    
    // create view controller
    viewController = [[ViewController alloc] init];
    
    [self.window setRootViewController:viewController];
    [self.window makeKeyAndVisible];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // pass event to view controller holding the c++ application
    [viewController appWillResignActive];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // pass event to view controller holding the c++ application
    [viewController appDidBecomeActive];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // pass event to view controller holding the c++ application
    [viewController appWillTerminate];
}

@end
