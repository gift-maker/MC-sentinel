#include<iostream>
#include "ProcessMgr.h"
#include<csignal>
#include<unistd.h>
#include<fcntl.h>
//定义全局变量running 判断程序是不是在运行
bool running =true;
void handler(int /*sig*/)
{
running =false;


}

int main(){
signal(SIGINT,handler);//截取信号，实现函数
ProcessMgr proc("/bin/cat");//定义一个对象，初始填入bin/cat这个路径
proc.start();//调用成员函数启动进程
proc.sendCommand("hello from sentinel"); //test
//只要子进程挂了就直接重启
while(running)
{
sleep(1);//每秒检查一下

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
    std::cout<<"MC输出:"<<line;

}






}
}


//程序结束
proc.stop();

//子进程启动成功

std::cout<<"子进程启动成功.PID："<<std::endl;

return 0;
}