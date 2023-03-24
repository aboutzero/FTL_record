# FTL IO

## FE IO Data Structure Form

FeReqNode是与FE的唯一交互位置，所以从Host过来的数据，经过FE处理之后，全部以FeReqNode的形式插入Queue当中使之传向FTL。

FTL层通过`FTL_DisPatchCacheReq`处理FeReqNode。

主程序循环调用`TasksSchedule`进行task调度。在`TasksSchedule`里面，遍历`g_eluOpArry`数组可以获得不同类型的task的执行顺序，(elu0: core0 ergent elu1: FE elu2: FTL elu3: core0 other task, VU, Background Task)，task任务执行是顺序遍历的。

## FeDisPatchCacheReq

`FeDisPatchCacheReq`结构设计思路。

Q: 要求解决Fe处理过后的FeReqNode信息。需要将其command拆解，获取具体执行的IO操作。在解析过后，要求找到对应的nand区域，想后端发送对应的FtlReq。

A: 找到FeReqNode之间的联系与区别FtlReq。比较不同和关联，找到相应的联系，然后找到对应的函数去考虑数据的变化，然后，找到对应的函数实现，最后考虑其数据变化过程的调度关系，关心函数的非正常情况。

### FeReqNode

FeReqNode是[FeReqNode_t](#fereqnode_t)数据类型。

FeReqNode包含自己的ReqId、ReqType、以及一个union(共用体)用于给三种不同ReqType来进行内存的分配。

- ReqId 作为其FeReqNode的标识，通过ReqId可以从FE和FTL共同访问的g_FeReqArry指针中去找到与ReqId所对应的FeReqNode。

- ReqType 是作为FeReqNode的操作请求类型标识的变量，可以被赋予三个值分别对应三种操作请求标识。

- union 中的内容有三种操作请求[FeReqWrite](#fereqwrite)、[FeReqRead](#fereqread)、[FeReqAdmin](#fereqadmin)。

在这样的structure设计中，由于三种操作共用一片内存，所以，在解析FeReqNode请求对nand进行什么操作的时候，必须要先进行ReqType的判断，然后才能将这片共用内存以正确的结构形式进行解析。

### FeReqWrite

FeReqWrite是[FeReqWrite_t](#fereqwrite_t)类型。

### FeReqRead

FeReqRead是[fereqread_t](#fereqread_t)类型。

### FeReqAdmin

FeReqAdmin是[fereqadmin_t](#fereqadmin_t)类型。

## 数据类型

### FeReqNode_t

```cpp
    typedef struct _FeReqNode_t
    {
        U08 u08ReqId; // Request ID
        U08 u08ReqType; // Request Type
        union
        {
            /* 三种IO命令格式 */
            FeReqWrite_t FeReqWriteNode; // w
            FeReqRead_t FeReqReadNode; // r
            FeReqAdmin_t FeReqAdminNode; // Manage
        }
    }FeReqNode_t;
```

### FeReqWrite_t

```cpp
    typedef struct _FeReqWrite_t
    {
        U08 u08Lun; // Logic Unit Number
        U32 u32Fua; // Force Unit Access
        U08 u08Rev; // Reserve // 保留
        IDXList_t SubReqList;
        U08 *pu08DataBuffer;
    } FeReqWrite_t;
```

### FeReqRead_t

```cpp
    typedef struct _FeReqRead_t
    {
        U08 u08Lun; //
        U08 u08Rev[2]; // Reserve
        U08 u08IsHPB; // 
        U32 u32Lba; // logic block address
        U32 u32paa[8]; // 在一个read req中最多有8个 paa
        U08 u08PaaCnt; // req中paa的数量
        U32 u32SectCnt; // sector的数量 每个sector是4k
        U08 *pu08DataBuffer; // physical unit的data buffer地址
    } FeReqRead_t;
```

### FeReqAdmin_t

```cpp
    typedef struct _FeReqAdmin_t
    {
        U08 u08Lun; // logic uinit number
        U08 u08Rev; // reserve word section
        U32 u32Lba; // Logic Block Address
        U32 u32SectCnt; // Sector Count
        U32 u32Rev0;
        U32 u32Rev1;
    } FeReqAdmin_t;

```

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

## Question Record

abbreviation        |         defination
---                 |         ---
LBA                 |         logic block address
PBA                 |         physic block address
LUN                 |         logic unit number
LAA                 |
LDA                 |         logic data address
PAA                 |
PDA                 |         physic data address
spb                 |         super block
LU                  |         logical unit
PU                  |         physical unit
ELU                 |
FFU                 |         field firmware update
l2pp                |         logic to physic
op                  |         Operation
Fsm                 |         finite state machine 状态机
flush               |         刷新
sldFlush            |
Cli                 |         表示client 来自client的请求
FUA                 |         force unit access 强迫写入(此标记意味着必须写入nand)
lrange              |

每个sector是4k，sector指向的是什么的地址？
