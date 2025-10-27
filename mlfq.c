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

// MLFQ: 3 queues with increasing time quantums
#define QUEUE0_QUANTUM 2000   // Highest priority, shortest quantum
#define QUEUE1_QUANTUM 4000   // Medium priority, medium quantum
#define QUEUE2_QUANTUM 8000   // Lowest priority, longest quantum

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
	int queue1, queue2, queue3, queue4;  // Track which queue each process is in
	struct timeval t1_start, t2_start, t3_start, t4_start;
	struct timeval t1_end, t2_end, t3_end, t4_end;
	int first1 = 0, first2 = 0, first3 = 0, first4 = 0;

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

	// Initialize: all processes start in queue 0 (highest priority)
	running1 = 1; queue1 = 0;
	running2 = 1; queue2 = 0;
	running3 = 1; queue3 = 0;
	running4 = 1; queue4 = 0;

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0){
		// Queue 0 (highest priority) - shortest quantum
		if (running1 > 0 && queue1 == 0){
			if (!first1){ gettimeofday(&t1_start, NULL); first1 = 1; }
			kill(pid1, SIGCONT);
			usleep(QUEUE0_QUANTUM);
			kill(pid1, SIGSTOP);
			queue1 = 1;  // Demote to queue 1
		}
		if (running2 > 0 && queue2 == 0){
			if (!first2){ gettimeofday(&t2_start, NULL); first2 = 1; }
			kill(pid2, SIGCONT);
			usleep(QUEUE0_QUANTUM);
			kill(pid2, SIGSTOP);
			queue2 = 1;
		}
		if (running3 > 0 && queue3 == 0){
			if (!first3){ gettimeofday(&t3_start, NULL); first3 = 1; }
			kill(pid3, SIGCONT);
			usleep(QUEUE0_QUANTUM);
			kill(pid3, SIGSTOP);
			queue3 = 1;
		}
		if (running4 > 0 && queue4 == 0){
			if (!first4){ gettimeofday(&t4_start, NULL); first4 = 1; }
			kill(pid4, SIGCONT);
			usleep(QUEUE0_QUANTUM);
			kill(pid4, SIGSTOP);
			queue4 = 1;
		}

		// Queue 1 (medium priority) - medium quantum
		if (running1 > 0 && queue1 == 1){
			if (!first1){ gettimeofday(&t1_start, NULL); first1 = 1; }
			kill(pid1, SIGCONT);
			usleep(QUEUE1_QUANTUM);
			kill(pid1, SIGSTOP);
			queue1 = 2;  // Demote to queue 2
		}
		if (running2 > 0 && queue2 == 1){
			if (!first2){ gettimeofday(&t2_start, NULL); first2 = 1; }
			kill(pid2, SIGCONT);
			usleep(QUEUE1_QUANTUM);
			kill(pid2, SIGSTOP);
			queue2 = 2;
		}
		if (running3 > 0 && queue3 == 1){
			if (!first3){ gettimeofday(&t3_start, NULL); first3 = 1; }
			kill(pid3, SIGCONT);
			usleep(QUEUE1_QUANTUM);
			kill(pid3, SIGSTOP);
			queue3 = 2;
		}
		if (running4 > 0 && queue4 == 1){
			if (!first4){ gettimeofday(&t4_start, NULL); first4 = 1; }
			kill(pid4, SIGCONT);
			usleep(QUEUE1_QUANTUM);
			kill(pid4, SIGSTOP);
			queue4 = 2;
		}

		// Queue 2 (lowest priority) - longest quantum
		if (running1 > 0 && queue1 == 2){
			if (!first1){ gettimeofday(&t1_start, NULL); first1 = 1; }
			kill(pid1, SIGCONT);
			usleep(QUEUE2_QUANTUM);
			kill(pid1, SIGSTOP);
			// Stay in queue 2
		}
		if (running2 > 0 && queue2 == 2){
			if (!first2){ gettimeofday(&t2_start, NULL); first2 = 1; }
			kill(pid2, SIGCONT);
			usleep(QUEUE2_QUANTUM);
			kill(pid2, SIGSTOP);
		}
		if (running3 > 0 && queue3 == 2){
			if (!first3){ gettimeofday(&t3_start, NULL); first3 = 1; }
			kill(pid3, SIGCONT);
			usleep(QUEUE2_QUANTUM);
			kill(pid3, SIGSTOP);
		}
		if (running4 > 0 && queue4 == 2){
			if (!first4){ gettimeofday(&t4_start, NULL); first4 = 1; }
			kill(pid4, SIGCONT);
			usleep(QUEUE2_QUANTUM);
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