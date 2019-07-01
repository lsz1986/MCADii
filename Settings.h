// Settings.h: interface for the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGS_H__7C86702C_BD84_45CD_BE7C_10999ABBB7A2__INCLUDED_)
#define AFX_SETTINGS_H__7C86702C_BD84_45CD_BE7C_10999ABBB7A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define COLOR_PIECESELD	RGB(0,200,200)   //裁片被选中颜色(青色)
#define COLOR_PEN		RGB(255,255,255)       //笔的颜色(白色)
#define COLORM_MM		RGB(255,0,0)	 //原位置铣颜色(红色)
#define COLORM_DRILL	RGB(255,0,255)    //钻孔颜色(白色)

class CColorSettings
{
public:
	CColorSettings();
	virtual ~CColorSettings();

public:
	inline COLORREF getClrPieceSelected(){ return COLOR_PIECESELD;};
	inline COLORREF getClrPen(){ return COLOR_PEN;};
	inline COLORREF getClrMM(){ return COLORM_MM;};
	inline COLORREF getClrMDrill(){ return COLORM_DRILL;};
};

class CTextProp
{
public:
	CTextProp();
	virtual ~CTextProp();

public:
	inline void setTextPosX(double d){m_dTextPosX = d;};
	inline double getTextPosX(){return m_dTextPosX;};

	inline void setTextPosY(double d){m_dTextPosY = d;};
	inline double getTextPosY(){return m_dTextPosY;};

	inline void setTextInflate(int n){m_nTextInflate = n;};
	inline int getTextInflate(){return m_nTextInflate;};

	inline void setTextAngle(double d){m_dTextAngle = d;};
	inline double getTextAngle(){return m_dTextAngle;};

	CString m_strTextAdd;

private:
	double m_dTextPosX;
	double m_dTextPosY;
	double m_dTextAngle;
	int m_nTextInflate;

};

class COutLineProp //外框属性
{
public:
	COutLineProp();
	virtual ~COutLineProp();
public:
	inline void setClockWise(int b) {m_bClockWise = b;};
	inline int getClockWise() {return m_bClockWise;};

	inline void setRoundLT(int b) {m_bLT = b;};
	inline int getRoundLT() {return m_bLT;};

	inline void setRoundLB(int b) {m_bLB = b;};
	inline int getRoundLB() {return m_bLB;};

	inline void setRoundRT(int b) {m_bRT = b;};
	inline int getRoundRT() {return m_bRT;};

	inline void setRoundRB(int b) {m_bRB = b;};
	inline int getRoundRB() {return m_bRB;};

	inline void setMmTop(double d) {m_dMmTop = d;};
	inline double getMmTop() {return m_dMmTop;};

	inline void setMmBot(double d) {m_dMmBot = d;};
	inline double getMmBot() {return m_dMmBot;};

	inline void setMmLeft(double d) {m_dMmLeft = d;};
	inline double getMmLeft() {return m_dMmLeft;};

	inline void setMmRight(double d) {m_dMmRight = d;};
	inline double getMmRight() {return m_dMmRight;};

	inline void setMmOutlineRadius(double d) {m_dMmOutlineRadius = d;};
	inline double getMmOutlineRadius() {return m_dMmOutlineRadius;};

	inline void setCutWidth(double d) {m_dCutWidth = d;};
	inline double getCutWidth() {return m_dCutWidth;};

	inline void setCutDepth(double d) {m_dCutDepth = d;};
	inline double getCutDepth() {return m_dCutDepth;};

	inline void setCutDist(double d) {m_dCutDist = d;};
	inline double getCutDist() {return m_dCutDist;};

	inline void setCutPos(int n) {m_nCutPos = n;};
	inline int getCutPos() {return m_nCutPos;};
	
private:
	int m_bLT; //左上倒圆角
	int m_bLB; //左下
	int m_bRT; //右上
	int m_bRB; //右下
	int m_bClockWise; //顺时针

	double m_dCutWidth;
	double m_dCutDepth;
	double m_dCutDist; //间隔
	int m_nCutPos; //0左边 1下边 2右边 3上边

	double m_dMmOutlineRadius;
	double m_dMmTop;
	double m_dMmBot;
	double m_dMmLeft;
	double m_dMmRight;
};

class CSettings
{
public:
	CSettings();
	virtual ~CSettings();

public: //切割参数
	inline void setSpdPlot(int n) {m_nSpdPlot = n;};
	inline int getSpdPlot() {return m_nSpdPlot;};

	inline void setSpdMilling(int n) {m_nSpdMilling = n;};
	inline int getSpdMilling() {return m_nSpdMilling;};

