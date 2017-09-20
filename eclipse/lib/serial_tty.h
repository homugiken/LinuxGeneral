/*==============================================================================
 * serial_tty.h
 *----------------------------------------------------------------------------*/
#ifndef SERIAL_TTY_H_
#define SERIAL_TTY_H_

#include "lib.h"

#if DEBUG_LIB
#define DEBUG_SERIAL_TTY				1
#else
#define DEBUG_SERIAL_TTY				0
#endif

#define SRLD(fmt, ...) \
	do { if (DEBUG_SERIAL_TTY) { LIBDBG(fmt, ##__VA_ARGS__); }} while (false)

#define SRLD_NULL_ARGUMENT(ARG)				CHECK_NULL_ARGUMENT(ARG, SRLD)
#define SRLD_NEGATIVE_ARGUMENT(ARG)			CHECK_NEGATIVE_ARGUMENT(ARG, SRLD)

#define SRLD_NULL_RETURN(RET, FUNC)			CHECK_NULL_RETURN(RET, FUNC, SRLD)
#define SRLD_NEGATIVE_RETURN(RET, FUNC)			CHECK_NEGATIVE_RETURN(RET, FUNC, SRLD)

#define SERIAL_TTY_PATH_LENGTH				128
#define SERIAL_TTY_BUFFER_LENGTH			1024
#define SERIAL_TTY_SPEED_DEFAULT			B115200


typedef struct SERIAL_TTY_ENTITY {
	int fd;
	char path[SERIAL_TTY_PATH_LENGTH];
	struct termios cfg;
	struct termios cfg_old;
	uint8_t buf[SERIAL_TTY_BUFFER_LENGTH];
} SERIAL_TTY_ENTITY;

int serial_tty_test (void);

#endif /* SERIAL_TTY_H_ */
