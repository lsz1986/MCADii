#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include "Settings.h"
#include "GlobalDef.h"
#include "MyUSB.h"
#include "MyReg.h"
#include "MySoftDog.h"

long IntRound(double x);
BOOL IsAlpha(char ch);
BOOL IsDigit(char ch);
int ftoi(double f);
int PtCrossLine(CPoint pt,CPoint ptStart,CPoint ptEnd);
CPoint ptHpglToScr(CPoint ptHpgl);
CPoint ptScrToHpgl(CPoint ptScr);

void delayus(int num); //延时num个us数

extern CMySoftDog gMySoftDog;
extern CSettings gSet; //全局变量
extern CMyReg gReg;
extern COutLineProp gOutLineProp;
extern CTextProp gTextProp;

extern CColorSettings gColorSet;
extern const int YHZIntArray[16];

extern bool g_bParaRead;
extern CPtrArray g_ptrCncList;
extern CWnd* g_pMainfrm;

typedef struct _type_dpoint_
{
	double x;
	double y;
} ST_DPOINT;

typedef struct _type_line_
{
	ST_DPOINT Start;
	ST_DPOINT End;
} ST_LINE;

extern ST_MAC_PARA gPara;
extern UNION_MAC_PARA gTempPara;
extern ST_SYS_STATE gSysState;
extern UNION_SYS_STATE gTempSysState;
extern CMyUSB gUSB;

ST_DPOINT GetLineIntersection(ST_LINE RefLine1, ST_LINE RefLine2);
double getdistance(CPoint pt1,CPoint pt2);

int ReadMacPara();
void WriteMacPara();


#endif
