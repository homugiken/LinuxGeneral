/*==============================================================================
 * clock.h
 *----------------------------------------------------------------------------*/

#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "common.h"

#if DEBUG_ALL
#define DEBUG_CLOCK					1
#else
#define DEBUG_CLOCK					0
#endif

#define CLKD(fmt, ...) \
	do { if (DEBUG_CLOCK) { DBG(fmt, ##__VA_ARGS__); }} while (false)

#define MSEC_PER_SEC					1000L
#define USEC_PER_MSEC					1000L
#define NSEC_PER_USEC					1000L
#define NSEC_PER_MSEC					1000000L
#define USEC_PER_SEC					1000000L
#define NSEC_PER_SEC					1000000000L
#define FSEC_PER_SEC					1000000000000000LL

#define CLOCK_COMPARE_EARLIER				0
#define CLOCK_COMPARE_SAME				1
#define CLOCK_COMPARE_LATER				2

#define CLOCK_TEST_STEP					(NSEC_PER_SEC / 4)
#define CLOCK_TEST_LENGTH				(((2 * NSEC_PER_SEC / CLOCK_TEST_STEP) * 2) + 1)

#define CLOCK_ID_DEFAULT				CLOCK_MONOTONIC

typedef struct timespec clockspec;
typedef struct CLOCK_TYPE {
	clockid_t 	id;
	char *		name;
} CLOCK_TYPE;

int clock_add (const clockspec * const clkA, const clockspec * const clkB, clockspec * const clkSum);
int clock_sub (const clockspec * const clkA, const clockspec * const clkB, clockspec * const clkDiff);
int clock_cmp (const clockspec * const clkA, const clockspec * const clkB, int * const compare);
int clock_wait (const clockspec * const clkNow, const clockspec * const clkOld, const clockspec * const clkWait);
void clock_show (void);
void clock_test (void);

#endif	/*__CLOCK_H__*/
