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
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;
}


void CAnimator3D::finaltick()
{
	if (m_pController == nullptr)
		return;

	m_pController->finaltick();
}

void CAnimator3D::UpdateData()
{
	if (m_pController == nullptr)
		return;

	m_pController->UpdateData(m_pBoneFinalMatBuffer);

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
	return;
}

void CAnimator3D::SetController(Ptr<CAnimatorController> _pController)
{
	m_pController = _pController; 
	m_pController->SetAnimator(this);
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
	if (itr == mapVoidEvent.end())
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
std::function<void()>& CAnimator3D::GetVOID_EventFunc(wstring _Key)
{
	map<wstring, std::function<void()>>& mapVoidEvent = m_AniEvent.m_mapVoid;
	return mapVoidEvent[_Key];
}
std::function<void(float)>& CAnimator3D::GetFLOAT_EventFunc(wstring _Key)
{
	map<wstring, std::function<void(float)>>& mapFloatEvent = m_AniEvent.m_mapFloat;
	return mapFloatEvent[_Key];
}
std::function<void(int)>& CAnimator3D::GetINT_EventFunc(wstring _Key)
{
	map<wstring, std::function<void(int)>>& mapIntEvent = m_AniEvent.m_mapInt;
	return mapIntEvent[_Key];
}
std::function<void(string)>& CAnimator3D::GetSTRING_EventFunc(wstring _Key)
{
	map<wstring, std::function<void(string)>>& mapStringEvent = m_AniEvent.m_mapString;
	return mapStringEvent[_Key];
}
std::function<void(CGameObject*)>& CAnimator3D::GetOBJ_EventFunc(wstring _Key)
{
	map<wstring, std::function<void(CGameObject*)>>& mapObjEvent = m_AniEvent.m_mapObj;
	return mapObjEvent[_Key];
}
UINT CAnimator3D::GetBoneCount()
{
	return m_pController->GetBoneCount();
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