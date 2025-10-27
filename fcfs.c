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
	int status;
	struct timeval t1_start, t2_start, t3_start, t4_start;
	struct timeval t1_end, t2_end, t3_end, t4_end;

	// Create all processes
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

	// FCFS: Run processes in order of arrival
	// P1 -> P2 -> P3 -> P4

	// Run P1 (first created)
	gettimeofday(&t1_start, NULL);
	kill(pid1, SIGCONT);
	waitpid(pid1, &status, 0);
	gettimeofday(&t1_end, NULL);

	// Run P2
	gettimeofday(&t2_start, NULL);
	kill(pid2, SIGCONT);
	waitpid(pid2, &status, 0);
	gettimeofday(&t2_end, NULL);

	// Run P3
	gettimeofday(&t3_start, NULL);
	kill(pid3, SIGCONT);
	waitpid(pid3, &status, 0);
	gettimeofday(&t3_end, NULL);

	// Run P4 (last created)
	gettimeofday(&t4_start, NULL);
	kill(pid4, SIGCONT);
	waitpid(pid4, &status, 0);
	gettimeofday(&t4_end, NULL);

	// Calculate execution times
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