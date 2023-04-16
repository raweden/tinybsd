#ifndef __FDLIBM_H
#define __FDLIBM_H

#include <sys/types.h>

#if INT_MAX > 32767
#  define IC(x) ((int32_t) x)
#  define UC(x) ((uint32_t) x)
#else
#  define IC(x) ((int32_t) x##L)
#  define UC(x) ((uint32_t) x##UL)
#endif

typedef union
{
  double value;
  struct
  {
    uint32_t msw;
    uint32_t lsw;
  } parts;
} ieee_double_shape_type;

#define GET_DOUBLE_WORDS(ix0,ix1,d)				\
{								\
  const ieee_double_shape_type *ew_u = (const ieee_double_shape_type *)&(d);					\
  (ix0) = ew_u->parts.msw;					\
  (ix1) = ew_u->parts.lsw;					\
}

/* All floating-point numbers can be put in one of these categories.  */
#ifndef FP_NAN
enum
{
  FP_NAN,
# define FP_NAN FP_NAN
  FP_INFINITE,
# define FP_INFINITE FP_INFINITE
  FP_ZERO,
# define FP_ZERO FP_ZERO
  FP_SUBNORMAL,
# define FP_SUBNORMAL FP_SUBNORMAL
  FP_NORMAL
# define FP_NORMAL FP_NORMAL
};
#endif


#endif