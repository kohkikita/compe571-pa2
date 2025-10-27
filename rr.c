#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

#define QUANTUM1 2000
#define QUANTUM2 2000
#define QUANTUM3 2000
#define QUANTUM4 2000

void myfunction(int param){
	int i = 2;
	int j, k;
	while(i < param){
		k = i;
		for (j = 2; j <= k; j++){
			if (k % j == 0){
				k = k/j;
				j--;
				if (k == 1){
					break;
				}
			}
		}
		i++;
	}
}

int main(){
	pid_t pid1, pid2, pid3, pid4;
	int running1, running2, running3, running4;
	struct timeval t0, t1, t2, t3, t4;
	int first1 = 0, first2 = 0, first3 = 0, first4 = 0;

	pid1 = fork();
	if (pid1 == 0){ myfunction(WORKLOAD1); exit(0); }
	kill(pid1, SIGSTOP);

	pid2 = fork();
	if (pid2 == 0){ myfunction(WORKLOAD2); exit(0); }
	kill(pid2, SIGSTOP);

	pid3 = fork();
	if (pid3 == 0){ myfunction(WORKLOAD3); exit(0); }
	kill(pid3, SIGSTOP);

	pid4 = fork();
	if (pid4 == 0){ myfunction(WORKLOAD4); exit(0); }
	kill(pid4, SIGSTOP);

	gettimeofday(&t0, NULL);

	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0){
		if (running1 > 0){
			kill(pid1, SIGCONT);
			if (!first1){ gettimeofday(&t1, NULL); first1 = 1; }
			usleep(QUANTUM1);
			kill(pid1, SIGSTOP);
		}
		if (running2 > 0){
			kill(pid2, SIGCONT);
			if (!first2){ gettimeofday(&t2, NULL); first2 = 1; }
			usleep(QUANTUM2);
			kill(pid2, SIGSTOP);
		}
		if (running3 > 0){
			kill(pid3, SIGCONT);
			if (!first3){ gettimeofday(&t3, NULL); first3 = 1; }
			usleep(QUANTUM3);
			kill(pid3, SIGSTOP);
		}
		if (running4 > 0){
			kill(pid4, SIGCONT);
			if (!first4){ gettimeofday(&t4, NULL); first4 = 1; }
			usleep(QUANTUM4);
			kill(pid4, SIGSTOP);
		}
		waitpid(pid1, &running1, WNOHANG);
		waitpid(pid2, &running2, WNOHANG);
		waitpid(pid3, &running3, WNOHANG);
		waitpid(pid4, &running4, WNOHANG);
	}

	long r1 = (t1.tv_sec - t0.tv_sec) * 1000000L + (t1.tv_usec - t0.tv_usec);
	long r2 = (t2.tv_sec - t0.tv_sec) * 1000000L + (t2.tv_usec - t0.tv_usec);
	long r3 = (t3.tv_sec - t0.tv_sec) * 1000000L + (t3.tv_usec - t0.tv_usec);
	long r4 = (t4.tv_sec - t0.tv_sec) * 1000000L + (t4.tv_usec - t0.tv_usec);
	double avg = (r1 + r2 + r3 + r4) / 4.0;

	printf("P1 response_time_us %ld\n", r1);
	printf("P2 response_time_us %ld\n", r2);
	printf("P3 response_time_us %ld\n", r3);
	printf("P4 response_time_us %ld\n", r4);
	printf("Average response_time_us %.2f\n", avg);

	return 0;
}
