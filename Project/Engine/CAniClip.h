#pragma once
#include "CRes.h"
class CFBXLoader;
class CStructuredBuffer;
class CAnimation3D;
struct tAnimClip;
struct tKeyFrame;

class CAniClip :
    public CRes
{
private:
    tMTAnimClip             m_tInfo;
    vector<tMTBone>			m_vecBones;

    CStructuredBuffer*      m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
    CStructuredBuffer*      m_pBoneOffset;	    // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)
private:
    
    virtual int Load(const wstring& _strFilePath)override;
public:
    static void CreateBoneFrameData(CFBXLoader& _loader,tAnimClip*& _clipData, vector<tKeyFrame> _frameData,int _boneIdx);
    void CreateStructBuffer();
    CStructuredBuffer* GetBoneFrameBuffer() { return m_pBoneFrameData; }
    CStructuredBuffer* GetBoneOffsetBuffer() { return m_pBoneOffset; }
    UINT GetBoneCount() { return m_vecBones.size(); }
    virtual int Save(const wstring& _strRelativePath)override;

    void ActiveLoop() { m_tInfo.bLoop = true; }
public:
    CAniClip();
    ~CAniClip();
private:
    friend class CAnimation3D;
};

