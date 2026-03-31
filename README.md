
# MC-Sentinel

> 一个用 C++17 编写的 Linux 原生 Minecraft 服务端守护进程。

## 功能特性

- **进程守护**：通过 `pipe + fork` 接管 MC 服务端进程，崩溃后自动重启
- **心跳检测**：每秒检测子进程存活状态，响应时间 < 1 秒
- **内存监控**：实时读取 `/proc/[pid]/status`，超过阈值自动重启
- **日志解析**：正则匹配玩家登录/退出/TPS 警告等关键事件
- **定时备份**：定时调用 `tar` 打包世界文件，按时间戳归档
- **优雅关闭**：捕获 `SIGINT`，退出前向 MC 发送 stop 指令保证数据安全

## 技术栈

- 语言：C++17
- 系统调用：`fork` / `execvp` / `pipe` / `waitpid` / `signal`
- 资源监控：Linux `/proc` 文件系统
- 构建工具：GNU Make

## 编译

\```bash
sudo apt install g++ make
make
\```

## 运行

\```bash
./bin/mc-sentinel
\```

##项目结构

\
MC-Sentinel/
├── src/          # C++ 源代码
├── include/      # 头文件
├── bin/          # 编译产物
└── Makefile
\