#include "PipelineLayer.h"
#include "pfGLRenderLayer.hpp"
#include <objc/objc-runtime.h>
#include <OpenGL/gl3.h>
#import <QuartzCore/QuartzCore.h>
#include <cstdio>

Class PipelineLayer::LayerClass = nullptr;

static void drawInCGLContext_pixelFormat_forLayerTime_displayTime_(id self, SEL _cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const CVTimeStamp* ts)
{
    CGLSetCurrentContext(ctx);

    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    struct objc_super super = {
        self,
        class_getSuperclass(object_getClass(self))
    };
    ((void (*)(objc_super*, SEL, CGLContextObj, CGLPixelFormatObj, CFTimeInterval, const CVTimeStamp*))objc_msgSendSuper)(&super, sel_getUid("drawInCGLContext:pixelFormat:forLayerTime:displayTime:"), ctx, pf, t, ts);
}

void PipelineLayer::IInitLayerClass() {
    LayerClass = objc_allocateClassPair(objc_getClass("CAOpenGLLayer"), "PGLRenderLayer", sizeof(PipelineLayer*));

    Method meth = class_getInstanceMethod(objc_getClass("CAOpenGLLayer"), sel_getUid("drawInCGLContext:pixelFormat:forLayerTime:displayTime:"));
    const char* methodEnc = method_getTypeEncoding(meth);
    class_addMethod(LayerClass, sel_getUid("drawInCGLContext:pixelFormat:forLayerTime:displayTime:"), (IMP)drawInCGLContext_pixelFormat_forLayerTime_displayTime_, methodEnc);

    objc_registerClassPair(LayerClass);
}

void PipelineLayer::IInitLayer() {
    fLayer = ((CALayerType (*)(id,SEL))objc_msgSend)((id)LayerClass, sel_getUid("layer"));
    PipelineLayer** vars = (PipelineLayer**)object_getIndexedIvars(fLayer);
    vars[0] = this;
}

PipelineLayer::PipelineLayer() : fLayer(nullptr) {
    if (!PipelineLayer::LayerClass) {
        PipelineLayer::IInitLayerClass();
    }

    IInitLayer();
}

pfGLRenderLayer* PipelineLayer::getLayer() const {
    return pfGLRenderLayer::getLayer();
}
