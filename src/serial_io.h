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

#define SIOD(fmt, ...) \
	do { if (DEBUG_SERIAL_IO) { DBG(fmt, ##__VA_ARGS__); }} while (false)

#define SERIAL_IO_PATH_LENGTH				128
#define SERIAL_IO_SPEED_DEFAULT				B115200
#define SERIAL_IO_TEST_COUNT				20
#define SERIAL_IO_TEST_BUFFER_LENGTH			1024
#define SERIAL_IO_TEST_STRING				"sending"

int serial_io_open (const char * const name, int * const fd);
int serial_io_close (const int fd);
void serial_io_attribution_dump (const int fd);
int serial_io_rx (const int fd, void * const data, const size_t count);
int serial_io_tx (const int fd, void * const data, const size_t count);
int serial_io_test (void);

#endif /*__SERIAL_IO_H__*/
