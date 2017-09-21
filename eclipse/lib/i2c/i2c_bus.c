/*==============================================================================
 * i2c_bus.c
 *----------------------------------------------------------------------------*/

#include "i2c_bus.h"


int i2c_bus_open (I2C_BUS_ENTITY * const i2c, const char * const name)
{
	int ret = -1;
	char * path;

	I2CD_NULL_ARGUMENT(i2c);

	path = i2c->path;

	if (name == NULL) {
		printf("\nPlease enter i2c bus path: ");
		ret = scanf("%128s", path);
		I2CD_NEGATIVE_RETURN(ret, scanf);
	}
	else if ((i2c->fd >= 0) && (strcmp(path, name) == 0)){
		ret = i2c->fd;
		I2CD("%s already opened fd=%d\n", path, i2c->fd);
		goto exit;
	}
	else {
		I2CD_NULL_RETURN(strcpy(path, name), strcpy);
	}

	I2CD("try to open %s\n", path);
	ret = open(path, O_RDWR);
	I2CD_NEGATIVE_RETURN(ret, open);

	i2c->fd = ret;
	printf("open %s fd=%d\n", path, i2c->fd);

exit:
	return (ret);
}


int i2c_bus_test (void)
{
	int ret = -1;





exit:
	return (ret);
}
