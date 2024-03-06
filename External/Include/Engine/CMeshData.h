#pragma once
#include "CRes.h"

#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"


class CMeshData :
	public CRes
{
	class CMeshDataNode
	{
	public:
		Ptr<CMesh> m_pMesh;
		vector<Ptr<CMaterial>>	m_vecMtrl;
		bool					m_bAni;
	};
	vector<CMeshDataNode*> m_vecDataNode;
public:
	static CMeshData* LoadFromFBX(const wstring& _strFilePath);

	virtual int Save(const wstring& _strFilePath) override;
	virtual int Load(const wstring& _strFilePath) override;

	CGameObject* Instantiate();

	CLONE_DISABLE(CMeshData)
public:
	CMeshData(bool _bEngine = false);
	virtual ~CMeshData();
};

