#include <stdio.h>
#include <pthread.h>

// 쓰레드 간 공유되는 전역 변수
long long sum = 0;

// 각 쓰레드가 몇 번 연산을 반복할지 정의 (500,000번)
void* thread_main(void *arg);

int main(int argc, char *argv[])
{
    pthread_t t_id1, t_id2;
    int thread_param = 500000;

    // 1번 쓰레드 생성 (sum에 thread_param 만큼 더함)
    if (pthread_create(&t_id1, NULL, thread_main, (void*)&thread_param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    }

    // 2번 쓰레드 생성 (sum에 thread_param 만큼 더함)
    if (pthread_create(&t_id2, NULL, thread_main, (void*)&thread_param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    }

    // 두 쓰레드가 종료될 때까지 대기
    pthread_join(t_id1, NULL);
    pthread_join(t_id2, NULL);

    // 최종 결과 출력 (기대값: 1,000,000)
    printf("result: %lld \n", sum);

    return 0;
}

// 쓰레드 함수 정의: sum 변수에 cnt 만큼 1씩 더하는 연산을 반복합니다.
void* thread_main(void *arg)
{
    int i;
    int cnt = *((int*)arg);

    for (i = 0; i < cnt; i++)
    {
        // 이 부분이 '임계 영역(Critical Section)'입니다.
        // 두 쓰레드가 동시에 sum 변수에 접근하여 연산하면 문제 발생.
        sum += 1;
    }

    return NULL;
}