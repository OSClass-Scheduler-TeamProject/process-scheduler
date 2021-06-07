/**
 * txt 파일로 프로세스의 도착 시간, CPU 버스트 시간, 데드라인이 정해짐
 * 프로세스의 도착 시간, CPU 버스트 시간, 데드라인은 , 로 구분되어 있음
 * 범용 스케줄러에서는 데드라인은 무시
*/

#include "Headers/OsFileReader.h"
#include "Headers/OsGanttChart.h"
#include "Headers/OsFileWriter.h"

// Round Robin Scheduler - context_switch
int context_switch_RR(FILE *output_file, int cnt, Process s[], int quantum)
{
    int tm;                    // 탈출했을 때 프로세스 인덱스
    int time = 0;              // 시간
    int tmp_t = -1;            // 임시 시간 변수
    int longWait = -1;         // 가장 오래기다린 프로세스 인덱스
    int inter = 1;             // clock 인터럽트 발생 여부 판단 변수
    int exit = 0;              // 종료 프로세스 개수
    int average_wait = 0;      // 평균 대기 시간
    double average_return = 0; // 평균 반환 시간
    int context_switching = 0; // context switching 횟수

    // 모든 프로세스가 끝날 때까지 계속 반복된다.
    while (1)
    {
        // 모든 프로세스를 순회하며
        for (int i = 0; i < cnt; i++)
        {
            // 지금 도착한 프로세스가 있는지 확인하고
            if (g_process[i].arrival_time == time)
                g_process[i].judge = 1; // 실행 여부를 1로 바꿔준다.

            // 실행 중인데 클럭인터럽트가 발생한 경우 -> 다음 프로세스로 넘어가야 한다는 소리
            if (g_process[i].judge == 1 && inter == 1)
            {
                // 아직 할 작업이 남았고 && 가장 오래 기다린 프로세스의 대기시간보다 큰 대기시간을 가질때
                if (g_process[i].burst_duration != 0 && g_process[i].waiting_time >= g_process[longWait].waiting_time)
                {
                    // 지금 프로세스가 가장 오래 기다린 프로세스이므로 최신화
                    longWait = i;
                }
            }
        }

        if (longWait == -1)
        {

            for (int i = 0; i < cnt; i++)
            {
                s[i].gantt[time] = ' ';
            }
            time++;
            continue;
        }

        // context switching
        if (tmp_t == -1)
        {
            tmp_t++;
            context_switching++;
            // context switching 시간을 고려하지 않는 경우 주석 처리
            // time++;
            // continue;
        }

        if (g_process[longWait].judge == 1)
        {

            // 실행 후 대기시간 0으로 초기화
            g_process[longWait].waiting_time = 0;

            // 실행 했으므로 수행 시간 -1
            g_process[longWait].burst_duration--;

            // 타임 퀀텀 시간동안 카운트 할 임시 시간 지표 ++
            s[longWait].gantt[time] = '*';
        }
        tmp_t++;

        // 모든 프로세스를 순회하며
        for (int i = 0; i < cnt; i++)
        {
            // 대기중인 프로세스 && 현재 비 실행 프로세스일 경우
            if (i != longWait && g_process[i].judge == 1)
            {
                g_process[i].waiting_time++; // 대기시간 +1
                s[i].gantt[time] = ' ';
            }
        }

        //

        // 실행해야 할 시간이 남아있고 타임 퀀텀의 시간이 아직 안 되었으면 인터럽트를 발생시키지 않는다.
        if (g_process[longWait].burst_duration != 0 && tmp_t != quantum)
            inter = 0;

        else
        {
            // 수행시간이 0일 때 프로세스 실행 종료
            if (g_process[longWait].burst_duration == 0)
            {
                tm = longWait;           // 탈출 index 저장
                g_process[tm].judge = 0; // 종료 상태로 변경
                s[tm].end_time = time;

                exit++; // 탈출한 프로세스 갯수 증가

                average_wait += (time - s[tm].arrival_time - s[tm].burst_duration);             // 평균 대기 시간 증가
                average_return += ((time - s[tm].arrival_time) / (double)s[tm].burst_duration); // 평균 반환 시간 증가
            }

            tmp_t = -1;    // 임시 시간 지표 초기화
            inter = 1;     // 인터럽트 발생 여부 초기화
            longWait = -1; // longWait index 초기화
        }
        time++; //시간 +1

        if (exit == cnt) // 프로세스 개수 == 종료된 프로세스 개수
            break;       // -> while문 종료
    }

    // 출력 파일 생성
    concat_output_file(output_file, quantum, cnt, s, average_wait / cnt, average_return / cnt, time, context_switching);

    return time;
}

int main(int argc, char *argv[])
{

    // argv[1]: quantum-max
    // argv[2]: context 여부

    if (argc < 2)
    {
        printf("Input ERROR!\n");
        printf("Usage: ./scheduler.out [quantum-max]\n");
        return -1;
    }

    FILE *output_file = create_output_file();

    int time;

    for (int quantum = 1; quantum < atoi(argv[1]); quantum++)
    {
        Process copy_sys[MAX_QUEUE] = {
            0,
        };

        int count = read_file(); // header file methods input.txt 파일을 읽고 프로세스 갯수를 리턴

        for (int i = 0; i < count; i++)
        {
            copy_sys[i] = g_process[i];
        }

        // context_switch 를 고려한 RR
        time = context_switch_RR(output_file, count, copy_sys, quantum); // output.md 셍성

        // 간트차트 그리기
        // draw_gantt_chart(copy_sys, count, time, quantum);
    }

    finish_output_file(output_file);

    return 0;
}