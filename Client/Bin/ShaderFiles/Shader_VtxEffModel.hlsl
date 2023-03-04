
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D		g_DiffuseTexture;
texture2D		g_NoiseTexture;
texture2D		g_DissolveTexture;
texture2D		g_MaskTexture;

float			g_fPostProcesesingValue;
float			g_fEndALPHA;
float			g_fAlphaRatio;

float4			g_vColor;
float3			g_vGlowColor;

vector			g_vCamPosition;

bool			g_bDisappearStart;				// 디졸브 시작때까지 디졸브 텍스쳐의 값을 0으로 만들기 위한 변수
bool			g_bDissolve;					// false 는 디졸브 true는 알파
bool			g_bUseMask;
bool			g_bGlow;						//	글로우를 사용합니다.
bool			g_bUseGlowColor;				//	글로우 색상을 독립적으로 사용합니다.
bool			g_bUseRGB;						//	텍스쳐의 RGB를 사용합니다. false == a 사요
bool			g_bUseColor;					//	g_vColor를 사용합니다.. false == g_DiffuseTexture 사용
bool			g_bFlowMap;						//	FlowMap을 사용합니다.		//	추가 동반 텍스쳐 : Noise

												//	New
int				g_iMulUV_U;
int				g_iMulUV_V;

int				g_iFrame;
int				g_iNumUV_U;
int				g_iNumUV_V;

bool			g_bUVUpset = false;

//	Flow Texture Value

float			g_fDistortionScale = 1.f;		//	왜곡 수치
float			g_fDistortionBias = 0.f;		//	초기 왜곡값

float			g_fDistortionU;					//	UV좌표의 U 왜곡 값
float			g_fDistortionV;					//	UV좌표의 V 왜곡 값

float			g_fMoveUV_U;					//	기본적인 UV좌표의 움직임값입니다.
float			g_fMoveUV_V;					//	기본적인 UV좌표의 움직임값입니다.

float			g_fDisappearTimeRatio;			//	사라지는 시간 비율


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

	float2	vTexUVMul = float2(g_iMulUV_U, g_iMulUV_V);

	//Out.vTexUV = In.vTexUV * vTexUVMul;

	float			fTexU = ((g_iFrame % g_iNumUV_U) + In.vTexUV.x) / g_iNumUV_U;
	float			fTexV = ((g_iFrame / g_iNumUV_U) + In.vTexUV.y) / g_iNumUV_V;

	if (g_iNumUV_U == 0)
		fTexU = 0;
	if (g_iNumUV_V == 0)
		fTexV = 0;

	Out.vTexUV = In.vTexUV * vTexUVMul + float2(fTexU, fTexV);

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
	Out.vProjPos = Out.vPosition;

	float2	vTexUVMul = float2(g_iMulUV_U, g_iMulUV_V);
	Out.vTexUV = In.vTexUV * vTexUVMul;
	Out.vTexCoord1.x = In.vTexUV.x + ((1.f - g_fEndALPHA) * 0.1f);
	Out.vTexCoord1.y = In.vTexUV.y + ((1.f - g_fEndALPHA) * 0.1f);


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
	vector		vDissolveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_vColor;

	Out.vColor.a = g_vColor.a * ((g_bDissolve * saturate(DiffuseTex.a - g_fAlphaRatio)) +
		((1 - g_bDissolve) * saturate(DiffuseTex.a - saturate(vDissolveTexture.r * g_bDisappearStart + g_fAlphaRatio))));

	if (Out.vColor.a <= 1 - g_fEndALPHA)
		discard;

	return Out;
}

PS_OUT_COLOR PS_COLORTEST(PS_IN In)
{
	PS_OUT_COLOR		Out = (PS_OUT_COLOR)0;

	float4 DiffuseTex = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector		vDissolveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_vColor;

	Out.vColor.a = g_vColor.a * ((g_bDissolve * saturate(DiffuseTex.a - g_fAlphaRatio)) +
		((1 - g_bDissolve) * saturate(DiffuseTex.a - saturate(vDissolveTexture.r * g_bDisappearStart + g_fAlphaRatio))));

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
	float4		vEffect : SV_TARGET2;
};

