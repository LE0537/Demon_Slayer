
matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_ViewMatrixInv, g_ProjMatrixInv;

matrix		    g_matLightView;
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
texture2D		g_GlowTexture;
texture2D		g_DistortionTexture;
texture2D		g_AOTexture;
texture2D		g_BlurTexture;
texture2D		g_GrayScaleTexture;
texture2D		g_AddTexture;

float			g_fWinSizeX = 1280.f;
float			g_fWinSizeY = 720.f;
float			g_fFar;

float			g_fSSAORadius;
float			g_fAOValue;
float			g_fGlowBlurCount;
float			g_fDistortionValue;
float			g_fOutLineValue;
float			g_fInnerLineValue;

const float		g_fWeight[13] =
{
	0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.0f,
	0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f
};
const float		g_fWeightTotal = 6.2108;

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
		Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(g_vLightDir2) * -1.f, normalize(vNormal))) + (g_vLightAmbient * g_vMtrlAmbient));
	}

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

	vector		vFogColor = vector(1.f, 1.f, 1.f, 1.f);

	float		fFogPower = 0.0f;

	float		fFogDistance = 5.f;

	fFogPower = min((max((vDepth.y * g_fFar) - fFogDistance, 0.f) / 90.0f), 0.3f);

	Out.vColor = ((vDiffuse - vAO) * vShade + vSpecular) + vFogColor * fFogPower;
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

	vWorldPos = mul(vWorldPos, g_matLightView);

	vector		vUVPos = mul(vWorldPos, g_matLightProj);
	float2		vNewUV;

	vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;

	vector		vShadowDepthInfo = g_ShadowDepthTexture.Sample(DepthSampler, vNewUV);

	if (vWorldPos.z - 0.1f > vShadowDepthInfo.x * g_fFar)
		Out.vColor -= vector(0.2f, 0.2f, 0.2f, 0.f);

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

	float		fValue = g_NormalTexture.Sample(LinearSampler, In.vTexUV).a;
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

	float gray_normal = saturate(1 - 3.f * (inline_color.r + inline_color.g + inline_color.b));
	Out.vColor = vector(gray_normal, gray_normal, gray_normal, 1);

	if (gray_normal > 0.1f)
		discard;

	return Out;
}

PS_OUT PS_GLOWX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//	GlowX
	float weight0, weight1, weight2, weight3, weight4, weight5, weight6;

	weight0 = 1.0f;
	weight1 = 0.9231f;
	weight2 = 0.7261f;
	weight3 = 0.4868f;
	weight4 = 0.278f;
	weight5 = 0.1353f;
	weight6 = 0.0561f;

	float normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4 + weight5 + weight6));
	// Normalize the weights.

	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;
	weight5 = weight5 / normalization;
	weight6 = weight6 / normalization;

	float2 texCoord1 = In.vTexUV + float2(-6.0f / g_fWinSizeX, 0.0f);
	float2 texCoord2 = In.vTexUV + float2(-5.0f / g_fWinSizeX, 0.0f);
	float2 texCoord3 = In.vTexUV + float2(-4.0f / g_fWinSizeX, 0.0f);
	float2 texCoord4 = In.vTexUV + float2(-3.0f / g_fWinSizeX, 0.0f);
	float2 texCoord5 = In.vTexUV + float2(-2.0f / g_fWinSizeX, 0.0f);
	float2 texCoord6 = In.vTexUV + float2(-1.0f / g_fWinSizeX, 0.0f);
	float2 texCoord7 = In.vTexUV + float2(0.0f / g_fWinSizeX, 0.0f);
	float2 texCoord8 = In.vTexUV + float2(1.0f / g_fWinSizeX, 0.0f);
	float2 texCoord9 = In.vTexUV + float2(2.0f / g_fWinSizeX, 0.0f);
	float2 texCoord10 = In.vTexUV + float2(3.0f / g_fWinSizeX, 0.0f);
	float2 texCoord11 = In.vTexUV + float2(4.0f / g_fWinSizeX, 0.0f);
	float2 texCoord12 = In.vTexUV + float2(5.0f / g_fWinSizeX, 0.0f);
	float2 texCoord13 = In.vTexUV + float2(6.0f / g_fWinSizeX, 0.0f);

	float3 colorTest = 0;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord1) * weight6;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord2) * weight5;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord3) * weight4;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord4) * weight3;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord5) * weight2;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord6) * weight1;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord7) * weight0;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord8) * weight1;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord9) * weight2;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord10) * weight3;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord11) * weight4;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord12) * weight5;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord13) * weight6;

	Out.vColor.rgb = colorTest;

	return Out;
}

