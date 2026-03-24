#include "ProcessMgr.h"
#include<unistd.h>
ProcessMgr::ProcessMgr(const std::string&command)
{
    this->command_=command;


}
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
ProcessMgr::~ProcessMgr(){};