PS_EFFECT_OUT PS_EFF_MAIN(PS_EFFECT_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	vector	vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector	vDissolveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);

	float	fColorPower = max(max(vTexture.x, vTexture.y), max(vTexture.y, vTexture.z));
	float fTexAlpha = saturate((1 - g_bUseRGB) * vTexture.a) + (g_bUseRGB * fColorPower);
	float fDissolveAlpha = saturate((((1 - g_bDissolve) * max(max(vDissolveTexture.x, vDissolveTexture.y), max(vDissolveTexture.y, vDissolveTexture.z))) * g_bDisappearStart + g_fAlphaRatio) +
		(g_bDissolve * g_fAlphaRatio));

	Out.vColor.a = saturate(saturate(g_bUseColor * (g_vColor.a * fTexAlpha))
		+ saturate((1 - g_bUseColor) * (fTexAlpha)) - fDissolveAlpha);

	Out.vEffect = Out.vColor;

	if (Out.vColor.a < 0.03f)
		discard;

	return Out;
}

PS_EFFECT_OUT PS_EFF_COLORBLEND(PS_EFFECT_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_bUseColor * g_vColor + (1.f - g_bUseColor) * vTexture;
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor)) * g_bGlow;

	vector		vDissolveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
	vector		vMaskTexture = g_MaskTexture.Sample(LinearSampler, In.vTexUV);
	//g_bUseColor  g_bUseRGB

	float	fColorPower = max(max(vTexture.x, vTexture.y), max(vTexture.y, vTexture.z));
	float fTexAlpha = saturate((1 - g_bUseRGB) * vTexture.a) + (g_bUseRGB * fColorPower);
	float fDissolveAlpha = saturate((((1 - g_bDissolve) * max(max(vDissolveTexture.x, vDissolveTexture.y), max(vDissolveTexture.y, vDissolveTexture.z))) * g_bDisappearStart + g_fAlphaRatio) +
		(g_bDissolve * g_fAlphaRatio));

	Out.vColor.a = saturate(saturate(g_bUseColor * (g_vColor.a * fTexAlpha))
		+ saturate((1 - g_bUseColor) * fTexAlpha) - fDissolveAlpha);

	//Out.vColor.a = saturate((g_vColor.a * ((g_bDissolve * saturate(g_bUseRGB * vTexture.x + (1 - g_bUseRGB) * vTexture.a - g_fAlphaRatio)) +
	//((1 - g_bDissolve) * saturate(g_bUseRGB * vTexture.x + (1 - g_bUseRGB) * vTexture.a - saturate(vDissolveTexture.r * g_bDisappearStart + g_fAlphaRatio))))) - (g_UseMask * (vMaskTexture.r)));

	Out.vColor.a = Out.vColor.a * saturate((1 - g_bUseMask) + vMaskTexture.r);

	Out.vGlowColor.a = Out.vColor.a * g_bGlow;
	Out.vEffect = Out.vColor;


	if (Out.vColor.a < 0.03f)
		discard;

	return Out;
}

