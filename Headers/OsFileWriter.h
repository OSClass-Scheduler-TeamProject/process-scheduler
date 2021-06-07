#ifndef OS_OSFILEWRITER_H
#define OS_OSFILEWRITER_H

#include "OsHeaders.h"
#include "OsProcess.h"
#include <time.h>

typedef struct Optimal
{
    int average_time;
    double return_time;
    double optimal_data;
    int optimal_quantum;
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

    fprintf(output_file, "## Restult \n");
    fprintf(output_file, "| time quantum |context switching|평균 대기시간 | 평균 반환시간| 실행 시간|\n");
    fprintf(output_file, "| ---|---| ---| ---| ---|\n");

    optimal_result.optimal_data = 9999;

    return output_file;
}

void concat_output_file(FILE *output_file, int quantum, int count, Process *process_list, double average_time, double return_time, int time, int context_switching)
{

    if (optimal_result.return_time == return_time && optimal_result.average_time == average_time)
    {
        return;
    }

    // 프로세스별로 출력

    // fprintf(output_file, "## Time Quantum: %d\n\n", quantum);
    // fprintf(output_file, "context switching 횟수: %d\n\n", context_switching);

    // fprintf(output_file, "|프로세스 id| 도착시간|서비스 시간| 종료 시간| 반환 시간| 정규화된 반환 시간|\n");
    // fprintf(output_file, "|---| ---|---| ---| ---| ---|\n");

    // for (int i = 0; i < count; i++)
    // {
    //     fprintf(output_file, "|%d| %ds|  %ds|  %ds| %ds| %.2fs |\n", process_list[i].process_id, process_list[i].arrival_time, process_list[i].burst_duration,
    //             process_list[i].end_time, process_list[i].end_time - process_list[i].arrival_time, (process_list[i].end_time - process_list[i].arrival_time) / (double)process_list[i].arrival_time);
    // }

    //
    fprintf(output_file, "|%d| %d회| %.1fs| %.1fs| %ds |\n", quantum, context_switching, average_time, return_time, time);

    // fprintf(output_file, "\n---\n");

    // fprintf(output_file, "**총 실행 시간**: %d\n", time);
    // fprintf(output_file, "**평균 대기시간**: %d\n", average_time);
    // fprintf(output_file, "**평균 반환시간**: %.2f\n", return_time);

    // 최종 결과만 출력

    // 반환 시간에 가중치를 주는 방식
    int tmp_data = average_time + time;

    if (tmp_data < optimal_result.optimal_data)
    {
        optimal_result.average_time = average_time;
        optimal_result.return_time = return_time;
        optimal_result.optimal_data = tmp_data;
        optimal_result.optimal_quantum = quantum;
    }
}

void finish_output_file(FILE *output_file)
{
    fprintf(output_file, "## Optimal Time quantum\n");
    fprintf(output_file, "**최적의 time quantum**: %d\n", optimal_result.optimal_quantum);
    fprintf(output_file, "**평균 대기시간**: %d\n", optimal_result.average_time);
    fprintf(output_file, "**평균 반환시간**: %.2f\n", optimal_result.return_time);
}

#endif