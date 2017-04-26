/*
 * Interval-timer demo program.
 * Hebrew University OS course.
 * Questions: os@cs.huji.ac.il
 */

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>


int gotit = 0;

void timer_handler(int sig)
{
  gotit = 1;
  printf("Timer expired\n");
}


int main(void) {
	struct sigaction sa;
	struct itimerval timer;

	// Install timer_handler as the signal handler for SIGVTALRM.
	sa.sa_handler = &timer_handler;
	if (sigaction(SIGVTALRM, &sa,NULL) < 0) {
		printf("sigaction error.");
	}

	// Configure the timer to expire after 1 sec... */
	timer.it_value.tv_sec = 2;		// first time interval, seconds part
	timer.it_value.tv_usec = 0;		// first time interval, microseconds part

	// configure the timer to expire every 3 sec after that.
	timer.it_interval.tv_sec = 3;	// following time intervals, seconds part
	timer.it_interval.tv_usec = 0;	// following time intervals, microseconds part

	// Start a virtual timer. It counts down whenever this process is executing.
	if (setitimer (ITIMER_VIRTUAL, &timer, NULL)) {
		printf("setitimer error.");
	}

    while(1){

    }
//    int i =0;
//	while (i < 5){
//		if (gotit) {
//			printf("Got it!\n");
//			gotit = 0;
//            i++;
//		}
//	}
//    printf("setitimer er***************************************************8888888*****\n ****************"
//                   "*********************************************************\nror.");
//    // Configure the timer to expire after 1 sec... */
//    timer.it_value.tv_sec = 0;		// first time interval, seconds part
//    timer.it_value.tv_usec = 1;		// first time interval, microseconds part
//
//    // configure the timer to expire every 3 sec after that.
//    timer.it_interval.tv_sec = 0;	// following time intervals, seconds part
//    timer.it_interval.tv_usec = 1;	// following time intervals, microseconds part
//    setitimer (ITIMER_VIRTUAL, &timer, NULL);
//    int j=0;
//    while (j < 1000) {
//        if (gotit) {
//            printf("Got it!\n");
//            gotit = 0;
//            j++;
//        }
//    }
    return 0;
}

