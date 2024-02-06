#pragma once
#include "CRes.h"
#include "CFBXLoader.h"

class CStructuredBuffer;

struct tIndexInfo
{
	ComPtr<ID3D11Buffer>    pIB;
	D3D11_BUFFER_DESC       tIBDesc;
	UINT				    iIdxCount;
	void*					pIdxSysMem;
};

class CMesh
	: public CRes
{
private:
	ComPtr<ID3D11Buffer>	m_VB;
	D3D11_BUFFER_DESC		m_tVBDesc;
	UINT					m_VtxCount;
	void*					m_pVtxSys;

	// 하나의 버텍스버퍼에 여러개의 인덱스버퍼가 연결
	vector<tIndexInfo>		m_vecIdxInfo;

	//Animation 
	vector<wstring> m_strAniClipNameList;
public:
	Vtx* GetVtxSysMem() { return (Vtx*)m_pVtxSys; }
	UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }
	const vector<wstring>& GetAniClipList() { return m_strAniClipNameList; }
public:
	static CMesh* CreateFromContainer(CFBXLoader& _loader,int _idx);
	void Create(void* _VtxSysMem, UINT _iVtxCount, void* _IdxSysMem, UINT _IdxCount);
	bool IsAni() { return m_strAniClipNameList.size() > 0; }
private:
	virtual int Load(const wstring& _strFilePath);
public:
	virtual int Save(const wstring& _strRelativePath);

	void render(UINT _iSubset);
	void render_particle(UINT _iParticleCount);
	void render_instancing(UINT _isubset);

private:	
	void UpdateData(UINT _iSubset);
	void UpdateData_Inst(UINT _iSubset);

public:
	CMesh(bool _bEngine = false);
	~CMesh();
};