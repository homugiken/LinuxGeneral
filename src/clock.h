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

#define CLOCK_COMPARE_SAME				0
#define CLOCK_COMPARE_EARLIER				-1
#define CLOCK_COMPARE_OLDER				1

#define CLOCK_ID_DEFAULT				CLOCK_MONOTONIC

typedef struct timespec clockspec;
typedef struct CLOCK_TYPE {
	clockid_t 	id;
	char *		name;
} CLOCK_TYPE;

int clock_add (
	const clockspec * const ta,
	const clockspec * const tb,
	clockspec * const result
	);

int clock_sub (
	const clockspec * const ta,
	const clockspec * const tb,
	clockspec * const result
	);

int clock_compare (
	const clockspec * const ta,
	const clockspec * const tb,
	int * const result
	);

int clock_wait (
	const clockspec * const told,
	const clockspec * const tnow,
	const clockspec * const wait
	);

void clock_show (void);
void clock_test (void);

#endif	/*__CLOCK_H__*/
