#ifndef MINI_OS_STDDEF_H_
#define MINI_OS_STDDEF_H_

#define NULL ((void*)0)

#define ZERO (0)

#define UINT8_MIN ZERO
#define UINT8_MAX (0xFF)
#define INT8_MIN (-0x7f)
#define INT8_MAX (0x7e)
#define UINT16_MIN ZERO
#define UINT16_MAX (0xFFFF)
#define INT16_MIN (-0x8000)
#define INT16_MAX (0x7fff)
#define UINT32_MIN ZERO
#define UINT32_MAX (0xFFFFFFFF)
#define INT32_MIN (-0x80000000)
#define INT32_MAX (0x7FFFFFFFF)

#undef ZERO

typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef uint32_t size_t;

#endif /* MINI_OS_STDDEF_H_ */