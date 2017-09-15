/*
 ============================================================================
 Name        : eclipse.c
 Author      : homu
 Version     :
 Copyright   :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "input_event.h"

int main(void) {
	int ret;

	puts("hello"); /* prints hello */

	ret = input_event_test();


	return EXIT_SUCCESS;
}
