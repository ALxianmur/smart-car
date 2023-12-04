/*
 * math_ex.h
 *
 *  Created on: 2022-10-09
 *      Author: sundm
 */

#ifndef _MATH_EX_H
#define _MATH_EX_H
#include "headfile.h"

#if !defined(MYMIN)
#define MYMIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#if !defined(MYMAX)
#define MYMAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/*! @brief Computes the number of elements in an array. */
#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

/*! @name UINT16_MAX/UINT32_MAX value */
/* @{ */
#if !defined(UINT16_MAX)
#define UINT16_MAX ((uint16_t)-1)
#endif

#if !defined(UINT32_MAX)
#define UINT32_MAX ((uint32_t)-1)
#endif
/* @} */

/*! @name Timer utilities
 *
 */
#define swap(a, b)   \
    {                \
        uint8 t = a; \
        a = b;       \
        b = t;       \
    }

#endif
