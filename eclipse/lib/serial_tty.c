/*==============================================================================
 * serial_tty.c
 *----------------------------------------------------------------------------*/

#include "serial_tty.h"

/*==============================================================================
 * serial_tty_open()
 * Open serial tty.
 * Return opened fd when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_tty_open (SERIAL_TTY_ENTITY * const serial, const char * const name)
{
	int ret = -1;

	SRLD_NULL_ARGUMENT(serial);

	if (name == NULL) {
		printf("\nPlease enter serial tty path:\n");
		ret = scanf("%128s", serial->path);
		SRLD_NEGATIVE_RETURN(ret, scanf);
	}
	else if ((serial->fd >= 0) && (strcmp(serial->path, name) == 0)) {
		ret = serial->fd;
		SRLD("%s already opened fd=%d\n", serial->path, serial->fd);
		goto exit;
	}
	else {
		SRLD_NULL_RETURN(strcpy(serial->path, name), strcpy);
	}

	SRLD("try to open %s\n", serial->path);
	ret = open(serial->path, O_RDWR | O_NOCTTY | O_NDELAY);
	SRLD_NEGATIVE_RETURN(ret, open);

	if (isatty(ret) == 1) {
		serial->fd = ret;
		printf("open %s fd=%d\n", serial->path, serial->fd);
	}
	else {
		SRLD_NEGATIVE_RETURN(close(ret), close);
		ret = -1;
	}

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_tty_close()
 * Close serial tty.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_tty_close (SERIAL_TTY_ENTITY * const serial)
{
	int ret = -1;

	SRLD_NULL_ARGUMENT(serial);
	SRLD_NEGATIVE_ARGUMENT(serial->fd);

	ret = close(serial->fd);
	SRLD_NEGATIVE_RETURN(ret, close);

	printf("close %s fd=%d\n", serial->path, serial->fd);
	memset(serial, 0, sizeof (SERIAL_TTY_ENTITY));
	serial->fd = -1;

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_tty_configure()
 * Configure serial tty.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_tty_configure (SERIAL_TTY_ENTITY * const serial, const struct termios * const new)
{
	int ret = -1;
	int fd;
	struct termios * cfg;

	SRLD_NULL_ARGUMENT(serial);
	SRLD_NEGATIVE_ARGUMENT(serial->fd);

	fd = serial->fd;
	cfg = &serial->cfg;

	ret = tcgetattr(fd, &serial->cfg_old);
	SRLD_NEGATIVE_RETURN(ret, tcgetattr);

	ret = tcflush(fd, TCIOFLUSH);
	SRLD_NEGATIVE_RETURN(ret, tcflush);

	if (new == NULL) {
		memcpy(cfg, new, sizeof (struct termios));
	}
	else {
		cfmakeraw(cfg);
		cfg->c_cc[VMIN] = 1;
		cfg->c_cc[VTIME] = 0;

		ret = cfsetspeed(cfg, SERIAL_TTY_SPEED_DEFAULT);
		SRLD_NEGATIVE_RETURN(ret, cfsetspeed);
	}

	ret = tcsetattr(fd, TCSAFLUSH, cfg);
	SRLD_NEGATIVE_RETURN(ret, tcsetattr);

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_tty_configure()
 * Configure serial tty.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_tty_set_speed (SERIAL_TTY_ENTITY * const serial, unsigned int speed)
{
	int ret = -1;
	int fd;
	struct termios * cfg;

	SRLD_NULL_ARGUMENT(serial);
	SRLD_NEGATIVE_ARGUMENT(serial->fd);

	fd = serial->fd;
	cfg = &serial->cfg;

	ret = tcgetattr(fd, cfg);
	SRLD_NEGATIVE_RETURN(ret, tcgetattr);

	ret = tcflush(fd, TCIOFLUSH);
	SRLD_NEGATIVE_RETURN(ret, tcflush);

	ret = cfsetspeed(cfg, speed);
	SRLD_NEGATIVE_RETURN(ret, cfsetspeed);

	ret = tcsetattr(fd, TCSAFLUSH, cfg);
	SRLD_NEGATIVE_RETURN(ret, tcsetattr);

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_tty_read()
 * Read from serial tty.
 * Return number of bytes read. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_tty_read (SERIAL_TTY_ENTITY * const serial, const unsigned int count)
{
	int ret = -1;

	SRLD_NULL_ARGUMENT(serial);
	SRLD_NEGATIVE_ARGUMENT(serial->fd);

	ret = read(serial->fd, serial->buf, (count < sizeof (serial->buf)) ? count : sizeof (serial->buf));
	SRLD_NEGATIVE_RETURN(ret, read);

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_tty_write()
 * Write to serial tty.
 * Return number of bytes written. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_tty_write (SERIAL_TTY_ENTITY * const serial, const size_t count)
{
	int ret = -1;

	SRLD_NULL_ARGUMENT(serial);
	SRLD_NEGATIVE_ARGUMENT(serial->fd);

	ret = write(serial->fd, serial->buf, (count < sizeof (serial->buf)) ? count : sizeof (serial->buf));
	SRLD_NEGATIVE_RETURN(ret, write);

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

#define SERIAL_TTY_PRINT_CONFIG(ARG)			printf("\t%s=0x%08X\n", #ARG, cfg->ARG)

void serial_tty_dump (SERIAL_TTY_ENTITY * const serial)
{
	int ret;
	struct termios * cfg;

	SRLD_NULL_ARGUMENT(serial);
	SRLD_NEGATIVE_ARGUMENT(serial->fd);

	cfg = &serial->cfg;

	ret = tcgetattr(serial->fd, cfg);
	SRLD_NEGATIVE_RETURN(ret, tcgetattr);

	SERIAL_TTY_PRINT_CONFIG(c_iflag);
	SERIAL_TTY_PRINT_CONFIG(c_oflag);
	SERIAL_TTY_PRINT_CONFIG(c_cflag);
	SERIAL_TTY_PRINT_CONFIG(c_lflag);
	SERIAL_TTY_PRINT_CONFIG(c_cc[VMIN]);
	SERIAL_TTY_PRINT_CONFIG(c_cc[VTIME]);

exit:
	return;
}


int serial_tty_test (void)
{
	int ret = -1;

	printf("\nSerial tty test:\n");
	ret = system("ls -l /dev/ | grep \"ttyS\\|ttyUSB\"");

exit:
	return (ret);
}
