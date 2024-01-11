//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "PGLDefines.hpp"
#include "PGLPrivate.hpp"

#include "NSObject.hpp"

#if defined(__GNU_LIBOBJC__)
typedef struct _CGLContextObject* CGLContextObj;
typedef struct _CGLPixelFormatObject* CGLPixelFormatObj;
struct CVTimeStamp;
#else
#include <QuartzCore/QuartzCore.h>
#endif

namespace PGL
{

class Layer : public NS::Referencing<Layer>
{
private:
    static Class kClass;


    static void drawInCGLContext(Layer* self, SEL cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const CVTimeStamp* ts);
    static BOOL canDrawInCGLContext(Layer* self, SEL cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const CVTimeStamp* ts);
    static CGLPixelFormatObj copyCGLPixelFormatForDisplayMask(Layer* self, SEL cmd, uint32_t mask);

public:
    static class Layer* layer();
};

}

_PGL_INLINE PGL::Layer* PGL::Layer::layer()
{
    return NS::Object::sendMessage<PGL::Layer*>(PGL::Layer::kClass, _PGL_PRIVATE_SEL(layer));
}
