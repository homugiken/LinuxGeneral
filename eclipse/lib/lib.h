/*==============================================================================
 * lib.h
 *----------------------------------------------------------------------------*/

#ifndef LIB_H_
#define LIB_H_

#include <errno.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define DEBUG_LIB					1
#define LIBDBG(fmt, ...) \
	do { if (DEBUG_LIB) { printf("%s %04d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); } } while (false)

#endif /* LIB_H_ */
