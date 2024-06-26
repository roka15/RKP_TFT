#pragma once


#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP		
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define MAX_LAYER	33
#define MAX_MIP		8

#define SINGLE(type) private: type(); ~type(); friend class CSingleton<type>;
#define MAXLEN 255
#define UNICODELEN 2

#define DEGREE2RADIAN(num) num * XM_PI / 180
#define RADIAN2DEGREE(num) num * 180 / XM_PI

#define DEBUG_VisualStudioMSG(str) OutputDebugStringA((LPCSTR)(__FILE__"("ValueToStr(__LINE__)"): -------"#str"-------\r\n"))
#define DEBUG_VisualStudioMSG2(str) OutputDebugStringA((LPCSTR)(str))
enum class COMPONENT_TYPE
{
	// update
	TRANSFORM,			// 위치, 크기, 회전
	COLLIDER2D,			// 2차원 충돌
	COLLIDER3D,			// 3차원 충돌
	ANIMATOR2D,			// Sprite Animation
	ANIMATOR3D,			// Bone Sknning Animation
	LIGHT2D,			// 2차원 광원
	LIGHT3D,			// 3차원 광원
	CAMERA,				// Camera

	// render
	MESHRENDER,			// 기본적인 렌더링
	PARTICLESYSTEM,		// 입자 렌더링
	TILEMAP,			// 2차원 타일
	LANDSCAPE,			// 3차원 지형
	SKYBOX,				// SkyBox
	DECAL,				// 내부 렌더링
	CANVASRENDER,       // ui 렌더링
	BILLBOARD,
	
	// ui
	CANVAS,
	UI,
	TEXT,
	IMAGE,
	BUTTON,

	END,

	// custom
	SCRIPT,
};

extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END];
extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END];


enum class RES_TYPE
{
	MESHDATA,
	MATERIAL,
	PREFAB,

	MESH,			// 형태
	TEXTURE,		// 이미지
	SOUND,
	ANICLIP,
	ANICONTROLLER,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END,
};

extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END];
extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END];




enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2
	END,
};


enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,	
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	TEX_END,
};

enum PIPELINE_STAGE
{
	PS_VERTEX = 0x01,
	PS_HULL = 0x02,
	PS_DOMAIN = 0x04,
	PS_GEOMETRY = 0x08,
	PS_PIXEL = 0x10,	

	PS_ALL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL,	
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			// LESS, DepthWrite X
	NO_TEST_NO_WRITE,	// Test X, DepthWrite X
	END,
};


enum class BS_TYPE
{
	DEFAULT,		// No Blending
	MASK,			// Alpha Coverage
	ALPHA_BLEND,	// Alpha 계수 
	ONE_ONE,		// 1:1 혼합

	DEFEREED_DECAL_BLEND, // 0 타겟은 AlphaBlend, 1 타겟은 ONE-ONE Blend
	END,
};





enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,	
};

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

enum class SHADER_DOMAIN
{
	DOMAIN_DEFERRED,		// 지연 렌더링 오브젝트
	DOMAIN_DEFERRED_DECAL,	// Deferred Decal(광원 적용 가능한 Decal)

	DOMAIN_OPAQUE,			// 불투명 오브젝트
	DOMAIN_MASK,			// 불투명, 완전 투명
	DOMAIN_DECAL,			// 데칼 오브젝트
	DOMAIN_TRANSPARENT,		// 반투명
	DOMAIN_POSTPROCESS,		// 후 처리
	DOMAIN_UI,

	DOMAIN_LIGHT,			// 광원 타입 
	DOMAIN_UNDEFINED,		// 미정
};


enum class EVENT_TYPE
{
	CREATE_OBJECT,	// wParam : GameObject, lParam : Layer Index
	DELETE_OBJECT,  // wParam : GameObject

	ADD_CHILD,
	CHANGE_ADD_CHILD,

	DELETE_RESOURCE,	// wParam : RES_TYPE, lParam : Resource Adress

	LEVEL_CHANGE,	
};


enum class SHAPE_TYPE
{
	RECT,
	CIRCLE,
	CUBE,
	SPHERE,
	END,
};


enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
};

enum class COLLIDER3D_TYPE
{
	BOX,
	SPHERE,
};


enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};

enum class SB_TYPE
{
	READ_ONLY,
	READ_WRITE,
};

enum class PARTICLE_MODULE
{
	PARTICLE_SPAWN,
	COLOR_CHANGE,
	SCALE_CHANGE,
	ADD_VELOCITY,

	DRAG,
	NOISE_FORCE,
	RENDER,
	DUMMY_3,

	END,
};

enum class MRT_TYPE
{
	SWAPCHAIN,

	DEFERRED,

	DEFERRED_DECAL,

	LIGHT,

	SHADOWMAP,

	END,
};

enum class PARAM_TYPE
{
	INT,
	FLOAT,
	BOOL,
	TRIGGER,
	VOID_TYPE,
	STRING,
	OBJECT,
	END
};

enum class COMPARISON_TYPE
{
	GREATER,
	LESS,
	EQUAL,
	END,
};

enum class ANI_NODE_RETURN
{
	NOTHING,
	ENTRY,
	EXIT,
	RESET,
	CHANGE,
};

enum class CANVAS_RENDER_MODE
{
	ScreenSpace_Overlay,
	ScreenSpace_Camera,
	WorldSpace
};

enum class MOUSE_INPUT_TYPE
{
	LEFT = 0,
	RIGHT,
	MIDDLE,
};
enum class CURSOR_TYPE
{
	CURSOR_2D,
	CURSOR_3D,
	END,
};

enum class GAUGE_TYPE
{
	LEFT, // Left 부터 discard
	RIGHT,// Right 부터 discard
	END,
};