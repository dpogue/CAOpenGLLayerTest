#import <Cocoa/Cocoa.h>
#include "PipelineLayer.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
    PipelineLayer* pipe;
}

@property (assign) IBOutlet NSWindow *window;
@property (strong) IBOutlet NSView *layerHostView;
@property (strong, nonatomic) CALayer *layer;
@end

@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    pipe = new PipelineLayer();

    NSUInteger windowStyle = (NSTitledWindowMask  | NSClosableWindowMask | NSResizableWindowMask);
    NSRect windowRect = NSMakeRect(100, 100, 800, 600);

    self.window = [[NSWindow alloc] initWithContentRect:windowRect styleMask:windowStyle backing:NSBackingStoreBuffered defer:NO];
    self.layerHostView = [[NSView alloc] initWithFrame:[[self.window contentView] bounds]];

    [self.layerHostView setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
    [self.layerHostView setWantsBestResolutionOpenGLSurface:YES];

    [self.window setTitle:@"Uru"];
    //[self.window makeMainWindow];
    [self.window orderFrontRegardless];
    [self.window.contentView addSubview:self.layerHostView];

    self.layer = (__bridge CALayer*)pipe->getLayer();
    [self.layer setAutoresizingMask:kCALayerWidthSizable|kCALayerHeightSizable];
    [self.layer setBackgroundColor:[NSColor blackColor].CGColor];

    [self.layerHostView setLayer:self.layer];
    self.layerHostView.wantsLayer = YES;

    [NSApp activateIgnoringOtherApps:YES];

    [self.layer setNeedsDisplay];
}
@end

int main(int argc, const char * argv[])
{
    @autoreleasepool {
        NSApplication * application = [NSApplication sharedApplication];
        AppDelegate *delegate = [AppDelegate new];
        application.delegate = delegate;
        [application run];
    }

    return 0;
}
