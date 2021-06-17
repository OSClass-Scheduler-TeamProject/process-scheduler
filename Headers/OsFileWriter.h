#ifndef OS_OSFILEWRITER_H
#define OS_OSFILEWRITER_H

#include "OsHeaders.h"
#include "OsProcess.h"
#include <time.h>

typedef struct Optimal
{
    int run_time;        // 총 실행시간
    double average_time; // 평균 대기시간
    double return_time;  // 평균 반환시간
    double optimal_data; // 비교 지표
    int optimal_quantum; //최적의 quantum
} Optimal;

Optimal optimal_result;

FILE *create_output_file()
{
    // 현재 시간 구하기
    time_t timer = time(NULL);
    struct tm *t;
    t = localtime(&timer);
    FILE *output_file = fopen("output.md", "w"); // 파일 출력 정의
    fprintf(output_file, "# RR Scheduler Output File\n");
    fprintf(output_file, "%d년 %d월 %d일 %d시 %d분 실행\n\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);

    // fprintf(output_file, "## Restult \n");
    // fprintf(output_file, "| time quantum |context switching|평균 대기시간 | 평균 반환시간| 실행 시간|\n");
    // fprintf(output_file, "| ---|---| ---| ---| ---|\n");

    optimal_result.optimal_data = 9999;

    return output_file;
}

void concat_output_file(FILE *output_file, int quantum, int count, double average_waiting_time, double average_return_time, int time, int context_switching, long real_execute_time)
{

    if (optimal_result.run_time == time && optimal_result.return_time == average_return_time && optimal_result.average_time == average_waiting_time)
    {
        return;
    }

    // 프로세스별로 출력

    fprintf(output_file, "## Time Quantum: %d\n\n", quantum);
    fprintf(output_file, "context switching 횟수: %d\n\n", context_switching);

    fprintf(output_file, "|프로세스 id| 도착시간|서비스 시간| 종료 시간|대기 시간| 반환 시간| 정규화된 반환 시간|\n");
    fprintf(output_file, "|---| ---|---| ---| ---| ---| ---|\n");

    for (int i = 0; i < count; i++)
    {
        // 반환 시간: 대기시간 + 수행시간
        int return_time = g_process[i].stack_waiting_time + g_process[i].burst_duration;

        fprintf(output_file, "|%d| %ds|  %ds| %ds|  %ds| %ds| %.2fs |\n", g_process[i].process_id, g_process[i].arrival_time, g_process[i].burst_duration,
                g_process[i].end_time, g_process[i].stack_waiting_time, return_time, (double)(return_time) / (double)g_process[i].burst_duration);
    }

    fprintf(output_file, "\n---\n");

    fprintf(output_file, "**총 실행 시간**: %d\n", time);
    fprintf(output_file, "**실제 스케줄러 수행 시간**: %lu\n", real_execute_time);
    fprintf(output_file, "**평균 대기시간**: %.2f\n", average_waiting_time);
    fprintf(output_file, "**평균 반환시간**: %.2f\n", average_return_time);

    // 최종 결과만 출력

    // 반환 시간에 가중치를 주는 방식
    int tmp_data = average_waiting_time + average_return_time * 9;

    if (tmp_data < optimal_result.optimal_data)
    {
        optimal_result.average_time = average_waiting_time;
        optimal_result.return_time = average_return_time;
        optimal_result.optimal_data = tmp_data;
        optimal_result.optimal_quantum = quantum;
        optimal_result.run_time = time;
    }
}

void finish_output_file(FILE *output_file)
{
    fprintf(output_file, "## Optimal Time quantum\n");
    fprintf(output_file, "**최적의 time quantum**: %d\n", optimal_result.optimal_quantum);
    fprintf(output_file, "**총 실행 시간**: %d\n", optimal_result.run_time);
    fprintf(output_file, "**평균 대기시간**: %.2f\n", optimal_result.average_time);
    fprintf(output_file, "**평균 반환시간**: %.2f\n", optimal_result.return_time);
}

#endif