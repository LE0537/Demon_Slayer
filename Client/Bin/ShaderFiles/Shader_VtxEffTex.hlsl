#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
texture2D		g_NormalTexture;
texture2D		g_NoiseTexture[3];

vector			g_vCamPosition;

float4			g_vColor;
float3			g_vGlowColor;

float			g_fEndALPHA;
bool			g_bDisjolve;
bool			g_bGlow;			//	�۷ο츦 ����մϴ�.
bool			g_bUseGlowColor;	//	�۷ο� ������ ���������� ����մϴ�.
bool			g_bUseRGB;			//	�ؽ����� RGB�� ����մϴ�. false == a ���
bool			g_bUseColor;		//	g_vColor�� ����մϴ�.. false == g_DiffuseTexture ���
bool			g_bFlowMap;			//	FlowMap�� ����մϴ�.
									//	�߰� ���� �ؽ��� : Noise
bool			g_iNumFlowMap;		//	����� FlowMap ( Noise Texture ) �Դϴ�.

float			g_fAliveTimeRatio;
float			g_fPostProcesesingValue;

float			g_fStatHp;
float			g_fStatDmg;
float			g_fStatSDmg;
float			g_fStatDef;
float			g_fStatSDef;
float			g_fStatSpeed;

float4			g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f); //���ǻ�
float4			g_vLightAmbient = float4(0.5f, 0.5f, 0.5f, 1.f); //���� �ּ� ���
float4			g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f); //���� ���̶���Ʈ (�����Ÿ��� ���)

float4			g_vLightDir = float4(3.f, -3.f, -3.f, 0.f); // ���ǹ��� //���⼺���� // ��

float4			g_vMtrlAmbient = float4(1.f, 1.f, 1.f, 1.f);  // ������ ������
float4			g_vMtrlSpecular = float4(0.1f, 0.1f, 0.1f, 1.f);  // ������ ���̶���Ʈ (�����Ÿ��´���)

																  //	Flow Texture Value
float	g_fDistortionScale = 1.f;		//	�ְ� ��ġ
float	g_fDistortionBias = 0.f;		//	�ʱ� �ְ

float g_fHP;

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
struct VS_OUTrefract
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
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

struct VS_FLOWMAP_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;

	float2		vTexCoord1 : TEXCOORD1;		//	�÷ο�� ������ �����մϴ�.
};

VS_FLOWMAP_OUT VS_FLOWMAP(VS_IN In)
{
	VS_FLOWMAP_OUT		Out = (VS_FLOWMAP_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	Out.vTexCoord1 = (In.vTexUV * 1.f);
	Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fAliveTimeRatio * 0.1f);

	return Out;
}




//**********************************************************************
//***************************  Pixel Shader  ***************************
//**********************************************************************
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
	float4		vGlowColor : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_COLORBLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_bUseColor * (min(vTexture.r, vTexture.a) * g_vColor) +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	Out.vColor.a = vTexture.a * g_fEndALPHA;
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a < 0.03f)
		discard;

	return Out;
}

PS_OUT PS_COLORTEST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_bUseColor * (min(vTexture.r, vTexture.a) * g_vColor) +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	Out.vColor.a = vTexture.a * g_fEndALPHA;
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a <= 0.3f)
		discard;

	Out.vColor.a = 1.f;

	return Out;
}



struct PS_POSTPROCESSING_OUT
{
	float4		vValue : SV_TARGET0;
};

PS_POSTPROCESSING_OUT PS_DISTORTION(PS_IN In)
{
	PS_POSTPROCESSING_OUT		Out = (PS_POSTPROCESSING_OUT)0;
	//	x ��ŭ ���������� �ְ��մϴ�.
	//	�ʿ��� ������ = r

	float4 DiffuseTex = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	float4 NoiseTex = g_NoiseTexture[0].Sample(LinearSampler, In.vTexUV);
	float fValueX = min(DiffuseTex.r, DiffuseTex.a) * (1.f - g_fAliveTimeRatio);
	float fValueY = fValueX * NoiseTex.r;		//	������ NoiseTexture�� ���󰡵�, �ְ� ������ DiffuseTexture�� ���󰩴ϴ�.


	Out.vValue = fValueX * g_fPostProcesesingValue;
	Out.vValue.y = fValueY * g_fPostProcesesingValue;

	if (Out.vValue.x <= 0.03f)
		discard;

	return Out;
}

PS_POSTPROCESSING_OUT PS_GRAYSCALE(PS_IN In)
{
	PS_POSTPROCESSING_OUT		Out = (PS_POSTPROCESSING_OUT)0;
	//	rgb���� r�� �������� g, b�� x ��ŭ �������� �ؼ� �����մϴ�. (1 == ȸ��, 0 == ���� ��)
	//	�ʿ��� ������ = r

	float4 DiffuseTex = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	float fValue = (g_bUseRGB * DiffuseTex.r) + ((1.f - g_bUseRGB) * DiffuseTex.a);
	Out.vValue = fValue * g_fPostProcesesingValue;

	if (Out.vValue.r <= 0.03f)
		discard;

	return Out;
}

PS_OUT PS_ALPHAGLOW(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	//	Alpha�� Glow �Դϴ�. - Ÿ���� �ٸ��ϴ�.

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_bUseColor * (min(vTexture.r, vTexture.a) * g_vColor) +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	Out.vColor.a = vTexture.a * g_fEndALPHA;
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	return Out;
}




struct PS_FLOWMAP_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;

	float2		vTexCoord1 : TEXCOORD1;
};

PS_OUT PS_FLOWMAP_ALPHAGLOW(PS_FLOWMAP_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//	NoiseTexture�� UV = In.vTexCoord1
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

	Out.vColor = g_bUseColor * (min(vTexture.r, vTexture.a) * g_vColor) +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	Out.vColor.a = vTexture.a * g_fEndALPHA;
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;


	return Out;
}

PS_OUT PS_FLOWMAP_ALPHATEST(PS_FLOWMAP_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//	NoiseTexture�� UV = In.vTexCoord1
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

	Out.vColor = g_bUseColor * (min(vTexture.r, vTexture.a) * g_vColor) +
		(1.f - g_bUseColor) * ((vTexture * g_bUseRGB) + (vTexture.a * (1.f - g_bUseRGB)));
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	Out.vColor.a = vTexture.a * g_fEndALPHA;
	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}



technique11 DefaultTechnique
{
	pass Default //0
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass COLORBLEND //1
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_COLORBLEND();
	}

	pass COLORTEST //2
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_COLORTEST();
	}

	pass Distortion		//	3
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISTORTION();
	}

	pass GrayScale	//	4
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GRAYSCALE();
	}

	pass AlphaGlow	//	5
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHAGLOW();
	}

	pass FlowMap_Alpha	//	6
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_FLOWMAP();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FLOWMAP_ALPHAGLOW();
	}

	pass FlowMap_AlphaTest	//	7
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_FLOWMAP();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FLOWMAP_ALPHATEST();
	}
}