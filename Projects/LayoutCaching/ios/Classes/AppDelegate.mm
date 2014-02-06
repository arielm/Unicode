#import "AppDelegate.h"
#import "GLViewController.h"

#include "Sketch.h"

@implementation AppDelegate

- (BOOL) application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    viewController = [[GLViewController alloc] initWithProperties:[NSDictionary dictionaryWithObjectsAndKeys:
        [NSNumber numberWithInt:kEAGLRenderingAPIOpenGLES1], kGLViewControllerPropertyRenderingAPI,
        [NSNumber numberWithInt:GLKViewDrawableDepthFormatNone], kGLViewControllerPropertyDepthFormat,
        [NSNumber numberWithBool:YES], kGLViewControllerPropertyMultipleTouchEnabled,
        [NSNumber numberWithInt:UIInterfaceOrientationMaskLandscapeRight], kGLViewControllerPropertyInterfaceOrientationMask,
         nil]];
    
    viewController.cinderDelegate = self;
    sketch = new Sketch(self);
    
    window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    window.backgroundColor = [UIColor blackColor];
    
    [window setRootViewController:viewController];
    [window makeKeyAndVisible];
    
    return YES;
}

- (void) dealloc
{
    [viewController release];
    [window release];
    
    [super dealloc];
}

@end

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
