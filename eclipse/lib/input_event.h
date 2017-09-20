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

#define IPEVD_NULL_ARGUMENT(ARG)			CHECK_NULL_ARGUMENT(ARG, IPEVD)
#define IPEVD_NEGATIVE_ARGUMENT(ARG)			CHECK_NEGATIVE_ARGUMENT(ARG, IPEVD)

#define IPEVD_NULL_RETURN(RET, FUNC)			CHECK_NULL_RETURN(RET, FUNC, IPEVD)
#define IPEVD_NEGATIVE_RETURN(RET, FUNC)		CHECK_NEGATIVE_RETURN(RET, FUNC, IPEVD)

#define INPUT_EVENT_PATH_LENGTH				128
#define INPUT_EVENT_INFO_LENGTH				1024
#define INPUT_EVENT_TEST_TIMEOUT			10

typedef struct INPUT_EVENT_ENTITY {
	int fd;
	char path[INPUT_EVENT_PATH_LENGTH];
	char info[INPUT_EVENT_INFO_LENGTH];
	struct input_event event;
} INPUT_EVENT_ENTITY;

int input_event_open (INPUT_EVENT_ENTITY * const input, const char * const name);
int input_event_close (INPUT_EVENT_ENTITY * const input);
int input_event_read (INPUT_EVENT_ENTITY * const input);
void input_event_device_dump (INPUT_EVENT_ENTITY * const input);
void input_event_dump (INPUT_EVENT_ENTITY * const input);
int input_event_test (void);

#endif /* INPUT_EVENT_H_ */
