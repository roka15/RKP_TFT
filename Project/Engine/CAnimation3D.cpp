#include "pch.h"
#include "CAnimation3D.h"
#include "CAnimator3D.h"
#include "CResMgr.h"
#include "CTimeMgr.h"

CAnimation3D::CAnimation3D() :
	m_dCurTime(0.)
	, m_iFrameCount(30)
	, m_bFinalMatUpdate(false)
	, m_bFinish(false)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
{
}
CAnimation3D::CAnimation3D(Ptr<CAniClip> _clip) :
	m_pClip(_clip)
	, m_dCurTime(0.)
	, m_iFrameCount(0)
	, m_bFinalMatUpdate(false)
	, m_bFinish(false)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
{
	SetName(_clip->GetName());
	switch (_clip->m_tInfo.eMode)
	{
	case fbxsdk::FbxTime::EMode::eFrames24:
		m_iFrameCount = 21;
		break;
	case fbxsdk::FbxTime::EMode::eFrames30:
		m_iFrameCount = 30;
		break;
	}
}
CAnimation3D::~CAnimation3D()
{
}
void CAnimation3D::finaltick()
{
	if (m_bFinish)
		return;
	m_dCurTime = 0.f;
	m_dUpdateTime += DT;

	if (m_dUpdateTime >= m_pClip->m_tInfo.dTimeLength)
	{
		m_dUpdateTime = 0.f;
		m_bFinish = true;
	}

	m_dCurTime = m_pClip->m_tInfo.dStartTime + m_dUpdateTime;

	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)dFrameIdx;

	if (m_iFrameIdx >= m_pClip->m_tInfo.iFrameLength - 1)
	{
		m_iNextFrameIdx = m_iFrameIdx;
	}
	else
	{
		m_iNextFrameIdx = m_iFrameIdx + 1;
	}

	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

	m_bFinalMatUpdate = false;
}
void CAnimation3D::Reset()
{
	m_dCurTime = 0.f;
	switch (m_pClip->m_tInfo.eMode)
	{
	case fbxsdk::FbxTime::EMode::eFrames24:
		m_iFrameCount = 21;
		break;
	case fbxsdk::FbxTime::EMode::eFrames30:
		m_iFrameCount = 30;
		break;
	}
	m_bFinalMatUpdate = false;
	m_iFrameIdx = 0;
	m_iNextFrameIdx = 0;
	m_fRatio = 0.f;
	m_bFinish = false;
}
void CAnimation3D::SetClip(const std::wstring& _strKey)
{
	m_pClip = CResMgr::GetInst()->FindRes<CAniClip>(_strKey);
	SetName(_strKey);
	Reset();
}

