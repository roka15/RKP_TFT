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
            
    CStructuredBuffer*               m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
    CStructuredBuffer*               m_pBoneOffset;	    // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)
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

