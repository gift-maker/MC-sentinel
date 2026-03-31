#include "ProcessMgr.h"
#include<unistd.h>
#include<sys/wait.h>
#include<cstring>
#include<fcntl.h>
ProcessMgr::ProcessMgr(const std::string&command)//构造函数实现
{
    this->command_=command;


}
ProcessMgr::~ProcessMgr(){};//析构函数实现
//进程开始函数
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
        fcntl(fd_out[0],F_SETFL,O_NONBLOCK);//这个read()默认是阻塞的  将其设置为非阻塞，没数据就返回空
    }
}
//判断进程是否存活
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
//停止进程
void ProcessMgr::stop()
{std::string str="stop";
    
sendCommand(str);//显示发送stop
waitpid( child_pid_,NULL,0);//等待子进程退出
close(fd_in[1]);//关闭写端
close(fd_out[0]);//关闭读端

}
//读取输入的指令
void ProcessMgr::sendCommand(const std::string&cmd)
{std::string line = cmd + "\n";//因为指令服务是按照行读取的因此要进行加\n
    const char*str=line.c_str();
write(fd_in[1],str,strlen(str));


} 
//获取内存
long ProcessMgr::getmemoryKB()
{
std::string path ="/proc/"+std::to_string(child_pid_)+"/status";//将路径拼接好，子进程路径
FILE* fp = fopen(path.c_str(), "r");
if (fp == nullptr) {
    return -1; // 打开文件失败
}
else{
    char line[256];//定义一个字符数组装读到的行
    while(fgets(line,sizeof(line),fp)!=NULL)
    {
if(strncmp(line,"VmRSS:",6)==0)
{
long value;
sscanf(line,"VmRSS:%ld",&value);
fclose(fp);
return value;
}    }

return -1;

} 
}
//读取输出
std::string ProcessMgr::readOutput()
{
std::string result;
char c;
while(true)
{
ssize_t n=read(fd_out[0],&c,1);//每次读一个字符，将字符传到c中，将数量传到n中
if(n<=0)break;  // n == 0：没数据了
                // n == -1：非阻塞模式下没数据也返回 -1
result+=c;
if(c=='\n')break;

}
return result;

}