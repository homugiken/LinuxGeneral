/*==============================================================================
 * clock_time.c
 *----------------------------------------------------------------------------*/

#include "clock_time.h"

static char * clock_name[CLOCK_ID_MAX + 1] =
{
	[0 ... CLOCK_ID_MAX] = NULL,
	NAME_INITIALIZER(CLOCK_REALTIME),
	NAME_INITIALIZER(CLOCK_MONOTONIC),
	NAME_INITIALIZER(CLOCK_PROCESS_CPUTIME_ID),
	NAME_INITIALIZER(CLOCK_THREAD_CPUTIME_ID),
	NAME_INITIALIZER(CLOCK_MONOTONIC_RAW),
	NAME_INITIALIZER(CLOCK_REALTIME_COARSE),
	NAME_INITIALIZER(CLOCK_MONOTONIC_COARSE),
	NAME_INITIALIZER(CLOCK_BOOTTIME),
	NAME_INITIALIZER(CLOCK_REALTIME_ALARM),
	NAME_INITIALIZER(CLOCK_BOOTTIME_ALARM)
};

static char * compare_name[CLOCK_COMPARE_MAX + 1] =
{
	[0 ... CLOCK_COMPARE_MAX] = NULL,
	NAME_INITIALIZER(CLOCK_SAME),
	NAME_INITIALIZER(CLOCK_EARLIER),
	NAME_INITIALIZER(CLOCK_LATER)
};

/*==============================================================================
 * clock_normalize()
 * Normalize clock data.
 *----------------------------------------------------------------------------*/
