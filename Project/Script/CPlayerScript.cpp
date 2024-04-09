#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>
#include <Engine\CMouseMgr.h>
#include "CTileMgr.h"

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
			//select�� ������ ĳ���� �ʱ� ��ġ ����
			if (m_ePlayerType != PLAYER_TYPE::AI)
				pCharacterScript->CurStartTile();
		case GAME_STATE::LOADING:
			//select�� ������ �ʱ� ��ġ�� �̵�.
			//���¿� ���� �͵鵵 �ʱ�ȭ(atk,move ��)
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
	, m_Money(100)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Player Speed");
}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::begin()
{
	// ���� ���� ����
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





void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
}