PS_OUT PS_GLOWXY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//	GlowX
	float weight0, weight1, weight2, weight3, weight4, weight5, weight6;

	weight0 = 1.0f;
	weight1 = 0.9231f;
	weight2 = 0.7261f;
	weight3 = 0.4868f;
	weight4 = 0.278f;
	weight5 = 0.1353f;
	weight6 = 0.0561f;

	float normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4 + weight5 + weight6));
	// Normalize the weights.

	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;
	weight5 = weight5 / normalization;
	weight6 = weight6 / normalization;

	float2 texCoord1 = In.vTexUV + float2(0.0f, -6.0f / g_fWinSizeY);
	float2 texCoord2 = In.vTexUV + float2(0.0f, -5.0f / g_fWinSizeY);
	float2 texCoord3 = In.vTexUV + float2(0.0f, -4.0f / g_fWinSizeY);
	float2 texCoord4 = In.vTexUV + float2(0.0f, -3.0f / g_fWinSizeY);
	float2 texCoord5 = In.vTexUV + float2(0.0f, -2.0f / g_fWinSizeY);
	float2 texCoord6 = In.vTexUV + float2(0.0f, -1.0f / g_fWinSizeY);
	float2 texCoord7 = In.vTexUV + float2(0.0f, 0.0f / g_fWinSizeY);
	float2 texCoord8 = In.vTexUV + float2(0.0f, 1.0f / g_fWinSizeY);
	float2 texCoord9 = In.vTexUV + float2(0.0f, 2.0f / g_fWinSizeY);
	float2 texCoord10 = In.vTexUV + float2(0.0f, 3.0f / g_fWinSizeY);
	float2 texCoord11 = In.vTexUV + float2(0.0f, 4.0f / g_fWinSizeY);
	float2 texCoord12 = In.vTexUV + float2(0.0f, 5.0f / g_fWinSizeY);
	float2 texCoord13 = In.vTexUV + float2(0.0f, 6.0f / g_fWinSizeY);

	float3 colorTest = 0;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord1) * weight6;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord2) * weight5;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord3) * weight4;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord4) * weight3;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord5) * weight2;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord6) * weight1;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord7) * weight0;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord8) * weight1;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord9) * weight2;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord10) * weight3;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord11) * weight4;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord12) * weight5;
	colorTest += g_GlowTexture.Sample(LinearSampler, texCoord13) * weight6;

	//	colorTest += g_GlowTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.rgb = colorTest;

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
	float		fDistortionValue = vDistortionTexture.x * g_fDistortionValue * vDistortionTexture.a;
	float2		vDistortionUV = float2(In.vTexUV.x + (fDistortionValue / g_fWinSizeX), In.vTexUV.y);

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, vDistortionUV);

	return Out;
}

PS_OUT PS_GRAYSCALE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor = vColor;
	float		fGrayScaleWeight = g_GrayScaleTexture.Sample(LinearSampler, In.vTexUV).r;
	if (0.f == fGrayScaleWeight)
		return Out;



	Out.vColor = vColor.r + ((vColor.r - vColor) * fGrayScaleWeight);
	Out.vColor.a = vColor;

	return Out;
}

PS_OUT PS_ADD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector vAddColor = g_AddTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vColor + vAddColor;
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
	
	float		fNumSamples = 150.f;
	int			iValue = fNumSamples;

	vector		vLightDir = -g_vLightDir;

	for (int i = 0; i < fNumSamples; ++i)
	{
		vector		vRayPos = g_vCamPosition + (i * normalize(vWorldPos - g_vCamPosition) * 0.35f);
		vector		vWorldPos_InLight = mul(vRayPos, g_matLightView);

		vector		vUVPos = mul(vWorldPos_InLight, g_matLightProj);
		float2		vNewUV = (float2)0.f;
		vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
		vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;
		vector		vShadowDepthInfo = g_ShadowDepthTexture.Sample(DepthSampler, vNewUV);

		if (vWorldPos_InLight.z > vShadowDepthInfo.x * g_fFar)
			iValue -= 4;

	}

	float		fLightPower = 0.3f;

	iValue = max(0, iValue);

	Out.vColor.rgb = fLightPower * (iValue / fNumSamples);
	Out.vColor.a = 1.f;

	
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

}