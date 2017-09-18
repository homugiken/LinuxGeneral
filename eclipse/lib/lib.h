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

#define NAME_INITIALIZER(NAME)				[NAME] = #NAME

#define CHECK_NULL_ARGUMENT(ARG, DBG)			if (ARG == NULL) { DBG("argument %s=NULL\n", #ARG); goto exit; }
#define CHECK_NEGATIVE_ARGUMENT(ARG, DBG)		if (ARG < 0) { DBG("argument %s=%d\n", #ARG, ARG); goto exit; }

#define CHECK_NULL_RETURN(RET, FUNC, DBG)		if (RET == NULL) { DBG("%s ret=NULL\n", #FUNC); goto exit; }
#define CHECK_NEGATIVE_RETURN(RET, FUNC, DBG)		if (RET < 0) { DBG("%s ret=%d\n", #FUNC, RET); goto exit; }

#endif /* LIB_H_ */
