#ifndef _CNC_H_H_
#define _CNC_H_H_

void CalculateAngles();
void GetLongAxialSteps();
void DumpDataToFile();
void ReleaseCncDataBuffer();
void GetCncMaxPulse();
void CalcCurvature();
void CalcCncSpeed();
void CalcSdPercent();
int SendCncData(void);

#endif
