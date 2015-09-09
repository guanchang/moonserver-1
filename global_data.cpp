#include "global_data.hpp"
#include "singleton_buff.hpp"

Queue<BufBlock_t*> g_receive_queue;  //接受队列
Queue<BufBlock_t*> g_send_queue;     //发送队列

dll_func_t dll;

ServerConfig_t  g_server_conf;
