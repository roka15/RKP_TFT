#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

struct VS_IN
{
	float3 vLocalPos : POSITION;
	float3 vColor : COLOR;
	float2 vUV : TEXCOORD;
};

struct VS_OUT
{
	float4 vPosition : SV_Position;
	float4 vColor : COLOR;
	float2 vUV : TEXCOORD;
};
struct VTX_IN_INST
{
	float3 vPos : POSITION;
	float2 vUV : TEXCOORD;

	float3 vTangent : TANGENT;
	float3 vNormal : NORMAL;
	float3 vBinormal : BINORMAL;

	float4 vWeights : BLENDWEIGHT;
	float4 vIndices : BLENDINDICES;

	uint iInstID : SV_InstanceID;

	// Per Instance Data
	row_major matrix matWorld : WORLD;
	row_major matrix matWV : WV;
	row_major matrix matWVP : WVP;
	uint             iRowIndex : ROWINDEX;
};

// ============================
// Std2DShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_int_0              : AnimUse
// g_vec2_0             : AnimAtlas LeftTop
// g_vec2_1             : AnimAtlas Slice
//
// g_tex_0              : Output Texture
// ============================
VS_OUT VS_Std2D(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
	output.vUV = _in.vUV;

	return output;
}

VS_OUT VS_Std2D_Inst(VTX_IN_INST _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = mul(float4(_in.vPos, 1.f), _in.matWVP);
	output.vUV = _in.vUV;

	return output;
}

struct GS_OUT
{
	float4  vPosition : SV_Position;
	float2  vUV : TEXCOORD;
};

[maxvertexcount(6)]
void GS_Billboard_Render(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _outstream)
{
	float3 vViewPos = mul(_in[0].vPosition, g_matProjInv).xyz;
	float4 Scale = g_vec4_0;

	float3 NewPos[4] =
	{
		 float3(-Scale.x / 2.f, +Scale.y / 2.f, 0.f),
		 float3(+Scale.x / 2.f, +Scale.y / 2.f, 0.f),
		 float3(+Scale.x / 2.f, -Scale.y / 2.f, 0.f),
		 float3(-Scale.x / 2.f, -Scale.y / 2.f, 0.f)
	};

	GS_OUT output[4] = { (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f };

	output[0].vPosition = mul(float4(NewPos[0] + vViewPos, 1.f), g_matProj);
	output[0].vUV = float2(0.f, 0.f);
	
	output[1].vPosition = mul(float4(NewPos[1] + vViewPos, 1.f), g_matProj);
	output[1].vUV = float2(1.f, 0.f);
	
	output[2].vPosition = mul(float4(NewPos[2] + vViewPos, 1.f), g_matProj);
	output[2].vUV = float2(1.f, 1.f);

	output[3].vPosition = mul(float4(NewPos[3] + vViewPos, 1.f), g_matProj);
	output[3].vUV = float2(0.f, 1.f);

	_outstream.Append(output[0]);
	_outstream.Append(output[1]);
	_outstream.Append(output[2]);
	_outstream.RestartStrip();

	_outstream.Append(output[0]);
	_outstream.Append(output[2]);
	_outstream.Append(output[3]);
	_outstream.RestartStrip();
}

// 레스터라이저 스테이트
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
	float4 vOutColor = g_vec4_0;

	if (g_btex_0)
	{
		vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	}
	else
	{
		vOutColor = g_vec4_0;
	}
	//if (0.f == vOutColor.a)
	//    discard; // 픽셀 쉐이더 중단

	if (g_int_2 == 1)
		vOutColor = float4(1.f, 0.f, 0.f, 1.f);

	return vOutColor;
}