	inline void setSpdCut(int n) {m_nSpdCut = n;};
	inline int getSpdCut() {return m_nSpdCut;};

	inline void setSpdMove(int n){m_nSpdMove = n;};
	inline int getSpdMove() {return m_nSpdMove;};

	inline void setPwmPen(int n) {m_nPwmPen = n;};
	inline int getPwmPen() {return m_nPwmPen;};

	inline void setPwmCut(int n) {m_nPwmCut = n;};
	inline int getPwmCut() {return m_nPwmCut;};

	inline void setZPosMove(int n) {m_nZPosMove = n;};
	inline int getZPosMove() {return m_nZPosMove;};

	inline void setZPosWork(int n) {m_nZPosWork = n;};
	inline int getZPosWork() {return m_nZPosWork;};

	inline void setMPDistX(int n){ m_nMPDistX = n;}; //刀笔补偿(脉冲数)
	inline void setMPDistY(int n){ m_nMPDistY = n;};
	inline int getMPDistX(){ return m_nMPDistX;}; //刀笔补偿
	inline int getMPDistY(){ return m_nMPDistY;};

	inline void setKPDistX(int n){ m_nKPDistX = n;}; //刀笔补偿(脉冲数)
	inline void setKPDistY(int n){ m_nKPDistY = n;};
	inline int getKPDistX(){ return m_nKPDistX;}; //刀笔补偿
	inline int getKPDistY(){ return m_nKPDistY;};

	inline void setDrillerDimension(int n) {m_nDrillerDimension = n;};
	inline int getDrillerDimension() {return m_nDrillerDimension;};

	inline void setShrinkDist(double d) { m_dShrinkDist = d; };
	inline double getShrinkDist() { return m_dShrinkDist; };

	inline void setInflateDist(double d) { m_dInflateDist = d; };
	inline double getInflateDist() { return m_dInflateDist; };


	inline void setBreakAngle(int n) {m_nBreakAngle = n;};
	inline int getBreakAngle() {return m_nBreakAngle;};


	inline void setDrillDist(int n) {m_nDrillDist = n;};
	inline int getDrillDist() {return m_nDrillDist;};

	inline void setCurveLen(int n){ m_nCurveLen = n; };
	inline int getCurveLen() { return m_nCurveLen; };

	inline void setZLogic(int n){ m_nZLogic = n; };
	inline int getZLogic() { return m_nZLogic; };

	inline void setSlotWidth(double d) { m_dSlotWidth = d; };
	inline double getSlotWidth() { return m_dSlotWidth; };

public:
	inline void setAngleAdjust(int n){m_nAngleAdjust = n;};
	inline int getAngleAdjust(){return m_nAngleAdjust;};

	inline void set10000X(int n){ m_n10000X = n;}; //比例调整
	inline void set10000Y(int n){ m_n10000Y = n;};
	inline int get10000X(){return m_n10000X;};
	inline int get10000Y(){return m_n10000Y;};

	inline void setPPMMX(double d){ m_dPPMMX = d;};
	inline double getPPMMX(){return m_dPPMMX;};
	inline void setPPMMY(double d){ m_dPPMMY = d;};
	inline double getPPMMY(){return m_dPPMMY;};
	inline void setPPMMZ(double d){ m_dPPMMZ = d;};
	inline double getPPMMZ(){return m_dPPMMZ;};

	inline void setMacSizeX(int n){ m_nMacSizeXmm = n;};
	inline int getMacSizeX(){return m_nMacSizeXmm;};
	inline void setMacSizeY(int n){ m_nMacSizeYmm = n;};
	inline int getMacSizeY(){return m_nMacSizeYmm;};

	inline void setMcuCode(int n){m_nMcuCode = n;};
	inline int getMcuCode(){return m_nMcuCode;};

	void getDirectorys();
	void checkTempDir();

	BOOL getQueryEnable(){return m_bQueryEnable;};
	void setQueryEnable(BOOL b){m_bQueryEnable = b;Sleep(10);};

	BOOL getAppExitFlag(){return m_bAppExitFlag;}
	void setAppExitFlag(BOOL b){m_bAppExitFlag = b;};

	BOOL getDemoEndFlag() { return m_bDemoEndFlag; }
	void setDemoEndFlag(BOOL b) { m_bDemoEndFlag = b; };

	BOOL getVmEnable(){return m_bVMEnable;}; //开始工作时是否自动开启真空泵
	void setVmEnable(BOOL b){m_bVMEnable = b;};

	int getMMotorOnDelay(){return m_nMMOnDelay;};
	void setMMotorOnDelay(int n){m_nMMOnDelay = n;};

