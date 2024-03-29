#pragma once
#include "CRes.h"
class CFBXLoader;
class CStructuredBuffer;
class CScript;
class CAnimation3D;
class CBaseCharacterScript;
struct tAnimClip;
struct tKeyFrame;

struct t_AniEventPoint
{
    double                             Time;
    wstring                            Function;
   
    float                              Float;
    int                                Int;
    string                             String;
    CGameObject*                       Obj;
};

class CAniClip :
    public CRes
{
private:
    tMTAnimClip                      m_tInfo;
    vector<tMTBone>			         m_vecBones;
            
    CStructuredBuffer*               m_pBoneFrameData;   // ��ü �� ������ ����(ũ��, �̵�, ȸ��) (������ ������ŭ)
    CStructuredBuffer*               m_pBoneOffset;	    // �� ���� offset ���(�� ���� ��ġ�� �ǵ����� ���) (1�� ¥��)
    vector<t_AniEventPoint*>         m_Events;
private:
    virtual int Load(const wstring& _strFilePath)override;
    void RegisterEvents(int _iFrame, t_AniEventPoint* _point) { m_Events[_iFrame] = _point; }
public:
    static void CreateBoneFrameData(CFBXLoader& _loader,tAnimClip*& _clipData, vector<tKeyFrame> _frameData,int _boneIdx);
    void CreateStructBuffer();
    CStructuredBuffer* GetBoneFrameBuffer() { return m_pBoneFrameData; }
    CStructuredBuffer* GetBoneOffsetBuffer() { return m_pBoneOffset; }
    UINT GetBoneCount() { return m_vecBones.size(); }
    virtual int Save(const wstring& _strRelativePath)override;

    void SetLoop(bool _flag) { m_tInfo.bLoop = _flag; }
 
    vector<int>  GetEventPointNumber();
public:
    CAniClip();
    ~CAniClip();
private:
    friend class CAnimation3D;
    friend class AniClipUI;
};

