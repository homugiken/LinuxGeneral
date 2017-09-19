/*==============================================================================
 * common.h
 *----------------------------------------------------------------------------*/

#ifndef __COMMON_H__
#define __COMMON_H__

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


#define DEBUG_ALL					1

#define DBG(fmt, ...) \
	do { if (DEBUG_ALL) { printf("%s %04d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); } } while (false)



#endif	/*__COMMON_H__*/
