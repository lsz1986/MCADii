// HpglFile.h: interface for the CHpglFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HPGLFILE_H__8CB4A146_F567_4924_BDC1_F3FA4BBDDE77__INCLUDED_)
#define AFX_HPGLFILE_H__8CB4A146_F567_4924_BDC1_F3FA4BBDDE77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"
#include "Charactor.h"
#include "Curve.h"

#define NONE_TYPE			10
#define DIGIT_TYPE			11
#define STRING_TYPE			12
#define ESC_TYPE			13
#define DT_TYPE				14
//图元类型
#define PLOT_CMD_PD		20  //线段：点1 X1，Y1，点2 X2，Y2,...
#define PLOT_CMD_PU		21  //线段：点1 X1，Y1，点2 X2，Y2,...
#define PLOT_CMD_PG			22
#define PLOT_CMD_LT			23 //线型
#define PLOT_CMD_SP			24

#define	PROCESS_MNEMONIC	30//命令
#define PROCESS_PARAMETER	31//参数
#define PROCESS_FIND		32//结束符

class CHpglFile  
{
public:
	CHpglFile();
	virtual ~CHpglFile();

public:
	CPoint CPToLP(CPoint ptCP, int nHeight, CPoint ptPointLU, double dSin, double dCos, double dScale);
	CPoint GetNextCoodinate(WORD wDirection, WORD wLength, CPoint ptPoint);
	void WriteTextData(BOOL bFirst, long x, long y);
	void LineTo(long x, long y);
	void MoveTo(long x, long y);
	void Text2LineInit(void);
	void PlotChar(CPoint ptPointLT,BYTE *pbyData,WORD wBytes,int nLeft,int nDown,int nFontScale,double dTextAngle);
	void Text2Line(CPoint dp,LPCTSTR lpszString, int nCount);
	void CalcTextStartPoint(CDPoint *dp, int strLen);
	void InitHPGL();

	void RemoveBuffer();
	int GetParameterType(char cName[]);
	void Instruct2PlotCmd(CInstruct *pInstruct);
	int OpenPltFile();

public:
	CString m_strPathAndName; //完整的路径加文件名
	CString m_strName; //用于显示
	CPtrArray m_pCmdList_ORG; //原始数据

public:
	int m_nLineType;
	CPoint m_dpCurrent;//当前坐标
	unsigned char m_cTerminator;//标志终结符
	unsigned char m_cTerminatorMode;//模式
	CString m_strText;//字符
	
	double m_dTextAngle;//字符方向
	double m_dTextHeight;//字符高度
	double m_dTextWidth;//字符宽度
	int m_nTextPosition;//字符相对原点的位置
	BOOL m_bPenDown;//笔是否落下
	
private:
	CArray <CCharactor,CCharactor> m_aChinese_char;
	CArray <CCharactor,CCharactor> m_aEnglish_char;
	int m_nInternalLeading_E,m_nHeight_E,m_nDescent_E; //m_nHeight_E是windows中字体的Ascent-InternalLeading
	int m_nHeight_C,m_nWidth_C;
	CPoint m_ptCurrentPosition;   //当前位置(坐标)
	CPoint m_ptPrevPosition;      //前一位置(坐标)

};
#endif // !defined(AFX_HPGLFILE_H__8CB4A146_F567_4924_BDC1_F3FA4BBDDE77__INCLUDED_)
