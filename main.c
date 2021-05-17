#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESS 512
#define MAX_QUEUE 256

typedef struct process
{
    int process_id;   // 프로세스 번호
    int arrive_time;  // 도착 시간
    int cpu_time;     // 수행 시간
    int remain_time;  // 수행 후 남아있는 시간
    int waiting_time; // 대기 시간
    int end_time;     // 종료 시간
    int judge;        // 실행 판단 (1: 실행, 0: 종료)
} process;

process g_process[MAX_PROCESS];

void RR(int cnt, process s[], int quntum)
{
    int tm;                                 // 탈출했을 때 프로세스 인덱스
    int time = 0;                           // 시간
    int tmp_t = 0;                          // 임시 시간 변수
    int longWait = 0;                       // 가장 오래기다린 프로세스 인덱스
    int inter = 1;                          // clock 인터럽트 발생 여부 판단 변수
    int exit = 0;                           // 종료 프로세스 개수
    int average_wait = 0;                   // 평균 대기 시간
    double average_banhwan = 0;             // 평균 반환 시간
    FILE *file2 = fopen("output.txt", "w"); // 파일 출력 정의

    // 모든 프로세스가 끝날 때까지 계속 반복된다.
    while (1)
    {
        // 모든 프로세스를 순회하며
        for (int i = 0; i < cnt; i++)
        {
            // 지금 도착한 프로세스가 있는지 확인하고
            if (g_process[i].arrive_time == time)
                g_process[i].judge = 1; // 실행 여부를 1로 바꿔준다.

            // 실행 중인데 클럭인터럽트가 발생한 경우 -> 다음 프로세스로 넘어가야 한다는 소리
            if (g_process[i].judge == 1 && inter == 1)
            {
                // 아직 할 작업이 남았고 && 가장 오래 기다린 프로세스의 대기시간보다 큰 대기시간을 가질때
                if (g_process[i].cpu_time != 0 && g_process[i].waiting_time >= g_process[longWait].waiting_time)
                {
                    // 지금 프로세스가 가장 오래 기다린 프로세스이므로 최신화
                    longWait = i;
                }
            }
        }

        // 실행 후 대기시간 0으로 초기화
        g_process[longWait].waiting_time = 0;

        // 실행 했으므로 수행 시간 -1
        g_process[longWait].cpu_time--;

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
        if (g_process[longWait].cpu_time != 0 && tmp_t != quntum)
            inter = 0;

        else
        {
            // 수행시간이 0일 때 프로세스 실행 종료
            if (g_process[longWait].cpu_time == 0)
            {
                tm = longWait;           // 탈출 index 저장
                g_process[tm].judge = 0; // 종료 상태로 변경

                /**
                 * 프로세스 정보 출력
                 * 프로세스 id, 도착시간, 수행시간, 종료시간, 반환시간 (종료시간 - 도착시간), 정규화된 반환시간 (반환시간 / 수행시간)
                */
                printf("%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", s[tm].process_id, s[tm].arrive_time, s[tm].cpu_time,
                       time + 1, time + 1 - s[tm].arrive_time, (time + 1 - s[tm].arrive_time) / (double)s[tm].cpu_time);
                fprintf(file2, "프로세스 id\t 도착시간\t 서비스 시간\t 종료 시간\t 반환 시간\t 정규화된 반환 시간\n");
                fprintf(file2, "%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", s[tm].process_id, s[tm].arrive_time, s[tm].cpu_time,
                        time + 1, time + 1 - s[tm].arrive_time, (time + 1 - s[tm].arrive_time) / (double)s[tm].cpu_time);

                exit++; // 탈출한 프로세스 갯수 증가

                average_wait += (time + 1 - s[tm].arrive_time - s[tm].cpu_time);              // 평균 대기 시간 증가
                average_banhwan += ((time + 1 - s[tm].arrive_time) / (double)s[tm].cpu_time); // 평균 반환 시간 증가
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
    FILE *file = NULL;

    char c[MAX_PROCESS];
    char *a = NULL;

    process copy_sys[MAX_QUEUE] = {
        0,
    };

    // count: process 갯수, i: for 문 index
    int count = 0, i;

    // 메모리 세팅
    memset(g_process, 0, sizeof(g_process));

    // input.txt 읽어오기
    file = fopen("input.txt", "r");
    if (file == NULL)
    {
        printf("파일이 존재하지 않습니다.");
        return -1;
    }

    // 파일을 끝까지 다 읽을 때까지
    while (fgets(c, MAX_PROCESS, file) != NULL)
    {
        if (c[0] == '\n')
            continue;

        a = strtok(c, ","); // ,로 구분해서 데이터를 읽는다.

        // input.txt 의 마지막 줄에 보면 0 이 있는데 이걸 읽으면 끝으로 인식
        if (*a == '0')
            break;

        g_process[count].process_id = atoi(a); // atoi: 문자열을 숫자로

        a = strtok(NULL, ","); // 계속 이어서 자르고 싶은 경우 첫번째 인자에 NULL 을 넣어준다.
        g_process[count].arrive_time = atoi(a);
        a = strtok(NULL, ",");
        g_process[count].remain_time = g_process[count].cpu_time = atoi(a);
        g_process[count].waiting_time = 0;
        count++;
    }

    fclose(file); // 파일 닫기

    printf("input.txt\n");
    for (i = 0; i < count; i++)
    {
        copy_sys[i] = g_process[i];
        printf("%d %d %d \n", g_process[i].process_id, g_process[i].arrive_time, g_process[i].cpu_time);
    }

    printf("타임퀀텀을 입력하세요\n");
    scanf("%d", &quntum);

    puts("*****************************************************");
    printf("\n<RR 스케줄링>\n\n");
    printf("프로세스 id\t 도착시간\t 서비스 시간\t 종료 시간\t 반환 시간\t 정규화된 반환 시간\n");
    printf("###################################################################################################\n");
    RR(count, copy_sys, quntum);
    printf("###################################################################################################\n");
}