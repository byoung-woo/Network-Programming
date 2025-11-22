#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h> // for atoi

// 쓰레드 함수 선언
// 쓰레드 함수의 반환형은 항상 void* 이다.
void* thread_main(void *arg);

int main(int argc, char *argv[])
{
    pthread_t t_id;
    int param = 5;
    void *thr_ret; // 쓰레드 함수의 반환 값을 저장할 포인터

    if (argc != 2)
    {
        printf("Usage: %s <count>\n", argv[0]);
        return -1;
    }
    param = atoi(argv[1]);

    // 쓰레드 생성
    if (pthread_create(&t_id, NULL, thread_main, (void*)&param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    }

    // pthread_join 호출: t_id 쓰레드가 종료될 때까지 main 함수는 블로킹(대기) 상태가 됩니다.
    // thr_ret 변수를 통해 쓰레드 함수가 반환하는 값을 받습니다.
    if (pthread_join(t_id, &thr_ret) != 0)
    {
        puts("pthread_join() error");
        return -1;
    }

    // 쓰레드 함수에서 반환된 값을 출력
    // 반환된 값은 (void*)형태로 전달되므로, 원하는 자료형으로 캐스팅하여 사용합니다.
    printf("Thread return msg: %s \n", (char*)thr_ret);
    puts("end of main");

    // 쓰레드 함수가 동적 할당한 메모리를 해제합니다.
    free(thr_ret);

    return 0;
}

// 쓰레드 함수 정의
void* thread_main(void *arg)
{
    int i;
    int cnt = *((int*)arg);
    char *msg = (char*)malloc(sizeof(char) * 50); // 동적 메모리 할당
    strcpy(msg, "Hello, I am a thread!");

    for (i = 0; i < cnt; i++)
    {
        sleep(1);
        puts("running thread");
    }

    // 동적 할당된 메모리 주소를 반환합니다. 이 값은 pthread_join의 두 번째 인자로 전달됩니다.
    return (void*)msg;
}