	BOOL getCutAllInnerLineFirst(){return m_bCutAllInnerLineFirst;};
	void setCutAllInnerLineFirst(BOOL b){m_bCutAllInnerLineFirst = b;};	
	
public:
	inline CString getMcuSn(){return m_strMcuSn;};
	inline void setMcuSn(CString str){m_strMcuSn = str;};

	inline CString getAuthCode(){return m_strAuthCode;};
	inline void setAuthCode(CString str){m_strAuthCode = str;};

	inline int getExpandBefore(){return m_nExpandBefore;};
	inline void setExpandBefore(int n){m_nExpandBefore = n;};

	inline int getExpandAfter(){return m_nExpandAfter;};
	inline void setExpandAfter(int n){m_nExpandAfter = n;};

	int getPltExportPenNum(){return m_nPltExportPenNum;};
	void setPltExportPenNum(int n){m_nPltExportPenNum = n;};

	int getPltExportCutNum(){return m_nPltExportCutNum;};
	void setPltExportCutNum(int n){m_nPltExportCutNum = n;};

	int getPltExportDrillNum(){return m_nPltExportDrillNum;};
	void setPltExportDrillNum(int n){m_nPltExportDrillNum = n;};

	inline void setDispStartX(int n){m_nDispStartX = n;};
	inline int getDispStartX(){return m_nDispStartX;};
	inline void setDispStartY(int n){m_nDispStartY = n;};
	inline int getDispStartY(){return m_nDispStartY;};

	inline void setScrXPixel(int n){m_nScrXPixel = n;};
	inline int getScrXPixel(){return m_nScrXPixel;};

	inline void setScrYPixel(int n){m_nScrYPixel = n;};
	inline int getScrYPixel(){return m_nScrYPixel;};	

	inline void setDispScale(double d){m_dDispScale = d;};
	inline double getDispScale(){return m_dDispScale;};

	inline void setDispSizeX(int n){m_nDispSizeXmm = n;};
	inline int getDispSizeX() { return m_nDispSizeXmm; };
	
	inline void setDispSizeY(int n){m_nDispSizeYmm = n;};
	inline int getDispSizeY() { return m_nDispSizeYmm; };

	inline void setWorkStartX(int n){m_nWorkStartX = n;};
	inline int getWorkStartX(){return m_nWorkStartX;};

	inline void setWorkStartY(int n){m_nWorkStartY = n;};
	inline int getWorkStartY(){return m_nWorkStartY;};

public:
	CString m_strTempDir;
	CString m_strAppDir;
	CString m_strAppName;
	CString m_strFonHz;
	CString m_strFonEn;

private:
	double m_dDispScale;
	int m_nScrXPixel;
	int m_nScrYPixel;
	int m_nDispSizeXmm;
	int m_nDispSizeYmm;
	int m_nDispStartX;
	int m_nDispStartY;

	int m_nWorkStartX;
	int m_nWorkStartY;

private:
	int m_nCurveLen;
	int m_nExpandBefore;
	int m_nExpandAfter;
	
	int m_nDrillerDimension; //铣刀直径
	int m_nBreakAngle;
	int m_nScaleLevel;
	int m_nMcuCode;

	int m_nZLogic;
	CString m_strMcuSn;
	CString m_strAuthCode;

	BOOL m_bQueryEnable;
	BOOL m_bSuperUserOn;

private:
	double m_dSlotWidth;
	double m_dShrinkDist;
	double m_dInflateDist;

	double m_dPPMMX;
	double m_dPPMMY;
	double m_dPPMMZ;

	int m_nMacSizeXmm;
	int m_nMacSizeYmm;

	int m_nMPDistX; //刀笔补偿
	int m_nMPDistY;
	int m_nKPDistX; //刀笔补偿
	int m_nKPDistY;

	int m_n10000X; //比例调整
	int m_n10000Y;

	int m_nSpdPlot;
	int m_nSpdMilling;
	int m_nSpdCut;
	int m_nSpdMove;

	int m_nAngleAdjust;
	int m_nZPosMove;
	int m_nZPosWork;
	int m_nPwmPen; //笔压
	int m_nPwmCut;
	int m_nMMOnDelay;

	int m_nPltExportPenNum;
	int m_nPltExportCutNum;
	int m_nPltExportDrillNum;

	BOOL m_bVMEnable;
	BOOL m_bAppExitFlag;
	BOOL m_bDemoEndFlag;
	BOOL m_bCutAllInnerLineFirst;
	int m_nDrillDist;
};
#endif // !defined(AFX_SETTINGS_H__7C86702C_BD84_45CD_BE7C_10999ABBB7A2__INCLUDED_)
