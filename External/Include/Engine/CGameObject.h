#pragma once
#include "CEntity.h"


class CComponent;
class CTransform;
class CMeshRender;
class CParticleSystem;
class CCollider2D;
class CCollider3D;
class CAnimator2D;
class CAnimator3D;
class CLight2D;
class CLight3D;
class CCamera;
class CRenderComponent;
class CSkyBox;
class CBillboard;
class CTileMap;
class CDecal;
class CLandScape;
class CScript;
class CText;
class CImage;
class CButton;
struct t_FuncPtrList;
#define GET_COMPONENT(Type, TYPE) C##Type* Type() const { return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }



class CGameObject :
	public CEntity
{
private:
	CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];
	CRenderComponent* m_RenderCom;
	vector<CScript*>        m_vecScript;

	CGameObject* m_Parent;
	vector<CGameObject*>    m_vecChild;

	int                     m_iLayerIdx; // 소속된 레이어 인덱스값
	bool                    m_bDead;
	float                   m_LifeTime;
	float                   m_CurLifeTime;
	bool                    m_bLifeSpan;

	t_FuncPtrList           m_FuncPtr;
public:
	// 레벨이 시작될 때 호출 or 시작 된 레벨에 합류할 때 호출
	// 생성자
	void begin();

	void tick();
	virtual void finaltick();
	void finaltick_module();
	void render();
	void render_shadowmap();

public:
	void AddComponent(CComponent* _Component);
	void AddChild(CGameObject* _Object);

	CComponent* GetComponent(COMPONENT_TYPE _ComType) { return m_arrCom[(UINT)_ComType]; }
	const vector<CGameObject*>& GetChild() { return m_vecChild; }
	CGameObject* GetChild(wstring _strName);
	CGameObject* GetParent() const { return m_Parent; }

	GET_COMPONENT(Transform, TRANSFORM);
	GET_COMPONENT(MeshRender, MESHRENDER);
	GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
	GET_COMPONENT(Camera, CAMERA);
	GET_COMPONENT(Collider2D, COLLIDER2D);
	GET_COMPONENT(Collider3D, COLLIDER3D);
	GET_COMPONENT(Light2D, LIGHT2D);
	GET_COMPONENT(TileMap, TILEMAP);
	GET_COMPONENT(Animator2D, ANIMATOR2D);
	GET_COMPONENT(Light3D, LIGHT3D);
	GET_COMPONENT(SkyBox, SKYBOX);
	GET_COMPONENT(Billboard, BILLBOARD);
	GET_COMPONENT(Decal, DECAL);
	GET_COMPONENT(LandScape, LANDSCAPE);
	GET_COMPONENT(Text, TEXT);
	GET_COMPONENT(Image, IMAGE);
	GET_COMPONENT(Button, BUTTON);



	CAnimator3D* Animator3D();
	CRenderComponent* GetRenderComponent() const { return m_RenderCom; }


	int GetLayerIndex() { return m_iLayerIdx; }

	template<typename T>
	T* GetScript();

	const vector<CScript*>& GetScripts() { return m_vecScript; }

	void SetLifeSpan(float _fTime)
	{
		if (m_bLifeSpan)
			return;

		m_LifeTime = _fTime;
		m_bLifeSpan = true;
	}

	bool IsDead() { return m_bDead; }
	bool IsAncestor(CGameObject* _Target);

	void RegisterFucnPtrVOID(wstring _strFunc, std::function<void()> _pFunc);
	void RegisterFucnPtrINT(wstring _strFunc, std::function<void(int)> _pFunc);
	void RegisterFucnPtrFLOAT(wstring _strFunc, std::function<void(float)> _pFunc);
	void RegisterFucnPtrSTRING(wstring _strFunc, std::function<void(string)> _pFunc);
	void RegisterFucnPtrOBJ(wstring _strFunc, std::function<void(CGameObject*)> _pFunc);

	std::optional<std::reference_wrapper<std::function<void()>>> GetFuncPtrVOID(wstring _strFunc);
	std::optional<std::reference_wrapper<std::function<void(int)>>> GetFuncPtrINT(wstring _strFunc);
	std::optional<std::reference_wrapper<std::function<void(float)>>> GetFuncPtrFLOAT(wstring _strFunc);
	std::optional<std::reference_wrapper<std::function<void(string)>>> GetFuncPtrSTRING(wstring _strFunc);
	std::optional<std::reference_wrapper<std::function<void(CGameObject*)>>> GetFuncPtrOBJ(wstring _strFunc);
	vector<wstring> GetFuncPtrListName();
	PARAM_TYPE GetFindFuncType(wstring _strFunc);
private:
	void DisconnectFromParent();
	void ChangeToChildType();
	void AddParentList();
	void ChangeLayerIdx(int _iIdx);


	CLONE(CGameObject)
public:
	CGameObject();
	CGameObject(const CGameObject& _Other);
	~CGameObject();

	friend class CLayer;
	friend class CEventMgr;
};

template<typename T>
inline T* CGameObject::GetScript()
{
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		T* pScript = dynamic_cast<T*> (m_vecScript[i]);
		if (nullptr != pScript)
			return pScript;
	}

	return nullptr;
}
