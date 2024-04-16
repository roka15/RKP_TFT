#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CMeshRender.h"

#include "CScript.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CRenderComponent.h"
#include "CKeyMgr.h"
#include "CEventMgr.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_LifeTime(0.f)
	, m_CurLifeTime(0.f)
	, m_bLifeSpan(false)
{
}

CGameObject::CGameObject(const CGameObject& _Other)
	: CEntity(_Other)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_LifeTime(0.f)
	, m_CurLifeTime(0.f)
	, m_bLifeSpan(false)
{
	// Component 복사
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (_Other.m_arrCom[i])
		{
			AddComponent(_Other.m_arrCom[i]->Clone());
		}
	}

	// Script 복사
	for (size_t i = 0; i < _Other.m_vecScript.size(); ++i)
	{
		AddComponent(_Other.m_vecScript[i]->Clone());
	}

	// 자식 오브젝트 복사
	for (size_t i = 0; i < _Other.m_vecChild.size(); ++i)
	{
		AddChild(_Other.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
	Safe_Del_Vec(m_vecScript);
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->begin();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

void CGameObject::tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (GetName().compare(L"Zed") == 0)
			int a = 0;
		m_vecScript[i]->tick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	if (m_bLifeSpan)
	{
		m_CurLifeTime += DT;
		if (m_LifeTime < m_CurLifeTime)
		{
			DestroyObject(this);
		}
	}

	// 소속 레이어가 없는데 finaltick 이 호출되었다.
	assert(-1 != m_iLayerIdx);

	// 레이어 등록
	CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterObject(this);

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}
}

void CGameObject::finaltick_module()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick_module();
	}
}

void CGameObject::render()
{
	if (nullptr != m_RenderCom)
		m_RenderCom->render();
}

void CGameObject::render_shadowmap()
{
	if (nullptr != m_RenderCom)
		m_RenderCom->render_shadowmap();
}

void CGameObject::AddComponent(CComponent* _Component)
{
	_Component->m_pOwner = this;

	// 컴포넌트가 스크립트인 경우
	if (COMPONENT_TYPE::SCRIPT == _Component->GetType())
	{
		((CScript*)_Component)->start();
		m_vecScript.push_back((CScript*)_Component);
	}

	// 스크립트를 제외한 일반 컴포넌트인 경우
	else
	{
		// 이미 보유하고 있는 컴포넌트인 경우
		assert(!m_arrCom[(UINT)_Component->GetType()]);
		m_arrCom[(UINT)_Component->GetType()] = _Component;

		// RenderComponent 확인
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);
		if (pRenderCom)
		{
			// 이미 1개 이상의 렌더컴포넌트를 보유하고 있다면 assert
			assert(!m_RenderCom);
			m_RenderCom = pRenderCom;
		}
	}
}

void CGameObject::AddChild(CGameObject* _Object)
{
	if (_Object->m_Parent)
	{
		// 기존 부모가 있으면 연결 해제 후 연결
		_Object->DisconnectFromParent();
		tEvent evn = {};

		evn.Type = EVENT_TYPE::CHANGE_ADD_CHILD;
		CEventMgr::GetInst()->AddEvent(evn);
	}

	else
	{
		// 기존 부모가 없으면, 소속 레이어에서 최상위부모 목록에서 제거된 후 연결
		_Object->ChangeToChildType();
	}

	// 부모 자식 연결
	_Object->m_Parent = this;

	//_Object->ChangeLayerIdx(m_iLayerIdx);

	m_vecChild.push_back(_Object);
}

CGameObject* CGameObject::GetChild(wstring _strName)
{
	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->GetName().compare(_strName) == 0)
			return m_vecChild[i];
		CGameObject* pObj = m_vecChild[i]->GetChild(_strName);
		if (pObj != nullptr)
			return pObj;
	}

	return nullptr;
}


