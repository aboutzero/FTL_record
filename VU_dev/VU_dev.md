# VU Dev

## Overview

The VU module is designed to address communication between the host and the device. For example, the operations of data about reading and writing could be achieved by the VU module.

## Flow of VU

It is necessary to discuss according to the condition of the difference because there are many categories of commands which are in the VU module.

### Category of VU Command

- Write: write a stream of data into nand.

- Read: read a stream of data from nand.

- Manage

### Flow of VU Command Address

It is split into two steps to address the flow of VU, which are first to write a correlative command and to handle it based on the first step.

As for the three categories of VU command, they should be written into a buffer by the command 'write buffer'.

It is an interaction flow drawing between the host and device that:

1. The host sends the command UPIU to the device.(Wrtie buffer command)
2. The device is ready to receive and will send back to the host RTT UPIU.
3. The host receives and sends to the device DATA OUT UPIU.(include VU cmd)
4. The device receives and back RESPONSE UPIU.

![image_vu_interaction.png](/VU_dev/image_vu_interaction.png)

It could be accepted to define a serial of personal commands except write/read/set that make FW handle those commands.

For the second step, it is split of write/read/set.

It is a difference between the first step and writing in step two that putting data rather than command into 'DATA OUT UPIU'.

For example,

![image_vu_step2_write.png](/VU_dev/image_vu_step2_write.png)

It is about step of read that:

The host sends 'COMMAND UPIU(read buffer command)' to the device, and the device returns with 'DATA IN UPIU'(looping until the end of sending). At the last of the flow, the device would send 'RESPONSE UPIU' to the host.

![image_vu_step2_read.png](/VU_dev/image_vu_step2_read.png)

---

## Query its PBA by appointing an LBA
