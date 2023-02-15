
matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_ViewMatrixInv, g_ProjMatrixInv;

matrix		    g_matLightView;
matrix		    g_matLightView_Static;
matrix			g_matLightProj;

vector			g_vCamPosition;

vector			g_vLightDir;
vector			g_vLightDir2 = vector(1.f, -1.f, 1.f, 0.f);
vector			g_vLightPos;
float			g_fLightRange;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;

vector			g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

bool			g_bRenderAO;

texture2D		g_DiffuseTexture;
texture2D		g_NormalTexture;
texture2D		g_DepthTexture;
texture2D		g_ShadeTexture;
texture2D		g_SpecularTexture;
texture2D       g_ShadowDepthTexture;
texture2D       g_ShadowDepthTexture_Once;
texture2D		g_GlowTexture;
texture2D		g_DistortionTexture;
texture2D		g_AOTexture;
texture2D		g_BlurTexture;
texture2D		g_GrayScaleTexture;
texture2D		g_AddTexture;
texture2D		g_PlayerTexture;
texture2D		g_EffectTexture;

float			g_fWinSizeX = 1280.f;
float			g_fWinSizeY = 720.f;
float			g_fFar;

float			g_fSSAORadius;
float			g_fAOValue;
float			g_fGlowBlurCount;
float			g_fDistortionValue;
float			g_fOutLineValue;
float			g_fInnerLineValue;
float			g_fFogDistance;
float			g_fFogRange;
float3			g_vFogColor;
float			g_fEnvLightValue = 1.f;
float			g_fLightPower = 1.f;
float			g_fShadowTestLength = 1.f;

float			g_fAddValue;
float			g_fMapGrayScaleTimeRatio;
float			g_fMapGrayScaleFogRange;
float			g_fMapGrayScalePower;

const float		g_fWeight[13] =
{
	0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.0f,
	0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f
};
const float		g_fWeightTotal = 6.2108;


const float		g_fWeight29[29] =
{
	0.018f, 0.031f, 0.052f, 0.083f, 0.128f, 0.189f, 0.268f, 0.365f, 0.477f, 0.598f, 0.719f, 0.831f, 0.921f, 0.98f, 1.0f,
	0.98f, 0.921f, 0.831f, 0.719f, 0.598f, 0.477f, 0.365f, 0.268f, 0.189f, 0.128f, 0.083f, 0.052f, 0.031f, 0.018f
};
const float		g_fWeight29Total = 12.339f;

const float		g_fWeight41[41] =
{
	0.007, 0.011, 0.017, 0.027, 0.041, 0.06, 0.086, 0.121, 0.165, 0.22, 0.287, 0.363, 0.449, 0.542, 0.638, 0.732, 0.819, 0.894, 0.951, 0.988, 1,
	0.988, 0.951, 0.894, 0.819, 0.732, 0.638, 0.542, 0.449, 0.363, 0.287, 0.22, 0.165, 0.121, 0.086, 0.06, 0.041, 0.027, 0.017, 0.011, 0.007
};
const float		g_fWeight41Total = 12.339f;

const float		g_fWeight41_Big[41] =
{
	0.041,0.056,0.075,0.099,0.129,0.165,0.208,0.259,0.316,0.38, 0.449,0.523,0.599,0.676,0.75, 0.819,0.88, 0.931,0.969,0.992,1,
	0.992, 0.969, 0.931, 0.88,  0.819, 0.75,  0.676, 0.599, 0.523, 0.449, 0.38,  0.316, 0.259, 0.208, 0.165, 0.129, 0.099, 0.075, 0.056, 0.041
};
const float		g_fWeight41BigTotal = 19.632;


float3 g_vRandom[16] =
{
	float3(0.2024537f, 0.841204f, -0.9060141f),
	float3(-0.2200423f, 0.6282339f, -0.8275437f),
	float3(0.3677659f, 0.1086345f, -0.4466777f),
	float3(0.8775856f, 0.4617546f, -0.6427765f),
	float3(0.7867433f, -0.141479f, -0.1567597f),
	float3(0.4839356f, -0.8253108f, -0.1563844f),
	float3(0.4401554f, -0.4228428f, -0.330118f),
	float3(0.0019193f, -0.8048455f, 0.0726584f),
	float3(-0.7578573f, -0.5583301f, 0.2347527f),
	float3(-0.4540f, -0.2527426f, 0.59244601f),
	float3(-0.0483543f, -0.2527567f, 0.5921463f),
	float3(-0.4192315f, 0.2084311f, -0.3672158f),
	float3(-0.8433158f, 0.1546315f, 0.2204781f),
	float3(-0.4031129f, -0.8261332f, 0.4619123f),
	float3(-0.6657215f, 0.6291122f, 0.6120921f),
	float3(-0.0001812f, 0.2781219f, 0.8001631f),
};

