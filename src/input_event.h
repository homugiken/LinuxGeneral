/*==============================================================================
 * input.h
 *----------------------------------------------------------------------------*/

#ifndef __INPUT_EVENT_H__
#define __INPUT_EVENT_H__

#include "common.h"

#if DEBUG_ALL
#define DEBUG_INPUT					1
#else
#define DEBUG_INPUT					0
#endif

#define INPTD(fmt, ...) \
	do { if (DEBUG_INPUT) { DBG(fmt, ##__VA_ARGS__); }} while (false)

#define INPUT_EVENT_TEST_TIMEOUT			10
#define INPUT_EVENT_PATH_LENGTH				128
#define INPUT_EVENT_DUMP_LENGTH				1024

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

int input_event_open (const char * const name, int * const fd);
int input_event_close (const int fd);
int input_event_read (const int fd, inputevent * const input);
void input_event_device_dump (const int fd);
void input_event_dump (const inputevent * const input);
int input_event_test (void);

#endif	/*__INPUT_EVENT_H__*/
