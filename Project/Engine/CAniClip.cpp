#include "pch.h"
#include "CResMgr.h"
#include "CAniClip.h"
#include "CFBXLoader.h"
#include "CStructuredBuffer.h"
#include "CScript.h"

CAniClip::CAniClip() :CRes(RES_TYPE::ANICLIP, true)
{
}



CAniClip::~CAniClip()
{
	if (m_pBoneOffset != nullptr)
		delete m_pBoneOffset;
	if (m_pBoneFrameData != nullptr)
		delete m_pBoneFrameData;

	for (int i = 0; i < m_Events.size(); ++i)
		delete m_Events[i];
}

void CAniClip::CreateBoneFrameData(CFBXLoader& _loader, tAnimClip*& _pClipData, vector<tKeyFrame> _vecframeData, int _iBoneIdx)
{
	Ptr<CAniClip> pClip = nullptr;
	wstring strPath;
	strPath = _pClipData->strName;
	pClip = CResMgr::GetInst()->FindRes<CAniClip>(strPath).Get();

	if (pClip == nullptr)
	{
		pClip = new CAniClip();
		UINT iBoneCount = _loader.GetBones().size();
		pClip->m_vecBones.resize(iBoneCount);

		tMTAnimClip tClip = {};
		tClip.strAnimName = _pClipData->strName;
		tClip.dStartTime = _pClipData->tStartTime.GetSecondDouble();
		tClip.dEndTime = _pClipData->tEndTime.GetSecondDouble();
		tClip.dTimeLength = tClip.dEndTime - tClip.dStartTime;

		tClip.iStartFrame = (int)_pClipData->tStartTime.GetFrameCount(_pClipData->eMode);
		tClip.iEndFrame = (int)_pClipData->tEndTime.GetFrameCount(_pClipData->eMode);
		tClip.iFrameLength = tClip.iEndFrame - tClip.iStartFrame;
		tClip.bLoop = false;
		tClip.eMode = _pClipData->eMode;

		pClip->m_tInfo = std::move(tClip);

		int Length = pClip->m_tInfo.iFrameLength;
		pClip->m_Events.resize(Length);
		
		CResMgr::GetInst()->AddRes<CAniClip>(strPath, pClip);
	}
	
	//이미 이전 Mesh에서 다 설정해두었다면
	vector<tBone*> Bones = _loader.GetBones();
	tBone* CurBone = Bones[_iBoneIdx];
	FbxAMatrix MatBone = Bones[_iBoneIdx]->matBone;
	FbxAMatrix MatBoneOffset = Bones[_iBoneIdx]->matOffset;
	
	
	tMTBone Bone = {};
	Bone.iDepth = CurBone->iDepth;
	Bone.iParentIndx = CurBone->iParentIndx;
	Bone.matBone = GetMatrixFromFbxMatrix(CurBone->matBone);
	Bone.matOffset = GetMatrixFromFbxMatrix(CurBone->matOffset);
	Bone.strBoneName = CurBone->strBoneName;

	int iFrameCnt = _vecframeData.size();
	for (int i = 0; i < iFrameCnt; ++i)
	{
		tMTKeyFrame tKeyFrame = {};
		tKeyFrame.dTime = _vecframeData[i].dTime;
		tKeyFrame.iFrame = i;
		tKeyFrame.vTranslate.x = (float)_vecframeData[i].matTransform.GetT().mData[0];
		tKeyFrame.vTranslate.y = (float)_vecframeData[i].matTransform.GetT().mData[1];
		tKeyFrame.vTranslate.z = (float)_vecframeData[i].matTransform.GetT().mData[2];

		tKeyFrame.vScale.x = (float)_vecframeData[i].matTransform.GetS().mData[0];
		tKeyFrame.vScale.y = (float)_vecframeData[i].matTransform.GetS().mData[1];
		tKeyFrame.vScale.z = (float)_vecframeData[i].matTransform.GetS().mData[2];

		tKeyFrame.qRot.x = (float)_vecframeData[i].matTransform.GetQ().mData[0];
		tKeyFrame.qRot.y = (float)_vecframeData[i].matTransform.GetQ().mData[1];
		tKeyFrame.qRot.z = (float)_vecframeData[i].matTransform.GetQ().mData[2];
		tKeyFrame.qRot.w = (float)_vecframeData[i].matTransform.GetQ().mData[3];

		Bone.vecKeyFrame.push_back(tKeyFrame);
	}

	pClip->m_vecBones[_iBoneIdx] = Bone;
	//pClip->m_vecBones.push_back(Bone);
	
	return;
}

void CAniClip::CreateStructBuffer()
{
	// BoneOffet 행렬
	vector<Matrix> vecOffset;
	vector<tFrameTrans> vecFrameTrans;
	vecFrameTrans.resize((UINT)m_vecBones.size() * m_tInfo.iFrameLength);

	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		vecOffset.push_back(m_vecBones[i].matOffset);

		for (size_t j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
		{
			vecFrameTrans[(UINT)m_vecBones.size() * j + i]
				= tFrameTrans{ Vec4(m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f)
				, Vec4(m_vecBones[i].vecKeyFrame[j].vScale, 0.f)
				, m_vecBones[i].vecKeyFrame[j].qRot };
		}
	}

    m_pBoneOffset = new CStructuredBuffer;
	m_pBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), SB_TYPE::READ_ONLY, false, vecOffset.data());

	m_pBoneFrameData = new CStructuredBuffer;
	m_pBoneFrameData->Create(sizeof(tFrameTrans), (UINT)vecOffset.size() * m_tInfo.iFrameLength
		, SB_TYPE::READ_ONLY, false, vecFrameTrans.data());
}
vector<int>  CAniClip::GetEventPointNumber()
{
	vector<int> vecNumbers;
	for (int i = 0; i < m_Events.size(); ++i)
	{
		vecNumbers.push_back(m_Events[i]->Time);
	}
	return vecNumbers;
}

