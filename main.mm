#import <Cocoa/Cocoa.h>
#include "PipelineLayer.h"

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate> {
    PipelineLayer* pipe;
}

@property (assign) IBOutlet NSWindow *window;
@property (weak) IBOutlet NSView *layerHostView;
@property (strong, nonatomic) CALayer *layer;
@end

@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    pipe = new PipelineLayer();

    self.layer = pipe->getLayer();
    self.layerHostView.wantsLayer = YES;
    self.layerHostView.layer = self.layer;
}
@end

int main(int argc, const char * argv[])
{
    return NSApplicationMain(argc, argv);
}
