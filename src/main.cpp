#include <cstdio>
#include <string>
#include <iostream>
using namespace std;
// 执行一条 shell 命令，返回其标准输出
string runCommand(const std::string& cmd) {
    string result;
    const char *tianru=cmd.c_str();
    char buffer[1024];
    FILE*fp;
    fp=popen(tianru,"r");
    //检查popen是否成功驱动
    if(fp==NULL)
    {
perror("popen执行失败");
return " ";
    }
while(fgets(buffer,sizeof(buffer),fp)!=NULL)
{
result+=buffer;

}
int status=pclose(fp);
if(status==-1){perror("pclose失败");}
cout<<"命令执行完毕，退出状态码:"<<status;
    return result;
}

int main() {
    std::string output = runCommand("ps -ef");
    std::cout << output << std::endl;
    return 0;
}
