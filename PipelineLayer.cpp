#include "PipelineLayer.h"
#include <objc/objc-runtime.h>
#include <OpenGL/OpenGL.h>

PipelineLayer::LayerClass = nullptr;

static void drawInCGLContext_pixelFormat_forLayerTime_displayTime(id self, SEL _cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const CVTimeStamp* ts)
{
    CGLSetCurrentContext(ctx);

    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    struct objc_super super {
        self,
        class_getSuperclass(objc_getClass(self))
    };
    (void (*)(id, SEL, CGLContextObj, CGLPixelFormatObj, CFTimeInterval, const CVTimeStamp*))objc_msgSendSuper(super, sel_getUid("drawInCGLContext:pixelFormat:forLayerTime:displayTime"), ctx, pf, t, ts);
}

void PipelineLayer::IInitLayerClass() {
    LayerClass = objc_allocateClassPair(objc_getClass("CAOpenGLLayer"), "PGLRenderLayer", sizeof(PipelineLayer*));

    class_addMethod(LayerClass, sel_getUid("drawInCGLContext:pixelFormat:forLayerTime:displayTime"), (IMP)drawInCGLContext_pixelFormat_forLayerTime_displayTime, "v@:^{_CGLContextObject=}^{_CGLPixelFormatObject=}d^{_CVTimeStamp=IiqQdq{CVSMPTETime=ssIIIssss}QQ}");

    objc_registerClassPair(LayerClass);
}

void PipelineLayer::IInitLayer() {
    fLayer = class_createInstance(PipelineLayer::LayerClass, 0);
    PipelineLayer** vars = (PipelineLayer**)object_getIndexedIvars(fLayer);
    vars[0] = this;
}

PipelineLayer::PipelineLayer() : fLayer(nullptr) {
    if (!PipelineLayer::LayerClass) {
        PipelineLayer::IInitLayerClass();
    }

    IInitLayer();
}

PipelineLayer::CALayerType PipelineLayer::getLayer() const {
    return fLayer;
}
