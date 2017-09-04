/*==============================================================================
 * input.c
 *----------------------------------------------------------------------------*/

#include "input.h"

/*==============================================================================
 * input_open()
 * Open input event.
 *----------------------------------------------------------------------------*/
int input_open (const char * const name)
{
	int ret = -1;
	char path[INPUT_EVENT_PATH_LENGTH];

	if (name == NULL) {
		memset(path, 0, sizeof (path));
		printf("\nPlease enter input event path: ");
		ret = scanf("%1024s", path);
		if (ret <= 0) {
			goto exit;
		}

		printf("Entered input event path: %s\n", path);
		ret = open(path, O_RDONLY);
	} else {
		ret = open(name, O_RDONLY);
	}

	if (ret < 0) {
		INPTD("open failed ret=%d\n", ret);
	}
	else {
		INPTD("open fd=%d\n", ret);
	}

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_close()
 * Close input event.
 *----------------------------------------------------------------------------*/
int input_close (const int fd)
{
	int ret;

	ret = close(fd);
	if (ret < 0) {
		INPTD("close failed ret=%d\n", ret);
	}
	else {
		INPTD("close fd=%d\n", fd);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_read()
 * Return number of bytes read. Return -1 when error.
 *----------------------------------------------------------------------------*/
int input_read (const int fd, inputevent * const input)
{
	int ret;

	ret = read(fd, input, sizeof (inputevent));
	if (ret < 0) {
		INPTD("read failed ret=%d\n", ret);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_key_dump()
 * Dump input key.
 *----------------------------------------------------------------------------*/
void input_key_dump (const inputevent * const input)
{
	if (input == NULL) {
		INPTD("parameter error\n");
		return;
	}
	if (input->type != EV_KEY) {
		return;
	}

	printf("\t{KEY %u|0x%X} ", input->code, input->code);

	switch (input->value) {
	case (INPUT_KEY_RELEASED):
		printf("released\n");
		break;
	case (INPUT_KEY_PRESSED):
		printf("pressed\n");
		break;
	case (INPUT_KEY_REPEATED):
		printf("repeated\n");
		break;
	default:
		printf("unknown operation\n");
		break;
	}
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_abs_dump()
 * Dump input absolute axes.
 *----------------------------------------------------------------------------*/
void input_abs_dump (const inputevent * const input)
{
	if (input == NULL) {
		INPTD("parameter error\n");
		return;
	}
	if (input->type != EV_ABS) {
		return;
	}

	printf("\t{ABS ");

	switch (input->code) {
	case (ABS_X):
		printf("X ");
		break;
	case (ABS_Y):
		printf("Y ");
		break;
	default:
		printf("axe=%u|0x%X ", input->code, input->code);
		break;
	}

	printf("value=%u|0x%X}\n", input->value, input->value);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_rel_dump()
 * Dump input relative axes.
 *----------------------------------------------------------------------------*/
void input_rel_dump (const inputevent * const input)
{
	if (input == NULL) {
		INPTD("parameter error\n");
		return;
	}
	if (input->type != EV_REL) {
		return;
	}

	printf("\t{REL ");

	switch (input->code) {
	case (REL_X):
		printf("X ");
		break;
	case (REL_Y):
		printf("Y ");
		break;
	default:
		printf("axe=%u|0x%X ", input->code, input->code);
		break;
	}

	printf("value=%u|0x%X}\n", input->value, input->value);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_dump()
 * Dump input event.
 *----------------------------------------------------------------------------*/
void input_dump (const inputevent * const input)
{
	if (input == NULL) {
		INPTD("parameter error\n");
		return;
	}

	switch (input->type) {
	case (EV_SYN):
		break;
	case (EV_KEY):
		input_key_dump(input);
		break;
	case (EV_REL):
		input_rel_dump(input);
		break;
	case (EV_ABS):
		input_abs_dump(input);
		break;
	case (EV_MSC):
		break;
	// case (EV_SW):
	// 	break;
	// case (EV_LED):
	// 	break;
	// case (EV_SND):
	// 	break;
	// case (EV_REP):
	// 	break;
	// case (EV_FF):
	// 	break;
	// case (EV_PWR):
	// 	break;
	// case (EV_FF_STATUS):
	// 	break;
	default:
		INPTD("time=[%lds:%06ldus] type=%u|0x%X code=%u|0x%X value=%u|0x%X\n",
		      input->time.tv_sec, input->time.tv_usec,
		      input->type, input->type,
		      input->code, input->code,
		      input->value, input->value
		      );
		break;
	}
}
/*----------------------------------------------------------------------------*/

int input_test (void)
{
	int ret = -1;
	int fd = -1;
	int i;
	inputevent input;

	printf("input test start\n");

	ret = system("ls -lR /dev/input/");

	ret = input_open(NULL);
	if (ret < 0) {
		goto exit;
	}
	fd = ret;

	while (1) {
		ret = input_read(fd, &input);
		if (ret > 0) {
			input_dump(&input);
		}
	}

exit:
	if (fd >= 0) {
		ret = input_close(fd);
	}

	return (ret);
}
