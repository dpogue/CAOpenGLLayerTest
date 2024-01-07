#define NS_PRIVATE_IMPLEMENTATION
#define PGL_PRIVATE_IMPLEMENTATION

#include <stdio.h>
#include "PGLLayer.hpp"

#ifndef __has_include
#define __has_include(x) 0
#endif

#if defined(__GNU_LIBOBJC__)
#include <GL/gl.h>
#else
#define GL_SILENCE_DEPRECATION
#include <QuartzCore/QuartzCore.h>
#if __has_include(<OpenGL/gl3.h>)
#include <OpenGL/gl3.h>
#else
#include <OpenGL/OpenGL.h>
#endif
#endif

Class PGL::Layer::kClass = []{
    Class klass = objc_allocateClassPair((Class)_PGL_PRIVATE_CLS(CAOpenGLLayer), "PGLLayer", 0);

    Method m = nullptr;
    const char* me = nullptr;

    m = class_getInstanceMethod(class_getSuperclass(klass), _PGL_PRIVATE_SEL(drawInCGLContext_pixelFormat_forLayerTime_displayTime_));
    me = method_getTypeEncoding(m);
    class_addMethod(klass, _PGL_PRIVATE_SEL(drawInCGLContext_pixelFormat_forLayerTime_displayTime_), (IMP)&PGL::Layer::drawInCGLContext, me);

    printf("%s -- %s\n", "drawInCGLContext:pixelFormat:forLayerTime:displayTime:", me);

    objc_registerClassPair(klass);

    return klass;
}();

void PGL::Layer::drawInCGLContext(PGL::Layer* self, SEL cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const struct CVTimeStamp* ts)
{
#if !defined(__GNU_LIBOBJC__)
    CGLSetCurrentContext(ctx);
#endif

    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    return NS::Object::sendMessageSuper<void>(self, cmd, ctx, pf, t, ts);
}
