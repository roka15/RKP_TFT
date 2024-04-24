#include "pch.h"

#include "CAnimator3D.h"
#include "CAnimation3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	:m_pBoneFinalMatBuffer(nullptr)
	, m_iCurIdx(0)
	, m_bBlending(false)
	, m_fBlendingTime(0.f)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pBoneFinalMatBuffer(nullptr)
	, m_iCurIdx(0)
	, m_bBlending(false)
	, m_fBlendingTime(0.f)
	, m_pController(_origin.m_pController)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
	, m_pCurAnimation(nullptr)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
	m_pController->CopyParams(this);
	for (auto itr = _origin.m_mapAnimation.begin(); itr != _origin.m_mapAnimation.end(); ++itr)
	{
		wstring Key = itr->first;
		CAnimation3D* Value = itr->second;
		CAnimation3D* newValue = new CAnimation3D(*Value);
		newValue->SetOwner(this);
		m_mapAnimation.insert(std::make_pair(Key, newValue));
	}
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;
}


bool CAnimator3D::ChangeAnimation(wstring _AniKey, float _fBlendTime)
{
	CAnimation3D* pAnimation3D = m_mapAnimation[_AniKey];
	if (pAnimation3D == nullptr)
		return false;
	if (pAnimation3D == m_pCurAnimation)
		return false;
	if (pAnimation3D == m_pNextAnimation)
		return false;
	m_pNextAnimation = pAnimation3D;
	m_fBlendingTime = _fBlendTime;
	return true;
}


void CAnimator3D::finaltick()
{
	if (m_pController == nullptr)
		return;
	bool bFinish = false;
	bool bLoop = false;
	ANI_NODE_RETURN eType;
	if (m_bBlending)
	{
		m_fCurTime += DT;
	}
	else
	{
		if (m_pCurAnimation)
		{
			bFinish = m_pCurAnimation->IsFinish();
			bLoop = m_pCurAnimation->IsLoop();

			wstring CurAniName = m_pCurAnimation->GetClip()->GetKey();
			ANI_NODE_RETURN eType = m_pController->NextNode(bFinish, bLoop, CurAniName, this);

			while (eType == ANI_NODE_RETURN::ENTRY || eType == ANI_NODE_RETURN::EXIT)
			{
				switch (eType)
				{
				case ANI_NODE_RETURN::ENTRY:
					eType = m_pController->NextNode(bFinish, bLoop, L"Entry", this);
					break;
				case ANI_NODE_RETURN::EXIT:
					eType = m_pController->NextNode(bFinish, bLoop, L"Exit", this);
					break;
				}

			}

			switch (eType)
			{
			case ANI_NODE_RETURN::CHANGE:
				m_bBlending = true;
				break;
			case ANI_NODE_RETURN::RESET:
				if (m_pCurAnimation)
					m_pCurAnimation->Reset();
				break;
			}
		}//현재 설정된 Ani가 없다면 Entry.
		else
		{
			eType = m_pController->NextNode(bFinish, bLoop, L"Entry", this);
			m_pCurAnimation = m_pNextAnimation;
		}

		if (m_pCurAnimation)
			m_pCurAnimation->finaltick();
	}
}

void CAnimator3D::UpdateData()
{
	if (m_pController == nullptr)
		return;

	if (m_pCurAnimation)
	{
		wstring strNextClip;
		if (m_pNextAnimation)
			strNextClip = m_pNextAnimation->GetClip()->GetKey();
		m_pCurAnimation->UpdateData(m_pBoneFinalMatBuffer, strNextClip);

		// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
		m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
	}
	return;
}

void CAnimator3D::SetController(Ptr<CAnimatorController> _pController)
{
	m_pController = _pController;
	m_pController->CopyParams(this);
}

