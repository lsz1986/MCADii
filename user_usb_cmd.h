#ifndef _USER_USB_CMD_H_
#define _USER_USB_CMD_H_

//-------ϵͳ״̬������-----------------------------------
#define SYS_INIT        0 //ϵͳ��ʼ��
#define SEEK_ZP         1 
#define INIT_USB        2 //��ʼ������ USB
#define MOVE_TO_DEFZP   3 
#define READY           4 //׼������ 
#define READY_PAUSE     5 //��ͣ
#define WORKING         6 //�����и�
#define WORK_PAUSE      7 //�и���ͣ
#define ERROR_EXPIRED   8
#define ERROR_REG       9
//-------------------------------------------------------

#define CMD_GET_STATE   0x0a //��ѯ����״̬

#define CMD_PARA_READ   0x10 //������
#define CMD_PARA_WRITE  0x11 //д����

#define CMD_NEW_JOB    0x15  //�����и���Ϣ
#define CMD_START_JOB  0x16 //��ʼ�и�

#define CMD_START_TEST 0x18 //�ֶ�����������
 
#define TCMD_TO_DEFZP  0x21 //��Ĭ��ԭ��
#define TCMD_PEN_DOWN  0x22 //���oŗ��
#define TCMD_CUT_DOWM  0x23 //�䵶oŗ�� 
#define TCMD_MM_DOWM   0x24 //ϳ������oŗ�� 
#define TCMD_VM_ON	   0x25 //����or�ر�����
#define TCMD_MM_ON	   0x26 //����ϳ�����������OR�ر�

#define CMD_GET_CHIPID     0x30 //��ȡSTM32ID��
#define CMD_GET_LAST_TIME  0x31 //��ȡ���һ��ʹ��ʱ��
#define CMD_SET_LAST_TIME  0x32 //�������һ��ʹ��ʱ��

#define CMD_ERASE         0x4a

#define CMD_GET_DEB_CODE  0x2a //�ж��Ƿ���ȫ����
#define CMD_SET_DEB_CODE  0x2b //������ȫ������

#define CMD_WORK_PAUSE   0x50 //������ͣ
#define CMD_WORK_RESUME  0x51 //��������
#define CMD_WORK_CANCEL  0x52 //ȡ������

#define ERROR_NONE      0
#define ERROR_BYTESIZE  1
#define ERROR_XYSIZE    2
#define ERROR_DATA      3
#define ERROR_NOTREADY  4
#endif
