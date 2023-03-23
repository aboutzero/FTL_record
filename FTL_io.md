# FTL IO

## FE IO Data Structure Form

FeReqNode是与FE的唯一交互位置，所以从Host过来的数据，经过FE处理之后，全部以FeReqNode的形式插入Queue当中使之传向FTL。

FTL层通过`FTL_DisPatchCacheReq`处理FeReqNode。

主程序循环调用`TasksSchedule`进行task调度。在`TasksSchedule`里面，遍历`g_eluOpArry`数组可以获得不同类型的task的执行顺序，(elu0: core0 ergent elu1: FE elu2: FTL elu3: core0 other task, VU, Background Task)，task任务执行是顺序遍历的。

## FeDisPatchCacheReq

解析[ReqNode](#fereqnode)

FE根据ReqNode的类型是挂在三种[`Queue`](#queue_t) `Admin Read Write`

`FeDisPatchCacheReq`就是从三种队列上取出ReqNode

### FeReqNode

### Queue_t

```cpp
    typedef struct _Queue_t
    {
        BYTE *indexArry; // queue不直接存放数据，而是存放数据的地址索引数据(32bit 地址)
        BYTE u08head; // 队头
        BYTE u08depth; // 队长
        BYTE u08tail; // 队尾
        BYTE u08Flag; // 是标志什么???
    } Queue_t;
```

## 变量

### global variables basic

```cpp
    LWORD g32UnitStartStop; // 32个bit表示Lun(logic uinit) 1: start 0: stop
    LWORD g32UnitAttention; // 32个bit表示Lun(logic uinit) 1: attention 0: not
    BYTE g08WUnitAttention; // bit 4: report Lun ID, bit 5: UFS device ID, bit 6: Boot ID
    BYTE g08Initial_Phase; // UFS phase
    BYTE g08UFS_State; // UFS state
```

### TASK_ID

```cpp
    typedef enum
    {
        // 共37个Task id 和1个Task 计数
        // 带FE 标识的有21个
        // 带FTL 标识的有4个
        // 带BE 标识的有4个
        // 其他: 2个VU、2个CLI、1个UART、1个IRQ、1个TIMER、1个HPB
    } TASK_ID;
```

### TaskMng

```cpp
    typedef struct _TaskMng_t
    {
        BYTE u8taskId; // task index
        pFunc handler; // function handler
        BYTE u8elu; // elu in which the task run
        BYTE u8priority; // 优先级
        BYTE u8TimerOut; // 超时时间???
        BYTE u8Active; // 激活状态???
    } TaskMng_t;
```

### EluMng_t

```cpp
    typedef struct _EluMng_t
    {
        BYTE u8elu; // elu number
        BYTE u8core; // core in which the elu run
        WORD u16timeout; // elu 的运行时间
    } EluMng_t;
```

```txt
LBA: logic block address
LUN: logic unit
LAA: 
LDA: 
PAA: 
PDA: 
spb: super block
LU: 
PU: 
ELU: 
l2pp: logic to physic
op: Option
Fsm: finite state machine 状态机
flush: 刷新
sldFlush: 
Cli: 表示client 来自client的请求
```
