/**
 * txt 파일로 프로세스의 도착 시간, CPU 버스트 시간, 데드라인이 정해짐
 * 프로세스의 도착 시간, CPU 버스트 시간, 데드라인은 , 로 구분되어 있음
 * 범용 스케줄러에서는 데드라인은 무시
*/

#include "Headers/OsFileReader.h"

// Round Robin Scheduler
void RR(int cnt, Process s[], int quntum)
{
    int tm;                     // 탈출했을 때 프로세스 인덱스
    int time = 0;               // 시간
    int tmp_t = 0;              // 임시 시간 변수
    int longWait = 0;           // 가장 오래기다린 프로세스 인덱스
    int inter = 1;              // clock 인터럽트 발생 여부 판단 변수
    int exit = 0;               // 종료 프로세스 개수
    int average_wait = 0;       // 평균 대기 시간
    double average_banhwan = 0; // 평균 반환 시간

    FILE *file2 = fopen("output.txt", "w"); // 파일 출력 정의

    // 모든 프로세스가 끝날 때까지 계속 반복된다.
    while (1)
    {
        // 모든 프로세스를 순회하며
        for (int i = 0; i < cnt; i++)
        {
            // 지금 도착한 프로세스가 있는지 확인하고
            if (g_process[i].arrivalTime == time)
                g_process[i].judge = 1; // 실행 여부를 1로 바꿔준다.

            // 실행 중인데 클럭인터럽트가 발생한 경우 -> 다음 프로세스로 넘어가야 한다는 소리
            if (g_process[i].judge == 1 && inter == 1)
            {
                // 아직 할 작업이 남았고 && 가장 오래 기다린 프로세스의 대기시간보다 큰 대기시간을 가질때
                if (g_process[i].burstDuration != 0 && g_process[i].waiting_time >= g_process[longWait].waiting_time)
                {
                    // 지금 프로세스가 가장 오래 기다린 프로세스이므로 최신화
                    longWait = i;
                }
            }
        }

        // 실행 후 대기시간 0으로 초기화
        g_process[longWait].waiting_time = 0;

        // 실행 했으므로 수행 시간 -1
        g_process[longWait].burstDuration--;

        // 타임 퀀텀 시간동안 카운트 할 임시 시간 지표 ++
        tmp_t++;

        // 모든 프로세스를 순회하며
        for (int i = 0; i < cnt; i++)
        {
            // 대기중인 프로세스 && 현재 비 실행 프로세스일 경우
            if (i != longWait && g_process[i].judge == 1)
                g_process[i].waiting_time++; // 대기시간 +1
        }

        // 실행해야 할 시간이 남아있고 타임 퀀텀의 시간이 아직 안 되었으면 인터럽트를 발생시키지 않는다.
        if (g_process[longWait].burstDuration != 0 && tmp_t != quntum)
            inter = 0;

        else
        {
            // 수행시간이 0일 때 프로세스 실행 종료
            if (g_process[longWait].burstDuration == 0)
            {
                tm = longWait;           // 탈출 index 저장
                g_process[tm].judge = 0; // 종료 상태로 변경

                /**
                 * 프로세스 정보 출력
                 * 프로세스 id, 도착시간, 수행시간, 종료시간, 반환시간 (종료시간 - 도착시간), 정규화된 반환시간 (반환시간 / 수행시간)
                */
                printf("%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", s[tm].process_id, s[tm].arrivalTime, s[tm].burstDuration,
                       time + 1, time + 1 - s[tm].arrivalTime, (time + 1 - s[tm].arrivalTime) / (double)s[tm].burstDuration);
                fprintf(file2, "프로세스 id\t 도착시간\t 서비스 시간\t 종료 시간\t 반환 시간\t 정규화된 반환 시간\n");
                fprintf(file2, "%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", s[tm].process_id, s[tm].arrivalTime, s[tm].burstDuration,
                        time + 1, time + 1 - s[tm].arrivalTime, (time + 1 - s[tm].arrivalTime) / (double)s[tm].burstDuration);

                exit++; // 탈출한 프로세스 갯수 증가

                average_wait += (time + 1 - s[tm].arrivalTime - s[tm].burstDuration);              // 평균 대기 시간 증가
                average_banhwan += ((time + 1 - s[tm].arrivalTime) / (double)s[tm].burstDuration); // 평균 반환 시간 증가
            }

            tmp_t = 0; // 임시 시간 지표 초기화
            inter = 1; // 인터럽트 발생 여부 초기화
        }
        time++; //시간 +1

        if (exit == cnt) // 프로세스 개수 == 종료된 프로세스 개수
            break;       // -> while문 종료
    }

    printf("\n평균 대기시간: %d\n", average_wait / cnt);
    printf("\n평균 반환시간: %.2f\n", average_banhwan / cnt);
    // 출력 파일 작성
    fprintf(file2, "\n평균 대기시간: %d\n", average_wait / cnt);
    fprintf(file2, "\n평균 반환시간: %.2f\n", average_banhwan / cnt);
}

int main()
{

    int quntum; // 타임 퀀텀

    Process copy_sys[MAX_QUEUE] = {
        0,
    };

    int count = read_file(); // header file methods -> process count

    printf("input.txt\n");
    for (int i = 0; i < count; i++)
    {
        copy_sys[i] = g_process[i];
        printf("%d %d %d \n", g_process[i].process_id, g_process[i].arrivalTime, g_process[i].burstDuration);
    }

    printf("타임퀀텀을 입력하세요\n");
    scanf("%d", &quntum);

    puts("*****************************************************");
    printf("\n<RR 스케줄링>\n\n");
    printf("프로세스 id\t 도착시간\t 서비스 시간\t 종료 시간\t 반환 시간\t 정규화된 반환 시간\n");
    printf("###################################################################################################\n");
    RR(count, copy_sys, quntum);
    printf("###################################################################################################\n");

    // 간트 차트 구현 (예정)
}