int CAniClip::Save(const wstring& _strRelativePath)
{
	// 상대경로 저장
	SetRelativePath(_strRelativePath);
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// 키값, 상대 경로	
	SaveWString(GetName(), pFile);
	SaveWString(GetKey(), pFile);
	SaveWString(GetRelativePath(), pFile);

	//tMTAnimClip
	SaveWString(m_tInfo.strAnimName, pFile);
	fwrite(&m_tInfo.iStartFrame, sizeof(int), 1, pFile);
	fwrite(&m_tInfo.iEndFrame, sizeof(int), 1, pFile);
	fwrite(&m_tInfo.iFrameLength, sizeof(int), 1, pFile);
	fwrite(&m_tInfo.dStartTime, sizeof(double), 1, pFile);
	fwrite(&m_tInfo.dEndTime, sizeof(double), 1, pFile);
	fwrite(&m_tInfo.dTimeLength, sizeof(double), 1, pFile);
	fwrite(&m_tInfo.fUpdateTime, sizeof(float), 1, pFile);
	fwrite(&m_tInfo.bLoop, sizeof(bool), 1, pFile);
	fwrite(&m_tInfo.eMode, sizeof(FbxTime::EMode), 1, pFile);

	//tMTBone
	UINT iBoneCount = (UINT)m_vecBones.size();
	fwrite(&iBoneCount, sizeof(int), 1, pFile);
	for (int i = 0; i < iBoneCount; ++i)
	{
		SaveWString(m_vecBones[i].strBoneName, pFile);
		fwrite(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fwrite(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		int iFrameCount = (int)m_vecBones[i].vecKeyFrame.size();
		fwrite(&iFrameCount, sizeof(int), 1, pFile);

		for (int j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
		{
			fwrite(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, pFile);
		}
	}
	//event
	UINT iEventCnt =  m_Events.size();
	fwrite(&iEventCnt, sizeof(int), 1, pFile);
	for (int i = 0; i < iEventCnt; ++i)
	{
		t_AniEventPoint* point = m_Events[i];
		bool bFlag = false;
		if (point == nullptr)
			bFlag = false;
		else
			bFlag = true;
		fwrite(&bFlag, sizeof(bool), 1, pFile);

		if(bFlag)
		fwrite(point, sizeof(t_AniEventPoint), 1, pFile);
	}
	
	fclose(pFile);
	return 0;
}
int CAniClip::Load(const wstring& _strFilePath)
{
	// 읽기모드로 파일열기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	// 키값, 상대경로
	wstring strName, strKey, strRelativePath;
	LoadWString(strName, pFile);
	LoadWString(strKey, pFile);
	LoadWString(strRelativePath, pFile);

	SetName(strName);
	SetKey(strKey);
	SetRelativePath(strRelativePath);

	//tMTAnimClip
	LoadWString(m_tInfo.strAnimName, pFile);
	fread(&m_tInfo.iStartFrame, sizeof(int), 1, pFile);
	fread(&m_tInfo.iEndFrame, sizeof(int), 1, pFile);
	fread(&m_tInfo.iFrameLength, sizeof(int), 1, pFile);
	fread(&m_tInfo.dStartTime, sizeof(double), 1, pFile);
	fread(&m_tInfo.dEndTime, sizeof(double), 1, pFile);
	fread(&m_tInfo.dTimeLength, sizeof(double), 1, pFile);
	fread(&m_tInfo.fUpdateTime, sizeof(float), 1, pFile);
	fread(&m_tInfo.bLoop, sizeof(bool), 1, pFile);
	fread(&m_tInfo.eMode, sizeof(FbxTime::EMode), 1, pFile);

	//tMTBone
	UINT iBoneCount = 0;
	fread(&iBoneCount, sizeof(UINT), 1, pFile);
	m_vecBones.resize(iBoneCount);
	UINT _iFrameCount = 0;
	for (int i = 0; i < iBoneCount; ++i)
	{
		LoadWString(m_vecBones[i].strBoneName, pFile);
		fread(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fread(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		UINT iFrameCount = 0;
		fread(&iFrameCount, sizeof(int), 1, pFile);
		_iFrameCount = iFrameCount;
		m_vecBones[i].vecKeyFrame.resize(iFrameCount);
		for (UINT j = 0; j < iFrameCount; ++j)
		{
			fread(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, pFile);
		}
	}
	
	if (m_vecBones.size() > 0)
	{
		CreateStructBuffer();
	}

	//event
	UINT iEventCnt = 0;
	fread(&iEventCnt, sizeof(int), 1, pFile);
	m_Events.resize(iEventCnt);
	for (int i = 0; i < iEventCnt; ++i)
	{
		bool bFlag = false;
		fread(&bFlag, sizeof(bool), 1, pFile);
		if (bFlag)
		{
			t_AniEventPoint* point = new t_AniEventPoint();
			fread(point, sizeof(t_AniEventPoint), 1, pFile);
			m_Events[i] = point;
		}
		else
		{
			m_Events[i] = nullptr;
		}
	}

	fclose(pFile);
	return 0;
}

void CAniClip::RemoveEvents(int _iFrame)
{
	t_AniEventPoint* pPoint = m_Events[_iFrame];
	m_Events[_iFrame] = nullptr;
	delete pPoint;
}

