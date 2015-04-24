
#import <UIKit/UIKit.h>

@class BView;

@interface AppDelegate : NSObject <UIApplicationDelegate> { }

@property (nonatomic, strong) IBOutlet UIWindow *window;
@property (nonatomic, strong) IBOutlet BView *bView;

@end

