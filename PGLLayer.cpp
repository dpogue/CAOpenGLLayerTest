#define NS_PRIVATE_IMPLEMENTATION
#define PGL_PRIVATE_IMPLEMENTATION

#include "PGLLayer.hpp"

#if defined(__GNU_LIBOBJC__)
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

objc_class* PGL::Layer::kClass = nullptr;

void PGL::Layer::load()
{
    kClass = objc_allocateClassPair((objc_class*)_PGL_PRIVATE_CLS(CAOpenGLLayer), "PGLLayer", 0);

    Method m = nullptr;
    const char* me = nullptr;

    m = class_getInstanceMethod(class_getSuperclass(kClass), _PGL_PRIVATE_SEL(drawInCGLContext_pixelFormat_forLayerTime_displayTime_));
    me = method_getTypeEncoding(m);
    class_addMethod(kClass, _PGL_PRIVATE_SEL(drawInCGLContext_pixelFormat_forLayerTime_displayTime_), (IMP)&PGL::Layer::drawInCGLContext, me);

    printf("%s -- %s\n", "drawInCGLContext:pixelFormat:forLayerTime:displayTime:", me);

    objc_registerClassPair(kClass);
}

void PGL::Layer::drawInCGLContext(PGL::Layer* self, SEL cmd, struct _CGLContextObject* ctx, struct _CGLPixelFormatObject* pf, CFTimeInterval t, const struct CVTimeStamp* ts)
{
    //CGLSetCurrentContext(ctx);

    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    return NS::Object::sendMessageSuper<void>(self, cmd, ctx, pf, t, ts);
}
