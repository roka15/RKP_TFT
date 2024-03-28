#pragma once
#include "CRes.h"
class CFBXLoader;
class CStructuredBuffer;
class CAnimation3D;
struct tAnimClip;
struct tKeyFrame;

struct t_AniEventPoint
{
    double                             Time;
    std::function<void()>              mNormalFunc;
    std::function<void(float)>         mFloatFunc;
    std::function<void(int)>           mIntFunc;
    std::function<void(std::string)>   mStringFunc;
    std::function<void(CGameObject*)>  mObjFunc;

   
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
    vector<t_AniEventPoint>         m_Events;
private:
    
    virtual int Load(const wstring& _strFilePath)override;
public:
    static void CreateBoneFrameData(CFBXLoader& _loader,tAnimClip*& _clipData, vector<tKeyFrame> _frameData,int _boneIdx);
    void CreateStructBuffer();
    CStructuredBuffer* GetBoneFrameBuffer() { return m_pBoneFrameData; }
    CStructuredBuffer* GetBoneOffsetBuffer() { return m_pBoneOffset; }
    UINT GetBoneCount() { return m_vecBones.size(); }
    virtual int Save(const wstring& _strRelativePath)override;

    void SetLoop(bool _flag) { m_tInfo.bLoop = _flag; }
    void RegisterEvents(t_AniEventPoint _point) { m_Events.push_back(_point); }
    vector<int> GetEventPointNumber();
public:
    CAniClip();
    ~CAniClip();
private:
    friend class CAnimation3D;
    friend class AniClipUI;
};

