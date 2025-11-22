#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 100

void * thread_inc(void * arg); // 증가 쓰레드 함수 선언
void * thread_des(void * arg); // 감소 쓰레드 함수 선언

long long num = 0;             // 쓰레드 간 공유되는 전역 변수
pthread_mutex_t mutex;         // 뮤텍스 변수 선언

int main(int argc, char *argv[])
{
    pthread_t thread_id[NUM_THREAD];
    int i;

    // 뮤텍스 초기화
    pthread_mutex_init(&mutex, NULL);

    // 100개의 쓰레드 생성
    for(i=0; i<NUM_THREAD; i++)
    {
        // 짝수일 경우 증가 쓰레드 함수 (thread_inc)
        if(i % 2 == 0)
        {
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        }
        // 홀수일 경우 감소 쓰레드 함수 (thread_des)
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

    printf("result: %lld \n", num);

    // 뮤텍스 소멸
    pthread_mutex_destroy(&mutex);

    return 0;
}

void * thread_inc(void * arg)
{
    int i;
    for(i=0; i<5000000; i++)
    {
        // 임계 영역 시작: 뮤텍스 잠금(Lock)
        pthread_mutex_lock(&mutex);
        
        // 임계 영역: num 변수 증가
        num += 1;
        
        // 임계 영역 끝: 뮤텍스 잠금 해제(Unlock)
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void * thread_des(void * arg)
{
    int i;
    for(i=0; i<5000000; i++)
    {
        // 임계 영역 시작: 뮤텍스 잠금(Lock)
        pthread_mutex_lock(&mutex);
        
        // 임계 영역: num 변수 감소
        num -= 1;
        
        // 임계 영역 끝: 뮤텍스 잠금 해제(Unlock)
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}