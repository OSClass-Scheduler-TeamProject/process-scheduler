#ifndef OS_OSFILEWRITER_H
#define OS_OSFILEWRITER_H

#include "OsHeaders.h"
#include "OsProcess.h"
#include <time.h>

void create_output_file(int quantum, Process *process_list, int average_time, double return_time)
{
    // 현재 시간 구하기
    time_t timer = time(NULL);
    struct tm *t;
    t = localtime(&timer);
    FILE *file2 = fopen("output.md", "w"); // 파일 출력 정의
    fprintf(file2, "# RR Scheduler Output File\n");
    fprintf(file2, "%d년 %d월 %d일 %d시 %d분 실행\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
    fprintf(file2, "## Time Quantum\n");
    fprintf(file2, "quantum: %d\n", quantum);

    fprintf(file2, "\n---\n");

    fprintf(file2, "|프로세스 id| 도착시간|서비스 시간| 종료 시간| 반환 시간| 정규화된 반환 시간|\n");
    fprintf(file2, "|---| ---|---| ---| ---| ---|\n");

    for (int i = 0; i < sizeof(process_list); i++)
    {
        fprintf(file2, "|%d| %ds|  %ds|  %ds| %ds| %.2fs |\n", process_list[i].process_id, process_list[i].arrival_time, process_list[i].burst_duration,
                process_list[i].end_time, process_list[i].end_time - process_list[i].arrival_time, (process_list[i].end_time - process_list[i].arrival_time) / (double)process_list[i].arrival_time);
    }

    fprintf(file2, "\n---\n");

    fprintf(file2, "**평균 대기시간**: %d\n", average_time);
    fprintf(file2, "**평균 반환시간**: %.2f\n", return_time);
}

#endif