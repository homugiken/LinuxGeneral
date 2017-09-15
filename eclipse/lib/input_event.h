/*==============================================================================
 * input_event.h
 *----------------------------------------------------------------------------*/

#ifndef INPUT_EVENT_H_
#define INPUT_EVENT_H_

#include "lib.h"

#if DEBUG_LIB
#define DEBUG_INPUT_EVENT				1
#else
#define DEBUG_INPUT_EVENT				0
#endif

#define IPEVD(fmt, ...) \
	do { if (DEBUG_INPUT_EVENT) { LIBDBG(fmt, ##__VA_ARGS__); }} while (false)

#define NAME_INITIALIZER(NAME)				[NAME] = #NAME

#define INPUT_EVENT_TEST_TIMEOUT			10
#define INPUT_EVENT_PATH_LENGTH				128
#define INPUT_EVENT_DUMP_LENGTH				1024

#define KEY_EVENT_RELEASED				0
#define KEY_EVENT_PRESSED				1
#define KEY_EVENT_REPEATED				2

typedef struct input_event inputevent;

int input_event_open (const char * const name, int * const fd);
int input_event_close (const int fd);
int input_event_read (const int fd, inputevent * const input);
void input_event_device_dump (const int fd);
void input_event_dump (const inputevent * const input);
int input_event_test (void);

#endif /* INPUT_EVENT_H_ */
