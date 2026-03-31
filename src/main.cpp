#include<iostream>
#include "ProcessMgr.h"
#include"BackupMgr.h"
#include<csignal>
#include<unistd.h>
#include<fcntl.h>
#include<regex>
//定义全局变量running 判断程序是不是在运行
bool running =true;
void handler(int /*sig*/)
{
running =false;


}

int main(){
signal(SIGINT,handler);//截取信号，实现函数
ProcessMgr proc("/bin/cat");//定义一个对象，初始填入bin/cat这个路径
BackupMgr backup("/tmp/mc-backups", "/tmp/mc-world", 5);//test创建一个备份对象
proc.start();//调用成员函数启动进程

//只要子进程挂了就直接重启
while(running)
{
sleep(1);//每秒检查一下
backup.tick();//test调用备份 每5s
//如果子进程挂了就重启
if(!proc.isAlive())//子进程挂了
{
std::cout<<"子进程挂了,重启中..."<<std::endl;
proc.start();//重启
}
else {
//判断内存是不是超出阈值
    long mem =proc.getmemoryKB();
    std::cout<<"子进程存活  内存占用："<<mem<<"KB"<<std::endl;
if(mem>4096000)
{
std::cout<<"内存占用超出阈值，重启中..."<<std::endl;
proc.stop();//停止当前进程
proc.start();//重新启动当前进程
}
//读取子进程输出
std::string line=proc.readOutput();
if(!line.empty())
{
    std::cout<<"[MC输出]:"<<line;
    //匹配玩家登录
    std::regex joinPattern("(\\w+) joined the game");
    std::smatch match;
    if(std::regex_search(line,match,joinPattern))
    {

std::cout<<"[事件]玩家登录："<<match[1]<<std::endl;

    }
    //匹配玩家退出
    std::regex exitpattern("(\\w+) left the game");
    std::smatch match1;
    if(std::regex_search(line,match1,exitpattern))
    { 

std::cout<<"[事件]玩家退出:"<<match1[1]<<std::endl;

    }
    std::regex cannotpattern("Can't Keep Up");
    if(std::regex_search(line,cannotpattern))
    { 

std::cout<<"[事件]服务器警告:TPS过低"<<std::endl;

    }

}
}
}


//程序结束
proc.stop();

//子进程启动成功

std::cout<< "[Sentinel] 已安全退出"<<std::endl;

return 0;
}