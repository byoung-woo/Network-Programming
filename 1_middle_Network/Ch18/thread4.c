#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 10

// 쓰레드 간 공유되는 전역 변수
long long num = 0;

// 뮤텍스 변수 선언
pthread_mutex_t mutex;

// 증가 작업을 수행하는 쓰레드 함수
void* thread_inc(void *arg)
{
    int i;
    for(i=0; i<5000000; i++)
    {
        // 임계 영역 시작: 뮤텍스 잠금(Lock)
        pthread_mutex_lock(&mutex);
        
        // 임계 영역: num 변수 변경
        num += 1;
        
        // 임계 영역 끝: 뮤텍스 잠금 해제(Unlock)
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// 감소 작업을 수행하는 쓰레드 함수
void* thread_des(void *arg)
{
    int i;
    for(i=0; i<5000000; i++)
    {
        // 임계 영역 시작: 뮤텍스 잠금(Lock)
        pthread_mutex_lock(&mutex);
        
        // 임계 영역: num 변수 변경
        num -= 1;
        
        // 임계 영역 끝: 뮤텍스 잠금 해제(Unlock)
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread_id[NUM_THREAD];
    int i;

    printf("sizeof long long: %zd \n", sizeof(long long)); // long long 크기 출력

    // 뮤텍스 초기화
    pthread_mutex_init(&mutex, NULL); // [cite: 153]

    // 10개의 쓰레드 생성
    for(i=0; i<NUM_THREAD; i++)
    {
        // 짝수 인덱스 쓰레드는 증가(thread_inc), 홀수 인덱스 쓰레드는 감소(thread_des) 작업을 할당
        if(i % 2 == 0)
        {
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        }
        else
        {
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
        }
    }

    // 모든 쓰레드의 종료를 대기
    for(i=0; i<NUM_THREAD; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
    
    // 최종 결과 출력 (기대값: 0)
    printf("result: %lld \n", num);

    // 뮤텍스 소멸
    pthread_mutex_destroy(&mutex);

    return 0;
}