float3 randomNormal(float2 vTex)
{
	float fnoiseX = (frac(sin(dot(vTex, float2(13.2135f, 64.215f) * 1.f)) * 12345.2785f));
	float fnoiseY = (frac(sin(dot(vTex, float2(10.231f, 41.9234f) * 2.f)) * 30514.6549f));
	float fnoiseZ = (frac(sin(dot(vTex, float2(16.1345f, 97.4231f) * 3.f)) * 59401.1312f));

	return normalize(float3(fnoiseX, fnoiseY, fnoiseZ));
};



sampler LinearSampler = sampler_state
{
	filter = min_mag_mip_Linear;
};
sampler DepthSampler = sampler_state
{
	filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = clamp;
	AddressV = clamp;
};



struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* 이렇게 만들어진 픽셀을 PS_MAIN함수의 인자로 던진다. */
/* 리턴하는 색은 Target0 == 장치에 0번째에 바인딩되어있는 렌더타겟(일반적으로 백버퍼)에 그린다. */
/* 그래서 백버퍼에 색이 그려진다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_SSAO(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (0 == g_DepthTexture.Sample(LinearSampler, In.vTexUV).y)
		return Out;

	float3	vRay;
	float3	vReflect;
	float2	vRandomUV;

	int		iColor = 0;

	vector	vNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV);

	for (int i = 0; i < 16; ++i)
	{
		vRay = reflect(randomNormal(In.vTexUV), g_vRandom[i]);
		vReflect = normalize(reflect(vRay, vNormal)) * g_fSSAORadius;
		vReflect.x *= -1.f;
		vRandomUV = In.vTexUV + vReflect.xy;
		float	fDepth = (g_DepthTexture.Sample(LinearSampler, In.vTexUV)).y;
		float	fOccNorm = (g_DepthTexture.Sample(LinearSampler, vRandomUV)).y;

		iColor += -1.f * floor(fOccNorm - (fDepth + (g_fAOValue * (g_DepthTexture.Sample(LinearSampler, In.vTexUV)).y)));
	}

	Out.vColor = abs((iColor / 16.f) - 1.f);


	return Out;
}

struct PS_OUT_LIGHT
{
	float4		vShade : SV_TARGET0;
	float4		vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	/* 0 ~ 1 => -1 ~ 1*/
	vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);

	float			fViewZ = vDepthDesc.y * g_fFar;

	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);



	if (vNormalDesc.w == 1.f)
	{
		float fDot = ceil(saturate(dot(normalize(g_vLightDir) * -1.f, normalize(vNormal))) * 3.f) / 3.f;


		Out.vShade = g_vLightDiffuse * ((fDot + 0.4f) + (g_vLightAmbient * g_vMtrlAmbient));
	}
	else
	{
		Out.vShade = g_vLightDiffuse * g_fEnvLightValue * (saturate(dot(normalize(g_vLightDir2) * -1.f, normalize(vNormal))) + (g_vLightAmbient * g_vMtrlAmbient));
	}

	Out.vShade *= g_fLightPower;
	Out.vShade.a = 1.f;

	vector			vWorldPos = (vector)0.f;

	/* 투영 공간상의 위치를 구했다. */
	/* 투영 공간 == 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 / w */
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.r;
	vWorldPos.w = 1.0f;

	/* 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
	vWorldPos *= fViewZ;

	/* 뷰 공간상의 위치르 ㄹ구한다. */
	/* 로컬점의위치 * 월드행렬 * 뷰행렬  */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬점의위치 * 월드행렬   */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector			vReflect;
	if (vNormalDesc.w == 1.f)
	{
		vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
	}
	else
	{
		vReflect = reflect(normalize(g_vLightDir2), normalize(vNormal));
	}
	vector			vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = 0.f;

	//Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 256.f);
	//Out.vSpecular.a = 0.f;
	

	return Out;
}
PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	/* 0 ~ 1 => -1 ~ 1*/
	vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);

	float			fViewZ = vDepthDesc.y * g_fFar;

	vector			vWorldPos = (vector)0.f;

	/* 투영 공간상의 위치를 구했다. */
	/* 투영 공간 == 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 / w */
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.r;
	vWorldPos.w = 1.0f;

	/* 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
	vWorldPos *= fViewZ;

	/* 뷰 공간상의 위치르 ㄹ구한다. */
	/* 로컬점의위치 * 월드행렬 * 뷰행렬  */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬점의위치 * 월드행렬   */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);


	vector			vLightDir = vWorldPos - g_vLightPos;

	float			fDistance = length(vLightDir);
	float			fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);

	if (vNormalDesc.w == 2.f)
	{
		Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(vLightDir) * -1.f, normalize(vNormal))) + 0.6f);

		Out.vShade *= fAtt;
	}
	else
	{
		fAtt = saturate((1.f - fDistance) / 1.f);
		Out.vShade *= fAtt;
	}
	Out.vShade.a = 1.f;


	vector			vReflect = reflect(normalize(vLightDir), normalize(vNormal));
	vector			vLook = vWorldPos - g_vCamPosition;


	if (vNormalDesc.w == 2.f)
	{
		Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 20.f);
		Out.vSpecular *= fAtt;
		Out.vSpecular.a = 0.f;
	}
	else
		Out.vSpecular = 0.f;


	return Out;
}

PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);

	vector			vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector			vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);
	vector			vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
	vector			vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	vector			vAO = 0.1f * g_AOTexture.Sample(LinearSampler, In.vTexUV) * g_bRenderAO;

	Out.vColor = ((vDiffuse - vAO) * vShade + vSpecular);

	//=================== 그림자 =======================================================

	vector			vDepthDesc = g_DepthTexture.Sample(DepthSampler, In.vTexUV);

	float			fViewZ = vDepthDesc.y * g_fFar;

	vector			vWorldPos = (vector)0.f;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.r;
	vWorldPos.w = 1.0f;

	vWorldPos *= fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);



	vector		vWorldPos_Every = mul(vWorldPos, g_matLightView);

	vector		vUVPos = mul(vWorldPos_Every, g_matLightProj);
	float2		vNewUV;
	vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;

	vector		vShadowDepthInfo = g_ShadowDepthTexture.Sample(DepthSampler, vNewUV);





	vector		vWorldPos_Once = mul(vWorldPos, g_matLightView_Static);

	vUVPos = mul(vWorldPos_Once, g_matLightProj);
	vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;

	vector		vShadowDepthInfo_Once = g_ShadowDepthTexture_Once.Sample(DepthSampler, vNewUV);

	Out.vColor -= max(step(vShadowDepthInfo_Once.x * g_fFar, vWorldPos_Once.z - g_fShadowTestLength) * vector(0.2f, 0.2f, 0.2f, 0.f),
		step(vShadowDepthInfo.x * g_fFar, vWorldPos_Every.z - g_fShadowTestLength) * vector(0.2f, 0.2f, 0.2f, 0.f));



	//=============================  Fog  =============================
	float		fFogValue = saturate(min((max((vDepth.y * g_fFar) - g_fFogDistance, 0.f) / g_fFogRange), 0.3f));
	//	g_vFogColor에 근접하는 Value입니다.
	//	Depth에 의해 차이가 나타납니다.

	vector		vFog_Final = 0;
	Out.vColor.rgb += ((g_vFogColor.rgb - Out.vColor.rgb) * fFogValue);
	//	위에서 구한 FogValue를 통해 카메라에 가까워질 수록 g_vFogColor 에 근접합니다.
	//===========================  Fog End  =============================

	if (Out.vColor.a == 0.f)
		discard;

	return Out;
}

PS_OUT PS_OUTLINE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (g_DepthTexture.Sample(LinearSampler, In.vTexUV).a == 0.f)
		discard;


	float4 color = 0;
	float mask[9] =
	{ -1, -1, -1,
		-1, 8, -1,
		-1, -1, -1 };
	float coord[3] = { -1, 0, 1 };

	for (int i = 0; i < 9; ++i)
		color += g_fOutLineValue * (mask[i] * (g_DepthTexture.Sample(LinearSampler, In.vTexUV + float2(coord[i % 3] / g_fWinSizeX, coord[i / 3] / g_fWinSizeY))));

	color = abs(color);
	//	float gray = 1 - (color.r * 0.3f + color.g * 0.59f + color.b *	0.11f);
	float gray = saturate(1.f - 3.f * (color.r + color.g + color.b));
	Out.vColor = vector(gray, gray, gray, 1);

	if (gray > 0.1f)
		discard;

	return Out;
}

