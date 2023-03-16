# Flow Chart

## main

1. `pthread_mutex_init` // 初始化线程互斥锁
2. pthread_t Host // 定义Host 线程 15处创建线程资源
3. `InitMyCmdQueue` // 初始化CmdQueue
4. `InitDataUPIUStructMem` // 初始化UPIU
5. `logcreate` // 创建log
6. TaskMng_t gUUTaskMngArry[TASK_CNT] // 创建Task Management 数组
7. `ISP_InitOSAPI` // 初始化OS API
8. `TimerStart` // 启动timer OS_TimerInit = TimerInit
9. `UFS_TasksFsmInit(gUUTaskMngArry)` // 初始化Task Management
10. `ISP_InitFtlAPI` // 初始化配置FTL API
11. `API_CTL_MainFlow_FuncSel` // BE API 设置
12. `FTL_BootStart` // 启动FTL boot
13. 一些变量初始化
14. 通过windows 获取启动时间
15. `pthread_create()` // 创建Host 线程
16. while(1) 循环: // 当timeUpFlag 为真时 调度结束d
    1. timeUpFlag // 时间耗尽
    2. 此处等待的是 HostProcess 在write例子中就是GetInfo 中的CaseRegister 循环等待SLOTDOWN 或者timerUpFlag // slot 解释
    3. for循环:(写索引值(WIndex)必须小于最大odder长度)
       1. WriteOdder 里的数据长度必须大于0
       2. WriteCMDCnt++ WCMDOnceCyCnt++ 取LBA write.length write.CmdType
       3. 判断CmdType类型 0 Write 1 Read 2 Trim
       4. IOinfo.FUA == 1 则设置fuaFlag = 1
       5. `SetCMDUPIUToQueue`

    4. while(1) 循环
       1. `TimerPolling` // 计时器轮询
       2. `TaskSchedule` // 启动任务调度器 在这一次循环的task调度中进入了UserWrite
17. 16处的循环跳出 意味着调度结束 实质上已经没有task 即程序结束
18. 任务调度器空载运行 以及 永远不会执行到的destroy 部分

## UserWrite Flow Chart

需要主机进程`GetInfo` 使用`CaseRegister` 创建一个IO_HOST_WRITE 进程

* 怎么进入GetInfo 的？是哪里的调度器？

    windows的线程创建函数创建了一个Host线程
  
    `int pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(*start)(void *), void *arg);`

    整个程序通过 `pthread_create(&Host, NULL, GetInfo, NULL);` 创建了一个Name: Host Process实现: GetInfo

    GetInfo 的函数使用是在一个win 线程上，作为Host 发送IO

## API

```cpp
    EvtTaskProcess()
    FTL_DisPatchCacheReq()
    FTL_DisPatchNandReqDone()
    FTL_Boot()
```

## Questions

1. odder 是什么
