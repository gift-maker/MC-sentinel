#pragma once 
#include <unistd.h>
#include<string>
class BackupMgr
{


public:
BackupMgr(const std::string& backupDir, const std::string& targetDir, int intervalSec);//构造函数
void backup();//立即执行一次备份
void tick();//每秒调用一次时间到了自动执行备份
private:
std::string backupDIr_;//备份存放的路径
std::string targetDIr_;//要备份的目录
int intervalSec_;  //备份间隔秒数
int elapsed_=0;//一个计时器告诉tick时间过了多久





};