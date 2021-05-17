# RR Schedule

## input.txt

| pid       | Arrival_time | CPU_time  |
| --------- | ------------ | --------- |
| \<entry\> | \<entry\>    | \<entry\> |

## process struct

```c
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
```

## use flow

1. input.txt 파일을 읽어온다.
2. `,`으로 데이터를 구분해서 프로세스 정보를 저장한다.
3. 프로세스 큐에(ready queue) 넣어준다.
4. time quntum 은 `scanf` 를 통해 사용자에게 입력을 직접 받는다.
5. RR 스케줄링을 통해 프로세스를 실행 시켜준다.

## output.txt

| pid       | Arrival_time | CPU_time  | return time | reg return time |
| --------- | ------------ | --------- | ----------- | --------------- |
| \<entry\> | \<entry\>    | \<entry\> | \<entry\>   | \<entry\>       |

평균 대기시간: `int`
평균 반환시간: `double`

## 참고 문헌 및 자료

- [sujinnaljin님 코드](https://github.com/sujinnaljin/CPU_Scheduler/blob/master/CPU_Scheduler/CPU_Scheduler/main.c)
- [Hanseunghoon님 코드](https://github.com/Hanseunghoon/Process-Scheduling-Algorithm/blob/master/SCHEDULING.c)
- [nathankim0님 코드](https://github.com/nathankim0/os-scheduler/blob/97266057a6a10322a74b3dc2f004460744563a5f/Project4/Project4/%EC%B5%9C%EC%A2%85_%EC%A3%BC%EC%84%9D.c)
- [호야님 블로그](https://kim-hoya.tistory.com/22)
- [호야님 블로그 - RR 개념 설명](https://kim-hoya.tistory.com/11)
- [C 언어 레퍼런스](https://modoocode.com/51)
