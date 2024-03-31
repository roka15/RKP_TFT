#include "pch.h"
#include "CAnimation3D.h"
#include "CAnimator3D.h"
#include "CResMgr.h"
#include "CTimeMgr.h"
#include "AniNode.h"
#include "CStructuredBuffer.h"
#include "CAnimation3DShader.h"
CAnimation3D::CAnimation3D() :
	m_dCurTime(0.)
	, m_iFrameCount(24)
	, m_bFinalMatUpdate(false)
	, m_bFinish(false)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
{
}
CAnimation3D::CAnimation3D(const CAnimation3D& _ref):
	  m_dCurTime(0.)
	, m_iFrameCount(_ref.m_iFrameCount)
	, m_bFinalMatUpdate(false)
	, m_bFinish(false)
	, m_iFrameIdx(0) 
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_pClip(_ref.m_pClip)
{
	m_bEvents.resize(_ref.m_bEvents.size(), false);
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
		m_iFrameCount = 23;
		break;
	case fbxsdk::FbxTime::EMode::eFrames30:
		m_iFrameCount = 30;
		break;
	}
	int size = m_pClip->m_Events.size();
	m_bEvents.resize(size, false);
}
CAnimation3D::~CAnimation3D()
{
}
void CAnimation3D::check_bone(CStructuredBuffer*& _finalMat)
{
	UINT iBoneCount =m_pClip->GetBoneCount();
	if (_finalMat->GetElementCount() != iBoneCount)
	{
		_finalMat->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
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

	int iEventSize = m_pClip->m_Events.size();
	if (m_iFrameIdx < 0 || m_iFrameIdx >= iEventSize)
		return;

	if (m_bEvents[m_iFrameIdx])
		return;
	t_AniEventPoint* point = m_pClip->m_Events[m_iFrameIdx];
	if (point == nullptr)
		return;

	string strKey = point->UIFunction;
	wstring wstrKey;
	wstrKey.assign(strKey.begin(), strKey.end());
	CAnimator3D* pAnimator3D = m_pOwner;
	
	auto voidFunc = pAnimator3D->GetVOID_EventFunc(wstrKey);
	if (voidFunc.has_value())
	{
		std::function<void()>& FuncRef = voidFunc.value().get();
		FuncRef();
	}
	auto floatFunc = pAnimator3D->GetFLOAT_EventFunc(wstrKey);
	if (floatFunc.has_value())
	{
		std::function<void(float)>& FuncRef = floatFunc.value().get();
		FuncRef(point->Float);
	}
	auto intFunc = pAnimator3D->GetINT_EventFunc(wstrKey);
	if (intFunc.has_value())
	{
		std::function<void(int)>& FuncRef = intFunc.value().get();
		FuncRef(point->Int);
	}
	auto stringFunc = pAnimator3D->GetSTRING_EventFunc(wstrKey);
	if (stringFunc.has_value())
	{
		std::function<void(string)>& FuncRef = stringFunc.value().get();
		FuncRef(point->String);
	}
	auto objFunc = pAnimator3D->GetOBJ_EventFunc(wstrKey);
	if (objFunc.has_value())
	{
		std::function<void(CGameObject*)>& FuncRef = objFunc.value().get();
		FuncRef(point->Obj);
	}
	m_bEvents[m_iFrameIdx] = true;

}
void CAnimation3D::UpdateData(CStructuredBuffer*& _bonMat)
{
	if (m_bFinalMatUpdate == false)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();
		check_bone(_bonMat);
		pUpdateShader->SetFrameDataBuffer(m_pClip->GetBoneFrameBuffer());
		pUpdateShader->SetOffsetMatBuffer(m_pClip->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(_bonMat);

		UINT iBoneCount = m_pClip->GetBoneCount();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		//pUpdateShader->SetBlendingFlag(m_bBlending);
		pUpdateShader->SetFrameRatio(m_fRatio);

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();

		m_bFinalMatUpdate = true;
	}
}
void CAnimation3D::Reset()
{
	m_dCurTime = 0.0;
	m_dUpdateTime = 0.0;
	switch (m_pClip->m_tInfo.eMode)
	{
	case fbxsdk::FbxTime::EMode::eFrames24:
		m_iFrameCount = 23;
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
	int size = m_bEvents.size();
	for (int i = 0; i != m_bEvents.size(); ++i)
		m_bEvents[i] = false;
}
void CAnimation3D::SetClip(const std::wstring& _strKey)
{
	m_pClip = CResMgr::GetInst()->FindRes<CAniClip>(_strKey);
	SetName(_strKey);
	Reset();
}

