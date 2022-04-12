#ifndef MCX_H
#define MCX_H
#include <memory.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t u;
typedef uintptr_t ptr;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef double d;

#define reset(obj, type) memset(obj, 0, sizeof(type))
#define new(type) ((type*)reset(malloc(sizeof(type)), type))
#define as(type, obj) ((type*)obj)
#define max(type) ((1 << (sizeof(type) * 8)) - 1)
#define clone(obj, type, count) memcpy(malloc(sizeof(type) * count), obj, sizeof(type) * count)

#define init(h) h##Init()//just to make it stand out

#endif //MCX_H
