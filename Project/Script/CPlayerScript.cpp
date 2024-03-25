#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>
#include <Engine\CMouseMgr.h>
#include "CTileMgr.h"

#include "CMissileScript.h"
#include "CBaseCharacterScript.h"


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
			pCharacterScript->CurStartTile();
			pCharacterScript->SetMove(true);
		case GAME_STATE::LOADING:
			//select때 설정한 초기 위치로 이동.
			pCharacterScript->ResetTile();
			break;
		}
	}
	
}

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(500.f)
	, m_Money(100)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Player Speed");
}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::begin()
{
	// 동적 재질 생성
	MeshRender()->GetDynamicMaterial(0);
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





void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
}