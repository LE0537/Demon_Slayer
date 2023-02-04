#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;	
texture2D		g_NormalTexture;
texture2D		g_MaskTexture;

#define PI 		3.14159265359
#define TWO_PI  6.28318530718

vector			g_vCamPosition;

float2			g_fViewPort;
float4			g_vColor;

bool			g_bInkEffDownCheck;
float			g_fMaxBar;
float			g_fCurBar;
int				g_iFrame;
int				g_iNumTexU;
int				g_iNumTexV;
float			g_fTime;
float			g_fAlpha;
float			g_fUvMoveTime;
float			g_fAlphaTime;
float			g_fMinusHp;
float			g_fMinus_BeforeHp;


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
//	float3		vNormal : NORMAL;
//	float		fShade : COLOR0;
//	float		fSpecular : COLOR0;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};
/* DrawIndexed�Լ��� ȣ���ϸ�. */
/* �ε������ۿ� ����ִ� �ε�����°�� ������ VS_MAIN�Լ��� ���ڷ� ������. VS_IN�� ����ȴ�. */
/* �Ϲ������� TriangleList�� �׸����, ���� ������ ���� VS_MAIN�Լ��� ���ڷ� ������. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* ������ ��ġ�� ���� �� ��������� ���Ѵ�. ���� ������ ViewSpace�� �����ϳ�. */
	/* ������ı��� ���ϸ� ������ġ�� w�� �佺���̽� ���� z�� �����Ѵ�. == Out.vPosition�� �ݵ�� float4�̾���ϴ� ����. */
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

VS_OUT VS_Sprite(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* ������ ��ġ�� ���� �� ��������� ���Ѵ�. ���� ������ ViewSpace�� �����ϳ�. */
	/* ������ı��� ���ϸ� ������ġ�� w�� �佺���̽� ���� z�� �����Ѵ�. == Out.vPosition�� �ݵ�� float4�̾���ϴ� ����. */
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	float         fTexU = ((g_iFrame % g_iNumTexU) + In.vTexUV.x) / g_iNumTexU;
	float         fTexV = ((g_iFrame / g_iNumTexU) + In.vTexUV.y) / g_iNumTexV;
	Out.vTexUV = float2(fTexU, fTexV);

	return Out;
}

/* TriangleList�� �׷ȴٸ� ���� ������ VS_MAIN�� ������ ��� ��ģ ����. */

/* w������ ������ �����Ͽ� ���������̽��� ����. */

/* !!!!!!!!!!!!!!!!���. ���������� ��� ������ ���ؼ��� ���� ������ �����Ѵ�. */

/* ����Ʈ�����̽�(Ŭ���̾�Ʈ���� == ��������ǥ)���� ��ȯ�� �����Ѵ�. */
/* �����Ͷ���� �����Ѵ�. == ������ ������ ��������Ͽ� �ȼ��� �����Ѵ�. VS_OUT == PS_IN�� ���� ����. */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* �̷��� ������� �ȼ��� PS_MAIN�Լ��� ���ڷ� ������. */
/* �����ϴ� ���� Target0 == ��ġ�� 0��°�� ���ε��Ǿ��ִ� ����Ÿ��(�Ϲ������� �����)�� �׸���. */
/* �׷��� ����ۿ� ���� �׷�����. */

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_HpBarMinus(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	
	if (g_fCurBar / g_fMaxBar < In.vTexUV.x)
	{
		Out.vColor.r = 1.f;
		Out.vColor.g = 0.f;
		Out.vColor.b = 0.f;
		Out.vColor.a = 0.7f; 
		
	}
	else
		Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
	
	if (g_fMinusHp / g_fMinus_BeforeHp < In.vTexUV.x)
		discard;

	return Out;
}

PS_OUT PS_SkillBarMinus(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (g_fCurBar / g_fMaxBar < In.vTexUV.x)
		discard;
	else
		Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_UltBarMinus(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (g_fCurBar / g_fMaxBar < In.vTexUV.x)
		discard;
	else
	{
		Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
		Out.vColor.rg = 1.f;
		Out.vColor.b = 0.f;
		Out.vColor.a = 1.f;
 	}

	return Out;
}

PS_OUT PS_LogoEff(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float2 vNewUV = In.vTexUV;

	vNewUV.x -= g_fUvMoveTime;

	Out.vColor = g_DiffuseTexture.Sample(PointSampler, vNewUV);

	Out.vColor.a += g_fAlphaTime;

	Out.vColor.a -= 0.95f;

	return Out;
}

PS_OUT PS_PatternWind(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float2 vNewUV = In.vTexUV;

	vNewUV.x -= g_fUvMoveTime;
	vNewUV.y -= g_fUvMoveTime;

	Out.vColor = g_DiffuseTexture.Sample(PointSampler, vNewUV);

	return Out;
}

PS_OUT PS_PatternOne(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float2 vNewUV = In.vTexUV;

	vNewUV.x *= 15.f;
	vNewUV.y *= 15.f;

	vNewUV.x += g_fUvMoveTime;
	vNewUV.y += g_fUvMoveTime;

	Out.vColor = g_DiffuseTexture.Sample(PointSampler, vNewUV);

	return Out;
}

PS_OUT PS_LogoLight(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);

	Out.vColor.a += g_fAlphaTime;

	Out.vColor.a = 0.f;

	return Out;
}