PS_EFFECT_OUT PS_EFF_COLORTEST(PS_EFFECT_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_bUseColor * g_vColor + (1.f - g_bUseColor) * vTexture;
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor)) * g_bGlow;

	vector		vDissolveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
	vector		vMaskTexture = g_MaskTexture.Sample(LinearSampler, In.vTexUV);

	float	fColorPower = max(max(vTexture.x, vTexture.y), max(vTexture.y, vTexture.z));
	float fTexAlpha = saturate((1 - g_bUseRGB) * vTexture.a) + (g_bUseRGB * fColorPower);
	float fDissolveAlpha = saturate((((1 - g_bDissolve) * max(max(vDissolveTexture.x, vDissolveTexture.y), max(vDissolveTexture.y, vDissolveTexture.z))) * g_bDisappearStart + g_fAlphaRatio) +
		(g_bDissolve * g_fAlphaRatio));

	Out.vColor.a = saturate(saturate(g_bUseColor * (g_vColor.a * fTexAlpha))
		+ saturate((1 - g_bUseColor) * (fTexAlpha)) - fDissolveAlpha);

	//Out.vColor.a = saturate((g_vColor.a * ((g_bDissolve * saturate(g_bUseRGB * vTexture.x + (1 - g_bUseRGB) * vTexture.a - g_fAlphaRatio)) +
	//((1 - g_bDissolve) * saturate(g_bUseRGB * vTexture.x + (1 - g_bUseRGB) * vTexture.a - saturate(vDissolveTexture.r * g_bDisappearStart + g_fAlphaRatio))))) - (g_UseMask * (vMaskTexture.r)));
	Out.vColor.a = Out.vColor.a * saturate((1 - g_bUseMask) + vMaskTexture.r);
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a <= 0.1f)
		discard;

	Out.vColor.a = 1.f;
	Out.vEffect = Out.vColor;


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
	float4 NoiseTex = g_NoiseTexture.Sample(LinearSampler, In.vTexUV);

	float	fDistortionValue = max(max(vTexture.r, vTexture.g), vTexture.b);
	float fValueX = (g_bUseRGB * fDistortionValue + (1 - g_bUseRGB) * vTexture.a) * g_fEndALPHA;
	float fValueY = fValueX * NoiseTex.r;		//	방향은 NoiseTexture를 따라가되, 왜곡 정도는 DiffuseTexture를 따라갑니다.


	Out.vValue = fValueX * g_fPostProcesesingValue;
	Out.vValue.y = fValueY * g_fPostProcesesingValue;
	Out.vValue.x = 1;
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

	float	fGrayScaleValue = max(max(vTexture.r, vTexture.g), vTexture.b);
	float fValue = (g_bUseRGB * fGrayScaleValue) + ((1.f - g_bUseRGB) * vTexture.a) * g_fEndALPHA;
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

	Out.vColor = g_bUseColor * g_vColor + (1.f - g_bUseColor) * vTexture;
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor)) * g_bGlow;

	Out.vColor = g_bUseColor * g_vColor +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor)) * g_bGlow;

	vector		vDissolveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
	vector		vMaskTexture = g_MaskTexture.Sample(LinearSampler, In.vTexUV);

	float	fColorPower = max(max(vTexture.x, vTexture.y), max(vTexture.y, vTexture.z));
	float fTexAlpha = saturate((1 - g_bUseRGB) * vTexture.a) + (g_bUseRGB * fColorPower);
	float fDissolveAlpha = saturate((((1 - g_bDissolve) * max(max(vDissolveTexture.x, vDissolveTexture.y), max(vDissolveTexture.y, vDissolveTexture.z))) * g_bDisappearStart + g_fAlphaRatio) +
		(g_bDissolve * g_fAlphaRatio));

	Out.vColor.a = saturate(saturate(g_bUseColor * (g_vColor.a * fTexAlpha))
		+ saturate((1 - g_bUseColor) * (fTexAlpha)) - fDissolveAlpha);

	Out.vColor.a = Out.vColor.a * saturate((1 - g_bUseMask) + vMaskTexture.r);
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;
	Out.vEffect = Out.vColor;

	if (Out.vColor.a <= 0.03f)
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

PS_EFFECT_OUT PS_FLOWMAP(PS_FLOWMAP_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	//	NoiseTexture의 UV = In.vTexCoord1
	float2 noise1 = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord1);
	float2 noise2 = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord1 * 2.f);
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;

	float2 distortion1 = float2(g_fDistortionU, g_fDistortionV);
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion1.xy;

	float2	finalNoise = noise1.x + noise2.y;		//	Noise[0]는 X를, Noise[1]는 Y를 왜곡합니다.
	float	fAliveTimeRatio = saturate(saturate((1.f - g_fEndALPHA)) - 0.01f);
	float	perturb = (fAliveTimeRatio * g_fDistortionScale) + g_fDistortionBias;

	float2		vNewUV = In.vTexUV;
	float2	noiseCoords = (finalNoise.xy * perturb) + vNewUV;
	noiseCoords.x += g_fMoveUV_U;
	noiseCoords.y += g_fMoveUV_V;
	//	텍스쳐 왜곡 끝


	float2	vTexUVMul = float2(g_iMulUV_U, g_iMulUV_V);

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, noiseCoords);

	Out.vColor = g_bUseColor * g_vColor +
		(1.f - g_bUseColor) * vTexture;
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor)) * g_bGlow;

	vector		vDissolveTexture = g_DissolveTexture.Sample(LinearSampler, noiseCoords);
	vector		vMaskTexture = g_MaskTexture.Sample(LinearSampler, noiseCoords);

	float	fColorPower = max(max(vTexture.x, vTexture.y), max(vTexture.y, vTexture.z));
	float fTexAlpha = saturate((1 - g_bUseRGB) * vTexture.a) + (g_bUseRGB * fColorPower);
	float fDissolveAlpha = saturate((((1 - g_bDissolve) * max(max(vDissolveTexture.x, vDissolveTexture.y), max(vDissolveTexture.y, vDissolveTexture.z))) * g_bDisappearStart + g_fAlphaRatio) +
		(g_bDissolve * g_fAlphaRatio));

	Out.vColor.a = saturate(saturate(g_bUseColor * (g_vColor.a * fTexAlpha))
		+ saturate((1 - g_bUseColor) * (fTexAlpha)) - fDissolveAlpha);

	Out.vColor.a = Out.vColor.a * saturate((1 - g_bUseMask) + vMaskTexture.r);
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;
	Out.vEffect = Out.vColor;

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}
PS_OUT PS_Map(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1300.f, 0.f, 0.f);

	//if (Out.vDiffuse.a <= 0.6f)
	//	discard;


	return Out;
}

