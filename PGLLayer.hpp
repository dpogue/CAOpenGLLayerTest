//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "PGLDefines.hpp"
#include "PGLPrivate.hpp"

#include "NSObject.hpp"

namespace PGL
{

class Layer : public NS::Referencing<Layer>
{
private:
    static Class kClass;

    typedef struct _CGLContextObject* CGLContextObj;

    static void drawInCGLContext(Layer* self, SEL cmd, CGLContextObj ctx, struct _CGLPixelFormatObject* pf, CFTimeInterval t, const struct CVTimeStamp* ts);

public:
    static class Layer* layer();
};

}

_PGL_INLINE PGL::Layer* PGL::Layer::layer()
{
    return NS::Object::sendMessage<PGL::Layer*>(PGL::Layer::kClass, _PGL_PRIVATE_SEL(layer));
}
