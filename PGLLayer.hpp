//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "PGLDefines.hpp"
#include "PGLPrivate.hpp"

#include "NSObject.hpp"

typedef struct _CGLContextObject* CGLContextObj;
typedef struct _CGLPixelFormatObject* CGLPixelFormatObj;

namespace PGL
{

class Layer : public NS::Referencing<Layer>
{
private:
    static Class kClass;


    static void drawInCGLContext(Layer* self, SEL cmd, CGLContextObj ctx, CGLPixelFormatObj pf, CFTimeInterval t, const struct CVTimeStamp* ts);

public:
    static class Layer* layer();
};

}

_PGL_INLINE PGL::Layer* PGL::Layer::layer()
{
    return NS::Object::sendMessage<PGL::Layer*>(PGL::Layer::kClass, _PGL_PRIVATE_SEL(layer));
}
