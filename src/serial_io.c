/*==============================================================================
 * serial_io.c
 *----------------------------------------------------------------------------*/

#include "serial_io.h"
// #include "clock.h"

/*==============================================================================
 * serial_io_open()
 * Open serial io.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_io_open (const char * const name, int * const fd)
{
	int ret = -1;
	char path[SERIAL_IO_PATH_LENGTH];

	if (fd == NULL) {
		SIOD("parameter error\n");
		goto exit;
	}

	if (name != NULL) {
		if (strcpy(path, name) != path) {
			SIOD("strcpy error\n");
			goto exit;
		}
	}
	else {
		printf("\nPlease enter serial io path: ");
		ret = scanf("%128s", path);
		if (ret <= 0) {
			SIOD("scanf error ret=%d\n", ret);
			goto exit;
		}
	}

	SIOD("try to open: %s\n", path);
	ret = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
	if (ret < 0) {
		SIOD("open error ret=%d\n", ret);
	}
	else if (isatty(ret) != 1) {
		SIOD("isatty error\n");
	}
	else {
		*fd = ret;
		printf("open %s fd=%d\n", path, *fd);
	}

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_io_close()
 * Close serial io.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_io_close (const int fd)
{
	int ret = -1;

	if (fd < 0) {
		goto exit;
	}

	ret = close(fd);
	if (ret < 0) {
		SIOD("close error ret=%d\n", ret);
	}
	else {
		SIOD("close success fd=%d\n", fd);
	}

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_io_configure()
 * Configure serial io.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int serial_io_configure (const int fd)
{
	int ret;
	struct termios _cfg;
	struct termios * cfg = &_cfg;

	ret = tcgetattr(fd, cfg);
	if (ret < 0) {
		SIOD("tcgetattr error ret=%d\n", ret);
		goto exit;
	}

	ret = tcflush(fd, TCIOFLUSH);
	if (ret < 0) {
		SIOD("tcflush error ret=%d\n", ret);
		goto exit;
	}

	cfmakeraw(cfg);
	cfg->c_cc[VMIN] = 1;
	cfg->c_cc[VTIME] = 0;

	ret = cfsetspeed(cfg, SERIAL_IO_SPEED_DEFAULT);
	if (ret < 0) {
		SIOD("cfsetspeed error ret=%d\n", ret);
		goto exit;
	}

	ret = tcsetattr(fd, TCSAFLUSH, cfg);
	if (ret < 0) {
		SIOD("tcsetattr error ret=%d\n", ret);
		goto exit;
	}

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_io_rx()
 * Read from serial io.
 * Return number of bytes read.
 *----------------------------------------------------------------------------*/
int serial_io_rx (const int fd, void * const data, const size_t count)
{
	int ret;

	ret = read(fd, data, count);
	if (ret < 0) {
		SIOD("read error ret=%d\n", ret);
	}
	else {
		SIOD("read success ret=%d\n", ret);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_io_tx()
 * Write to serial io.
 * Return number of bytes written.
 *----------------------------------------------------------------------------*/
int serial_io_tx (const int fd, void * const data, const size_t count)
{
	int ret;

	ret = write(fd, data, count);
	if (ret < 0) {
		SIOD("write error ret=%d\n", ret);
	}
	else {
		SIOD("write success ret=%d\n", ret);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_io_tx()
 * Write to serial io.
 * Return number of bytes written.
 *----------------------------------------------------------------------------*/
void serial_io_attribution_dump (const int fd)
{
	int ret;
	struct termios _attr;
	struct termios * attr = &_attr;

	ret = tcgetattr(fd, attr);
	if (ret < 0) {
		SIOD("tcgetattr error ret=%d\n", ret);
		return;
	}

	printf("current attribution:\n");
	printf("\tc_iflag\t\t0x%08X\n", attr->c_iflag);
	printf("\tc_oflag\t\t0x%08X\n", attr->c_oflag);
	printf("\tc_cflag\t\t0x%08X\n", attr->c_cflag);
	printf("\tc_lflag\t\t0x%08X\n", attr->c_lflag);
	printf("\tc_cc[VMIN]\t0x%08X\n", attr->c_cc[VMIN]);
	printf("\tc_cc[VTIME]\t0x%08X\n", attr->c_cc[VTIME]);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * serial_io_tx()
 * Write to serial io.
 * Return number of bytes written.
 *----------------------------------------------------------------------------*/
int serial_io_test (void)
{
	int ret = -1;
	int fd = -1;
	int i;
	int count;
	uint8_t buf[SERIAL_IO_TEST_BUFFER_LENGTH];

	ret = system("ls -l /dev/ | grep \"ttyS\\|ttyUSB\"");

	ret = serial_io_open(NULL, &fd);
	if (ret < 0) {
		goto exit;
	}
	serial_io_attribution_dump(fd);

	ret = serial_io_configure(fd);
	if (ret < 0) {
		goto exit;
	}
	memset(buf, 0, sizeof (buf));

	printf("serial io test start\n");
	for (count = 0; count < SERIAL_IO_TEST_COUNT; count++) {
		ret = serial_io_tx(fd, SERIAL_IO_TEST_STRING, sizeof (SERIAL_IO_TEST_STRING));
		if (ret > 0) {
			printf("tx %d bytes\n", ret);
		}

		ret = serial_io_rx(fd, buf, sizeof (buf));
		if (ret > 0) {
			printf("rx %d bytes: \"%s\"", ret, buf);
			for (i = 0; i < ret; i++) {
				printf("  %02X", buf[i]);
			}
			printf("\n");
		}

		sleep(1);
	}
	printf("serial io test done\n");

exit:
	if (fd >= 0) {
		ret = serial_io_close(fd);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/

