#include "ProcessMgr.h"
#include<unistd.h>
#include<sys/wait.h>
#include<cstring>
ProcessMgr::ProcessMgr(const std::string&command)//构造函数实现
{
    this->command_=command;


}
ProcessMgr::~ProcessMgr(){};//析构函数实现
void ProcessMgr::start()
{
    // 1. 创建两对管道
    pipe(fd_in);
    pipe(fd_out);

    // 2. fork
    child_pid_ = fork();

    if (child_pid_ == 0) // 子进程
    {
        // 3. 重定向 stdin：把 fd_in_[0] 绑到 STDIN_FILENO
        dup2(fd_in[0],STDIN_FILENO);//读数据
        // 4. 重定向 stdout：把 fd_out_[1] 绑到 STDOUT_FILENO
        dup2(fd_out[1],STDOUT_FILENO);//写数据

        // 5. 关掉所有管道 fd
        close(fd_in[0]); close(fd_in[1]);
        close(fd_out[0]); close(fd_out[1]);

        // 6. 执行命令，execvp 需要 char* argv[]
        // 这里先简单处理：用 execl 执行 /bin/sh -c command_
        execl("/bin/sh", "sh", "-c", command_.c_str(), nullptr);

        // 7. execl 失败会到这里
        _exit(1);
    }
    else // 父进程
    {
        // 8. 关掉不用的端
        close(fd_in[0]); // fd_in_ 关读端
        close(fd_out[1]); // fd_out_ 关写端
    }
}

bool ProcessMgr::isAlive()
{
int res=waitpid(child_pid_,NULL,WNOHANG);//WNOHANG意思是不阻塞
//如果返回值是0说明活着，返回true
if(res==0)return true;
//否则返回 false 将child_pid_置为-1
else {
child_pid_=-1;//说明有一个进程阻塞
return false;

}
}
void ProcessMgr::stop()
{std::string str="stop";
    
sendCommand(str);//显示发送stop
waitpid( child_pid_,NULL,0);//等待子进程退出
close(fd_in[1]);//关闭写端
close(fd_out[0]);//关闭读端

}
void ProcessMgr::sendCommand(std::string&cmd)
{std::string line = cmd + "\n";//因为指令服务是按照行读取的因此要进行加\n
    const char*str=line.c_str();
write(fd_in[1],str,strlen(str));


} 