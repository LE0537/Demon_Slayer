
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
texture2D		g_DissolveTexture;
vector			g_vCamPosition;

float4			g_vColor;
float3			g_vGlowColor;

float			g_fAlphaRatio;
float			g_fEndALPHA;

bool			g_bMask;
bool			g_bDisappearStart;
bool			g_bDissolve;
bool			g_bGlow;			//	�۷ο츦 ����մϴ�.
bool			g_bUseGlowColor;
bool			g_bUseRGB;			//	�ؽ����� RGB�� ����մϴ�. false == a ���
bool			g_bUseColor;		//	g_vColor�� ����մϴ�.. false == g_DiffuseTexture ���
bool			g_bFlowMap;			//	FlowMap�� ����մϴ�.
									//	�߰� ���� �ؽ��� : Noise
bool			g_iNumFlowMap;		//	����� FlowMap ( Noise Texture ) �Դϴ�.

bool			g_bBillboard;
bool			g_bYBillboard;

int				g_iFrame;
int				g_iNumUV_U;
int				g_iNumUV_V;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
};

struct VS_OUT
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	float4x4	TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector		vPosition = mul(vector(In.vPosition, 1.f), mul(TransformMatrix, g_WorldMatrix));

	Out.vPosition = vPosition.xyz;
	Out.vPSize.x = In.vPSize.x * length(In.vRight.xyz);
	Out.vPSize.y = In.vPSize.y * length(In.vUp.xyz);

	Out.vRight = mul(In.vRight, g_WorldMatrix);
	Out.vUp = mul(In.vUp, g_WorldMatrix);
	Out.vLook = mul(In.vLook, g_WorldMatrix);

	return Out;
}

struct GS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

/* ������Ʈ�����̴� == ���ϼ��̴�. */
/* ������ �����Ѵ�. */

/* �׸������ϴ� ������Ƽ�꿡 �����Ͽ� ȣ��ȴ�. */
/* Point : ���� �Ѱ��� 1��ȣ��. */
/* Line : ���� �ΰ��� 1��ȣ��. */
/* Triangle : ���������� �ѹ�ȣ�� */
[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
	GS_OUT			Out[4];

	float3			vLook = ((g_vCamPosition.xyz - In[0].vPosition) * g_bBillboard)
		+ ((g_vCamPosition.xyz - In[0].vPosition) * g_bYBillboard)
		+ (In[0].vLook * (1 - g_bBillboard) * (1 - g_bYBillboard));
	float3			vRight = ((normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].vPSize.x)) * g_bBillboard)
		+ ((normalize(cross(In[0].vUp, vLook)) * (In[0].vPSize.x)) * g_bYBillboard)
		+ (In[0].vRight * (1 - g_bBillboard) * (1 - g_bYBillboard));
	float3			vUp = ((normalize(cross(vLook, vRight)) * (In[0].vPSize.y)) * g_bBillboard)
		+ ((normalize(cross(vLook, vRight)) * (In[0].vPSize.y)) * g_bYBillboard)
		+ (In[0].vUp * (1 - g_bBillboard) * (1 - g_bYBillboard));

	matrix			matVP;

	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3 vPos = In[0].vPosition;

	Out[0].vPosition = float4(In[0].vPosition + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexUV = float2(0.f, 0.f);

	Out[1].vPosition = float4(In[0].vPosition - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexUV = float2(1.f, 0.f);

	Out[2].vPosition = float4(In[0].vPosition - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexUV = float2(1.f, 1.f);

	Out[3].vPosition = float4(In[0].vPosition + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexUV = float2(0.f, 1.f);

	DataStream.Append(Out[0]);
	DataStream.Append(Out[1]);
	DataStream.Append(Out[2]);
	DataStream.RestartStrip();

	DataStream.Append(Out[0]);
	DataStream.Append(Out[2]);
	DataStream.Append(Out[3]);
	DataStream.RestartStrip();

}

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

/* �̷��� ������� �ȼ��� PS_MAIN�Լ��� ���ڷ� ������. */
/* �����ϴ� ���� Target0 == ��ġ�� 0��°�� ���ε��Ǿ��ִ� ����Ÿ��(�Ϲ������� �����)�� �׸���. */
/* �׷��� ����ۿ� ���� �׷�����. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.5f)
		discard;

	return Out;
}

PS_OUT PS_COLORBLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_bUseColor * (min(vTexture.r, vTexture.a) * g_vColor) +
		(1.f - g_bUseColor) * vTexture;
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	float fTexAlpha = saturate((1 - g_bUseRGB) * vTexture.a) + saturate(g_bUseRGB * max(max(vTexture.r, vTexture.g), vTexture.b));

	Out.vColor.a = saturate(saturate(g_bUseColor * (g_vColor.a * fTexAlpha)) + saturate((1 - g_bUseColor) * fTexAlpha));

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
		(1.f - g_bUseColor) * vTexture;
	Out.vGlowColor.rgb = (((1.f - g_bUseGlowColor) * Out.vColor.rgb) +
		(g_bUseGlowColor * g_vGlowColor * min(vTexture.r, vTexture.a))) * g_bGlow;

	float fTexAlpha = saturate((1 - g_bUseRGB) * vTexture.a) + (g_bUseRGB * max(max(vTexture.x, vTexture.y), max(vTexture.y, vTexture.z)));
	Out.vColor.a = saturate(saturate(g_bUseColor * (g_vColor.a * fTexAlpha)) + saturate((1 - g_bUseColor) * (fTexAlpha)));

	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a <= 0.1f)
		discard;

	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass COLORBLEND //1
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_COLORBLEND();
	}

	pass COLORTEST //2
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_COLORTEST();
	}
}