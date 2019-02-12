
#include <stdio.h>
#include <unistd.h>

#include "stdhup.h"

void
main(void)
{
	int counter;

	if (stdhup_init()) {
		fprintf(stderr, "Failed stdhup init\n");
		return;
	}

	while (1) {
		printf("COUNTER: %i\n", counter);
		counter++;
		sleep(1);
	}
}
