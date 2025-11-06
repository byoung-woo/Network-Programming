#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// 쓰레드 함수(thread_main) 선언
void* thread_main(void *arg);

int main(int argc, char *argv[])
{
    pthread_t t_id;
    int thread_param = 5;

    // 쓰레드 생성
    // t_id: 생성될 쓰레드의 ID 저장 변수 주소
    // NULL: 쓰레드 특성 정보 (기본 특성 사용)
    // thread_main: 쓰레드의 main 함수 역할을 하는 함수 포인터 (쓰레드 함수)
    // (void*)&thread_param: 쓰레드 함수에 전달할 인자의 주소값
    if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    }

    // 메인 함수의 실행을 10초간 지연시켜, 생성된 쓰레드가 충분히 실행되도록 합니다.
    // 프로세스가 종료되면 쓰레드도 함께 소멸되기 때문에 이를 방지합니다. [cite: 117]
    sleep(10);
    puts("end of main");

    return 0;
}

// 쓰레드 함수 정의
void* thread_main(void *arg)
{
    int i;
    // 전달받은 인자 (thread_param)를 역참조하여 카운트 횟수를 얻습니다.
    int cnt = *((int*)arg);

    for (i = 0; i < cnt; i++)
    {
        sleep(1);
        puts("running thread");
    }
    // 쓰레드 함수의 실행이 완료되면 쓰레드는 종료됩니다. [cite: 106]
    return NULL;
}