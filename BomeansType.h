
#ifndef _BOMEANS_TYPE_H_
#define _BOMEANS_TYPE_H_

#include <deque>
#include <vector>
#include <stdint.h>

#ifndef XCHAR
#define XCHAR char
#endif 

#ifndef ArrayList
#define ArrayList std::deque
#endif 

#ifndef BomArray
#define BomArray std::vector
#endif

#define BOMEANS_LIB_NAME BOMEANS

#define SP_BOMEANS namespace BOMEANS_LIB_NAME {
#define SP_END }

#ifndef PURE
#define PURE =0
#endif


#ifndef Interface
#define Interface struct
#endif

#endif // of _BOMEANS_TYPE_H_