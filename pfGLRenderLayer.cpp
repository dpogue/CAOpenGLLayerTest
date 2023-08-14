#include "pfGLRenderLayer.hpp"
#include <objc/objc-runtime.h>
#include <functional>

#include <OpenGL/gl3.h>

#define _GL_CLS(name) (Private::Class::s_k##name)
#define _GL_SEL(name) (Private::Selector::s_k##name)

#define _GL_SUBCLASS_METHOD(name) \
    Method m_##name = class_getInstanceMethod(class_getSuperclass(getClass()), _GL_SEL(name)); \
    const char* me_##name = method_getTypeEncoding(m_##name); \
    class_addMethod(getClass(), _GL_SEL(name), (IMP)&pfGLRenderLayer::name, me_##name);


namespace Private
{
    namespace Class
    {
        struct objc_class* s_kCAOpenGLLayer = objc_lookUpClass("CAOpenGLLayer");
    }

    namespace Selector
    {
        struct objc_selector* s_klayer = sel_registerName("layer");
        struct objc_selector* s_kdrawInCGLContext_pixelFormat_forLayerTime_displayTime_ = sel_registerName("drawInCGLContext:pixelFormat:forLayerTime:displayTime:");
    }
}


struct objc_class* pfGLRenderLayer::sRenderLayerClass = nullptr;

void pfGLRenderLayer::IInitClass()
{
    sRenderLayerClass = objc_allocateClassPair(_GL_CLS(CAOpenGLLayer), "pfGLRenderLayer", 0);

    _GL_SUBCLASS_METHOD(drawInCGLContext_pixelFormat_forLayerTime_displayTime_);

    objc_registerClassPair(sRenderLayerClass);
}

pfGLRenderLayer* pfGLRenderLayer::getLayer()
{
    if (!getClass()) {
        IInitClass();
    }

    return sendMsg<pfGLRenderLayer*>(getClass(), _GL_SEL(layer));
}

template<typename _Ret, typename... _Args>
inline __attribute__((always_inline)) _Ret pfGLRenderLayer::sendMsg(const void* pObj, struct objc_selector* selector, _Args... args)
{
    using SendMessageProc = _Ret (*)(const void*, struct objc_selector*, _Args...);
    const SendMessageProc proc = reinterpret_cast<SendMessageProc>(&objc_msgSend);

    return (*proc)(pObj, selector, args...);
}

template<typename _Ret, typename... _Args>
inline __attribute__((always_inline)) _Ret pfGLRenderLayer::sendMsg(struct objc_selector* selector, _Args... args)
{
    using SendMessageProc = _Ret (*)(const void*, struct objc_selector*, _Args...);
    const SendMessageProc proc = reinterpret_cast<SendMessageProc>(&objc_msgSend);

    return (*proc)(this, selector, args...);
}

template<typename _Ret, typename... _Args>
inline __attribute__((always_inline)) _Ret pfGLRenderLayer::sendSuper(struct objc_selector* selector, _Args... args)
{
    using SendMessageProc = _Ret (*)(const struct objc_super*, struct objc_selector*, _Args...);
    const SendMessageProc proc = reinterpret_cast<SendMessageProc>(&objc_msgSendSuper);

    struct objc_super super = {
        (objc_object*)this,
        class_getSuperclass(getClass())
    };

    return (*proc)(&super, selector, args...);
}

void pfGLRenderLayer::drawInCGLContext_pixelFormat_forLayerTime_displayTime_(pfGLRenderLayer* self, struct obj_selector* cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const CVTimeStamp* ts)
{
    CGLSetCurrentContext(ctx);

    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    self->sendSuper<void>(_GL_SEL(drawInCGLContext_pixelFormat_forLayerTime_displayTime_), ctx, pf, t, ts);
}
