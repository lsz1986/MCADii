#ifndef _GLOBAL_DEF_H_
#define _GLOBAL_DEF_H_
#include "user_usb_cmd.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define USER_SHOWTASK			WM_USER+101
#define USER_DISP_STAT			WM_USER+102
#define USER_END_DEMO_DISPLAY	WM_USER+103
#define USER_DEMO_DRAW_LINE		WM_USER+104

#define LT_SOLID		0 //ʵ��
#define LT_DASH			1 //���� ����1 ��Ϊ����

#define MAC_PARA_SIZE 54
#define MAC_STATE_SIZE 10

#define PI 3.14159265359

#define PIECE_LIST_RECTSIZE 80.0

#define DISP_START_X 80
#define DISP_START_Y 30

#define TOTAL_SIZE_X 4800
#define TOTAL_SIZE_Y 700

#define TYPE_PLOT		'P'
#define TYPE_CUT		'C'
#define TYPE_MM			'X'
#define TYPE_MOVE		'M'

//------------------------------------------
#define OVERCUT_ANGLE  45.0 //�ǶȲ�����
//-------------20150411------------
#define SD_LV0_ANGLE 16.8  //(����16.8��)���ٵ��ȼ�0
#define SD_LV1_ANGLE 7.0  //(8.0~16.8)���ٵ��ȼ�1

#define CURVATURE_R002 100.0
#define CURVATURE_R004 45.0
#define CURVATURE_R005 36.0
#define CURVATURE_R010 18.0
#define CURVATURE_R020 9.0
#define CURVATURE_R050 3.6
#define CURVATURE_R100 1.8
#define CURVATURE_R150 1.13
#define CURVATURE_R200 1.0
#define CURVATURE_R300 0.6

#define CNC_MAX_SPD     6000

#define CURVE_SPD_R002  400 
#define CURVE_SPD_R004  500 
#define CURVE_SPD_R005  600
#define CURVE_SPD_R010  700
#define CURVE_SPD_R020  800
#define CURVE_SPD_R050  900
#define CURVE_SPD_R100  1200
#define CURVE_SPD_R150  1500
#define CURVE_SPD_R200  2000
#define CURVE_SPD_R300  3000
//----------------------------------
#define TOOL_DRAW_LINE     1  //�����߶�
#define TOOL_DRAW_CURVE    2
#define TOOL_DRAW_RECT     3
#define TOOL_ADD_DRILL     4
#define TOOL_ADD_TEXT      5
#define TOOL_SEL_CURVE     6
#define TOOL_SEL_PIECE     7
#define TOOL_SET_CUT_SEQ   8

#define CNC_DATA_SIZE 16
#define V2SOFTBIRTHDAY 1433837031


typedef struct
{
//24-----------------------------------------------
	double m_dPPMMX;
	double m_dPPMMY;
	double m_dPPMMZ;
//10-----------------------------------------------
	u16 m_sXSizeMm;
	u16 m_sYSizeMm;
	u16 m_s10000X;
	u16 m_s10000Y;
	u16 m_sZPosMove;  //�ƶ�ʱϳ��λ�� 
	u16 m_sZPosWork;  //����ʱϳ��λ��
//10------------------------------------------------
	u8 m_cMPDistX; //���ʲ���X
	u8 m_cMPDistY; //���ʲ���Y
	u8 m_cKPDistX; //���ʲ���X
	u8 m_cKPDistY; //���ʲ���Y
	u8 m_cAngleAdjust; //��ֱ��У��
	
	u8 m_cSpdPlot;    //��ͼ�ٶ�
	u8 m_cSpdMilling; //ϳ���ٶ�
	u8 m_cSpdCut;     //�и��ٶ�
	u8 m_cSpdMove;    //�ճ��ٶ�
	u8 m_cPwmPen;   //��ѹ
//3--------------------------------------
	u8 m_cPwmCut;   //�иѹ
	u8 m_cVMEnable; //��ʼ����ʱ�Ƿ������
	u8 m_cMMOnDelay; //ϳ�����������ʱ
}ST_MAC_PARA;

typedef union
{
	ST_MAC_PARA stBuff; 
	unsigned short shortBuff[MAC_PARA_SIZE/2];
	unsigned char charBuff[MAC_PARA_SIZE]; 
} UNION_MAC_PARA;

typedef struct
{
	u32 CncNo;
	u32 CncSum;
	u8 macState; //����״̬
	u8 lastCmdState; //����ִ�з���ֵ
}ST_SYS_STATE; //10�ֽ�

typedef union
{
	ST_SYS_STATE stBuff;
	unsigned char charBuff[MAC_STATE_SIZE];
}UNION_SYS_STATE;

typedef struct
{
	int m_iX; //Ŀ��X
	int m_iY; //Ŀ��Y
	int m_iDeltaX; //X�仯��
	int m_iDeltaY; //Y�仯��
	int m_nLongAxialStep; //�ֶ���
	double m_dAngle;
	double m_dDeltaAngle;
	double m_dCurvature;
	double m_dDccPerCent;

	int m_nEndSpeed; //����ʱ����int����Խ��
	int m_nMaxSpeed;
	u16 m_nCmdType;
}ST_CNC_DATA_ALL;

typedef struct
{
	int m_iX; //Ŀ��X
	int m_iY; //Ŀ��Y
	u16 m_nMaxSpeed;
	u16 m_nEndSpeed;
	u16 m_nCmdType;
	u16 m_nDccPerCent;
}ST_CNC_DATA_SEND;

typedef union
{
	u8 u8Buff[CNC_DATA_SIZE];
	ST_CNC_DATA_SEND stBuff;
}UNION_CNC_DATA_SEND;

class CDPoint  
{
public:
	double x;
	double y;
};

class CPlotCmd //һ�λ�ͼָ��
{
public:
	int m_nType;
	CArray <int,int> m_anData;
};

class CInstruct
{
public:
	char m_cName[3];
	CArray<double,double> m_adPara;
};

typedef union 
{
	u8 u8buff[2];
	u16 u16buff;
}UNION_U816;

typedef union 
{
	u8 u8buff[4];
	u32 u32buff;
}UNION_U832;

#define WM_SHOWTASK			WM_USER+101
#define END_WORK_THREAD		WM_USER+110
#define DISP_SYS_STATUS		WM_USER+112

#define START_MX_MM (gSet.getWorkStartX()+7)
#define START_PENX_MM (gSet.getWorkStartX()-30)

#define START_MY_MM (gSet.getWorkStartY())
#define START_PENY_MM (gSet.getWorkStartY()+65)

#endif
