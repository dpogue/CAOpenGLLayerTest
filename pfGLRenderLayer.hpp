#pragma once

#include <QuartzCore/QuartzCore.h>

class pfGLRenderLayer
{
public:
    struct objc_class* isa;

    static pfGLRenderLayer* getLayer();
    static struct objc_class* getClass() {
        return sRenderLayerClass;
    }

private:
    static struct objc_class* sRenderLayerClass;

    static void IInitClass();

    template<typename _Ret, typename... _Args>
    static _Ret sendMsg(const void* pObj, struct objc_selector* selector, _Args... args);

    template<typename _Ret, typename... _Args>
    _Ret sendMsg(struct objc_selector* selector, _Args... args);

    template<typename _Ret, typename... _Args>
    _Ret sendSuper(struct objc_selector* selector, _Args... args);

    pfGLRenderLayer() = delete;
    pfGLRenderLayer(const pfGLRenderLayer&) = delete;
    ~pfGLRenderLayer() = delete;
    pfGLRenderLayer& operator=(const pfGLRenderLayer&) = delete;

    static void drawInCGLContext_pixelFormat_forLayerTime_displayTime_(pfGLRenderLayer* self, struct obj_selector* cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const CVTimeStamp* ts);
};