PS_OUT PS_INNERLINE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNormalTexture = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	float		fValue = vNormalTexture.a;
	if (fValue == 0.f)
		discard;

	float4 inline_color = 0;
	float mask[9] =
	{ -1, -1, -1,
		-1, 8, -1,
		-1, -1, -1 };
	float coord[3] = { -1, 0, 1 };

	for (int i = 0; i < 9; ++i)
		inline_color += g_fInnerLineValue * (mask[i] * (g_NormalTexture.Sample(LinearSampler, In.vTexUV + float2(coord[i % 3] / g_fWinSizeX, coord[i / 3] / g_fWinSizeY))));

	inline_color = abs(inline_color);
	float gray_normal = saturate(1 - 3.f * (inline_color.r + inline_color.g + inline_color.b));
	Out.vColor = vector(gray_normal, gray_normal, gray_normal, 1);

	if (gray_normal > 0.1f)
		discard;


	//vector			vDepthDesc = g_DepthTexture.Sample(DepthSampler, In.vTexUV);

	//float			fViewZ = vDepthDesc.y * g_fFar;

	//vector			vWorldPos = (vector)0.f;

	//vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	//vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	//vWorldPos.z = vDepthDesc.r;
	//vWorldPos.w = 1.0f;

	//vWorldPos *= fViewZ;

	//vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	//vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	//float3 vPixelNormal = vNormalTexture.xyz;
	//float fAngle = degrees(acos(dot(g_vCamPosition - vWorldPos, vPixelNormal)));
	//
	//float	fTestValue = (sign(fAngle - 90.f) * (fAngle - 90.f));	//	-면 +로 변환.

	//float	fCutValue = 1.f;
	//Out.vColor = 0.f;

	//if (fTestValue >= fCutValue)
	//	discard;

	return Out;
}

PS_OUT PS_GLOWX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	float2	vNewUV = 0;
	vector	vColor = 0;
	vector	vAccColor = 0;

	for (int i = -20; i < 20; ++i)
	{
		vNewUV = In.vTexUV + float2(i / g_fWinSizeX, 0.f);

		vAccColor += g_fWeight41_Big[20 + i] * g_GlowTexture.Sample(LinearSampler, vNewUV);
	}

	Out.vColor.rgb = vAccColor / g_fWeight41BigTotal;
	//

	return Out;
}

PS_OUT PS_GLOWXY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2	vNewUV = 0;
	vector	vColor = 0;
	vector	vAccColor = 0;

	for (int i = -20; i < 20; ++i)
	{
		vNewUV = In.vTexUV + float2(0.f, i / (g_fWinSizeY / 2.f));

		vAccColor += g_fWeight41_Big[20 + i] * g_GlowTexture.Sample(LinearSampler, vNewUV);
	}

	Out.vColor.rgb = vAccColor / g_fWeight41BigTotal;
	//
	return Out;
}

PS_OUT PS_BLURX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2	vNewUV = 0;
	vector	vColor = 0;
	vector	vAccColor = 0;

	for (int i = -6; i < 6; ++i)
	{
		vNewUV = In.vTexUV + float2(i / g_fWinSizeX, 0.f);

		vColor = g_DiffuseTexture.Sample(LinearSampler, vNewUV);
		vAccColor += vColor * (g_fWeight[6 + i] * g_BlurTexture.Sample(LinearSampler, vNewUV));
	}

	Out.vColor.rgb = vAccColor / g_fWeightTotal;

	return Out;
}

PS_OUT PS_BLURXY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2	vNewUV = 0;
	vector	vColor = 0;
	vector	vAccColor = 0;

	for (int i = -6; i < 6; ++i)
	{
		vNewUV = In.vTexUV + float2(0.f, i / (g_fWinSizeY / 2.f));

		vColor = g_DiffuseTexture.Sample(LinearSampler, vNewUV);
		vAccColor += vColor * g_fWeight[6 + i] * g_BlurTexture.Sample(LinearSampler, vNewUV);
	}

	Out.vColor.rgb = vAccColor / g_fWeightTotal;

	return Out;
}

PS_OUT PS_DISTORTION(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDistortionTexture = g_DistortionTexture.Sample(LinearSampler, In.vTexUV);
	float		fDistortionValueX = vDistortionTexture.x * g_fDistortionValue;
	float		fDistortionValueY = vDistortionTexture.y * g_fDistortionValue;
	float2		vDistortionUV = float2(In.vTexUV.x + (fDistortionValueX / g_fWinSizeX), In.vTexUV.y + (fDistortionValueY / g_fWinSizeY));

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, vDistortionUV);

	return Out;
}

