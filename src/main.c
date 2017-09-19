/*==============================================================================
 * main.c
 *----------------------------------------------------------------------------*/

#include "common.h"
#include "clock.h"
#include "input_event.h"
#include "serial_io.h"

#define GENERAL_TEST_INDEX_CLOCK			'0'
#define GENERAL_TEST_INDEX_INPUT			'a'

#define TEST_ENTITY_MAX					'z'
typedef struct GENERAL_TEST_ENTITY
{
	char * name;
	int (*func)(void);
} GENERAL_TEST_ENTITY;

static const GENERAL_TEST_ENTITY test_entity[TEST_ENTITY_MAX + 1] =
{
	[0 ... TEST_ENTITY_MAX] = {0, NULL},
	['0'] = {"clock test", clock_test},
	['a'] = {"input event test", input_event_test},
	['b'] = {"serial io test", serial_io_test},
};


typedef struct GENERAL_TEST_OPTION
{
	char index;
	char *name;
} GENERAL_TEST_OPTION;

static GENERAL_TEST_OPTION general_test_list[] =
{
	{GENERAL_TEST_INDEX_CLOCK, "clock test"},
	{GENERAL_TEST_INDEX_INPUT, "input event test"},
	{'\0', NULL}
};

#if 0
int general_test (void)
{
	int ret = -1;
	char choice;
	int i;

	printf("Avaliable test functions: \n");
	for (i = 0; general_test_list[i].index != '\0'; i++) {
		if ((general_test_list[i].index != '\0') &&
		    (general_test_list[i].name != NULL)) {
			printf("\t%c\t%s\n",
			       general_test_list[i].index,
			       general_test_list[i].name
			       );
		}
	}

	printf("Please choose test to run: ");
	choice = getc(stdin);

	switch (choice) {
	case (GENERAL_TEST_INDEX_CLOCK):
		clock_test();
		break;
	case (GENERAL_TEST_INDEX_INPUT):
		ret = input_event_test();
		break;
	default:
		printf("Invalid choice\n");
		break;
	}

exit:
	return (ret);
}
#else
int general_test (void)
{
	int ret = -1;
	char choice;
	int i;

	printf("Avaliable test functions: \n");
	for (i = 0; i <= TEST_ENTITY_MAX; i++) {
		if ((test_entity[i].name != NULL) && (test_entity[i].func != NULL)) {
			printf("\t%c\t%s\n", (char)(i), test_entity[i].name);
		}
	}

	printf("Please choose test to run: ");
	choice = getc(stdin);
	if ((test_entity[choice].name != NULL) && (test_entity[choice].func != NULL)) {
		ret = test_entity[choice].func();
	}
	else {
		printf("Invalid choice\n");
	}

exit:
	return (ret);
}
#endif

int main (
	int argc,
	char *argv[]
	)
{
	int ret = -1;

	ret = general_test();

exit:
	return (ret);
}
