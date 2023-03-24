# JESD220e UFS 3.1

## 术语和定义

```txt
Application Client：主机SCSI命令和任务管理功能的发起实体
Byte：一个 8 位数据值，最高有效位标记为位 7，最低有效位标记为位 0
CDB：命令描述块，由应用客户端发送到设备服务端，最长16字节的固定长度或者12至260字节的可变长度
Decice ID：一个UFS设备的总线地址
Device Server：UFS设备中用于处理SCSI命令和任务管理功能的实体
Word：一个 16 位数据值，最高有效位标记为第 15 位，最低有效位标记为第0位
Doubleword / Dword：一个 32 位数据值，最高有效位标记为第 31 位，最低有效位标记为第0位
Quadword：一个 64 位数据值，最高有效位标记为第 63 位，最低有效位标记为第0位
Gigabyte：1,073,741,824 或者 2^30字节
Kilobyte: 1024 或者 2^10字节
Megabyte: 1,048,576 或者 2^20字节
Host：具有主要计算设备特征的设备或实体，包括一个或多个scsi发起方设备
Initiator device：在事务传输中，SCSI命令请求消息发送到目标法的组织方
Logical Unit：逻辑单元是一个总线设备的内部实体，它执行某种功能或定位总线设备内的特定空间或配置
Logical Unit Number：一个设备中逻辑单元的身份识别号
Segment：表示数据结构中指定数量的顺序寻址字节
Segment ID：一个 16 位值，表示表中的索引或段描述符的地址，或者只是作为绝对地址元素的绝对值
SCSI Request Block：一个包含多字节SCSI命令和其它命令所需附加内容的数据包，由主机生成，作用于总线设备
Target device：在一个传输事务中，SCSI命令和请求信息的接收者
Task：一个任务是一个 SCSI 命令，它包括完成所有数据传输的所有事务和一个满足该命令请求服务要求的状态响应
Transaction：导致在目标设备和发起设备之间传输串行数据包的UFS原语行为
UFS Protocol Information Unit：UFS控制器和设备之间的消息传输通过UPIU完成，这是由UFS定义的包含多种按不同信息字段排列的顺序寻址的数据结构
Unit：一个总线设备
Unit Attention：使用 SCSI 协议的总线设备在继续处理请求和响应之前需要得到服务的情况。
```

## 缩略词

缩写|定义
---|---
CDB|命令描述符块
CPort|CPort是设备内UniPro传输层(L4)上的服务接入点，用于面向连接的数据传输
DMA|直接内存访问
DSC|数码相机
FFU|现场固件升级
HCI|主机控制器接口
HPB|UFS主机性能增强器
IID|发起方编号
LBA|逻辑块地址
LUN|逻辑单元号
PDU|协议数据单元
PLL|锁相环
PMP|便携式媒体播放器
PSA|生产状态感知
RFU|保留供以后使用
RPMB|重放保护内存块
SBC|SCSI 块命令
SID|段号
SDU|服务数据单元
SPC|SCSI主要命令
T_PDU|MIPI UniPro协议数据单元
T_SDU|MIPI UniPro协议服务数据单元
UMPC|Ultra-Mobile PC
UniPro|Unified Protocol
UPIU|UFS Protocol Information Unit
UTP|UFS Transport
