# FTL_DisPatchCacheReq

## 概念解析

---

FUA: Force Unit Access
    强制数据直接写入flash

写入数据会进入缓冲后返回写入完成。而此时数据并未进入flash，所以通过FUA指令可以跳过缓存直接写入flash，但会牺牲性能。这是由于Host必须等待数据写入flash后才能进行操作。

OS(Host)会定时发出Flush Cache去清空硬盘缓冲区。

LUN: Logic Unit Number
    逻辑单元号

## Struct 解析

---

* FeReqNode_t

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

    typedef struct _FeReqWrite_t
    {
        U08 u08Lun; // Logic Unit
        U32 u32Fua; // Force Unit Access
        U08 u08Rev; // Reserve // 保留
        IDXList_t SubReqList;
        U08 *pu08DataBuffer;
    }FeReqWrite_t;
    
    typedef struct _FeReqReadNode
    {
        U08 u08Lun; // 等于LS logic space/unit
        U08 u08Rev[2]; // Reserve // 保留
        U32 u32Lba; // Logic Block Address
        U32 u32SectCnt; // Sector Count
        U08 *pu08DataBuffer;
    }FeReqReadNode;
    
    typedef struct _FeReqAdmin_t
    {
        U08 u08Lun;
        U08 u08Rev; // ? Reserve // 保留
        U32 u32Lba; // Logic Block Address
        U32 u32SectCnt; // Sector Count
        U32 u32Rev0;
        U32 u32Rev1;
    }FeReqAdmin_t;

    typedef struct _IDXList_t // 双链表 但是pre 和next作为 u_char 存在的而不是指针
    {
        Indexnode_t HeadNode; // 头节点
        BYTE u8arrySize; // 长度
        BYTE nodeCnt; // 节点数量
        Indexnode_t *indexArry; // 链
    }IDXList_t;

    typedef struct _Indexnode_t
    {
        BYTE u8pre; // BYTE: unsinged char
        BYTE u8next;
    }Indexnode_t;

    typedef FtlErr_t (*FeNodeProc)(FeReqNode_t *Node); // typedef声明了一种返回值是FtlErr_t 类型、函数参数是FeReqNode_t* 类型的函数指针类型 FeNodeProc

    typedef struct _LogicUnit_t
    {
        u8 LuId;
        u8 LuFlags;
        U16 IoBlockFlags;
        U32 Capacity; // 容量
        U32 sn;
        BlkQueue_t BlkQueue;
        GcReq_t gcReq;
        SysLogDesc_t Sld;
        SldFlush_t SldFlush;

        LogicUnitMethod_t intf; // 方法函数
    }LogicUnit_t;

```

## Function 解析

---

### FTL_DisPatchCacheReq()

分派Cache Request

---

进入FTL_DisPatchCacheReq():

for循环次数为QueueArray 中有几种Queue 类型

一个Queue 深度是32

`ARRAY_SIZE()` <==> `sizeof()/sizeof((x)[0])`

在write simulation 时循环3次
分别对应处理这三种类型Queue

1. queue空判定

   若当前队列是write类型

   若队列正在flushing且当前writeCtx忙则sys_assert 提交空cacheWrite 否则继续

2. 当前队列非空循环

   使用Queue_PeekHead获取ReqId 由此找到Request Node(结点)

   如果出现OOM或者Blocked则return

   如果出现挂起 则break

   其他正常情况执行一次Queue_Dequeue(从队首删除一个queue结点)
   Proc计数++
   对应前端g_FtlIoStatic相应节点数计数++

   如果Proc计数大于FE_MAX_PROC_CNT == 10则return

---

### FTL_DisPatchNandReqDone

分派Nand Request

---

---

### FtlWriteCmdProc()

---

如果ReqNode里面Lun等于USER_LU_ID则`FtlSubmitCacheWrite()`
而Lun等于或者SPECIAL_LU_ID则进行 `FtlSubmitSpecialWrite()`
而Lun等于DEVLOG_LU_ID则进行 `FtlSubmitDevLogWrite()`
其他情况printf log

---

#### FtlSubmitCacheWrite

---

`UserLu_t *UserLu = (UserLu_t *)LuGet(USER_LU_ID);`

返回`g_LogicUnits[0]`
类型`LogicUnit`
包含基本数据和数据方法
进行数据类型转换成`UserLu_t*`

于是此时，地址`&g_LogicUnits[0]`是作为一个Logical Unit而解析

判别`g_FtlBootDone`返回相应的`ErrCode`

当FTL Boot启动以后，ErrCode = FTL_ERR_OK

则启动FsmCtxRun()进入对应状态机

`return`

---

#### FtlSubmitDevLogWrite

---

---

## 流程解析

FTL_DisPatchCacheReq()作为FTL中的IO流程中的最前端接口，承接FE封装、打包好的所有数据和命令。

在FTL_DisPatchCacheReq()中：

对三种类型的Queue: Admin Read Write

`Queue_PeekHead()`读取出队头结点(ReqNode) <==> `ReqNode = &g_FeReqArry[ReqId]`

根据结点的opcode(option code)对结点ReqNode进行func并返回结果Ret
func <==> FtlWriteCmdProc | FtlReadCmdProc | FtlTrimCmdProc | FtlAdminCmdProc | NULL

## Questions

`PHL_PollingTMReq(16);`是什么？

IO Block Flags 有些什么值？
