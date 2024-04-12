#include <stdio.h>
#include <string.h>

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#include "devices/timer.h"

#include "projects/automated_warehouse/aw_manager.h"

struct robot* robots;

// test code for central control node thread
void test_cnt(){
        while(1){
                print_map(robots, 1);
                thread_sleep(1000);
                block_thread();
        }
}

// test code for robot thread
void test_thread(void* aux){
        int idx = *((int *)aux);
        int test = 0;
        while(1){
                printf("thread %d : %d\n", idx, test++);
                thread_sleep(idx * 1000);
        }
}

// entry point of simulator
void run_automated_warehouse(char **argv)
{
        init_automated_warehouse(argv); // do not remove this
        int num = *argv[1] - '0'; // number of robots by argument

        printf("implement automated warehouse!\n");

        // test case robots
        robots = malloc(sizeof(struct robot) * num);
        // setRobot(&robots[0], "R1", 5, 5, 0, 0); //초기에는 무조건 W에 위치해야 함
        // // setRobot(&robots[1], "R2", 0, 2, 0, 0);
        // // setRobot(&robots[2], "R3", 1, 1, 1, 1);
        // // setRobot(&robots[3], "R4", 5, 5, 0, 0);
        for (int i = 0; i < num; i++) {
                char name[4]; // 로봇 이름을 저장할 배열
                snprintf(name, sizeof(name), "R%d", i + 1); // 로봇 이름 생성
                setRobot(&robots[i], name, 5, 5, 0, 0); // 로봇 설정->모두 W에 배치
        }

        // // example of create thread
        // tid_t* threads = malloc(sizeof(tid_t) * (num+1));
        // int idxs[2] = {0};
        // threads[0] = thread_create("CNT", 0, &test_cnt, NULL); //중앙제어노드
        // threads[1] = thread_create("R1", 0, &test_thread, &idxs[0]);
        // // threads[2] = thread_create("R2", 0, &test_thread, &idxs[2]);
        // // threads[3] = thread_create("R3", 0, &test_thread, &idxs[3]);
        // example of create thread
        tid_t* threads = malloc(sizeof(tid_t) * (num + 1));
        int idxs[num]; // 인덱스 배열 생성
        threads[0] = thread_create("CNT", 0, &test_cnt, NULL); // 중앙 제어 노드 스레드
        for (int i = 0; i < num; i++) {
                printf("iterate %d\n", i);
            char name[4]; // 로봇 이름을 저장할 배열
            snprintf(name, sizeof(name), "R%d", i + 1); // 로봇 이름 생성
            idxs[i] = i; // 인덱스 배열 초기화
            threads[i + 1] = thread_create(name, 0, &test_thread, &idxs[i]); // 로봇 스레드
        }

        // if you want, you can use main thread as a central control node
        
}