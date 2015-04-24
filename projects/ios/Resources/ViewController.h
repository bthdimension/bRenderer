#pragma once

#import <UIKit/UIKit.h>


@interface ViewController : UIViewController

- (void)appWillResignActive;
- (void)appDidBecomeActive;
- (void)appWillTerminate;
@end
