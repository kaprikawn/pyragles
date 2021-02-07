#ifndef TYPES_HPP
#define TYPES_HPP

#include <stdint.h>

typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;
typedef int32     bool32;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;
typedef size_t    memory_index;
typedef float     real32;
typedef double    real64;
typedef real32    f32;
typedef real64    f64;

typedef unsigned char uchar;

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)


#endif //TYPES_HPP
