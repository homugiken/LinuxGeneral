/*==============================================================================
 * clock_time.h
 *----------------------------------------------------------------------------*/

#ifndef CLOCK_TIME_H_
#define CLOCK_TIME_H_

#include "lib.h"

#if DEBUG_LIB
#define DEBUG_CLOCK_TIME				1
#else
#define DEBUG_CLOCK_TIME				0
#endif

#define CLKTD(fmt, ...) \
	do { if (DEBUG_CLOCK_TIME) { LIBDBG(fmt, ##__VA_ARGS__); }} while (false)

#define CLKTD_NULL_ARGUMENT(ARG)			CHECK_NULL_ARGUMENT(ARG, CLKTD)
#define CLKTD_NEGATIVE_ARGUMENT(ARG)			CHECK_NEGATIVE_ARGUMENT(ARG, CLKTD)

#define CLKTD_NULL_RETURN(RET, FUNC)			CHECK_NULL_RETURN(RET, FUNC, CLKTD)
#define CLKTD_NEGATIVE_RETURN(RET, FUNC)		CHECK_NEGATIVE_RETURN(RET, FUNC, CLKTD)

#define CLOCK_ID_MAX					0x0F
#define CLOCK_ID_CNT					(CLOCK_ID_MAX + 1)
#define CLOCK_ID_DEFAULT				CLOCK_MONOTONIC

#define CLOCK_SAME					0x00
#define CLOCK_EARLIER					0x01
#define CLOCK_LATER					0x02
#define CLOCK_COMPARE_MAX				0x02
#define CLOCK_COMPARE_CNT				(CLOCK_COMPARE_MAX + 1)

#define MSEC_PER_SEC					1000L
#define USEC_PER_MSEC					1000L
#define NSEC_PER_USEC					1000L
#define NSEC_PER_MSEC					1000000L
#define USEC_PER_SEC					1000000L
#define NSEC_PER_SEC					1000000000L
#define FSEC_PER_SEC					1000000000000000LL

typedef struct timespec clkspec;

int clock_add (const clkspec * const clkA, const clkspec * const clkB, clkspec * const clkSum);
int clock_sub (const clkspec * const clkA, const clkspec * const clkB, clkspec * const clkDiff);
int clock_compare (const clkspec * const clkA, const clkspec * const clkB);
int clock_wait (const clkspec * const clkOld, const clkspec * const clkWait);
int clock_wait_nano (const clkspec * const clkOld, const long nano);
int clock_wait_sec (const clkspec * const clkOld, const long sec);
int clock_test (void);

#endif /* CLOCK_TIME_H_ */