void CAnimator3D::SetController(wstring _strName)
{
	m_pController = CResMgr::GetInst()->FindRes<CAnimatorController>(_strName);
	m_pController->CopyParams(this);
}
const wstring& CAnimator3D::GetCurControllerName()
{
	return m_pController->GetKey();
}

void CAnimator3D::RegisterAnimation(wstring _AniClipName)
{
	Ptr<CAniClip> pAniClip = CResMgr::GetInst()->FindRes<CAniClip>(_AniClipName);
	CAnimation3D* pAnimation = new CAnimation3D(pAniClip);
	auto itr = m_mapAnimation.find(_AniClipName);
	if (itr != m_mapAnimation.end())
		return;
	pAnimation->SetOwner(this);
	m_mapAnimation.insert(std::make_pair(_AniClipName, pAnimation));
}
bool CAnimator3D::SetIntParam(wstring _strName, int _iValue)
{
	auto itr = m_AniParams.mapIntParams.find(_strName);
	if (itr == m_AniParams.mapIntParams.end())
	{
		return false;
	}
	itr->second = _iValue;
	return true;
}
bool CAnimator3D::SetFloatParam(wstring _strName, float _fValue)
{
	auto itr = m_AniParams.mapFloatParams.find(_strName);
	if (itr == m_AniParams.mapFloatParams.end())
	{
		return false;
	}
	itr->second = _fValue;
	return true;
}
bool CAnimator3D::SetTriggerParam(wstring _strName, bool _bValue)
{
	auto itr = m_AniParams.mapTriggerParams.find(_strName);
	if (itr == m_AniParams.mapTriggerParams.end())
	{
		return false;
	}
	itr->second = _bValue;
	return true;
}
bool CAnimator3D::SetBoolParam(wstring _strName, bool _bValue)
{
	auto itr = m_AniParams.mapBoolParams.find(_strName);
	if (itr == m_AniParams.mapBoolParams.end())
	{
		return false;
	}
	if (_strName == L"Death")
		int a = 0;
	itr->second = _bValue;
	return true;
}
const int& CAnimator3D::GetIntParam(wstring _strName)
{
	auto itr = m_AniParams.mapIntParams.find(_strName);
	if (itr == m_AniParams.mapIntParams.end())
	{
		assert(nullptr);
	}
	return m_AniParams.mapIntParams[_strName];
}
const float& CAnimator3D::GetFloatParam(wstring _strName)
{
	auto itr = m_AniParams.mapFloatParams.find(_strName);
	if (itr == m_AniParams.mapFloatParams.end())
	{
		assert(nullptr);
	}
	return m_AniParams.mapFloatParams[_strName];
}

const bool& CAnimator3D::GetBoolParam(wstring _strName)
{
	auto itr = m_AniParams.mapBoolParams.find(_strName);
	if (itr == m_AniParams.mapBoolParams.end())
	{
		assert(nullptr);
	}
	return m_AniParams.mapBoolParams[_strName];
}
const bool& CAnimator3D::GetTriggerParam(wstring _strName)
{
	auto itr = m_AniParams.mapTriggerParams.find(_strName);
	if (itr == m_AniParams.mapTriggerParams.end())
	{
		assert(nullptr);
	}
	return m_AniParams.mapTriggerParams[_strName];
}
UINT CAnimator3D::GetBoneCount()
{
	return m_pCurAnimation->GetBoneCount();
}
void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	if (GetOwner()->MeshRender() == nullptr)
		return;
	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

std::vector<wstring> CAnimator3D::GetAniList()
{
	std::vector<wstring> result;
	for (auto itr = m_mapAnimation.begin(); itr != m_mapAnimation.end(); ++itr)
	{
		result.push_back(itr->first);
	}
	return result;
}

void CAnimator3D::BlendingEnd()
{
	m_bBlending = false;
	m_fCurTime = 0.f;
	m_pCurAnimation->Reset();
	m_pCurAnimation = m_pNextAnimation;
	m_pNextAnimation = nullptr;
}

void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
}