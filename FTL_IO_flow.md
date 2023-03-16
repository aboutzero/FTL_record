# FTL_IO_flow

数据从FE 过来，进入`FTL_DisPatchCacheReq()`

## FTL_DisPatchCacheReq

循环遍历三种队列(FeAdmin, FeRead, FeWrite)

通过`Queue_PeekHead()`获取Request Node ID
则可以由此从`g_FeReqArry`中取出`ReqNode`

通过`g_FeNodeOp`结构中的`FeNodeProc`函数指针指向`FtlWriteCmdProc`等函数
进入`FtlWriteCmdProc`

通过`FtlSubmitCacheWrite`提交`ReqNode`