PS_POSTPROCESSING_OUT PS_DISTORTION_FLOWMAP(PS_FLOWMAP_IN In)
{
	PS_POSTPROCESSING_OUT		Out = (PS_POSTPROCESSING_OUT)0;
	//	x 만큼 가로축으로 왜곡합니다.
	//	필요한 데이터 = r

	//	NoiseTexture의 UV = In.vTexCoord1
	float2 noise1 = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord1);
	float2 noise2 = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord1 * 2.f);
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;

	float2 distortion1 = float2(g_fDistortionU, g_fDistortionV);
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion1.xy;

	float2	finalNoise = noise1.x + noise2.y;		//	Noise[0]는 X를, Noise[1]는 Y를 왜곡합니다.
	float	fAliveTimeRatio = saturate(saturate((1.f - g_fEndALPHA)) - 0.01f);
	float	perturb = (fAliveTimeRatio * g_fDistortionScale) + g_fDistortionBias;

	float2		vNewUV = In.vTexUV;
	float2	noiseCoords = (finalNoise.xy * perturb) + vNewUV;
	noiseCoords.x += g_fMoveUV_U;
	noiseCoords.y += g_fMoveUV_V;
	//	텍스쳐 왜곡 끝



	float4 DiffuseTex = g_DiffuseTexture.Sample(LinearSampler, noiseCoords);
	float4 NoiseTex = g_NoiseTexture.Sample(LinearSampler, noiseCoords);

	float	fDistortionValue = max(max(DiffuseTex.r, DiffuseTex.g), DiffuseTex.b);
	float fValueX = (g_bUseRGB * fDistortionValue + (1 - g_bUseRGB) * DiffuseTex.a) * g_fEndALPHA;
	float fValueY = fValueX * NoiseTex.r;		//	방향은 NoiseTexture를 따라가되, 왜곡 정도는 DiffuseTexture를 따라갑니다.


	Out.vValue = fValueX * g_fPostProcesesingValue;
	Out.vValue.y = fValueY * g_fPostProcesesingValue;

	if (Out.vValue.x <= 0.03f)
		discard;

	return Out;
}

PS_POSTPROCESSING_OUT PS_GRAYSCALE_FLOWMAP(PS_FLOWMAP_IN In)
{
	PS_POSTPROCESSING_OUT		Out = (PS_POSTPROCESSING_OUT)0;
	//	rgb에서 r을 기준으로 g, b를 x 만큼 선형보간 해서 변조합니다. (1 == 회색, 0 == 원래 색)
	//	필요한 데이터 = r


	//	NoiseTexture의 UV = In.vTexCoord1
	float2 noise1 = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord1);
	float2 noise2 = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord1 * 2.f);
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;

	float2 distortion1 = float2(g_fDistortionU, g_fDistortionV);
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion1.xy;

	float2	finalNoise = noise1.x + noise2.y;		//	Noise[0]는 X를, Noise[1]는 Y를 왜곡합니다.
	float	fAliveTimeRatio = saturate(saturate((1.f - g_fEndALPHA)) - 0.01f);
	float	perturb = (fAliveTimeRatio * g_fDistortionScale) + g_fDistortionBias;

	float2		vNewUV = In.vTexUV;
	float2	noiseCoords = (finalNoise.xy * perturb) + vNewUV;
	noiseCoords.x += g_fMoveUV_U;
	noiseCoords.y += g_fMoveUV_V;
	//	텍스쳐 왜곡 끝


	float4 DiffuseTex = g_DiffuseTexture.Sample(LinearSampler, noiseCoords);
	float	fGrayScaleValue = max(max(DiffuseTex.r, DiffuseTex.g), DiffuseTex.b);

	float fValue = (g_bUseRGB * fGrayScaleValue) + ((1.f - g_bUseRGB) * DiffuseTex.a);
	Out.vValue = fValue * g_fPostProcesesingValue;

	if (Out.vValue.r <= 0.03f)
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
		PixelShader = compile ps_5_0 PS_FLOWMAP();
	}

	pass FlowMap_AlphaTest	//	10
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_FLOWMAP();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FLOWMAP();
	}
	pass Map // 11
	{
		SetRasterizerState(RS_Map);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Map();
	}

	pass FlowMap_Distortion 	//	12
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_FLOWMAP();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISTORTION_FLOWMAP();
	}

	pass FlowMap_GrayScale	//	13
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_FLOWMAP();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GRAYSCALE_FLOWMAP();
	}
}