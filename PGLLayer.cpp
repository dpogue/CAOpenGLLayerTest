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
    class_addMethod(klass, _PGL_PRIVATE_SEL(canDrawInCGLContext_pixelFormat_forLayerTime_displayTime_), (IMP)&PGL::Layer::canDrawInCGLContext, encode_method(&PGL::Layer::canDrawInCGLContext));
    class_addMethod(klass, _PGL_PRIVATE_SEL(copyCGLPixelFormatForDisplayMask_), (IMP)&PGL::Layer::copyCGLPixelFormatForDisplayMask, encode_method(&PGL::Layer::copyCGLPixelFormatForDisplayMask));

    objc_registerClassPair(klass);

    return klass;
}();

BOOL PGL::Layer::canDrawInCGLContext(PGL::Layer* self, SEL cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const CVTimeStamp* ts)
{
    return YES;
}

void PGL::Layer::drawInCGLContext(PGL::Layer* self, SEL cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const CVTimeStamp* ts)
{
    CGLSetCurrentContext(ctx);

    const GLubyte* str = glGetString(GL_VERSION);
    printf("Rendering with %s\n", str);
    str = glGetString(GL_VENDOR);
    printf("  Vendor: %s\n", str);
    str = glGetString(GL_RENDERER);
    printf("  Renderer: %s\n", str);

    printf("glGetString error? %04X\n", glGetError());
    glClearColor(1.f, 0.f, 0.f, 1.f);
    printf("glClearColor error? %04X\n", glGetError());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    printf("glClear error? %04X\n", glGetError());

    NS::Object::sendMessageSuper<void>(self, cmd, ctx, pf, t, ts);

    printf("glFlush from superclass error? %04X\n", glGetError());

    glFlush();
    printf("glFlush manually? %04X\n", glGetError());
    CGLFlushDrawable(ctx);
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
