#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>
#include <Engine\CMouseMgr.h>
#include "CTileMgr.h"
#include "CGameMgr.h"

#include "CMissileScript.h"
#include "CBaseCharacterScript.h"


vector<Vec3> CPlayerScript::GetItemPos()
{
	vector<Vec3> vecPos;
	for (int i = 0; i < m_vecItem.size(); ++i)
	{
		vecPos.push_back(m_vecItem[i]->Transform()->GetWorldPos());
	}
	return vecPos;
}

void CPlayerScript::SetGameStateInfo()
{
	for (int i = 0; i < m_vecItem.size(); ++i)
	{
		CGameObject* pItem = m_vecItem[i];
		CBaseCharacterScript* pCharacterScript = pItem->GetScript<CBaseCharacterScript>();
		switch ((GAME_STATE)m_GameState)
		{
		case GAME_STATE::BATTLE:
			//select때 설정한 캐릭터 초기 위치 저장
			if (m_ePlayerType != PLAYER_TYPE::AI)
				pCharacterScript->CurStartTile();
		case GAME_STATE::LOADING:
			//select때 설정한 초기 위치로 이동.
			//상태에 대한 것들도 초기화(atk,move 등)
			if (m_ePlayerType != PLAYER_TYPE::AI)
				pCharacterScript->Reset();
			pCharacterScript->BattleStateReset();
			break;
		}
	}

}

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(500.f)
	, m_Money(50)
	, m_Level(1)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Player Speed");
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::start()
{
	RegisterFuncPtr();
}

void CPlayerScript::begin()
{
	// 동적 재질 생성
	//MeshRender()->GetDynamicMaterial(0);
}

void CPlayerScript::tick()
{

}


void CPlayerScript::BeginOverlap(CCollider* _Other)
{
	CGameObject* pOtherObject = _Other->GetOwner();

	if (pOtherObject->GetName() == L"Monster")
	{
		DestroyObject(pOtherObject);
	}
}

void CPlayerScript::RegisterFuncPtr()
{
	CGameObject* pOwner = GetOwner();
	pOwner->RegisterFucnPtrVOID(L"BuyExp", std::bind(&CPlayerScript::BuyExp, this));
	pOwner->RegisterFucnPtrVOID(L"VoidTest", std::bind(&CPlayerScript::VoidTest, this));
	pOwner->RegisterFucnPtrINT(L"IntTest", std::bind(&CPlayerScript::IntTest, this, std::placeholders::_1));
	pOwner->RegisterFucnPtrFLOAT(L"FloatTest", std::bind(&CPlayerScript::FloatTest, this, std::placeholders::_1));
	pOwner->RegisterFucnPtrSTRING(L"StringTest", std::bind(&CPlayerScript::StringTest, this, std::placeholders::_1));
	pOwner->RegisterFucnPtrOBJ(L"ObjTest", std::bind(&CPlayerScript::ObjTest, this, std::placeholders::_1));
}

void CPlayerScript::BuyExp()
{
	CGameMgr::GetInst()->BuyExp(m_CurExp, m_Level,m_Money);
}

void CPlayerScript::VoidTest()
{
	int a = 0;
}

void CPlayerScript::IntTest(int _int)
{
	int a = 0;
}

void CPlayerScript::FloatTest(float _float)
{
	int a = 0;
}

void CPlayerScript::StringTest(string _str)
{
	int a = 0;
}

void CPlayerScript::ObjTest(CGameObject* _pObj)
{
	int a = 0;
}





void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
}