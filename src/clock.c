/*==============================================================================
 * clock.c
 *----------------------------------------------------------------------------*/

#include "clock.h"

static CLOCK_TYPE gClockType[] = {
	{CLOCK_REALTIME, "CLOCK_REALTIME"},
	{CLOCK_MONOTONIC, "CLOCK_MONOTONIC"},
	{CLOCK_PROCESS_CPUTIME_ID, "CLOCK_PROCESS_CPUTIME_ID"},
	{CLOCK_THREAD_CPUTIME_ID, "CLOCK_THREAD_CPUTIME_ID"}
};

/*==============================================================================
 * clock_norm()
 * Normalize the clock value.
 *----------------------------------------------------------------------------*/
static void clock_norm (
	clockspec * const clock
	)
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
 * Add the values of ca and cb, then save to result.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int clock_add (
	const clockspec * const ca,
	const clockspec * const cb,
	clockspec * const result
	)
{
	int ret = -1;

	if ((ca == NULL) || (cb == NULL) || (result == NULL)) {
		CLKD("parameter error\n");
		goto exit;
	}

	result->tv_sec = ca->tv_sec + cb->tv_sec;
	result->tv_nsec = ca->tv_nsec + cb->tv_nsec;
	clock_norm(result);
#if 0
	CLKD("[%ld:%ld]+[%ld:%ld]=[%ld:%ld]\n",
	     ca->tv_sec, ca->tv_nsec,
	     cb->tv_sec, cb->tv_nsec,
	     result->tv_sec, result->tv_nsec
	     );
#endif
	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_sub()
 * Subtract the values of cb from ca, then save to result.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int clock_sub (
	const clockspec * const ca,
	const clockspec * const cb,
	clockspec * const result
	)
{
	int ret = -1;

	if ((ca == NULL) || (cb == NULL) || (result == NULL)) {
		CLKD("parameter error\n");
		goto exit;
	}

	result->tv_sec = ca->tv_sec - cb->tv_sec;
	result->tv_nsec = ca->tv_nsec - cb->tv_nsec;
	clock_norm(result);
#if 0
	CLKD("[%ld:%ld]-[%ld:%ld]=[%ld:%ld]\n",
	     ca->tv_sec, ca->tv_nsec,
	     cb->tv_sec, cb->tv_nsec,
	     result->tv_sec, result->tv_nsec
	     );
#endif
	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_compare()
 * Compare the value of ca and cb, then save to result.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int clock_compare (
	const clockspec * const ca,
	const clockspec * const cb,
	int * const result
	)
{
	int ret = -1;
	clockspec sub;

	if ((ca == NULL) || (cb == NULL) || (result == NULL)) {
		CLKD("parameter error\n");
		goto exit;
	}

	ret = clock_sub(ca, cb, &sub);
	if (ret != 0) {
		goto exit;
	}

	if (sub.tv_sec > 0) {
		*result = CLOCK_COMPARE_OLDER;
	}
	else if (sub.tv_sec < 0) {
		*result = CLOCK_COMPARE_EARLIER;
	}
	else if (sub.tv_nsec > 0) {
		*result = CLOCK_COMPARE_OLDER;
	}
	else if (sub.tv_nsec < 0) {
		*result = CLOCK_COMPARE_EARLIER;
	}
	else {
		*result = CLOCK_COMPARE_SAME;
	}
#if 0
	CLKD("[%ld:%ld] compares [%ld:%ld]=%d\n",
	     ca->tv_sec, ca->tv_nsec,
	     cb->tv_sec, cb->tv_nsec,
	     result
	     );
#endif
	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_add()
 * Compare the value of ca and cb, then save to result.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int clock_wait (
	const clockspec * const cold,
	const clockspec * const cnow,
	const clockspec * const wait
	)
{
	int ret = -1;
	clockspec sub;
	int compare;

	if ((cold == NULL) || (cnow == NULL) || (wait == NULL)) {
		CLKD("parameter error\n");
		goto exit;
	}
	if ((wait->tv_sec <= 0) && (wait->tv_nsec <= 0)) {
		CLKD("wait=[%ld:%ld] not normalized\n", wait->tv_sec, wait->tv_nsec);
		goto exit;
	}

	ret = clock_sub(cnow, cold, &sub);
	if (ret != 0) {
		goto exit;
	}

	ret = clock_compare(&sub, wait, &compare);
	if (ret != 0) {
		goto exit;
	}

	if (compare >= CLOCK_COMPARE_SAME) {
		ret = 0;
	}
	else {
		ret = -1;
	}

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * clock_show()
 * Show all system clock values.
 *----------------------------------------------------------------------------*/
void clock_show (void)
{
	clockspec clock;
	uint8_t i;

	for (i = 0; i < (sizeof (gClockType) / sizeof (CLOCK_TYPE)); i++) {
		if (clock_gettime(gClockType[i].id, &clock) == 0) {
			printf("%s [%ld:%09ld]\n", gClockType[i].name, clock.tv_sec, clock.tv_nsec);
		}
	}
}
/*----------------------------------------------------------------------------*/

#if DEBUG_CLOCK
void clock_test (void)
{
	int ret;
	int i;
	clockspec ca;
	clockspec cb;
	clockspec clock;
	clockspec step;
	clockspec result;
	clockspec wait;
	int compare;

	CLKD("\tstart\n");

	clock_show();

	CLKD("\tadd test\n");
	ca.tv_sec = 1;
	ca.tv_nsec = NSEC_PER_SEC / 10;
	cb.tv_sec = -2;
	cb.tv_nsec = -(NSEC_PER_SEC / 5);
	step.tv_sec = 0;
	step.tv_nsec = NSEC_PER_SEC / 2;
	for (i = 0; i < 8; i++) {
		ret = clock_add(&cb, &step, &result);
		if (ret != 0) {
			continue;
		}
		cb = result;

		ret = clock_add(&ca, &cb, &result);
		if (ret == 0) {
			CLKD("[%ld:%ld]+[%ld:%ld]=[%ld:%ld]\n",
			     ca.tv_sec, ca.tv_nsec, cb.tv_sec, cb.tv_nsec, result.tv_sec, result.tv_nsec);
		}
	}

	ca.tv_sec = -1;
	ca.tv_nsec = -(NSEC_PER_SEC / 10);
	cb.tv_sec = -2;
	cb.tv_nsec = -(NSEC_PER_SEC / 5);
	step.tv_sec = 0;
	step.tv_nsec = NSEC_PER_SEC / 2;
	for (i = 0; i < 8; i++) {
		ret = clock_add(&cb, &step, &result);
		if (ret != 0) {
			continue;
		}
		cb = result;

		ret = clock_add(&ca, &cb, &result);
		if (ret == 0) {
			CLKD("[%ld:%ld]+[%ld:%ld]=[%ld:%ld]\n",
			     ca.tv_sec, ca.tv_nsec, cb.tv_sec, cb.tv_nsec, result.tv_sec, result.tv_nsec);
		}
	}

	CLKD("\tsub test\n");
	ca.tv_sec = 1;
	ca.tv_nsec = NSEC_PER_SEC / 10;
	cb.tv_sec = -2;
	cb.tv_nsec = -(NSEC_PER_SEC / 5);
	step.tv_sec = 0;
	step.tv_nsec = NSEC_PER_SEC / 2;
	for (i = 0; i < 8; i++) {
		ret = clock_add(&cb, &step, &result);
		if (ret != 0) {
			continue;
		}
		cb = result;

		ret = clock_sub(&ca, &cb, &result);
		if (ret == 0) {
			CLKD("[%ld:%ld]-[%ld:%ld]=[%ld:%ld]\n",
			     ca.tv_sec, ca.tv_nsec, cb.tv_sec, cb.tv_nsec, result.tv_sec, result.tv_nsec);
		}
	}

	ca.tv_sec = -1;
	ca.tv_nsec = -(NSEC_PER_SEC / 10);
	cb.tv_sec = -2;
	cb.tv_nsec = -(NSEC_PER_SEC / 5);
	step.tv_sec = 0;
	step.tv_nsec = NSEC_PER_SEC / 2;
	for (i = 0; i < 8; i++) {
		ret = clock_add(&cb, &step, &result);
		if (ret != 0) {
			continue;
		}
		cb = result;

		ret = clock_sub(&ca, &cb, &result);
		if (ret == 0) {
			CLKD("[%ld:%ld]-[%ld:%ld]=[%ld:%ld]\n",
			     ca.tv_sec, ca.tv_nsec, cb.tv_sec, cb.tv_nsec, result.tv_sec, result.tv_nsec);
		}
	}

	CLKD("\tcompare test\n");
	ca.tv_sec = 1;
	ca.tv_nsec = NSEC_PER_SEC / 10;
	cb.tv_sec = -1;
	cb.tv_nsec = -(NSEC_PER_SEC / 2);
	step.tv_sec = 0;
	step.tv_nsec = NSEC_PER_SEC / 2;
	for (i = 0; i < 10; i++) {
		ret = clock_add(&cb, &step, &result);
		if (ret != 0) {
			continue;
		}
		cb = result;

		ret = clock_compare(&ca, &cb, &compare);
		if (ret == 0) {
			CLKD("[%ld:%ld] compares [%ld:%ld]=%d\n",
			     ca.tv_sec, ca.tv_nsec, cb.tv_sec, cb.tv_nsec, compare);
		}
	}

	ca.tv_sec = 0;
	ca.tv_nsec = -(NSEC_PER_SEC / 10);
	cb.tv_sec = -1;
	cb.tv_nsec = -(NSEC_PER_SEC / 2);
	step.tv_sec = 0;
	step.tv_nsec = NSEC_PER_SEC / 2;
	for (i = 0; i < 10; i++) {
		ret = clock_add(&cb, &step, &result);
		if (ret != 0) {
			continue;
		}
		cb = result;

		ret = clock_compare(&ca, &cb, &compare);
		if (ret == 0) {
			CLKD("[%ld:%ld] compares [%ld:%ld]=%d\n",
			     ca.tv_sec, ca.tv_nsec, cb.tv_sec, cb.tv_nsec, compare);
		}
	}

	CLKD("\twait test\n");
	wait.tv_sec = 0;
	wait.tv_nsec = NSEC_PER_SEC / 5;
	step.tv_sec = 0;
	step.tv_nsec = NSEC_PER_SEC / 5;
	for (i = 0; i < 10; i++) {
		ret = clock_add(&wait, &step, &result);
		if (ret != 0) {
			continue;
		}
		wait = result;
		CLKD("wait\t[%ld:%ld]\n", wait.tv_sec, wait.tv_nsec);

		ret = clock_gettime(CLOCK_ID_DEFAULT, &ca);
		if (ret != 0) {
			continue;
		}


		while (1) {
			ret = clock_gettime(CLOCK_ID_DEFAULT, &cb);
			if (ret != 0) {
				continue;
			}

			if (clock_wait(&ca, &cb, &wait) == 0) {
				break;
			}
		}
		CLKD("begin\t[%ld:%ld]\n", ca.tv_sec, ca.tv_nsec);
		CLKD("end\t[%ld:%ld]\n", cb.tv_sec, cb.tv_nsec);
	}
}
#else
void clock_test (void) {}
#endif

