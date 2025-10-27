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
	struct timeval t1_start, t2_start, t3_start, t4_start;
	struct timeval t1_end, t2_end, t3_end, t4_end;
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

	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0){
		if (running1 > 0){
			if (!first1){ gettimeofday(&t1_start, NULL); first1 = 1; }
			kill(pid1, SIGCONT);
			usleep(QUANTUM1);
			kill(pid1, SIGSTOP);
		}
		if (running2 > 0){
			if (!first2){ gettimeofday(&t2_start, NULL); first2 = 1; }
			kill(pid2, SIGCONT);
			usleep(QUANTUM2);
			kill(pid2, SIGSTOP);
		}
		if (running3 > 0){
			if (!first3){ gettimeofday(&t3_start, NULL); first3 = 1; }
			kill(pid3, SIGCONT);
			usleep(QUANTUM3);
			kill(pid3, SIGSTOP);
		}
		if (running4 > 0){
			if (!first4){ gettimeofday(&t4_start, NULL); first4 = 1; }
			kill(pid4, SIGCONT);
			usleep(QUANTUM4);
			kill(pid4, SIGSTOP);
		}
		
		// Check for completion and record end time
		if (running1 > 0 && waitpid(pid1, &running1, WNOHANG) != 0){
			gettimeofday(&t1_end, NULL);
			running1 = 0;
		}
		if (running2 > 0 && waitpid(pid2, &running2, WNOHANG) != 0){
			gettimeofday(&t2_end, NULL);
			running2 = 0;
		}
		if (running3 > 0 && waitpid(pid3, &running3, WNOHANG) != 0){
			gettimeofday(&t3_end, NULL);
			running3 = 0;
		}
		if (running4 > 0 && waitpid(pid4, &running4, WNOHANG) != 0){
			gettimeofday(&t4_end, NULL);
			running4 = 0;
		}
	}

	long exec1 = (t1_end.tv_sec - t1_start.tv_sec) * 1000000L + (t1_end.tv_usec - t1_start.tv_usec);
	long exec2 = (t2_end.tv_sec - t2_start.tv_sec) * 1000000L + (t2_end.tv_usec - t2_start.tv_usec);
	long exec3 = (t3_end.tv_sec - t3_start.tv_sec) * 1000000L + (t3_end.tv_usec - t3_start.tv_usec);
	long exec4 = (t4_end.tv_sec - t4_start.tv_sec) * 1000000L + (t4_end.tv_usec - t4_start.tv_usec);
	double avg = (exec1 + exec2 + exec3 + exec4) / 4.0;

	printf("P1 execution_time_us %ld\n", exec1);
	printf("P2 execution_time_us %ld\n", exec2);
	printf("P3 execution_time_us %ld\n", exec3);
	printf("P4 execution_time_us %ld\n", exec4);
	printf("Average execution_time_us %.2f\n", avg);

	return 0;
}