#ifndef WORK_THREAD_HPP
#define WORK_THREAD_HPP

#include <pthread.h>
#include "global_data.hpp"

#define CYCLE_WAIT_NANO_SEC 2000000

#define PUSH_ERRMSG(TYPE) \
    do { \
        new_block->buf_head.buf_type = TYPE; \
        new_block->buf_head.len = 0; \
        if (stop) \
            return ((void *)0);\
            HandlerBase* h = reactor->get_handler(new_block->buf_head.sk.socket); \
            if (NULL == h) { \
                free_block(new_block); \
                continue; \
            }\
            while (!stop) { \
                if (!h->push_buf(new_block)) { \
                    usleep(1000); \
                } \
                else { \
                    break; \
                } \
            } \
    } while (0) \


typedef struct {
    pthread_t thread_tid;
} Thread;
Thread* tptr; 

extern volatile bool stop;

void* work_run(void* arg) {
    printf("create pthread %d\n", pthread_self());

    BufBlock_t* block = NULL;
    BufBlock_t* new_block = NULL;

    Reactor* reactor = (Reactor*) arg;

    while (!stop) {
        reactor->handle_events(1000);

        //将接收到的协议 串行处理
        while ((block=g_receive_queue.pop_queue()) != NULL) {
            char* send_data;
            int send_len = 0;

            int ret_code = dll.handle_process((char*)block->page_base, block->buf_head.len, &send_data, &send_len, &(block->buf_head.sk));

            new_block = alloc_block(send_len);
            new_block->buf_head = block->buf_head;
            new_block->buf_head.len = send_len;
            memcpy(new_block->page_base, send_data, send_len);

            free_block(block);
            if (send_len != 0 && g_server_conf.need_free_send_buf) {
                free(send_data);
            }

            if (ret_code == -1) { //关闭链接
                PUSH_ERRMSG(FIN_BLOCK);
            }
            else if (send_len == 0 || send_len > 8388608) { //长度不对
                LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "socket fd="<<new_block->buf_head.sk.socket<<" send too large pkg");
                PUSH_ERRMSG(CLEAR_BLOCK);
            }
            else if (send_len > 0) {
                new_block->buf_head.buf_type = DATA_BLOCK;
                if (1 == ret_code) {
                    new_block->buf_head.buf_type |= FIN_BLOCK;
                }

                if (stop) {
                    return ((void *)0);
                }

                HandlerBase* h = reactor->get_handler(new_block->buf_head.sk.socket);
                if (NULL == h) {
                    free_block(new_block);
                    continue;
                }

                if (!h->push_buf(new_block)) {
                    free_block(new_block);
                }
            }
        }
    }
}

void create_work_thread(Reactor* reactor, int i) {
    pthread_create(&tptr[i].thread_tid, NULL, work_run, (void *)reactor);
}

#endif
