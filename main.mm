#include <AvailabilityMacros.h>
#import <Cocoa/Cocoa.h>
#include <stdio.h>
#include "PGLLayer.hpp"

@class CALayer;

@interface AppDelegate : NSObject
#if MAC_OS_X_VERSION_MIN_REQUIRED >= 1060
<NSApplicationDelegate>
#endif
{
    NSWindow* _window;
    NSView* _layerHostView;
    CALayer* _layer;
}
@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSView *layerHostView;
@property (assign, nonatomic) CALayer *layer;
@end

@implementation AppDelegate

@synthesize window = _window;
@synthesize layerHostView = _layerHostView;
@synthesize layer = _layer;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    NSUInteger windowStyle = (NSTitledWindowMask  | NSClosableWindowMask | NSResizableWindowMask);
    NSRect windowRect = NSMakeRect(100, 100, 800, 600);

    self.window = [[NSWindow alloc] initWithContentRect:windowRect styleMask:windowStyle backing:NSBackingStoreBuffered defer:NO];
    self.layerHostView = [[NSView alloc] initWithFrame:[[self.window contentView] bounds]];

    [self.layerHostView setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
    if ([self.layerHostView respondsToSelector:@selector(setWantsBestResolutionOpenGLSurface:)]) {
        [self.layerHostView setWantsBestResolutionOpenGLSurface:YES];
    }

    [self.window setTitle:@"Uru"];
    [self.window orderFrontRegardless];
    [self.window.contentView addSubview:self.layerHostView];

#if __has_feature(objc_arc)
    self.layer = (__bridge CALayer*)PGL::Layer::layer();
#else
    self.layer = (CALayer*)PGL::Layer::layer();
#endif
    [self.layer setAutoresizingMask:kCALayerWidthSizable|kCALayerHeightSizable];
    [self.layer setBackgroundColor:CGColorGetConstantColor(kCGColorBlack)];
    ((CAOpenGLLayer*)self.layer).asynchronous = YES;

    [self.layerHostView setLayer:self.layer];
    [self.layerHostView setWantsLayer:YES];

    [NSApp activateIgnoringOtherApps:YES];

    [self.layer setNeedsDisplay];
}
@end

int main() {
#if __has_feature(objc_arc)
    @autoreleasepool {
#else
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
#endif

    Class cls = objc_lookUpClass("PGLLayer");
    if (cls != nil) {
        printf("Found the class\n");
    }

    NSApplication * application = [NSApplication sharedApplication];
    AppDelegate *delegate = [AppDelegate new];
    application.delegate = delegate;
    [application run];

#if __has_feature(objc_arc)
    }
#else
    [pool drain];
#endif

    return 0;
}