float4 PS_BillboardRender(GS_OUT _in) : SV_Target
{
	int   Mode = g_int_0;
	float4 vOutColor = g_vec4_0;
	float CurValue = g_float_0;
	float GoalValue = g_float_1;


	float Ratio = 0.f;

	if (Mode == 0)
	{
		Ratio = (1 - (CurValue / GoalValue));
		if (_in.vUV.x <= Ratio)
			discard;
	}
	else if (Mode == 1)
	{
		Ratio = (CurValue / GoalValue);
		if (_in.vUV.x >= Ratio)
			discard;
	}


	if (g_btex_0)
	{
		vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	}
	else
	{
		vOutColor = g_vec4_0;
	}

	return vOutColor;
}
float4 PS_Std2D_GaugeUI(VS_OUT _in) : SV_Target
{
	int   Mode = g_int_0;
	float4 vOutColor = g_vec4_0;
	float CurValue = g_float_0;
	float GoalValue = g_float_1;

	
	float Ratio = 0.f;

	if (Mode == 0)
	{
		Ratio = (1 - (CurValue / GoalValue));
		if (_in.vUV.x <= Ratio)
			discard;
	}
	else if(Mode == 1)
	{
		Ratio = (CurValue / GoalValue);
		if (_in.vUV.x >= Ratio)
			discard;
	}


	if (g_btex_0)
	{
		vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	}
	else
	{
		vOutColor = g_vec4_0;
	}

	return vOutColor;
}





// ======================================
// Std2DLightShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
#define bAnimUse        g_int_0
#define LeftTop         g_vec2_0
#define Slice           g_vec2_1
#define Offset          g_vec2_2
#define BackSize        g_vec2_3

// g_tex_0              : Output Texture
// g_tex_1              : Nomal Texture
// ======================================

struct VS_Light_IN
{
	float3 vLocalPos : POSITION;
	float2 vUV : TEXCOORD;
};

struct VS_Light_OUT
{
	float4 vPosition : SV_Position;
	float2 vUV : TEXCOORD;
	float3 vWorldPos : POSITION;
};



VS_Light_OUT VS_Std2DLight(VS_Light_IN _in)
{
	VS_Light_OUT output = (VS_Light_OUT)0.f;

	output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
	output.vUV = _in.vUV;
	output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_matWorld).xyz;

	return output;
}


float4 PS_Std2DLight(VS_Light_OUT _in) : SV_Target
{
	float4 vOutColor = (float4) 0.f;

	if (g_vec4_0.x == 3.14f)
	{
		return float4(1.f, 0.f, 0.f, 1.f);
	}

	if (g_btex_0)
	{
		if (bAnimUse)
		{
			float2 vUV = LeftTop + (BackSize * _in.vUV);
			vUV -= ((BackSize - Slice) / 2.f);
			vUV -= Offset;

			if (LeftTop.x < vUV.x && vUV.x < LeftTop.x + Slice.x
				&& LeftTop.y < vUV.y && vUV.y < LeftTop.y + Slice.y)
			{
				vOutColor = g_tex_0.Sample(g_sam_0, vUV);
			}
			else
			{
				vOutColor = float4(1.f, 1.f, 0.f, 1.f);
				//discard;
			}
		}
		else
		{
			vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
		}
	}
	else
	{
		vOutColor = float4(1.f, 0.f, 1.f, 1.f);
	}

	float3 vNormal = (float3)0.f;
	if (g_btex_1)
	{
		// Normal 값 추출
		vNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;

		// 0 ~ 1 범위를 -1 ~ 1 로 변경
		vNormal = (vNormal * 2.f) - 1.f;

		// NormalTexture 좌표계는 y축과 z 축이 반대로 되어있다.
		float f = vNormal.y;
		vNormal.y = vNormal.z;
		vNormal.z = f;

		// Texture 에서 추출한 Normal 방향을 월드로 변환시킨다.
		vNormal = normalize(mul(float4(vNormal, 0.f), g_matWorld)).xyz;
	}


	if (0.f == vOutColor.a)
	{
		return float4(1.f, 0.f, 1.f, 1.f);
		//discard;
	}


	// Lighting 처리
	tLightColor LightColor = (tLightColor)0.f;

	// vNormal 이 Zero Vector 라면
	if (dot(vNormal, vNormal) == 0.f)
	{
		CalcLight2D(_in.vWorldPos, LightColor);
	}
	else
	{
		CalcLight2D(_in.vWorldPos, vNormal, LightColor);
	}

	vOutColor.rgb *= (LightColor.vDiffuse.rgb + LightColor.vAmbient.rgb);


	return vOutColor;
}






#endif