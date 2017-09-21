/*==============================================================================
 * serial_tty.c
 *----------------------------------------------------------------------------*/

#include "serial_tty.h"
#include "clock_time.h"

/*==============================================================================
 * serial_tty_open()
 * Open serial tty.
 * Return opened fd when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_tty_open (SERIAL_TTY_ENTITY * const serial, const char * const name)
{
	int ret = -1;
	char * path;

	SRLD_NULL_ARGUMENT(serial);

	path = serial->path;

	if (name == NULL) {
		printf("\nPlease enter serial tty path: ");
		ret = scanf("%128s", path);
		SRLD_NEGATIVE_RETURN(ret, scanf);
	}
	else if ((serial->fd >= 0) && (strcmp(path, name) == 0)) {
		ret = serial->fd;
		SRLD("%s already opened fd=%d\n", path, serial->fd);
		goto exit;
	}
	else {
		SRLD_NULL_RETURN(strcpy(path, name), strcpy);
	}

	SRLD("try to open %s\n", path);
	ret = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
	SRLD_NEGATIVE_RETURN(ret, open);

	if (isatty(ret) == 1) {
		serial->fd = ret;
		printf("open %s fd=%d\n", path, serial->fd);
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

	if (new != NULL) {
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
int serial_tty_read (SERIAL_TTY_ENTITY * const serial)
{
	int ret = -1;

	SRLD_NULL_ARGUMENT(serial);
	SRLD_NEGATIVE_ARGUMENT(serial->fd);

	ret = read(serial->fd, serial->buf, sizeof (serial->buf));
	SRLD_NEGATIVE_RETURN(ret, read);
	serial->count = ret;
#if 0
	SRLD("read %d Bytes\n", ret);
#endif

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_tty_write()
 * Write to serial tty.
 * Return number of bytes written. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_tty_write (SERIAL_TTY_ENTITY * const serial)
{
	int ret = -1;
	int count;

	SRLD_NULL_ARGUMENT(serial);
	SRLD_NEGATIVE_ARGUMENT(serial->fd);

	count = (serial->count < sizeof (serial->buf)) ? serial->count : sizeof (serial->buf);

	ret = write(serial->fd, serial->buf, count);
	SRLD_NEGATIVE_RETURN(ret, write);
	serial->count -= ret;
#if 0
	SRLD("write %d Bytes\n", ret);
#endif

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

#define SERIAL_TTY_PRINT_CONFIG(ARG)			printf("\t%s=0x%08X\n", #ARG, cfg->ARG)
/*==============================================================================
 * serial_tty_dump_configuration()
 * Dump serial tty configuration.
 *----------------------------------------------------------------------------*/
void serial_tty_dump_configuration (SERIAL_TTY_ENTITY * const serial)
{
	int ret;
	struct termios * cfg;

	SRLD_NULL_ARGUMENT(serial);
	SRLD_NEGATIVE_ARGUMENT(serial->fd);

	cfg = &serial->cfg;

	ret = tcgetattr(serial->fd, cfg);
	SRLD_NEGATIVE_RETURN(ret, tcgetattr);

	printf("\n%s configuration:\n", serial->path);
	SERIAL_TTY_PRINT_CONFIG(c_iflag);
	SERIAL_TTY_PRINT_CONFIG(c_oflag);
	SERIAL_TTY_PRINT_CONFIG(c_cflag);
	SERIAL_TTY_PRINT_CONFIG(c_lflag);
	SERIAL_TTY_PRINT_CONFIG(c_cc[VMIN]);
	SERIAL_TTY_PRINT_CONFIG(c_cc[VTIME]);

exit:
	return;
}
/*----------------------------------------------------------------------------*/

#define SERIAL_TTY_DUMP_STRING				0
/*==============================================================================
 * serial_tty_dump_buffer()
 * Dump serial tty buffer.
 *----------------------------------------------------------------------------*/
static void serial_tty_dump_buffer (SERIAL_TTY_ENTITY * const serial)
{
	int i;

	SRLD_NULL_ARGUMENT(serial);
	SRLD_NEGATIVE_ARGUMENT(serial->fd);
	if (serial->count <= 0) {
		goto exit;
	}

	printf("%d Bytes:", serial->count);
#if SERIAL_TTY_DUMP_STRING
	printf(" %s\n", serial->buf);
#else
	for (i = 0; i < serial->count; i++) {
		printf(" 0x%02X", serial->buf[i]);
	}
	printf("\n");
#endif

exit:
	return;
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_tty_test()
 * Test serial tty.
 *----------------------------------------------------------------------------*/
int serial_tty_test (void)
{
	int ret = -1;
	long timeout = SERIAL_TTY_TEST_TIMEOUT;
	clkspec _clkStart;
	clkspec * clkStart = &_clkStart;
	SERIAL_TTY_ENTITY _serial;
	SERIAL_TTY_ENTITY * serial = &_serial;

	printf("\nSerial tty test:\n");
	ret = system("ls -l /dev/ | grep \"ttyS\\|ttyUSB\"");

	ret = serial_tty_open(serial, NULL);
	SRLD_NEGATIVE_RETURN(ret, serial_tty_open);

	ret = serial_tty_configure(serial, NULL);
	SRLD_NEGATIVE_RETURN(ret, serial_tty_configure);

	serial_tty_dump_configuration(serial);

	ret = clock_gettime(CLOCK_ID_DEFAULT, clkStart);
	SRLD_NEGATIVE_RETURN(ret, clock_gettime);
	printf("\nTest ends in %lds...\n", timeout);

	while(1) {
		sprintf(serial->buf, "%s", SERIAL_TTY_TEST_STRING);
		serial->count = strlen(serial->buf) + 1;
		ret = serial_tty_write(serial);

		ret = serial_tty_read(serial);
		if (ret > 0) {
			serial_tty_dump_buffer(serial);
		}

		ret = clock_wait_sec(clkStart, timeout);
		if (ret == 0) {
			break;
		}
		sleep(1);
	}

exit:
	if (serial->fd >= 0) {
		ret = serial_tty_close(serial);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/
