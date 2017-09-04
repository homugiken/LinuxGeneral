/*==============================================================================
 * common.h
 *----------------------------------------------------------------------------*/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <linux/input.h>

#define DEBUG_ALL					1

#define DBG(fmt, ...) \
	do { if (DEBUG_ALL) { printf("%s %04d: "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); } } while (false);



#endif	/*__COMMON_H__*/
