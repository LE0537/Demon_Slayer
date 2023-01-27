
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
texture2D		g_NoiseTexture[3];
texture2D		g_DissolveTexture;

vector			g_vCamPosition;

float4			g_vColor;
float3			g_vGlowColor;

float			g_fEndALPHA;
bool			g_UseMask;
bool			g_bDisjolve;
bool			g_bGlow;			//	글로우를 사용합니다.
bool			g_bUseGlowColor;	//	글로우 색상을 독립적으로 사용합니다.
bool			g_bUseRGB;			//	텍스쳐의 RGB를 사용합니다. false == a 사요
bool			g_bUseColor;		//	g_vColor를 사용합니다.. false == g_DiffuseTexture 사용
bool			g_bFlowMap;			//	FlowMap을 사용합니다.
									//	추가 동반 텍스쳐 : Noise
bool			g_iNumFlowMap;		//	사용할 FlowMap ( Noise Texture ) 입니다.

float			g_fAliveTimeRatio;
float			g_fPostProcesesingValue;

//	Flow Texture Value
float	g_fDistortionScale = 1.f;		//	왜곡 수치
float	g_fDistortionBias = 0.f;		//	초기 왜곡값

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	vector vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = vNormal;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	return Out;
}

struct VS_FLOWMAP_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float2		vTexCoord1 : TEXCOORD2;		//	플로우맵 정보를 저장합니다.
};

VS_FLOWMAP_OUT VS_FLOWMAP(VS_IN In)
{
	VS_FLOWMAP_OUT		Out = (VS_FLOWMAP_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	vector vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = vNormal;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	Out.vTexCoord1 = (In.vTexUV * 1.f);
	Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fAliveTimeRatio * 0.1f);

	return Out;
}





//========================================================================
//==============================Pixel Shader==============================
//========================================================================

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;

};
struct PS_OUT_SHADOW
{
	float4			vLightDepth :  SV_TARGET0;
};

struct PS_OUT_COLOR
{
	float4			vColor :  SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1300.f, 0.f, 0.f);


	if (Out.vDiffuse.a <= 0.3f)
		discard;

	return Out;
}
PS_OUT_SHADOW PS_SHADOW(PS_IN In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / 1300.f;

	Out.vLightDepth.a = 1.f;

	return Out;
}

PS_OUT_COLOR PS_COLORBLEND(PS_IN In)
{
	PS_OUT_COLOR		Out = (PS_OUT_COLOR)0;

	float4 DiffuseTex = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_vColor;

	Out.vColor.a = g_vColor.a * DiffuseTex.x * g_fEndALPHA;

	if (Out.vColor.a <= 1 - g_fEndALPHA)
		discard;

	return Out;
}

PS_OUT_COLOR PS_COLORTEST(PS_IN In)
{
	PS_OUT_COLOR		Out = (PS_OUT_COLOR)0;

	float4 DiffuseTex = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_vColor;

	Out.vColor.a = g_vColor.a * DiffuseTex.x * g_fEndALPHA;

	if (Out.vColor.a <= 0.3f)
		discard;

	return Out;
}



//=======================================================================
//=============================Effect Shader=============================
//=======================================================================

struct PS_EFFECT_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

struct PS_EFFECT_OUT
{
	float4		vColor : SV_TARGET0;
	float4		vGlowColor : SV_TARGET1;
};

PS_EFFECT_OUT PS_EFF_MAIN(PS_EFFECT_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	vector	vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a = g_vColor.a * min(vTexture.x, vTexture.a) * g_fEndALPHA;

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}

PS_EFFECT_OUT PS_EFF_COLORBLEND(PS_EFFECT_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_bUseColor * g_vColor +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	vector		vDissolveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a = (g_vColor.a * vTexture.x * g_fEndALPHA * (1 - g_UseMask)) + (g_UseMask * vDissolveTexture.r);
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a < 0.03f)
		discard;

	return Out;
}

PS_EFFECT_OUT PS_EFF_COLORTEST(PS_EFFECT_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_bUseColor * g_vColor +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	vector		vDissolveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a = (g_vColor.a * min(vTexture.x, vTexture.a) * (1 - g_UseMask) * g_fEndALPHA) + (g_UseMask * vDissolveTexture.r);
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a <= 0.1f)
		discard;

	Out.vColor.a = 1.f;

	return Out;
}



struct PS_POSTPROCESSING_OUT
{
	float4		vValue : SV_TARGET0;
};

