#define NS_PRIVATE_IMPLEMENTATION
#define PGL_PRIVATE_IMPLEMENTATION

#include <stdio.h>
#include "PGLLayer.hpp"
#include "type_encoder.hpp"

#ifndef __has_include
#define __has_include(x) 0
#endif

#if defined(__GNU_LIBOBJC__)
#include <GL/gl.h>
#else
#define GL_SILENCE_DEPRECATION
#if __has_include(<OpenGL/gl3.h>)
#include <OpenGL/gl3.h>
#else
#include <OpenGL/OpenGL.h>
#endif
#endif

Class PGL::Layer::kClass = []{
    Class klass = objc_allocateClassPair((Class)_PGL_PRIVATE_CLS(CAOpenGLLayer), "PGLLayer", 0);

    class_addMethod(klass, _PGL_PRIVATE_SEL(drawInCGLContext_pixelFormat_forLayerTime_displayTime_), (IMP)&PGL::Layer::drawInCGLContext, encode_method(&PGL::Layer::drawInCGLContext));
    class_addMethod(klass, _PGL_PRIVATE_SEL(copyCGLPixelFormatForDisplayMask_), (IMP)&PGL::Layer::copyCGLPixelFormatForDisplayMask, encode_method(&PGL::Layer::copyCGLPixelFormatForDisplayMask));

    objc_registerClassPair(klass);

    return klass;
}();

void PGL::Layer::drawInCGLContext(PGL::Layer* self, SEL cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const CVTimeStamp* ts)
{
    CGLSetCurrentContext(ctx);

    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    return NS::Object::sendMessageSuper<void>(self, cmd, ctx, pf, t, ts);
}

CGLPixelFormatObj PGL::Layer::copyCGLPixelFormatForDisplayMask(Layer* self, SEL cmd, uint32_t mask)
{
    CGLPixelFormatAttribute attribs[] = {
        kCGLPFADisplayMask, static_cast<CGLPixelFormatAttribute>(mask),
#if MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
        // OpenGL profiles introduced in 10.7
        kCGLPFAOpenGLProfile, static_cast<CGLPixelFormatAttribute>(kCGLOGLPVersion_3_2_Core),
#endif
        static_cast<CGLPixelFormatAttribute>(0)};

    CGLPixelFormatObj pixelFormat = nullptr;
    GLint numFormats              = 0;
    CGLChoosePixelFormat(attribs, &pixelFormat, &numFormats);
    return pixelFormat;
}
