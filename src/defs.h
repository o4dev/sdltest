#ifndef _DEFS_H_
#define _DEFS_H_

#define PI      3.14159265358979323846

#define BOOL    unsigned
#define TRUE    1
#define FALSE   0

#define WIDTH   160
#define HEIGHT  120
#define SCALE   4

#define TITLE   "sdltest"
#define TITLEL  7

#define __hot           __attribute__ ((hot))
#define __align(a)      __attribute__ ((aligned(a)))
#define __inline        __attribute__ ((always_inline))

#endif /* _DEFS_H_ */
