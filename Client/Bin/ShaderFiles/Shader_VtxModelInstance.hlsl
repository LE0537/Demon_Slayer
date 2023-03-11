
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
texture2D		g_NormalTexture;

texture2D		g_NoiseTexture;
texture2D		g_MaskTexture;

float			g_fGlowPower;
texture2D		g_GlowTexture;

float			g_fDistortionU;
float			g_fDistortionV;
float			g_fDistortionScale;
float			g_fDistortionBias;

float			g_fMoveUV_U;
float			g_fMoveUV_V;

float			g_iMulUV_U;
float			g_iMulUV_V;

float			g_fCurrentTime;

float		g_fFar;

float		g_fWindTime = 0.f;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vWorld : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float4x4	TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector		vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	vector vNormal = normalize(mul(vector(In.vNormal, 0.f), TransformMatrix));

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = vNormal;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	vector vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vWorld = vWorldPos;

	Out.vTangent = normalize(mul(In.vTangent, TransformMatrix));
	Out.vBinormal = cross(Out.vNormal, Out.vTangent);

	return Out;
}

VS_OUT VS_WIND(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float4x4	TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	vector		vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	float2	vNoiseUV = float2(frac(vPosition.x / 10.f), frac(vPosition.z / 10.f)) + g_fWindTime;
	vector	vNoiseMap = g_NoiseTexture.SampleLevel(LinearSampler, vNoiseUV, 0);
	vPosition.x += (pow(max(0.f, In.vPosition.y), 2.f) * ((vNoiseMap * 2.f) - 1.f));
	vPosition.z += (pow(max(0.f, In.vPosition.y), 2.f) * ((vNoiseMap * 2.f) - 1.f));

	vector vNormal = normalize(mul(vector(In.vNormal, 0.f), TransformMatrix));

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = vNormal;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	vector vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vWorld = vWorldPos;

	Out.vTangent = normalize(mul(In.vTangent, TransformMatrix));
	Out.vBinormal = cross(Out.vNormal, Out.vTangent);

	return Out;
}

struct VS_FLOWMAP_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float2		vTexCoord1 : TEXCOORD2;		//	�÷ο�� ������ �����մϴ�.
	float4		vWorld : TEXCOORD3;
};

VS_FLOWMAP_OUT VS_FLOWMAP(VS_IN In)
{
	VS_FLOWMAP_OUT		Out = (VS_FLOWMAP_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float4x4	TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector		vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	vector vNormal = normalize(mul(vector(In.vNormal, 0.f), TransformMatrix));

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = vNormal;
	Out.vProjPos = Out.vPosition;
	vector vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vWorld = vWorldPos;

	Out.vTexUV = In.vTexUV;
	Out.vTexCoord1.x = In.vTexUV.x + (frac(g_fCurrentTime) + 0.1f);
	Out.vTexCoord1.y = In.vTexUV.y + (0.1f);


	return Out;
}





struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vWorld : TEXCOORD2;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;

	float4		vWorld : SV_TARGET5;
};

struct PS_EFFECT_OUT
{
	float4		vColor : SV_TARGET0;
	float4		vGlow : SV_TARGET1;
	float4		vDrawEffect : SV_TARGET2;	//	Player, AnimMode Ȥ�� Effect �� ���� �׷���.
};

struct PS_OUT_SHADOW
{
	float4			vLightDepth :  SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float3	vNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV).xyz;

	vNormal = vNormal * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
	Out.vWorld = In.vWorld/* / g_fFar*/;

	if (Out.vColor.a < 0.5f)
		discard;

	return Out;
}

PS_OUT_SHADOW PS_SHADOW(PS_IN In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / g_fFar;

	Out.vLightDepth.a = 1.f;

	return Out;
}

struct PS_NONLIGHT_OUT
{
	float4		vColor : SV_TARGET0;
};

PS_NONLIGHT_OUT PS_NONLIGHT(PS_IN In)
{
	PS_NONLIGHT_OUT		Out = (PS_NONLIGHT_OUT)0;

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor = vTexture * 0.7f;

	if (Out.vColor.a < 0.3f)
		discard;

	return Out;
}

struct PS_FLOWMAP_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float2		vTexCoord1 : TEXCOORD2;
	float4		vWorld : TEXCOORD3;
};

PS_EFFECT_OUT PS_SMELL(PS_FLOWMAP_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	//	NoiseTexture�� UV = In.vTexCoord1
	float2 noise1 = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord1);
	float2 noise2 = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord1 * 2.f);
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;

	float2 distortion1 = float2(g_fDistortionU, g_fDistortionV);
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion1.xy;

	float2	finalNoise = noise1.x + noise2.y;		//	Noise[0]�� X��, Noise[1]�� Y�� �ְ��մϴ�.	
	float	perturb = (g_fDistortionScale)+g_fDistortionBias;

	float2		vNewUV = In.vTexUV;

	float2	noiseCoords = (finalNoise.xy * perturb) + vNewUV;
	noiseCoords.x += g_fMoveUV_U;
	noiseCoords.y += g_fMoveUV_V;
	//	�ؽ��� �ְ� ��


	float2	vTexUVMul = float2(g_iMulUV_U, g_iMulUV_V);

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, noiseCoords);
	Out.vColor = vTexture;

	vector		vMaskTexture = g_MaskTexture.Sample(LinearSampler, noiseCoords);

	Out.vDrawEffect = Out.vColor;

	if (Out.vColor.a < 0.3f)
		discard;

	return Out;
}






technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass SHADOW		//	1
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Shadow, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW();
	}

	pass Flowmap		//	2
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_FLOWMAP();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SMELL();
	}

	pass Wind		//	3
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_WIND();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass NonLightObj	//	4
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NONLIGHT();
	}

	pass Wind_NonLight	//	5
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_WIND();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NONLIGHT();
	}

}