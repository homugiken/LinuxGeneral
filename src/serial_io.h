/*==============================================================================
 * serial_io.h
 *----------------------------------------------------------------------------*/

#ifndef __SERIAL_IO_H__
#define __SERIAL_IO_H__

#include "common.h"

#if DEBUG_ALL
#define DEBUG_SERIAL_IO					1
#else
#define DEBUG_SERIAL_IO					0
#endif

#define TTYD(fmt, ...) \
	do { if (DEBUG_TTY) { DBG(fmt, ##__VA_ARGS__); }} while (false)


#endif /*__SERIAL_IO_H__*/
