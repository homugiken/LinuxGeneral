/*==============================================================================
 * main.c
 *----------------------------------------------------------------------------*/

#include "common.h"
#include "clock.h"
#include "input.h"

#define GENERAL_TEST_INDEX_CLOCK			'0'
#define GENERAL_TEST_INDEX_INPUT			'a'

typedef struct GENERAL_TEST_OPTION
{
	char index;
	char *name;
} GENERAL_TEST_OPTION;

static GENERAL_TEST_OPTION general_test_list[] =
{
	{GENERAL_TEST_INDEX_CLOCK, "clock test"},
	{GENERAL_TEST_INDEX_INPUT, "input test"},
	{'\0', NULL}
};

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
		ret = input_test();
		break;
	default:
		printf("Invalid choice\n");
		break;
	}

exit:
	return (ret);
}

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
