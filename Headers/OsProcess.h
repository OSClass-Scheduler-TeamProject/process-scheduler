#ifndef MINSOO_OS_PROCESS_H
#define MINSOO_OS_PROCESS_H

#define MAX_PROCESS 512
#define MAX_QUEUE 256

typedef struct Process
{
    int process_id;    // 프로세스 번호
    int arrivalTime;   // 도착 시간
    int burstDuration; // 수행 시간
    int remain_time;   // 수행 후 남아있는 시간
    int waiting_time;  // 대기 시간
    int end_time;      // 종료 시간
    int judge;         // 실행 판단 (1: 실행, 0: 종료)
    int deadlineTime;  // 데드라인 (실시간)
} Process;

Process g_process[MAX_PROCESS];

#endif