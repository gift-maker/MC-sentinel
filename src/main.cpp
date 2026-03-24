#include<iostream>
#include "ProcessMgr.h"
int main(){
ProcessMgr proc("/bin/cat");//定义一个对象，初始填入”bin/cat这个路径
proc.start();//调用成员函数启动进程
//子进程启动成功
std::cout<<"子进程启动成功.PID："<<std::endl;

return 0;




}