#ifndef OS_FILE_READER_H
#define OS_FILE_READER_H

#include "OsHeaders.h"
#include "OsProcess.h"

/**
 * input.txt 파일을 읽어 prcess 정보들을 저장한 뒤
 * g_process 배열에 넣는다.
 * {int} count: 프로세스 갯수 리턴
*/
int read_file()
{

    // count: Process 갯수, i: for 문 index
    int count = 0, i;

    FILE *file = NULL;
    char c[MAX_PROCESS];
    char *a = NULL;
    // 메모리 세팅
    memset(g_process, 0, sizeof(g_process));

    file = fopen("input.txt", "r");
    // input.txt 읽어오기

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

        g_process[count].process_id = count + 1;
        g_process[count].arrival_time = atoi(a); // atoi: 문자열을 숫자로

        a = strtok(NULL, ","); // 계속 이어서 자르고 싶은 경우 첫번째 인자에 NULL 을 넣어준다.
        g_process[count].remain_time = g_process[count].burst_duration = atoi(a);
        a = strtok(NULL, ",");
        g_process[count].deadline_time = atoi(a);

        g_process[count].waiting_time = 0;
        count++;
    }

    fclose(file); // 파일 닫기
    return count;
}

#endif