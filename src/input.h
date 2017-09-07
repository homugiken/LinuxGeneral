/*==============================================================================
 * input.h
 *----------------------------------------------------------------------------*/

#ifndef __INPUT_H__
#define __INPUT_H__

#include "common.h"

#if DEBUG_ALL
#define DEBUG_INPUT					1
#else
#define DEBUG_INPUT					0
#endif

#define INPTD(fmt, ...) \
	do { if (DEBUG_INPUT) { DBG(fmt, ##__VA_ARGS__); }} while (false)

#define INPUT_EVENT_PATH_LENGTH				128

#define INPUT_KEY_RELEASED				0
#define INPUT_KEY_PRESSED				1
#define INPUT_KEY_REPEATED				2

#define BITFIELD uint32_t
static __inline__ int test_bit(int nr, BITFIELD * addr)
{
	BITFIELD mask;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	return ((mask & *addr) != 0);
}

typedef struct input_event inputevent;

int input_open (const char * const name);
int input_close (const int fd);
void input_dump (const inputevent * const input);
int input_read (const int fd, inputevent * const input);
int input_test (void);

#endif	/*__CLOCK_H__*/
