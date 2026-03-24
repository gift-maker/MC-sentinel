#include<iostream>
#include "ProcessMgr.h"
int main(){
ProcessMgr proc("/bin/cat");//定义一个对象，初始填入”bin/cat这个路径
proc.start();//调用成员函数启动进程
//只要子进程挂了就直接重启
while(true)
{
sleep(1);//每秒检查一下
if(!proc.isAlive())//子进程挂了
{
std::cout<<"子进程挂了,重启中..."<<std::endl;
proc.start();//重启
}
else {
std::cout<<"子进程存活"<<std::endl;

}



}


//子进程启动成功

std::cout<<"子进程启动成功.PID："<<std::endl;





return 0;
}