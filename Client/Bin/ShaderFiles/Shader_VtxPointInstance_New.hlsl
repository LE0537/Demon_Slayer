
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
vector			g_vCamPosition;

float3			g_vGlowColor;

float			g_fAlphaRatio;
float			g_fTime;

bool			g_bGlow;			//	글로우를 사용합니다.
bool			g_bUseRGB;			//	텍스쳐의 RGB를 사용합니다. false == a 사요
bool			g_bUseColor;		//	g_vColor를 사용합니다.. false == g_DiffuseTexture 사용
bool			g_bAlphaDissolve;
bool			g_bFollow;

bool			g_bBillboard;
bool			g_bYBillboard;
bool			g_bBillboardTurn;

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

	float4		vColor : TEXCOORD5;
	float2		vSize : TEXCOORD6;

	float		fLifeTime : FLOAT1;
	float		fTime : FLOAT2;

	float		fGravity : FLOAT3;
	float		fSpeed : FLOAT4;
};

struct VS_OUT
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;

	float4		vColor : TEXCOORD5;
	float2		vSize : TEXCOORD6;

	float		fLifeTime : FLOAT1;
	float		fTime : FLOAT2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	float4x4	TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector		vPosition = mul(vector(In.vPosition, 1.f),
		(1 - g_bFollow) * mul(TransformMatrix, g_WorldMatrix) + g_bFollow * TransformMatrix);

	Out.vPosition = vPosition.xyz;
	Out.vPSize.x = In.vPSize.x * length(In.vRight.xyz);
	Out.vPSize.y = In.vPSize.y * length(In.vUp.xyz);

	/*Out.vRight = mul(In.vRight, g_WorldMatrix);
	Out.vUp = mul(In.vUp, g_WorldMatrix);
	Out.vLook = mul(In.vLook, g_WorldMatrix);*/

	Out.vRight = (1 - g_bFollow) * mul(In.vRight, g_WorldMatrix) + g_bFollow * In.vRight;
	Out.vUp = (1 - g_bFollow) * mul(In.vUp, g_WorldMatrix) + g_bFollow * In.vUp;
	Out.vLook = (1 - g_bFollow) * mul(In.vLook, g_WorldMatrix) + g_bFollow * In.vLook;

	Out.vColor = In.vColor;
	Out.vSize = In.vSize;
	Out.fTime = In.fTime;
	Out.fLifeTime = In.fLifeTime;

	return Out;
}

struct GS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize : PSIZE;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;

	float4		vColor : TEXCOORD5;
	float2		vSize : TEXCOORD6;

	float		fLifeTime : FLOAT1;
	float		fTime : FLOAT2;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;

	float4		vColor : TEXCOORD5;
	float2		vSize : TEXCOORD6;

	float		fLifeTime : FLOAT1;
	float		fTime : FLOAT2;
};

/* 지오메트리셰이더 == 기하셰이더. */
/* 정점을 생성한다. */

