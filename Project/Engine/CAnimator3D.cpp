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


void CAnimator3D::ChangeAnimation(wstring _AniKey)
{
	CAnimation3D* pAnimation3D = m_mapAnimation[_AniKey];
	if (pAnimation3D == nullptr)
		return;
	m_pCurAnimation = pAnimation3D;
}

void CAnimator3D::finaltick()
{
	if (m_pController == nullptr)
		return;
	bool bFinish = false;
	bool bLoop = false;

	if (m_pCurAnimation)
	{
		bFinish = m_pCurAnimation->IsFinish();
		bLoop = m_pCurAnimation->IsLoop();
	}

	ANI_NODE_RETURN eType = m_pController->NextNode(bFinish, bLoop);

	switch (eType)
	{
	case ANI_NODE_RETURN::RESET:
		if (m_pCurAnimation)
			m_pCurAnimation->Reset();
		break;
	case ANI_NODE_RETURN::CHANGE:
		if (m_pCurAnimation)
			m_pCurAnimation->Reset();
		ChangeAnimation(m_pController->GetCurAniKey());
		break;
	}

	if (m_pCurAnimation)
		m_pCurAnimation->finaltick();
}

void CAnimator3D::UpdateData()
{
	if (m_pController == nullptr)
		return;

	m_pCurAnimation->UpdateData(m_pBoneFinalMatBuffer);

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
	return;
}

void CAnimator3D::SetController(Ptr<CAnimatorController> _pController)
{
	m_pController = _pController;
}

void CAnimator3D::SetController(wstring _strName)
{
	m_pController = CResMgr::GetInst()->FindRes<CAnimatorController>(_strName);
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