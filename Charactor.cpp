// Charactor.cpp: implementation of the CCharactor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MCAD.h"
#include "Charactor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharactor::CCharactor()
{
	m_wCharCode=0;
	m_wBytes=0;
	m_dwPosition=0;
	m_wWidth=0;
}

CCharactor::~CCharactor()
{

}