/* 그리고자하는 프리미티브에 의존하여 호출된다. */
/* Point : 정점 한개당 1번호출. */
/* Line : 정점 두개당 1번호출. */
/* Triangle : 정점세개당 한번호출 */
[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
	GS_OUT			Out[4];

	float3 vLook = ((g_vCamPosition.xyz - In[0].vPosition) * g_bBillboard)
		+ ((g_vCamPosition.xyz - In[0].vPosition) * g_bYBillboard)
		+ ((g_vCamPosition.xyz - In[0].vPosition) * g_bBillboardTurn)
		+ (In[0].vLook * (1 - g_bBillboard) * (1 - g_bYBillboard) * (1 - g_bBillboardTurn));

	float3 vRight = ((normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].vPSize.x)) * g_bBillboard)
		+ ((normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * (In[0].vPSize.x)) * g_bYBillboard)
		+ ((normalize(cross(In[0].vUp, vLook)) * (In[0].vPSize.x)) * g_bBillboardTurn)
		+ (In[0].vRight * (1 - g_bBillboard) * (1 - g_bYBillboard) * (1 - g_bBillboardTurn));

	float3 vUp = ((normalize(cross(vLook, vRight)) * (In[0].vPSize.y)) * g_bBillboard)
		+ (float3(0.f, 1.f, 0.f) * (In[0].vPSize.y) * g_bYBillboard)
		+ ((normalize(cross(vLook, vRight)) * (In[0].vPSize.y)) * g_bBillboardTurn)
		+ (In[0].vUp * (1 - g_bBillboard) * (1 - g_bYBillboard) * (1 - g_bBillboardTurn));

	matrix			matVP;

	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	float3 vPos = In[0].vPosition;

	float fTexU;
	float fTexV;

	int iFrame;

	iFrame = (In[0].fTime / In[0].fLifeTime) * (g_iNumUV_U * g_iNumUV_V);

	if (g_iNumUV_U * g_iNumUV_V == iFrame)
		iFrame = g_iNumUV_U * g_iNumUV_V - 1;

	Out[0].vPosition = float4(In[0].vPosition + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	fTexU = ((iFrame % g_iNumUV_U) + 0.f) / g_iNumUV_U;
	fTexV = ((iFrame / g_iNumUV_U) + 0.f) / g_iNumUV_V;
	Out[0].vTexUV = float2(fTexU, fTexV);

	Out[0].vColor = In[0].vColor;
	Out[0].vSize = In[0].vSize;
	Out[0].fTime = In[0].fTime;
	Out[0].fLifeTime = In[0].fLifeTime;

	Out[1].vPosition = float4(In[0].vPosition - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	fTexU = ((iFrame % g_iNumUV_U) + 1.f) / g_iNumUV_U;
	fTexV = ((iFrame / g_iNumUV_U) + 0.f) / g_iNumUV_V;
	Out[1].vTexUV = float2(fTexU, fTexV);

	Out[1].vColor = In[0].vColor;
	Out[1].vSize = In[0].vSize;
	Out[1].fTime = In[0].fTime;
	Out[1].fLifeTime = In[0].fLifeTime;

	Out[2].vPosition = float4(In[0].vPosition - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	fTexU = ((iFrame % g_iNumUV_U) + 1.f) / g_iNumUV_U;
	fTexV = ((iFrame / g_iNumUV_U) + 1.f) / g_iNumUV_V;
	Out[2].vTexUV = float2(fTexU, fTexV);

	Out[2].vColor = In[0].vColor;
	Out[2].vSize = In[0].vSize;
	Out[2].fTime = In[0].fTime;
	Out[2].fLifeTime = In[0].fLifeTime;

	Out[3].vPosition = float4(In[0].vPosition + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	fTexU = ((iFrame % g_iNumUV_U) + 0.f) / g_iNumUV_U;
	fTexV = ((iFrame / g_iNumUV_U) + 1.f) / g_iNumUV_V;
	Out[3].vTexUV = float2(fTexU, fTexV);

	Out[3].vColor = In[0].vColor;
	Out[3].vSize = In[0].vSize;
	Out[3].fTime = In[0].fTime;
	Out[3].fLifeTime = In[0].fLifeTime;

	DataStream.Append(Out[0]);
	DataStream.Append(Out[1]);
	DataStream.Append(Out[2]);
	DataStream.RestartStrip();

	DataStream.Append(Out[0]);
	DataStream.Append(Out[2]);
	DataStream.Append(Out[3]);
	DataStream.RestartStrip();

}

[maxvertexcount(20)]
void GS_CROSSMAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> DataStream)
{
	GS_OUT			Out[8];

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



	float3 vLookSize = normalize(vLook) * In[0].vPSize.x;
	Out[4].vPosition = float4(In[0].vPosition + vRight + vLookSize, 1.f);
	Out[4].vPosition = mul(Out[4].vPosition, matVP);
	Out[4].vTexUV = float2(0.f, 0.f);

	Out[5].vPosition = float4(In[0].vPosition - vRight + vLookSize, 1.f);
	Out[5].vPosition = mul(Out[5].vPosition, matVP);
	Out[5].vTexUV = float2(1.f, 0.f);

	Out[6].vPosition = float4(In[0].vPosition - vRight - vLookSize, 1.f);
	Out[6].vPosition = mul(Out[6].vPosition, matVP);
	Out[6].vTexUV = float2(1.f, 1.f);

	Out[7].vPosition = float4(In[0].vPosition + vRight - vLookSize, 1.f);
	Out[7].vPosition = mul(Out[7].vPosition, matVP);
	Out[7].vTexUV = float2(0.f, 1.f);

	DataStream.Append(Out[4]);
	DataStream.Append(Out[5]);
	DataStream.Append(Out[6]);
	DataStream.RestartStrip();

	DataStream.Append(Out[4]);
	DataStream.Append(Out[6]);
	DataStream.Append(Out[7]);
	DataStream.RestartStrip();
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;

	float4		vColor : TEXCOORD5;
	float2		vSize : TEXCOORD6;

	float		fLifeTime : FLOAT1;
	float		fTime : FLOAT2;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
	float4		vGlowColor : SV_TARGET1;
};

/* 이렇게 만들어진 픽셀을 PS_MAIN함수의 인자로 던진다. */
/* 리턴하는 색은 Target0 == 장치에 0번째에 바인딩되어있는 렌더타겟(일반적으로 백버퍼)에 그린다. */
/* 그래서 백버퍼에 색이 그려진다. */
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

	Out.vColor = g_bUseColor * (In.vColor) + (1.f - g_bUseColor) * vTexture;
	Out.vGlowColor.rgb = g_vGlowColor * g_bGlow;

	float fFullTime = In.fLifeTime;
	float fTexAlpha = saturate((1 - g_bUseRGB) * vTexture.a) + saturate(g_bUseRGB * vTexture.r);

	float fTimeRatio = In.fTime / fFullTime;
	//saturate(In.fTime - fFullTime) / (fFullTime);

	float4 ColorAlpha = saturate(saturate(g_bUseColor * (In.vColor.a * fTexAlpha)) + saturate((1 - g_bUseColor) * (fTexAlpha)));

	Out.vColor.a = saturate(ColorAlpha - g_bAlphaDissolve * fTimeRatio);
	//Out.vColor.a = saturate(Out.vColor.a - ((g_fTime - In.fStartTime - fFullTime * g_fAlphaRatio) / (fFullTime * (1 - g_fAlphaRatio))));

	Out.vGlowColor.a = Out.vColor.a * g_bGlow;

	if (Out.vColor.a < 0.03f)
		discard;

	return Out;
}

PS_OUT PS_COLORTEST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = g_bUseColor * (In.vColor) + (1.f - g_bUseColor) * vTexture;
	Out.vGlowColor.rgb = g_vGlowColor * g_bGlow;

	float fTexAlpha = saturate((1 - g_bUseRGB) * vTexture.a) + saturate(g_bUseRGB * vTexture.r);

	Out.vColor.a = saturate(saturate(g_bUseColor * (In.vColor.a * fTexAlpha)) + saturate((1 - g_bUseColor) * (fTexAlpha)));

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