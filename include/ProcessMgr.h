#pragma once 
#include <unistd.h>
#include<string>

class ProcessMgr
{

public:
ProcessMgr(const std::string&command);//构造函数
~ProcessMgr();//析构函数
void stop();
void start();
bool  isAlive();
void sendCommand(std::string& command);
std::string readOutput();
long getmemoryKB();//返回子进程RSS内存  KB
private:
std::string command_;//表示要执行的命令
pid_t child_pid_=-1 ;//子进程pid=-1表示没有启动
int fd_in[2];
int fd_out[2];


};