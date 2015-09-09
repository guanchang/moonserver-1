#ifndef GLOBAL_DATA_HPP
#define GLOBAL_DATA_HPP

#include "queue.cpp"
#include "moonapi.h"

struct BufBlock_t;

extern Queue<BufBlock_t*> g_receive_queue;  //接受队列

extern dll_func_t dll;

extern ServerConfig_t  g_server_conf;


#endif
