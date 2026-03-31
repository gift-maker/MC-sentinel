#include<iostream>
#include "ProcessMgr.h"
#include<csignal>
#include<unistd.h>
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
//只要子进程挂了就直接重启
while(running)
{
sleep(1);//每秒检查一下
if(!proc.isAlive())//子进程挂了
{
std::cout<<"子进程挂了,重启中..."<<std::endl;
proc.start();//重启
}
else {
std::cout<<"子进程存活  内存占用："<<proc.getmemoryKB()<<"KB"<<std::endl;

}
}


//程序结束
proc.stop();

//子进程启动成功

std::cout<<"子进程启动成功.PID："<<std::endl;

return 0;
}