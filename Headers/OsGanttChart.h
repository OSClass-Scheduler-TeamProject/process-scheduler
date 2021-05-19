#ifndef OS_GANTTCHART_H
#define OS_GANTTCHART_H

#include "OsHeaders.h"
#include "OsProcess.h"

/**
 * 간트차트
 * arr: 프로세스 리스트가 담긴 구조체 배열
 * process_count: 프로세스의 갯수
 * max_time: 모든 프로세스가 종료되었을 때의 시간
*/
void draw_gantt_chart(Process *arr, int process_count, int max_time)
{
    for (int i = 0; i < process_count; i++)
    {
        printf("PID %d |", arr[i].process_id);
        for (int j = 0; j < arr[i].end_time + 1; j++)
        {
            if (j > 9)
                printf("%c  ", arr[i].gantt[j]);
            else
                printf("%c ", arr[i].gantt[j]);
        }
        printf("\n");
    }
    printf("       "); // PID %d 만큼 띄어쓰기
    for (int i = 0; i < max_time; i++)
        printf("%d ", i);

    printf("\n");
}

#endif