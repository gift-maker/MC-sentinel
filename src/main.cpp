// #include <cstdio>
// #include <string>
// #include <iostream>
// using namespace std;
// // 执行一条 shell 命令，返回其标准输出
// string runCommand(const std::string& cmd) {
//     string result;
//     const char *tianru=cmd.c_str();
//     char buffer[1024];
//     FILE*fp;
//     fp=popen(tianru,"r");
//     //检查popen是否成功驱动
//     if(fp==NULL)
//     {
// perror("popen执行失败");
// return " ";
//     }
// while(fgets(buffer,sizeof(buffer),fp)!=NULL)
// {
// result+=buffer;

// }
// int status=pclose(fp);
// if(status==-1){perror("pclose失败");}
// cout<<"命令执行完毕，退出状态码:"<<status;
//     return result;
// }

// int main() {
//     std::string output = runCommand("ps -ef");
//     std::cout << output << std::endl;
//     return 0;
// }
#include <iostream>
#include <unistd.h>
#include <cstring>    // strlen
#include <sys/wait.h> // waitpid
using namespace std;
int main() {
    int fd[2];
    
    // 1. 创建管道
    pipe(fd); 
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // 子进程：
        // 关闭读端 fd[0]
        close(fd[0]);//子进程不读
        // 用 write() 往 fd[1] 写 "hello from child\n"
        const char *msg="hello from child";
        write(fd[1],msg,strlen(msg));
        // 关闭写端，退出
        close(fd[1]);
        _exit(0);
        
    } else {
        // 父进程：
        // 关闭写端 fd[1]
        close(fd[1]);
char buffer[128]={};
        // 用 read() 从 fd[0] 读数据，打印
        read(fd[0],buffer,sizeof(buffer));
        cout<<"父进程收到"<<buffer;
        // waitpid 等子进程结束
waitpid(pid,NULL,0);


    }
    return 0;
}