#ifndef MEMORY_H
#define MEMORY_H

#include "core/typedefs.h"

// URGENT: look into implementation, is it asm?
extern void memCopy32(void *dst, const void *src, u32 wdn);

#endif // !MEMORY_H
