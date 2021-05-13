#pragma once

#ifndef PB_VERSION
#define PB_VERSION      "0.1.0"
#define PB_VERSION_MAJOR    0
#define PB_VERSION_MINOR    1
#define PB_VERSION_PATCH    0
#endif

#if defined(PB_DEBUG)

#define PB_STATIC_ASSERT(exp)   static_assert(exp, #exp)

#if PB_WINDOWS

#define PB_DEBUG_BREAK()        __debugbreak()

#else

void PB_DEBUG_BREAK();

#endif

#else

#define PB_DEBUG_BREAK()

#endif