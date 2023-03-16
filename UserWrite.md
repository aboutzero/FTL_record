# UserWrite( ) study

## 基本内容

``` txt
    Ctx: Context 上下文
    Lu: Logical Unit
    Pu: Physical Unit
    Ftl: Flash Translation Layer
    Blk: Block
    Req: Request
    Gc: Garbage Collection
    Du: Die Unit
    Rlut: Response Linearization Look Up Tables
```

2754的空间管理分为user lu/middle lu/sys lu/boot lu/rpmb lu 四种

middle lu/sys lu 用于FW 内部自己的管理空间

lu/boot lu/rpmb lu 供Host 直接访问

写通过进入data cache 中的cache node 依靠data cache 的策略和机制，江cache node 以FEReqNode 的形式放入queue

当前cache 设计有两个queue: high priority 和low priority

而FTL 最终将从这两个queue 上获取FTLReqNode 以进行不同的r/w 操作

## UserWrite 主要内容

组装Write Node 以符合PU(Physical Unit)的大小

## UserWrite 流程

内容             |           备注
----             |           ----
user(UserLu_t *)                |               获取传入的逻辑单元
req(FtlReq_t *)                 |               获取pReq
hostCtx(FtlIO_Ctx_t *)          |
type(UserPageAllocatorType_e)   |               设置User Page Allocator: Host GC MAX
reqCnt(U32)                     |               req->u8ReqCnt 请求页计数
duCnts(U32)                     |               Page2Du(reqCnt) Page 转换到Du
page_ordered(U32)|
pga(PageAllocate_t *)           |               指向User Page Allocator

### Step 1

---

根据user->Flags 断言: 此时用逻辑地址没有正在刷新

对flush dirty 进行判定

``` cpp
    /* Logic Unit make dirty */
    if (!(LuMakeDirty(lu)))// 如果make dirty Faulse
        {
            /* Logic Unit get GC request */ 
            if (LuGetGcReq(lu) != NULL)// 如果GC请求可用 返回GC request pointer非空
                /* check 用户空间的TLC Pool id 的Poll recycle */
                PollRecycleCheck(user->SpbAppl[0].TlcPoolId);
            /* Logical space 因为被make dirty 的block 请求 */
            LuIoBlkBy(lu, LU_IO_BLK_F_MAKE_DIRTY);
            /* queue ftl 请求logical space 的io queue */
            LuIoQueue(lu, req);
            return FTL_ERR_OK;
        }
```

### Step 2

---

如果nand request 无返回
则进行`FtlGetNandReq()` FTL 获取Nand 请求

如果nand request 无返回
则进行`NandReqPendLu()` 挂起Nand 逻辑单元的请求

如果Flag是FTL_REQ_F_GC 策略
则进行`UserGcWrite()`

如果IO 资源不足

``` cpp
    /* 如果ls 资源不足 则挂起 */
    if (LuGetIoBlkFlags(lu) != 0)// 如果IO of Logical Unit Block Flag 真 则挂起
    {
pend:
    /* pending this request */
    FtlDbgPrintf(LOG_DEBUG, "user io block by %x\n", LuGetIoBlkFlags(lu));
    req->u32Flags |= FTL_REQ_F_BLKING;
    LuIoQueue(lu, req);// 将ftl 请求插到到logical space io 队列中
    return FTLERR_OK;
    }
```

### Step 3

---

FTL 通过ID 获取(Ctx)上下文;

声明FC_Check()
进行power loss 检验；

g_powerLoss 值是多少? 没有对g_powerLoss 进行初始化是否会导致问题？
// 没有找到g_powerLoss的赋值
(该问题发生为第4级Log)

若产生该问题 则自我阻塞

### Step 4

---

Rlut order

如果`RlutEntryReserve()` 假
则
自行阻塞
打印debug 信息

跳转到got_return:
判断`PageAllocatorFull()`
如果已经满了
则`UserConvertTrigger()`

否则pend

### Step 5

---

Page order

使用`PageAllocatorOrder()` 根据传入的请求进行操作

若操作返回值为0
则发生了意外
立刻自我阻塞
`RlutReturn();`

### Step 6

如果没有任何例外情况发生
则进入`UserWriteCtxSubmit()`

## Struct 解析

### UserLu_t

---

包含

``` txt
    LogicUnit_t(LUA) 逻辑单元：同Die 是nand 里面执行cmd 和report 的最小单位;

    Flags: 标志位

    PageAllocate_t: 两个页分配器-> Host 和GC

    SpbAppl_t: 两个SPB 申请-> Host 和GC

    u32ReeadTraining

    L2ppRa_t: L2ppRa 需要L2PP_RA 开启

    CurWrCtx_t:

    AdminCmdCtx_t:

    Lut_t: 查找子模块表

    RlutLu_t: 响应线性查找表 Response Linearization Look Up Tables Logical Unit

    ConvertReq_t: 转换请求资源

    Wait_t: *GcGvrtSync-> 为GC 和Convert 提供互斥

    AspbInfo_t: Allocated SPB Info 激活SPB 信息

    FC_t: 流控制
```
