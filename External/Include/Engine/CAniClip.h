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

    CStructuredBuffer*      m_pBoneFrameData;   // ��ü �� ������ ����(ũ��, �̵�, ȸ��) (������ ������ŭ)
    CStructuredBuffer*      m_pBoneOffset;	    // �� ���� offset ���(�� ���� ��ġ�� �ǵ����� ���) (1�� ¥��)
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

