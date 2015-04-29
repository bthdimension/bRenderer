
#import "AppDelegate.h"
#import "ViewController.h"
#import "BView.h"

@interface AppDelegate () {
@private
    ViewController *viewController;
}

@end

@implementation AppDelegate

@synthesize window;
@synthesize bView;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
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
