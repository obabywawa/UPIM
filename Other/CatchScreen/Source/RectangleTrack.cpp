/**
*  RectangleTrack.cpp		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2000-2001 . All Rights Reserved.
*   定义矩形轨迹类
*======================================================
*   版本
*   Revision 1.0	2002/11/12    	 实现 CRectangleTrack 类
*/
#include "stdafx.h"
#include "..\Include\RectangleTrack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRectangleTrack::CRectangleTrack(HDC hDC, COLORREF crTrackColor) : CTrack(hDC, crTrackColor)
{
	m_strTrackName = _T("矩形");
}

CRectangleTrack::~CRectangleTrack()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名	: Draw
*  功能		: 画出矩形
*  参数		:	CPoint& ptStart	-	矩形左上角座标
*				CPoint& ptEnd	-	矩形右下角座标
*  算法		: 直接画矩形
*--------------------------------------------------------------------------------
*/
void CRectangleTrack::Draw(const CPoint &ptStart, const CPoint &ptEnd) const
{
	HDC hDC = m_hDC;
	if (ptStart.x == ptEnd.x && ptStart.y == ptEnd.y)
	{
		// 起点和终点相同，就画一个点
		SetPixel(hDC, ptStart.x, ptEnd.y, TRACK_PEN_COLOR);
		return;
	}
	if (ptStart.y == ptEnd.y && ptStart.x == ptEnd.x)
	{
		// 水平线或是垂直线
		MoveToEx(hDC, ptStart.x, ptEnd.y, NULL);
		LineTo(hDC, ptEnd.x, ptEnd.y);
	}
	else
	{
		Rectangle(hDC, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}
}


/*
*--------------------------------------------------------------------------------
*  函数名	: CreatePos
*  功能		: 计算出轨迹上所有点的集合，保存到列表中去
*  参数		: VOID
*  算法		: 通过 LineDDA 函数得出所有的轨迹点座标
*  前置条件 : 必须已经设定了轨迹起始点座标和终止点座标
*  后置条件 : 在 m_listTrackPoints 列表中保存所有轨迹点的座标
*--------------------------------------------------------------------------------
*/
BOOL CRectangleTrack::CreatePos()
{
	if (m_ptTrackStartPoint.y == m_ptTrackEndPoint.y && m_ptTrackStartPoint.x == m_ptTrackEndPoint.x)
	{
		// 水平线或是垂直线
		LineDDA(m_ptTrackStartPoint.x, m_ptTrackStartPoint.y, m_ptTrackEndPoint.x, m_ptTrackEndPoint.y, CTrack::AddPosToList, (LPARAM) this);
	}
	else
	{
		CPoint ptVertex[4];
		
		ptVertex[0] = m_ptTrackStartPoint;
		ptVertex[1].x = m_ptTrackEndPoint.x;
		ptVertex[1].y = m_ptTrackStartPoint.y;
		ptVertex[2] = m_ptTrackEndPoint;
		ptVertex[3].x = m_ptTrackStartPoint.x;
		ptVertex[3].y = m_ptTrackEndPoint.y;
				
		// 将矩形的四个顶点依次连线用 LineDDA 得到轨迹点
		LineDDA(ptVertex[0].x, ptVertex[0].y, ptVertex[1].x, ptVertex[1].y, CTrack::AddPosToList, (LPARAM) this);
		LineDDA(ptVertex[1].x, ptVertex[1].y, ptVertex[2].x, ptVertex[2].y, CTrack::AddPosToList, (LPARAM) this);
		LineDDA(ptVertex[2].x, ptVertex[2].y, ptVertex[3].x, ptVertex[3].y, CTrack::AddPosToList, (LPARAM) this);
		LineDDA(ptVertex[3].x, ptVertex[3].y, ptVertex[0].x, ptVertex[0].y, CTrack::AddPosToList, (LPARAM) this);
	}
	return TRUE;
}