bool CGameObject::IsAncestor(CGameObject* _Target)
{
	CGameObject* pParent = m_Parent;
	while (pParent)
	{
		if (pParent == _Target)
		{
			return true;
		}
		pParent = pParent->m_Parent;
	}

	return false;
}

void CGameObject::RegisterFucnPtrVOID(wstring _strFunc, std::function<void()> _pFunc)
{
	auto itr = m_FuncPtr.m_mapVoid.find(_strFunc);
	if (itr != m_FuncPtr.m_mapVoid.end())
		return;
	m_FuncPtr.m_mapVoid.insert(std::make_pair(_strFunc, _pFunc));
}

void CGameObject::RegisterFucnPtrINT(wstring _strFunc, std::function<void(int)> _pFunc)
{
	auto itr = m_FuncPtr.m_mapInt.find(_strFunc);
	if (itr != m_FuncPtr.m_mapInt.end())
		return;
	m_FuncPtr.m_mapInt.insert(std::make_pair(_strFunc, _pFunc));
}

void CGameObject::RegisterFucnPtrFLOAT(wstring _strFunc, std::function<void(float)> _pFunc)
{
	auto itr = m_FuncPtr.m_mapFloat.find(_strFunc);
	if (itr != m_FuncPtr.m_mapFloat.end())
		return;
	m_FuncPtr.m_mapFloat.insert(std::make_pair(_strFunc, _pFunc));
}

void CGameObject::RegisterFucnPtrSTRING(wstring _strFunc, std::function<void(string)> _pFunc)
{
	auto itr = m_FuncPtr.m_mapString.find(_strFunc);
	if (itr != m_FuncPtr.m_mapString.end())
		return;
	m_FuncPtr.m_mapString.insert(std::make_pair(_strFunc, _pFunc));
}

void CGameObject::RegisterFucnPtrOBJ(wstring _strFunc, std::function<void(CGameObject*)> _pFunc)
{
	auto itr = m_FuncPtr.m_mapObj.find(_strFunc);
	if (itr != m_FuncPtr.m_mapObj.end())
		return;
	m_FuncPtr.m_mapObj.insert(std::make_pair(_strFunc, _pFunc));
}

std::optional<std::reference_wrapper<std::function<void()>>> CGameObject::GetFuncPtrVOID(wstring _strFunc)
{
	map<wstring, std::function<void()>>& mapEventFunc = m_FuncPtr.m_mapVoid;
	auto itr = mapEventFunc.find(_strFunc);
	if (itr == mapEventFunc.end())
		return std::nullopt;
	return std::ref(mapEventFunc[_strFunc]);
}

std::optional<std::reference_wrapper<std::function<void(int)>>> CGameObject::GetFuncPtrINT(wstring _strFunc)
{
	map<wstring, std::function<void(int)>>& mapEventFunc = m_FuncPtr.m_mapInt;
	auto itr = mapEventFunc.find(_strFunc);
	if (itr == mapEventFunc.end())
		return std::nullopt;
	return std::ref(mapEventFunc[_strFunc]);
}

std::optional<std::reference_wrapper<std::function<void(float)>>> CGameObject::GetFuncPtrFLOAT(wstring _strFunc)
{
	map<wstring, std::function<void(float)>>& mapEventFunc = m_FuncPtr.m_mapFloat;
	auto itr = mapEventFunc.find(_strFunc);
	if (itr == mapEventFunc.end())
		return std::nullopt;
	return std::ref(mapEventFunc[_strFunc]);
}

std::optional<std::reference_wrapper<std::function<void(string)>>> CGameObject::GetFuncPtrSTRING(wstring _strFunc)
{
	map<wstring, std::function<void(string)>>& mapEventFunc = m_FuncPtr.m_mapString;
	auto itr = mapEventFunc.find(_strFunc);
	if (itr == mapEventFunc.end())
		return std::nullopt;
	return std::ref(mapEventFunc[_strFunc]);
}

