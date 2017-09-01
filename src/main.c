/*==============================================================================
 * main.c
 *----------------------------------------------------------------------------*/

#include "common.h"
#include "clock.h"

int main (
	int argc,
	char *argv[]
	)
{
	int ret = -1;

	clock_test();

exit:
	return (ret);
}
