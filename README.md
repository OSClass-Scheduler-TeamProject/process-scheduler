# RR Schedule

## input.txt

| Arrival_time | CPU_time  | Dead_line(Only Real Time) |
| ------------ | --------- | ------------------------- |
| \<entry\>    | \<entry\> | \<entry\>                 |

## process struct

```c
typedef struct process
{
    int process_id;     // 프로세스 번호
    int arrival_time;   // 도착 시간
    int burst_duration; // 수행 시간
    int remain_time;    // 수행 후 남아있는 시간
    int waiting_time;   // 대기 시간
    int end_time;       // 종료 시간
    int judge;          // 실행 판단 (1: 실행, 0: 종료)
    int deadline_time;  // 데드라인 (실시간)
    // char gantt[MAX_PROCESS]; -> 터미널 gantt chart 사용 안 하는 것으로 결정
} process;
```

## use flow

1. input.txt 파일을 읽어온다.
2. `,`으로 데이터를 구분해서 프로세스 정보를 저장한다. (데드라인도 저장은 하지만 사용은 하지 않음)
3. 프로세스 큐에(ready queue) 넣어준다. (g_process)
4. time quantum 은 `scanf` 를 통해 사용자에게 입력을 직접 받는다.
5. RR 스케줄링을 통해 프로세스를 실행 시켜준다.
   1. 사용자에게 입력받은 time quantum 까지 1 에서 키워가며 실행시킵니다.
   2. 평균 대기시간 + 평균 반환시간 + 총 실행시간 에 가중치를 두어 최적의 time quantum 을 찾습니다.

## How to Use

```bash
gcc -o scheduler.out <YOUR-PATH>/scheduler.c
./scheduler.out [quantum-max]
```

## output.md

### Time quantum $

context switching 횟수: $

| pid       | Arrival_time | CPU_time  | end_time  | return time | reg return time |
| --------- | ------------ | --------- | --------- | ----------- | --------------- |
| \<entry\> | \<entry\>    | \<entry\> | \<entry\> | \<entry\>   | \<entry\>       |

**총 실행 시간**: `int`
**평균 대기시간**: `double`
**평균 반환시간**: `double`

### Optimal Time quantum

**최적의 time quantum**: `int`
**총 실행 시간**:`int`
**평균 대기시간**:`double`
**평균 반환시간**:`double`

## 참고 문헌 및 자료

- [sujinnaljin님 코드](https://github.com/sujinnaljin/CPU_Scheduler/blob/master/CPU_Scheduler/CPU_Scheduler/main.c)
- [Hanseunghoon님 코드](https://github.com/Hanseunghoon/Process-Scheduling-Algorithm/blob/master/SCHEDULING.c)
- [nathankim0님 코드](https://github.com/nathankim0/os-scheduler/blob/97266057a6a10322a74b3dc2f004460744563a5f/Project4/Project4/%EC%B5%9C%EC%A2%85_%EC%A3%BC%EC%84%9D.c)
- [호야님 블로그](https://kim-hoya.tistory.com/22)
- [호야님 블로그 - RR 개념 설명](https://kim-hoya.tistory.com/11)
- [C 언어 레퍼런스](https://modoocode.com/51)
