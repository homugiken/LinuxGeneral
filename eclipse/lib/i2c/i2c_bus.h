/*==============================================================================
 * i2c_bus.h
 *----------------------------------------------------------------------------*/
#ifndef I2C_BUS_H_
#define I2C_BUS_H_

#include "lib.h"

#if DEBUG_LIB
#define DEBUG_I2C_BUS					1
#else
#define DEBUG_I2C_BUS					0
#endif

#define I2CD(fmt, ...) \
	do { if (DEBUG_I2C_BUS) { LIBDBG(fmt, ##__VA_ARGS__); }} while (false)

#define I2CD_NULL_ARGUMENT(ARG)				CHECK_NULL_ARGUMENT(ARG, I2CD)
#define I2CD_NEGATIVE_ARGUMENT(ARG)			CHECK_NEGATIVE_ARGUMENT(ARG, I2CD)

#define I2CD_NULL_RETURN(RET, FUNC)			CHECK_NULL_RETURN(RET, FUNC, I2CD)
#define I2CD_NEGATIVE_RETURN(RET, FUNC)			CHECK_NEGATIVE_RETURN(RET, FUNC, I2CD)

#define I2C_BUS_PATH_LENGTH				128
#define I2C_BUS_INFO_LENGTH				1024

typedef struct I2C_BUS_ENTITY{
	int fd;
	char path[I2C_BUS_PATH_LENGTH];
	char info[I2C_BUS_INFO_LENGTH];
} I2C_BUS_ENTITY;


int i2c_bus_test (void);


#endif /* I2C_BUS_H_ */
