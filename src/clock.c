/*==============================================================================
 * clock.c
 *----------------------------------------------------------------------------*/

#include "clock.h"

static const clockspec _clkZero = {0, 0};
static const clockspec * clkZero = &_clkZero;

static const CLOCK_TYPE clock_name[] =
{
	{CLOCK_REALTIME, "CLOCK_REALTIME"},
	{CLOCK_MONOTONIC, "CLOCK_MONOTONIC"},
	{CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID"},
	{CLOCK_THREAD_CPUTIME_ID, "CLOCK_THREAD_CPUTIME_ID"}
};

static const char * const cmp_name[3] =
{
	[CLOCK_COMPARE_EARLIER] = "earlier",
	[CLOCK_COMPARE_SAME] = "same",
	[CLOCK_COMPARE_LATER] = "later"
};

/*==============================================================================
 * clock_norm()
 * Normalize clock value.
 *----------------------------------------------------------------------------*/
static void clock_norm (clockspec * const clock)
{
	if (clock == NULL) {
		CLKD("parameter error\n");
		return;
	}

	while (clock->tv_nsec >= NSEC_PER_SEC) {
		++clock->tv_sec;
		clock->tv_nsec -= NSEC_PER_SEC;
	}
	while (clock->tv_nsec <= -NSEC_PER_SEC) {
		--clock->tv_sec;
		clock->tv_nsec += NSEC_PER_SEC;
	}
	while ((clock->tv_sec > 0) && (clock->tv_nsec < 0)) {
		--clock->tv_sec;
		clock->tv_nsec += NSEC_PER_SEC;
	}
	while ((clock->tv_sec < 0) && (clock->tv_nsec > 0)){
		++clock->tv_sec;
		clock->tv_nsec -= NSEC_PER_SEC;
	}
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_add()
 * clkA adds clkB = clkSum
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int clock_add (const clockspec * const clkA, const clockspec * const clkB, clockspec * const clkSum)
{
	int ret = -1;

	if ((clkA == NULL) || (clkB == NULL) || (clkSum == NULL)) {
		CLKD("parameter error\n");
		goto exit;
	}

	clkSum->tv_sec = clkA->tv_sec + clkB->tv_sec;
	clkSum->tv_nsec = clkA->tv_nsec + clkB->tv_nsec;
	clock_norm(clkSum);

	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_sub()
 * clkA subtracts clkB = clkDiff
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int clock_sub (const clockspec * const clkA, const clockspec * const clkB, clockspec * const clkDiff)
{
	int ret = -1;

	if ((clkA == NULL) || (clkB == NULL) || (clkDiff == NULL)) {
		CLKD("parameter error\n");
		goto exit;
	}

	clkDiff->tv_sec = clkA->tv_sec - clkB->tv_sec;
	clkDiff->tv_nsec = clkA->tv_nsec - clkB->tv_nsec;
	clock_norm(clkDiff);

	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_cmp()
 * clkA compares clkB = compare
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int clock_cmp (const clockspec * const clkA, const clockspec * const clkB, int * const compare)
{
	int ret = -1;
	clockspec clkDiff;

	if ((clkA == NULL) || (clkB == NULL) || (compare == NULL)) {
		CLKD("parameter error\n");
		goto exit;
	}

	if (clock_sub(clkA, clkB, &clkDiff) < 0) {
		goto exit;
	}

	if (clkDiff.tv_sec > 0) {
		*compare = CLOCK_COMPARE_LATER;
	}
	else if (clkDiff.tv_sec < 0) {
		*compare = CLOCK_COMPARE_EARLIER;
	}
	else if (clkDiff.tv_nsec > 0) {
		*compare = CLOCK_COMPARE_LATER;
	}
	else if (clkDiff.tv_nsec < 0) {
		*compare = CLOCK_COMPARE_EARLIER;
	}
	else {
		*compare = CLOCK_COMPARE_SAME;
	}

	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_wait()
 * Wait till clkNow is later than clkOld adds clkWait
 * Return 0 when timeout. Return -1 when not yet.
 *----------------------------------------------------------------------------*/
int clock_wait (const clockspec * const clkNow, const clockspec * const clkOld, const clockspec * const clkWait)
{
	int ret = -1;
	clockspec clkDiff;
	int compare;

	if ((clkOld == NULL) || (clkNow == NULL) || (clkWait == NULL)) {
		CLKD("parameter error\n");
		goto exit;
	}
	if (clock_cmp(clkWait, clkZero, &compare) < 0) {
		goto exit;
	}
	if (compare != CLOCK_COMPARE_LATER) {
		CLKD("clkWait=[%lds:%09ldns] error\n",
		     clkWait->tv_sec, clkWait->tv_nsec
		     );
		goto exit;
	}

	if (clock_sub(clkNow, clkOld, &clkDiff) < 0) {
		goto exit;
	}
	if (clock_cmp(clkNow, clkOld, &compare) < 0) {
		goto exit;
	}
	if ((compare == CLOCK_COMPARE_SAME) || (compare == CLOCK_COMPARE_LATER)) {
		ret = 0;
	}

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_show()
 * Print all system clock current values.
 *----------------------------------------------------------------------------*/
void clock_show (void)
{
	clockspec clock;
	uint8_t i;

	printf("\n");
	for (i = 0; i < (sizeof (clock_name) / sizeof (CLOCK_TYPE)); i++) {
		if (clock_gettime(clock_name[i].id, &clock) == 0) {
			printf("%s=[%lds:%09ldns]\n",
			       clock_name[i].name,
			       clock.tv_sec, clock.tv_nsec
			       );
		}
	}
}
/*----------------------------------------------------------------------------*/

#if DEBUG_CLOCK

static clockspec clkTest[CLOCK_TEST_LENGTH];
static const clockspec _clkStep = {0, CLOCK_TEST_STEP};
static const clockspec * clkStep = &_clkStep;

static void clock_test_load (void)
{
	int i;

	clkTest[0].tv_sec = -2;
	clkTest[0].tv_nsec = 0;
	for (i = 1; i < CLOCK_TEST_LENGTH; i++) {
		clock_add(&clkTest[i - 1], clkStep, &clkTest[i]);
	}
#if 0
	printf("\nclock test data:\n");
	printf("clkStep=[%lds:%09ldns]\n",
	       clkStep->tv_sec, clkStep->tv_nsec
	       );
	for (i = 0; i < CLOCK_TEST_LENGTH; i++) {
		printf("clkTest[%02d]=[%lds:%09ldns]\n",
		       i, clkTest[i].tv_sec, clkTest[i].tv_nsec
		       );
	}
	printf("\n");
#endif
}

static void clock_add_test (void)
{
	int i;
	clockspec clkSum;

	printf("\nclock add test\n");
	for (i = 0; i < CLOCK_TEST_LENGTH; i++) {
		if (clock_add(&clkTest[1], &clkTest[i], &clkSum) == 0) {
			printf("[%lds:%09ldns]+[%lds:%09ldns]=[%lds:%09ldns]\n",
			       clkTest[1].tv_sec, clkTest[1].tv_nsec,
			       clkTest[i].tv_sec, clkTest[i].tv_nsec,
			       clkSum.tv_sec, clkSum.tv_nsec
			       );
		}
	}
	for (i = 0; i < CLOCK_TEST_LENGTH; i++) {
		if (clock_add(&clkTest[CLOCK_TEST_LENGTH - 2], &clkTest[i], &clkSum) == 0) {
			printf("[%lds:%09ldns]+[%lds:%09ldns]=[%lds:%09ldns]\n",
			       clkTest[CLOCK_TEST_LENGTH - 2].tv_sec, clkTest[CLOCK_TEST_LENGTH - 2].tv_nsec,
			       clkTest[i].tv_sec, clkTest[i].tv_nsec,
			       clkSum.tv_sec, clkSum.tv_nsec
			       );
		}
	}
}

static void clock_sub_test (void)
{

}

static void clock_cmp_test (void)
{

}

static void clock_wait_test (void)
{

}


int clock_test (void)
{
	clock_test_load();

	printf("clock test start\n");
	clock_show();
	clock_add_test();

	return (0);

	clock_sub_test();
	clock_cmp_test();
	clock_wait_test();

	printf("clock test done\n");

	return (0);
}
#else
int clock_test (void) { return (0);}
#endif