PS_OUT PS_GRAYSCALE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor = vColor;
	float		fGrayScaleWeight = g_GrayScaleTexture.Sample(LinearSampler, In.vTexUV).r;
	if (0.1f >= fGrayScaleWeight)
		return Out;


	Out.vColor.rgb = lerp(vColor.rgb, 1.f - (vColor.rgb + ((vColor.r - vColor) * fGrayScaleWeight)), fGrayScaleWeight);
	Out.vColor.a = vColor;

	return Out;
}

PS_OUT PS_ADD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector vAddColor = g_AddTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vColor + (vAddColor * g_fAddValue);
	Out.vColor.a = vColor.a;

	return Out;
}

PS_OUT PS_LIGHTSHAFT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector			vDepthDesc = g_DepthTexture.Sample(DepthSampler, In.vTexUV);

	float			fViewZ = vDepthDesc.y * g_fFar;

	vector			vWorldPos = (vector)0.f;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.r;
	vWorldPos.w = 1.0f;

	vWorldPos *= fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);


	//	vector		vViewPos_InLight = mul(vWorldPos, g_matLightView);
	//	matrix		matLightVP = mul(g_matLightView, g_matLightProj);
	
	float		fNumSamples = 120.f;
	int			iValue = fNumSamples;

	vector		vLightDir = -g_vLightDir;

	for (int i = 0; i < fNumSamples; ++i)
	{
		vector		vRayPos = vWorldPos + (i * normalize(g_vCamPosition - vWorldPos) * 0.5f);
		vector		vWorldPos_InLight = mul(vRayPos, g_matLightView);

		vector		vUVPos = mul(vWorldPos_InLight, g_matLightProj);
		float2		vNewUV = (float2)0.f;
		vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
		vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;
		vector		vShadowDepthInfo = g_ShadowDepthTexture.Sample(DepthSampler, vNewUV);

		if (vWorldPos_InLight.z > (vShadowDepthInfo.x * g_fFar) - 0.3f)
		{
			iValue -= 1;
		}
	}

	float		fLightPower = 0.3f;

	iValue = min(fNumSamples, iValue);

	Out.vColor.rgb = fLightPower * (iValue / fNumSamples);
	Out.vColor.a = 1.f;

	
	return Out;
}

PS_OUT PS_MAPGRAYSCALE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	vector	vPlayer = g_PlayerTexture.Sample(LinearSampler, In.vTexUV);
	vector	vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexUV);
	vector	vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);

	vector	vGrayColor = (1.f - g_fMapGrayScalePower) * vColor +
		g_fMapGrayScalePower * (vColor.r * 0.3f + vColor.g * 0.59f + vColor.b * 0.11f);
	
	float		fFogDistance = g_fFar * g_fMapGrayScaleTimeRatio;
	float		fFogValue = saturate(min((max((vDepth.y * g_fFar) - fFogDistance, 0.f) / g_fMapGrayScaleFogRange), 1.f));

	bool		bWhite = step(abs(fFogValue - 0.5f), 0.499f);		//	Fog	되고있는 범위면 1, 아니면 0 (Fog 전이거나 Fog 이후)
	vGrayColor = vGrayColor + (bWhite * 0.3f);

	bool	bDrew = all(vPlayer.a + vEffect.a);	//	false == Player나 Effect가 그려지지 않은 픽셀.
	
	Out.vColor = bDrew * vColor + 
		(1.f - bDrew) * ((fFogValue * vColor) + (vGrayColor * (1.f - fFogValue)));

	return Out;
}





RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};
BlendState BS_Default
{
	BlendEnable[0] = false;
};
BlendState BS_LightBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = one;
	DestBlend = one;
	BlendOp = add;
};
DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_ZEnable_Disable_ZWrite_Disable
{
	DepthEnable = false;
	DepthWriteMask = zero;
};



technique11 DefaultTechnique
{
	pass DebugTarget	//	0
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Light_Directional	//	1
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_LightBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Light_Point	//	2
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_LightBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
	}

	pass Blend	//	3
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

	pass OutLine	//	4
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_OUTLINE();
	}

	pass InnerLine	//	5
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_INNERLINE();
	}

	pass GlowX	//	6
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GLOWX();
	}

	pass GlowXY	//	7
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GLOWXY();
	}

	pass All_Distortion	//	8
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISTORTION();
	}

	pass SSAO	//	9
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SSAO();
	}

	pass BlurX	//	10
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLURX();
	}

	pass BlurXY	//	11
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLURXY();
	}

	pass GrayScale		//	12
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GRAYSCALE();
	}

	pass Diffuse_Add		//	13
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ADD();
	}

	pass LightShaft		//	14
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_LIGHTSHAFT();
	}

	pass MapGrayScale		//	15
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAPGRAYSCALE();
	}

}