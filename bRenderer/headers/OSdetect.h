#pragma once

#ifdef _WIN32
/* Windows 32-bit and 64-bit */
#define OS_DESKTOP
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
/* iOS Simulator*/
#define OS_IOS
#elif TARGET_OS_IPHONE
/* iOS */
#define OS_IOS
#elif TARGET_OS_MAC
/* Mac OSX */
#define OS_DESKTOP
#else
/* Probably unsupported */
#define OS_DESKTOP
#endif
#elif __linux
/* Linux */
#define OS_DESKTOP
#else
/* Probably unsupported */
#define OS_DESKTOP
#endif