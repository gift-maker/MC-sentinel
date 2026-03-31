#include"BackupMgr.h"
#include<ctime>
#include<iostream>
#include <sys/wait.h>
BackupMgr::BackupMgr(const std::string &backupDIr,const std::string &targetDir,int intervalSec)//构造函数初始化成员变量
{

this->backupDIr_=backupDIr;
this->targetDIr_=targetDir;
this->intervalSec_=intervalSec;






}
void BackupMgr::tick()
{elapsed_+=1;
if(elapsed_>=intervalSec_){
    BackupMgr::backup();//调用backup函数
    elapsed_=0;//将这个置0
}

}
void BackupMgr::backup()
{
time_t now =time(NULL);
struct tm*t=localtime(&now);

char timebuf[32];
strftime(timebuf,sizeof(timebuf),"%Y%m%d_%H%M%S",t);//linux中时间格式
//生成文件名
std::string filename=backupDIr_+"/backup"+timebuf+".tar.gz";
std::cout<<"[备份] 开始备份:"<<filename<<std::endl;
pid_t pid =fork();
if(pid==0)//说明是子进程 
{

//子进程执行tar 

const char*args[]={"tar","-czvf",filename.c_str(),targetDIr_.c_str(),NULL};//linux中的打包命令
execvp("tar",(char*const*)args);
_exit(1);//失败了执行程序
/* 第一个参数 file： 要执行的程序名，会自动在 PATH 里找，所以写 "tar" 就够了，不需要写 /usr/bin/tar
第二个参数 argv[]： 参数数组，和命令行一样，第一个元素是程序名本身，最后一个必须是 nullptr//
*/
}
else  {

//等待子进程完成
waitpid(pid,NULL,0);
std::cout<<"[备份] 完成"<<std::endl;

}



}