PS_POSTPROCESSING_OUT PS_DISTORTION(PS_EFFECT_IN In)
{
	PS_POSTPROCESSING_OUT		Out = (PS_POSTPROCESSING_OUT)0;
	//	x 만큼 가로축으로 왜곡합니다.
	//	필요한 데이터 = r

	float4 vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	float4 NoiseTex = g_NoiseTexture[0].Sample(LinearSampler, In.vTexUV);
	float fValueX = min(vTexture.r, vTexture.a) * (1.f - g_fAliveTimeRatio);
	float fValueY = fValueX * NoiseTex.r;		//	방향은 NoiseTexture를 따라가되, 왜곡 정도는 DiffuseTexture를 따라갑니다.


	Out.vValue = fValueX * g_fPostProcesesingValue;
	Out.vValue.y = fValueY * g_fPostProcesesingValue;

	if (Out.vValue.x <= 0.03f)
		discard;

	return Out;
}

PS_POSTPROCESSING_OUT PS_GRAYSCALE(PS_EFFECT_IN In)
{
	PS_POSTPROCESSING_OUT		Out = (PS_POSTPROCESSING_OUT)0;
	//	rgb에서 r을 기준으로 g, b를 x 만큼 선형보간 해서 변조합니다. (1 == 회색, 0 == 원래 색)
	//	필요한 데이터 = r

	float4 vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	float fValue = (g_bUseRGB * vTexture.r) + ((1.f - g_bUseRGB) * vTexture.a) * (1.f - g_fAliveTimeRatio);
	Out.vValue = fValue * g_fPostProcesesingValue;

	if (Out.vValue.r <= 0.03f)
		discard;

	return Out;
}

PS_EFFECT_OUT PS_ALPHAGLOW(PS_EFFECT_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;
	//	Alpha용 Glow 입니다. - 타겟이 다릅니다.

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_bUseColor * g_vColor +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	Out.vColor.a = min(vTexture.x, vTexture.a) * g_fEndALPHA;
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a <= 0.1f)
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
};

PS_EFFECT_OUT PS_FLOWMAP_ALPHAGLOW(PS_FLOWMAP_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	//	NoiseTexture의 UV = In.vTexCoord1
	float2 noise1 = g_NoiseTexture[0].Sample(LinearSampler, In.vTexCoord1);
	noise1 = (noise1 - 0.5f) * 2.0f;

	float2 distortion1 = float2(0.1f, 0.1f);
	noise1.xy = noise1.xy * distortion1.xy;

	float2	finalNoise = noise1;

	float	fAliveTimeRatio = saturate(saturate(g_fAliveTimeRatio) - 0.01f);

	float	perturb = (fAliveTimeRatio * g_fDistortionScale) + g_fDistortionBias;


	float2		vNewUV = In.vTexUV;
	vNewUV.x = saturate(vNewUV.x);
	vNewUV.y = saturate(vNewUV.y);

	float2	noiseCoords = (finalNoise.xy * perturb) + vNewUV;



	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, noiseCoords);

	Out.vColor = g_bUseColor * g_vColor +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	Out.vColor.a = min(vTexture.x, vTexture.a) * g_fEndALPHA;
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a < 0.1f)
		discard;


	return Out;
}

PS_EFFECT_OUT PS_FLOWMAP_ALPHATEST(PS_FLOWMAP_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	//	NoiseTexture의 UV = In.vTexCoord1
	float2 noise1 = g_NoiseTexture[0].Sample(LinearSampler, In.vTexCoord1);
	noise1 = (noise1 - 0.5f) * 2.0f;

	float2 distortion1 = float2(0.1f, 0.1f);
	noise1.xy = noise1.xy * distortion1.xy;

	float2	finalNoise = noise1;

	float	fAliveTimeRatio = saturate(saturate(g_fAliveTimeRatio) - 0.01f);

	float	perturb = (fAliveTimeRatio * g_fDistortionScale) + g_fDistortionBias;


	float2		vNewUV = In.vTexUV;
	vNewUV.x = saturate(vNewUV.x);
	vNewUV.y = saturate(vNewUV.y);

	float2	noiseCoords = (finalNoise.xy * perturb) + vNewUV;



	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, noiseCoords);

	Out.vColor = g_bUseColor * g_vColor +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	Out.vColor.a = min(vTexture.x, vTexture.a) * g_fEndALPHA;
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a < 0.03f)
		discard;

	return Out;
}

technique11 DefaultTechnique
{
	pass Default //0
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass SHADOW //1
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Shadow, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW();
	}

	pass COLORBLEND //2
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_COLORBLEND();
	}

	pass COLORTEST //3
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_COLORTEST();
	}

	//	Effect Pass
	pass EffectBlend		//	4
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EFF_COLORBLEND();
	}

	pass EffectTest		//	5
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EFF_COLORTEST();
	}

	pass Distortion		//	6
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISTORTION();
	}

	pass GrayScale	//	7
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GRAYSCALE();
	}

	pass AlphaGlow	//	8
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHAGLOW();
	}

	pass FlowMap_Alpha	//	9
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_FLOWMAP();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FLOWMAP_ALPHAGLOW();
	}

	pass FlowMap_AlphaTest	//	10
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_FLOWMAP();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FLOWMAP_ALPHATEST();
	}
}