PS_OUT PS_SelCharIcon(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float4 DiffuseTexture = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
	float4 vMaskTexture = g_MaskTexture.Sample(PointSampler, In.vTexUV);
	
	Out.vColor.rgba = DiffuseTexture.rgba;

	if (vMaskTexture.r == 0)
		discard;
	
	return Out;
}

PS_OUT PS_SelNameShadow(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float2 vNewUV = In.vTexUV;
	//vNewUV.x += 0.5f;

	float4 DiffuseTexture = g_DiffuseTexture.Sample(PointSampler, vNewUV);

	Out.vColor.rgb = DiffuseTexture.rgb;

	float fAlpha = DiffuseTexture.a - 0.2f;
	Out.vColor.a = fAlpha;

	return Out;
}

PS_OUT PS_SelIconEff(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);

	if (Out.vColor.a > 0)
	{
		Out.vColor.r = 0.7f;
		Out.vColor.g = 0.f;
		Out.vColor.b = 0.f;
	}

	return Out;
}

PS_OUT PS_Fade(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float4 vFade = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);

	Out.vColor.a = vFade.a * g_fAlpha;
	Out.vColor.rgb = vFade.rgb;

	return Out;
}

PS_OUT PS_InkEff(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float4 vMask = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
	
	Out.vColor.rgb = vMask.rgb;

	if (!g_bInkEffDownCheck)
		Out.vColor.a = 1 - vMask.r;
	else
	{
		Out.vColor.a = vMask.r;
		Out.vColor.rgb = 1 - vMask.rgb;
	}

	return Out;
}

PS_OUT PS_GamePlyCharIcon(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float4 DiffuseTexture = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
	float4 vMaskTexture = g_MaskTexture.Sample(PointSampler, In.vTexUV);

	Out.vColor.rgba = DiffuseTexture.rgba;

	if (Out.vColor.a > 0.3f)
		Out.vColor.a = vMaskTexture.r;

	return Out;
}

PS_OUT PS_Circle_Progressbar(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	//float2 uvView = In.vTexUV.xy / g_fViewPort.xy;

	//float 	SMOOTH = 0.005;
	//float2 	uv = (uvView*2.0) - 1.0;
	//float2  origin = float2(0.0, 0.0);
	//uv.x /= 9.0 / 16.0; //remove this line in UE (compensating for 16:9 on ShaderToy)

	//float 	ir = 0.75;
	//float 	or = 0.95;
	//float 	d = length(uv);
	//float 	ring = smoothstep(or + SMOOTH, or - SMOOTH, d) - smoothstep(ir + SMOOTH, ir - SMOOTH, d);
	//float 	a = atan2(uv.y - origin.y, uv.x - origin.x);
	//float	theta = (a < 0.0) ? (a + TWO_PI) / TWO_PI : a / TWO_PI;
	//float	bar = step(theta, 0.7f);
	//float	ui = ring * bar;

	//float4 	colour = float4(ui, ui, ui, 1.f);

	float2 uv = In.vTexUV;//( - 0.5 * g_fViewPort.xy) / g_fViewPort.y;
	float circle = 0.5f;//smoothstep(0.4 + (1 / g_fViewPort.y), 0.4 - (1 / g_fViewPort.y), abs(length(uv) - 0.4) + 0.4 - 0.05);
	float radial = 0.5f;//(atan2(uv.x, -uv.y) / 3.14159265359 * 0.5) + 0.5;
	float time = g_fTime / (3.14159265359 * 2.0);
	radial += time;
	circle *= step(frac(radial), frac(time));

	Out.vColor = float4(circle, circle, circle, 1.0);

	return Out;
}

technique11 DefaultTechnique
{
	pass Default //0
	{ 
		SetRasterizerState(RS_UI);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass DefaultRenderBack //1
	{
		SetRasterizerState(RS_UI);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass P1HpBarDiscard //2
	{
		SetRasterizerState(RS_SkyBox);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HpBarMinus();
	}

	pass P2SkillBarDiscard //3
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SkillBarMinus();
	}

	pass Color //4
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GamePlyCharIcon();
	}

	pass P1SkillBarDiscard //5
	{
		SetRasterizerState(RS_SkyBox);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SkillBarMinus();
	}


	pass P2HpBarDiscard //6
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HpBarMinus();
	}

	pass LogoEff //7
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_LogoEff();
	}

	pass LogoLight //8
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_LogoLight();
	}

	pass SelCharIcon //9
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SelCharIcon();
	}

	pass SelNameShadow //10
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SelNameShadow();
	}

	pass SelFameEff //11
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SelIconEff();
	}

	pass Fade //12
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Fade();
	}

	pass SpriteInk //13
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_Sprite();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_InkEff();
	}

	pass Sprite //14
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_Sprite();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass UltBar //15
	{
		SetRasterizerState(RS_UI);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UltBarMinus();
	}

	pass GamePlyCharIcon //16
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GamePlyCharIcon();
	}

	pass CircleProgressBar //17
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Circle_Progressbar();
	}

	pass PatternWind //18
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PatternWind();
	}

	pass PatternOne //19
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PatternOne();
	}
	

	
}