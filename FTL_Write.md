# FTL Write

## FeDisPatchCacheReq

解析[ReqNode](#fereqnode_t)

ReqNode是与FE的唯一交互位置

FE根据ReqNode的类型是挂在三种[`Queue`](#queue_t) `Admin Read Write`

`FeDisPatchCacheReq`就是从三种队列上取出ReqNode

### FeReqNode_t

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

```txt
LBA: logic block address
LUN: 
LAA:
spb:
LU:
PU:
l2pp:
op: Option
Fsm: finite state machine 状态机
flush: 刷新
sldFlush: 
```
