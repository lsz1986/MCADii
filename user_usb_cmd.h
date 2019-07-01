#ifndef _USER_USB_CMD_H_
#define _USER_USB_CMD_H_

//-------系统状态定义区-----------------------------------
#define SYS_INIT        0 //系统初始化
#define SEEK_ZP         1 
#define INIT_USB        2 //初始化串口 USB
#define MOVE_TO_DEFZP   3 
#define READY           4 //准备就绪 
#define READY_PAUSE     5 //暂停
#define WORKING         6 //正在切割
#define WORK_PAUSE      7 //切割暂停
#define ERROR_EXPIRED   8
#define ERROR_REG       9
//-------------------------------------------------------

#define CMD_GET_STATE   0x0a //查询主机状态

#define CMD_PARA_READ   0x10 //读参数
#define CMD_PARA_WRITE  0x11 //写参数

#define CMD_NEW_JOB    0x15  //发送切割信息
#define CMD_START_JOB  0x16 //开始切割

#define CMD_START_TEST 0x18 //手动操作，测试
 
#define TCMD_TO_DEFZP  0x21 //回默认原点
#define TCMD_PEN_DOWN  0x22 //落笔or抬笔
#define TCMD_CUT_DOWM  0x23 //落刀or抬刀 
#define TCMD_MM_DOWM   0x24 //铣刀落下or抬起 
#define TCMD_VM_ON	   0x25 //开启or关闭气泵
#define TCMD_MM_ON	   0x26 //除尘铣刀电机，开启OR关闭

#define CMD_GET_CHIPID     0x30 //获取STM32ID号
#define CMD_GET_LAST_TIME  0x31 //获取最后一次使用时间
#define CMD_SET_LAST_TIME  0x32 //更新最后一次使用时间

#define CMD_ERASE         0x4a

#define CMD_GET_DEB_CODE  0x2a //判断是否完全解锁
#define CMD_SET_DEB_CODE  0x2b //更新完全解锁码

#define CMD_WORK_PAUSE   0x50 //工作暂停
#define CMD_WORK_RESUME  0x51 //工作继续
#define CMD_WORK_CANCEL  0x52 //取消工作

#define ERROR_NONE      0
#define ERROR_BYTESIZE  1
#define ERROR_XYSIZE    2
#define ERROR_DATA      3
#define ERROR_NOTREADY  4
#endif