static void clock_normalize (clkspec * const clk)
{
	CLKTD_NULL_ARGUMENT(clk);

	while ((clk->tv_nsec <= -NSEC_PER_SEC) || ((clk->tv_sec > 0) && (clk->tv_nsec < 0))) {
		clk->tv_sec--;
		clk->tv_nsec += NSEC_PER_SEC;
	}
	while ((clk->tv_nsec >= NSEC_PER_SEC) || ((clk->tv_sec < 0) && (clk->tv_nsec > 0))) {
		clk->tv_sec++;
		clk->tv_nsec -= NSEC_PER_SEC;
	}

exit:
	return;
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_add()
 * Calculate clkA + clkB.
 * Return calculation result. Return NULL when error.
 *----------------------------------------------------------------------------*/
int clock_add (const clkspec * const clkA, const clkspec * const clkB, clkspec * const clkSum)
{
	int ret = -1;

	CLKTD_NULL_ARGUMENT(clkA);
	CLKTD_NULL_ARGUMENT(clkB);
	CLKTD_NULL_ARGUMENT(clkSum);

	clkSum->tv_sec = clkA->tv_sec + clkB->tv_sec;
	clkSum->tv_nsec = clkA->tv_nsec + clkB->tv_nsec;
	clock_normalize(clkSum);
	ret = 0;
#if 0
	CLKTD("\nA[%lds:%09ldns]+B[%lds:%09ldns]=Sum[%lds:%09ldns]\n",
	      clkA->tv_sec, clkA->tv_nsec,
	      clkB->tv_sec, clkB->tv_nsec,
	      clkSum->tv_sec, clkSum->tv_nsec);
#endif

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_sub()
 * Calculate clkA - clkB.
 * Return calculation result. Return NULL when error.
 *----------------------------------------------------------------------------*/
int clock_sub (const clkspec * const clkA, const clkspec * const clkB, clkspec * const clkDiff)
{
	int ret = -1;

	CLKTD_NULL_ARGUMENT(clkA);
	CLKTD_NULL_ARGUMENT(clkB);
	CLKTD_NULL_ARGUMENT(clkDiff);

	clkDiff->tv_sec = clkA->tv_sec - clkB->tv_sec;
	clkDiff->tv_nsec = clkA->tv_nsec - clkB->tv_nsec;
	clock_normalize(clkDiff);
	ret = 0;
#if 0
	CLKTD("\nA[%lds:%09ldns]-B[%lds:%09ldns]=Diff[%lds:%09ldns]\n",
	      clkA->tv_sec, clkA->tv_nsec,
	      clkB->tv_sec, clkB->tv_nsec,
	      clkDiff->tv_sec, clkDiff->tv_nsec);
#endif

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_compare()
 * Compare clkA with clkB.
 * Return comparsion result. Return NULL when error.
 *----------------------------------------------------------------------------*/
int clock_compare (const clkspec * const clkA, const clkspec * const clkB)
{
	int ret = -1;
	clkspec _clkDiff;
	clkspec * clkDiff = &_clkDiff;

	CLKTD_NULL_ARGUMENT(clkA);
	CLKTD_NULL_ARGUMENT(clkB);

	ret = clock_sub(clkA, clkB, clkDiff);
	CLKTD_NEGATIVE_RETURN(ret, clock_sub);
	if ((clkDiff->tv_sec > 0) || (clkDiff->tv_nsec > 0)) {
		ret = CLOCK_LATER;
	}
	else if ((clkDiff->tv_sec < 0) || (clkDiff->tv_nsec < 0)) {
		ret = CLOCK_EARLIER;
	}
	else {
		ret = CLOCK_SAME;
	}
#if 0
	CLKTD("A[%lds:%09ldns] compares B[%lds:%09ldns] %s\n",
	      clkA->tv_sec, clkA->tv_nsec,
	      clkB->tv_sec, clkB->tv_nsec,
	      compare_name[ret]);
#endif

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_wait()
 * Wait from clkOld for clkWait of time.
 * Return 0 when timeout. Return -1 when not yet.
 *----------------------------------------------------------------------------*/
int clock_wait (const clkspec * const clkOld, const clkspec * const clkWait)
{
	int ret = -1;
	clkspec _clkNow;
	clkspec * clkNow = &_clkNow;
	clkspec _clkDue;
	clkspec * clkDue = &_clkDue;

	CLKTD_NULL_ARGUMENT(clkOld);
	CLKTD_NULL_ARGUMENT(clkWait);

	ret = clock_add(clkOld, clkWait, clkDue);
	CLKTD_NEGATIVE_RETURN(ret, clock_add);
	if ((clkDue->tv_sec <= 0) && (clkDue->tv_nsec <= 0)) {
		ret = 0;
		goto exit;
	}

	ret = clock_gettime(CLOCK_ID_DEFAULT, clkNow);
	CLKTD_NEGATIVE_RETURN(ret, clock_gettime);

	if (clock_compare(clkNow, clkDue) == CLOCK_EARLIER) {
		ret = -1;
	}
	else {
		ret = 0;
	}
#if 0
	CLKTD("Now[%lds:%09ldns] timeout\n", clkNow->tv_sec, clkNow->tv_nsec);
#endif

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_wait_nano()
 * Wait from clkOld for nano nanoseconds of time.
 * Return 0 when timeout. Return -1 when not yet.
 *----------------------------------------------------------------------------*/
int clock_wait_nano (const clkspec * const clkOld, const long nano)
{
	int ret;
	clkspec _clkWait;
	clkspec * clkWait = &_clkWait;

	clkWait->tv_sec = 0;
	clkWait->tv_nsec = nano;
	clock_normalize(clkWait);

	ret = clock_wait(clkOld, clkWait);

	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_wait_sec()
 * Wait from clkOld for sec seconds of time.
 * Return 0 when timeout. Return -1 when not yet.
 *----------------------------------------------------------------------------*/
int clock_wait_sec (const clkspec * const clkOld, const long sec)
{
	int ret;
	clkspec _clkWait;
	clkspec * clkWait = &_clkWait;

	clkWait->tv_sec = sec;
	clkWait->tv_nsec = 0;
	clock_normalize(clkWait);

	ret = clock_wait(clkOld, clkWait);

	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_show()
 * Show current clock data.
 *----------------------------------------------------------------------------*/
static void clock_show (void)
{
	clkspec _clk;
	clkspec * clk = &_clk;
	int i;

	printf("\nClock IDs:\n");
	for (i = 0; i < CLOCK_ID_MAX; i++) {
		if (clock_name[i] == NULL) {
			continue;
		}
		if (clock_gettime(i, clk) < 0) {
			continue;
		}
		printf("[%10lds:%09ldns] %s\n", clk->tv_sec, clk->tv_nsec, clock_name[i]);
	}

}
/*----------------------------------------------------------------------------*/

#define CLOCK_TEST_DATA_STEP				(NSEC_PER_SEC / 4)
#define CLOCK_TEST_DATA_LIMIT				17

static clkspec clock_test_data[CLOCK_TEST_DATA_LIMIT];
static const clkspec _clkStep = {0, CLOCK_TEST_DATA_STEP};
static const clkspec * clkStep = &_clkStep;

/*==============================================================================
 * clock_add_test()
 * Test clock add.
 *----------------------------------------------------------------------------*/
static void clock_add_test (void)
{
	clkspec _clkA;
	clkspec * clkA = &_clkA;
	clkspec _clkSum;
	clkspec * clkSum = &_clkSum;
	int i;
	int ret;

	printf("\nClock add test:\n");

	clkA->tv_sec = -1;
	clkA->tv_nsec = -(NSEC_PER_SEC / 2);
	for (i = 0; i < CLOCK_TEST_DATA_LIMIT; i++) {
		ret = clock_add(clkA, &clock_test_data[i], clkSum);
		CLKTD_NEGATIVE_RETURN(ret, clock_add);
		printf("[%lds:%09ldns]+[%lds:%09ldns]=[%lds:%09ldns]\n",
		       clkA->tv_sec, clkA->tv_nsec,
		       clock_test_data[i].tv_sec, clock_test_data[i].tv_nsec,
		       clkSum->tv_sec, clkSum->tv_nsec);
	}

exit:
	return;
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_sub_test()
 * Test clock sub.
 *----------------------------------------------------------------------------*/
static void clock_sub_test (void)
{
	clkspec _clkA;
	clkspec * clkA = &_clkA;
	clkspec _clkDiff;
	clkspec * clkDiff = &_clkDiff;
	int i;
	int ret;

	printf("\nClock sub test:\n");

	clkA->tv_sec = 1;
	clkA->tv_nsec = (NSEC_PER_SEC / 2);
	for (i = 0; i < CLOCK_TEST_DATA_LIMIT; i++) {
		ret = clock_sub(clkA, &clock_test_data[i], clkDiff);
		CLKTD_NEGATIVE_RETURN(ret, clock_sub);
		printf("[%lds:%09ldns]-[%lds:%09ldns]=[%lds:%09ldns]\n",
		       clkA->tv_sec, clkA->tv_nsec,
		       clock_test_data[i].tv_sec, clock_test_data[i].tv_nsec,
		       clkDiff->tv_sec, clkDiff->tv_nsec);
	}

exit:
	return;
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_compare_test()
 * Test clock compare.
 *----------------------------------------------------------------------------*/
static void clock_compare_test (void)
{
	clkspec _clkA;
	clkspec * clkA = &_clkA;
	int i;
	int ret;

	printf("\nClock compare test:\n");

	clkA->tv_sec = 0;
	clkA->tv_nsec = -(NSEC_PER_SEC / 2);
	for (i = 0; i < CLOCK_TEST_DATA_LIMIT; i += 2) {
		ret = clock_compare(clkA, &clock_test_data[i]);
		CLKTD_NEGATIVE_RETURN(ret, clock_compare);
		if ((ret < 0) || (ret > CLOCK_COMPARE_MAX) || (compare_name[ret] == NULL)) {
			continue;
		}
		printf("[%lds:%09ldns] compares [%lds:%09ldns] %s\n",
		       clkA->tv_sec, clkA->tv_nsec,
		       clock_test_data[i].tv_sec, clock_test_data[i].tv_nsec,
		       compare_name[ret]);
	}

	clkA->tv_sec = 0;
	clkA->tv_nsec = (NSEC_PER_SEC / 2);
	for (i = 0; i < CLOCK_TEST_DATA_LIMIT; i += 2) {
		ret = clock_compare(clkA, &clock_test_data[i]);
		CLKTD_NEGATIVE_RETURN(ret, clock_compare);
		if ((ret < 0) || (ret > CLOCK_COMPARE_MAX) || (compare_name[ret] == NULL)) {
			continue;
		}
		printf("[%lds:%09ldns] compares [%lds:%09ldns] %s\n",
		       clkA->tv_sec, clkA->tv_nsec,
		       clock_test_data[i].tv_sec, clock_test_data[i].tv_nsec,
		       compare_name[ret]);
	}

exit:
	return;
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_wait_test()
 * Test clock wait.
 *----------------------------------------------------------------------------*/
static void clock_wait_test (void)
{
	clkspec _clkOld;
	clkspec * clkOld = &_clkOld;
	clkspec _clkWait;
	clkspec * clkWait = &_clkWait;
	int i;
	int ret;

	printf("\nClock wait test:\n");

	clkWait->tv_sec = 0;
	clkWait->tv_nsec = (NSEC_PER_SEC / 2);
	for (i = 0; i < 10; i++) {
		ret = clock_add(clkWait, clkStep, clkWait);
		CLKTD_NEGATIVE_RETURN(ret, clock_add);

		ret = clock_gettime(CLOCK_ID_DEFAULT, clkOld);
		CLKTD_NEGATIVE_RETURN(ret, clock_gettime);
		printf("clkWait=[%lds:%09ldns] clkOld=[%lds:%09ldns]",
		       clkWait->tv_sec, clkWait->tv_nsec,
		       clkOld->tv_sec, clkOld->tv_nsec);

		while(clock_wait(clkOld, clkWait) != 0) {}
		ret = clock_gettime(CLOCK_ID_DEFAULT, clkOld);
		CLKTD_NEGATIVE_RETURN(ret, clock_gettime);
		printf(" clkNow=[%lds:%09ldns]\n", clkOld->tv_sec, clkOld->tv_nsec);
	}

exit:
	return;
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_test()
 * Test clock.
 *----------------------------------------------------------------------------*/
int clock_test (void)
{
	int ret = -1;
	int i;
	clkspec _clkTmp;
	clkspec * clkTmp = &_clkTmp;

	clock_show();

	clock_test_data[0].tv_sec = -2;
	clock_test_data[0].tv_nsec = 0;
	for (i = 1; i < CLOCK_TEST_DATA_LIMIT; i++) {
		ret = clock_add(&clock_test_data[i - 1], clkStep, clkTmp);
		CLKTD_NEGATIVE_RETURN(ret, clock_add);
		clock_test_data[i].tv_sec = clkTmp->tv_sec;
		clock_test_data[i].tv_nsec = clkTmp->tv_nsec;
	}
#if 0
	printf("\nclock_test_data:\n");
	printf("step=[%lds:%09ldns]\n", clkStep->tv_sec, clkStep->tv_nsec);
	for (i = 0; i < CLOCK_TEST_DATA_LIMIT; i++) {
		printf("data[%02d]=[%lds:%09ldns]\n",
		       i, clock_test_data[i].tv_sec, clock_test_data[i].tv_nsec);
	}
#endif

	clock_add_test();
	clock_sub_test();
	clock_compare_test();
	clock_wait_test();
	ret = 0;

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/
