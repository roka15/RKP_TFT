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
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pBoneFinalMatBuffer(nullptr)
	, m_iCurIdx(0)
	, m_bBlending(false)
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


bool CAnimator3D::ChangeAnimation(wstring _AniKey)
{
	CAnimation3D* pAnimation3D = m_mapAnimation[_AniKey];
	if (pAnimation3D == nullptr)
		return false;
	if (pAnimation3D == m_pCurAnimation)
		return false;
	m_pCurAnimation = pAnimation3D;
	return true;
}


void CAnimator3D::finaltick()
{
	if (m_pController == nullptr)
		return;
	bool bFinish = false;
	bool bLoop = false;
	ANI_NODE_RETURN eType;
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
		case ANI_NODE_RETURN::RESET:
		case ANI_NODE_RETURN::CHANGE:
			if (m_pCurAnimation)
				m_pCurAnimation->Reset();
			break;
		}
	}//현재 설정된 Ani가 없다면 Entry.
	else
	{
		eType = m_pController->NextNode(bFinish, bLoop, L"Entry", this);
	}

	if (m_pCurAnimation)
		m_pCurAnimation->finaltick();
}

void CAnimator3D::UpdateData()
{
	if (m_pController == nullptr)
		return;

	if (m_pCurAnimation)
	{
		m_pCurAnimation->UpdateData(m_pBoneFinalMatBuffer);

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
	return m_pController->GetName();
}
void CAnimator3D::RegisterAniEventInfoVOID(wstring _Key, std::function<void()> _Func)
{
	map<wstring, std::function<void()>>& mapVoidEvent = m_AniEvent.m_mapVoid;
	auto itr = mapVoidEvent.find(_Key);
	if (itr != mapVoidEvent.end())
		return;
	mapVoidEvent.insert(std::make_pair(_Key, _Func));
}
void CAnimator3D::RegisterAniEventInfoINT(wstring _Key, std::function<void(int)> _Func)
{
	map<wstring, std::function<void(int)>>& mapINTEvent = m_AniEvent.m_mapInt;
	auto itr = mapINTEvent.find(_Key);
	if (itr != mapINTEvent.end())
		return;
	mapINTEvent.insert(std::make_pair(_Key, _Func));
}
vector<wstring> CAnimator3D::GetAniEventList()
{
	vector<wstring> vecKeys;
	map<wstring, std::function<void()>>& mapVoidEvent = m_AniEvent.m_mapVoid;
	for (auto itr = mapVoidEvent.begin(); itr != mapVoidEvent.end(); ++itr)
	{
		wstring Key = itr->first;
		vecKeys.push_back(Key);
	}
	map<wstring, std::function<void(float)>>& mapFloatEvent = m_AniEvent.m_mapFloat;
	for (auto itr = mapFloatEvent.begin(); itr != mapFloatEvent.end(); ++itr)
	{
		wstring Key = itr->first;
		vecKeys.push_back(Key);
	}
	map<wstring, std::function<void(int)>>& mapIntEvent = m_AniEvent.m_mapInt;
	for (auto itr = mapIntEvent.begin(); itr != mapIntEvent.end(); ++itr)
	{
		wstring Key = itr->first;
		vecKeys.push_back(Key);
	}
	map<wstring, std::function<void(string)>>& mapStringEvent = m_AniEvent.m_mapString;
	for (auto itr = mapStringEvent.begin(); itr != mapStringEvent.end(); ++itr)
	{
		wstring Key = itr->first;
		vecKeys.push_back(Key);
	}
	map<wstring, std::function<void(CGameObject*)>>& mapObjEvent = m_AniEvent.m_mapObj;
	for (auto itr = mapObjEvent.begin(); itr != mapObjEvent.end(); ++itr)
	{
		wstring Key = itr->first;
		vecKeys.push_back(Key);
	}
	return vecKeys;
}
std::optional<std::reference_wrapper<std::function<void()>>> CAnimator3D::GetVOID_EventFunc(wstring _Key)
{
	map<wstring, std::function<void()>>& mapVoidEvent = m_AniEvent.m_mapVoid;
	auto itr = mapVoidEvent.find(_Key);
	if (itr == mapVoidEvent.end())
		return std::nullopt;
	return std::ref(mapVoidEvent[_Key]);
}
std::optional<std::reference_wrapper<std::function<void(float)>>> CAnimator3D::GetFLOAT_EventFunc(wstring _Key)
{
	map<wstring, std::function<void(float)>>& mapFloatEvent = m_AniEvent.m_mapFloat;
	auto itr = mapFloatEvent.find(_Key);
	if (itr == mapFloatEvent.end())
		return std::nullopt;
	return std::ref(mapFloatEvent[_Key]);
}
std::optional<std::reference_wrapper<std::function<void(int)>>>  CAnimator3D::GetINT_EventFunc(wstring _Key)
{
	map<wstring, std::function<void(int)>>& mapIntEvent = m_AniEvent.m_mapInt;
	auto itr = mapIntEvent.find(_Key);
	if (itr == mapIntEvent.end())
		return std::nullopt;
	return std::ref(mapIntEvent[_Key]);
}
std::optional<std::reference_wrapper<std::function<void(string)>>> CAnimator3D::GetSTRING_EventFunc(wstring _Key)
{
	map<wstring, std::function<void(string)>>& mapStringEvent = m_AniEvent.m_mapString;
	auto itr = mapStringEvent.find(_Key);
	if (itr == mapStringEvent.end())
		return std::nullopt;
	return std::ref(mapStringEvent[_Key]);
}
std::optional<std::reference_wrapper<std::function<void(CGameObject*)>>> CAnimator3D::GetOBJ_EventFunc(wstring _Key)
{
	map<wstring, std::function<void(CGameObject*)>>& mapObjEvent = m_AniEvent.m_mapObj;
	auto itr = mapObjEvent.find(_Key);
	if (itr == mapObjEvent.end())
		return std::nullopt;
	return std::ref(mapObjEvent[_Key]);
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
	itr->second = _bValue;
	return true;
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

void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
}