std::optional<std::reference_wrapper<std::function<void(CGameObject*)>>> CGameObject::GetFuncPtrOBJ(wstring _strFunc)
{
	map<wstring, std::function<void(CGameObject*)>>& mapEventFunc = m_FuncPtr.m_mapObj;
	auto itr = mapEventFunc.find(_strFunc);
	if (itr == mapEventFunc.end())
		return std::nullopt;
	return std::ref(mapEventFunc[_strFunc]);
}


vector<wstring> CGameObject::GetFuncPtrListName()
{
	vector<wstring> KeyList;
	{
		auto itr = m_FuncPtr.m_mapVoid.begin();
		for (; itr != m_FuncPtr.m_mapVoid.end(); ++itr)
		{
			wstring Key = itr->first;
			KeyList.push_back(Key);
		}
	}
	{
		auto itr = m_FuncPtr.m_mapInt.begin();
		for (; itr != m_FuncPtr.m_mapInt.end(); ++itr)
		{
			wstring Key = itr->first;
			KeyList.push_back(Key);
		}
	}
	{
		auto itr = m_FuncPtr.m_mapFloat.begin();
		for (; itr != m_FuncPtr.m_mapFloat.end(); ++itr)
		{
			wstring Key = itr->first;
			KeyList.push_back(Key);
		}
	}
	{
		auto itr = m_FuncPtr.m_mapString.begin();
		for (; itr != m_FuncPtr.m_mapString.end(); ++itr)
		{
			wstring Key = itr->first;
			KeyList.push_back(Key);
		}
	}
	{
		auto itr = m_FuncPtr.m_mapObj.begin();
		for (; itr != m_FuncPtr.m_mapObj.end(); ++itr)
		{
			wstring Key = itr->first;
			KeyList.push_back(Key);
		}
	}

	return KeyList;
}

PARAM_TYPE CGameObject::GetFindFuncType(wstring _strFunc)
{
	{
		auto itr = m_FuncPtr.m_mapVoid.find(_strFunc);
		if (itr != m_FuncPtr.m_mapVoid.end())
		{
			return PARAM_TYPE::VOID_TYPE;
		}
	}
	{
		auto itr = m_FuncPtr.m_mapInt.find(_strFunc);
		if (itr != m_FuncPtr.m_mapInt.end())
		{
			return PARAM_TYPE::INT;
		}
	}
	{
		auto itr = m_FuncPtr.m_mapFloat.find(_strFunc);
		if (itr != m_FuncPtr.m_mapFloat.end())
		{
			return PARAM_TYPE::FLOAT;
		}
	}
	{
		auto itr = m_FuncPtr.m_mapString.find(_strFunc);
		if (itr != m_FuncPtr.m_mapString.end())
		{
			return PARAM_TYPE::STRING;
		}
	}
	{
		auto itr = m_FuncPtr.m_mapObj.find(_strFunc);
		if (itr != m_FuncPtr.m_mapObj.end())
		{
			return PARAM_TYPE::OBJECT;
		}
	}

	return PARAM_TYPE::END;
}



void CGameObject::DisconnectFromParent()
{
	if (!m_Parent)
		return;

	vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (this == *iter)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	assert(nullptr);
}

void CGameObject::ChangeToChildType()
{
	assert(-1 <= m_iLayerIdx && m_iLayerIdx < MAX_LAYER);

	if (-1 != m_iLayerIdx)
	{
		CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
		pLayer->RemoveFromParentList(this);
	}
}

void CGameObject::AddParentList()
{
	CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
	pLayer->AddParentList(this);
}

void CGameObject::ChangeLayerIdx(int _iIdx)
{
	m_iLayerIdx = _iIdx;
	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->ChangeLayerIdx(_iIdx);
	}
}

CAnimator3D* CGameObject::Animator3D()
{
	CAnimator3D* pAnimator = (CAnimator3D*)m_arrCom[(UINT)COMPONENT_TYPE::ANIMATOR3D];
	if (pAnimator == nullptr)
	{
		if (m_Parent)
		{
			pAnimator = m_Parent->Animator3D();
		}
	}
	return pAnimator;
}
