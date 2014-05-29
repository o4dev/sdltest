#ifndef _DEFS_H_
#define _DEFS_H_

/* sdltest Settings */
#define WIDTH   160
#define HEIGHT  120
#define SCALE   4

#define TITLE   "sdltest"
#define TITLEL  7

/* Compilation Options */
#define _ALLOW_REGISTER_
#define _USE_STDINT_

/* Compiler Attributes */
#define __hot       __attribute__ ((hot))
#define __align(a)  __attribute__ ((aligned(a)))
#define __inline    __attribute__ ((always_inline))

/* Usage of Register */
#ifdef _ALLOW_REGISTER_
 #define register register
#else
 #define register
#endif /* _ALLOW_REGISTER_ */

/* Usage of Standardised Integer Sizes */
#ifdef _USE_STDINT_
 #include <stdint.h>
 #define INT8    int8_t
 #define INT16   int16_t
 #define INT32   int32_t
 #define INT64   int64_t
 #define UINT8   uint8_t
 #define UINT16  uint16_t
 #define UINT32  uint32_t
 #define UINT64  uint64_t
#else
 #define INT8    signed char
 #define INT16   signed short
 #define INT32   signed int
 #define INT64   signed long
 #define UINT8   unsigned char
 #define UINT16  unsigned short
 #define UINT32  unsigned int
 #define UINT64  unsigned long
#endif /* _USE_STDINT_ */

/* Integer Type Aliases */
#define char        INT8
#define short       INT16
#define int         INT32
#define long        INT64

#define uchar       UINT8
#define ushort      UINT16
#define uint        UINT32
#define ulong       UINT64

#define signed      INT32
#define unsigned    UINT32

/* Custom Boolean Type */
#define BOOL    UINT32
#define TRUE    1
#define FALSE   0

/* For Consistency */
#define bool    BOOL
#define true    TRUE
#define false   FALSE

/* Misc Definitions */
#define PI      3.14159265358979323846

#endif /* _DEFS_H_ */
