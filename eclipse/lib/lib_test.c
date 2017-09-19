/*==============================================================================
 * lib_test.c
 *----------------------------------------------------------------------------*/

#include "input_event.h"
#include "clock_time.h"
#include "serial_tty.h"

#define LIB_NULL_ARGUMENT(ARG)				CHECK_NULL_ARGUMENT(ARG, LIBDBG)
#define LIB_NEGATIVE_ARGUMENT(ARG)			CHECK_NEGATIVE_ARGUMENT(ARG, LIBDBG)

#define LIB_NULL_RETURN(RET, FUNC)			CHECK_NULL_RETURN(RET, FUNC, LIBDBG)
#define LIB_NEGATIVE_RETURN(RET, FUNC)			CHECK_NEGATIVE_RETURN(RET, FUNC, LIBDBG)

#define LIB_TEST_CLOCK_TIME				0x00
#define LIB_TEST_INPUT_EVENT				0x01
#define LIB_TEST_SERIAL_TTY				0x02
#define LIB_TEST_MAX					0x0F
#define LIB_TEST_CNT					(LIB_TEST_MAX + 1)

static char * lib_test_name[LIB_TEST_MAX + 1] =
{
	[0 ... LIB_TEST_MAX] = NULL,
	NAME_INITIALIZER(LIB_TEST_CLOCK_TIME),
	NAME_INITIALIZER(LIB_TEST_INPUT_EVENT),
	NAME_INITIALIZER(LIB_TEST_SERIAL_TTY)
};

typedef int (* general_test)(void);
static general_test lib_test_func[LIB_TEST_MAX + 1] =
{
	[0 ... LIB_TEST_MAX] = NULL,
	[LIB_TEST_CLOCK_TIME] = clock_test,
	[LIB_TEST_INPUT_EVENT] = input_event_test,
	[LIB_TEST_SERIAL_TTY] = serial_tty_test
};

/*==============================================================================
 * lib_test()
 * Test modules in lib.
 *----------------------------------------------------------------------------*/
int lib_test (void)
{
	int ret = -1;
	int i;

	printf("\nAvailable:\n");
	for (i = 0; i < LIB_TEST_MAX; i++) {
		if (lib_test_name[i] == NULL) {
			continue;
		}
		printf("\t[0x%02X]\t%s\n", i, lib_test_name[i]);
	}

	printf("\nPlease select: ");
	ret = scanf("%x", &i);
	LIB_NEGATIVE_RETURN(ret, scanf);
	if ((i < 0) || (i >= LIB_TEST_MAX) || (lib_test_func[i] == NULL) || (lib_test_func[i] == NULL)) {
		LIBDBG("selection invalid\n");
		goto exit;
	}

	printf("\t[0x%02X]\t%s selected\n", i, lib_test_name[i]);
	ret = lib_test_func[i]();
	LIB_NEGATIVE_RETURN(ret, scanf);



exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/
