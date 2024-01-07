//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <objc/runtime.h>

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#define _PGL_PRIVATE_CLS(symbol) (Private::Class::s_k##symbol)
#define _PGL_PRIVATE_SEL(accessor) (Private::Selector::s_k##accessor)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#if defined(PGL_PRIVATE_IMPLEMENTATION)

#ifdef METALCPP_SYMBOL_VISIBILITY_HIDDEN
#define _PGL_PRIVATE_VISIBILITY __attribute__((visibility("hidden")))
#else
#define _PGL_PRIVATE_VISIBILITY __attribute__((visibility("default")))
#endif // METALCPP_SYMBOL_VISIBILITY_HIDDEN

#define _PGL_PRIVATE_IMPORT __attribute__((weak_import))

#ifdef __OBJC__
#define _PGL_PRIVATE_OBJC_LOOKUP_CLASS(symbol) ((__bridge void*)objc_lookUpClass(#symbol))
#define _PGL_PRIVATE_OBJC_GET_PROTOCOL(symbol) ((__bridge void*)objc_getProtocol(#symbol))
#else
#define _PGL_PRIVATE_OBJC_LOOKUP_CLASS(symbol) objc_lookUpClass(#symbol)
#define _PGL_PRIVATE_OBJC_GET_PROTOCOL(symbol) objc_getProtocol(#symbol)
#endif // __OBJC__

#define _PGL_PRIVATE_DEF_CLS(symbol) void* s_k##symbol _PGL_PRIVATE_VISIBILITY = _PGL_PRIVATE_OBJC_LOOKUP_CLASS(symbol)
#define _PGL_PRIVATE_DEF_PRO(symbol) void* s_k##symbol _PGL_PRIVATE_VISIBILITY = _PGL_PRIVATE_OBJC_GET_PROTOCOL(symbol)
#define _PGL_PRIVATE_DEF_SEL(accessor, symbol) SEL s_k##accessor _PGL_PRIVATE_VISIBILITY = sel_registerName(symbol)
#define _PGL_PRIVATE_DEF_CONST(type, symbol)              \
    _PGL_EXTERN type const PGL##symbol _PGL_PRIVATE_IMPORT; \
    type const                       PGL::symbol = (nullptr != &PGL##symbol) ? PGL##symbol : nullptr

#else

#define _PGL_PRIVATE_DEF_CLS(symbol) extern void* s_k##symbol
#define _PGL_PRIVATE_DEF_PRO(symbol) extern void* s_k##symbol
#define _PGL_PRIVATE_DEF_SEL(accessor, symbol) extern SEL s_k##accessor
#define _PGL_PRIVATE_DEF_CONST(type, symbol) extern type const PGL::symbol

#endif // PGL_PRIVATE_IMPLEMENTATION

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace PGL::Private
{
    namespace Class
    {
        _PGL_PRIVATE_DEF_CLS(CAOpenGLLayer);
    }

    namespace Protocol { }

    namespace Selector
    {
        _PGL_PRIVATE_DEF_SEL(layer,
            "layer");
        _PGL_PRIVATE_DEF_SEL(copyCGLPixelFormatForDisplayMask_,
            "copyCGLPixelFormatForDisplayMask:");
        _PGL_PRIVATE_DEF_SEL(copyCGLContextForPixelFormat_,
            "copyCGLContextForPixelFormat:");
        _PGL_PRIVATE_DEF_SEL(canDrawInCGLContext_pixelFormat_forLayerTime_displayTime_,
            "canDrawInCGLContext:pixelFormat:forLayerTime:displayTime:");
        _PGL_PRIVATE_DEF_SEL(drawInCGLContext_pixelFormat_forLayerTime_displayTime_,
            "drawInCGLContext:pixelFormat:forLayerTime:displayTime:");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
