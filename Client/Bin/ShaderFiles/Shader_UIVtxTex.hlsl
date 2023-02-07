#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;	
texture2D		g_NormalTexture;
texture2D		g_MaskTexture;

float2 g_WinXY = float2(1.f, 1.f);
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
/* DrawIndexed함수를 호출하면. */
/* 인덱스버퍼에 담겨있는 인덱스번째의 정점을 VS_MAIN함수에 인자로 던진다. VS_IN에 저장된다. */
/* 일반적으로 TriangleList로 그릴경우, 정점 세개를 각각 VS_MAIN함수의 인자로 던진다. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* 정점의 위치에 월드 뷰 투영행렬을 곱한다. 현재 정점은 ViewSpace에 존재하낟. */
	/* 투영행렬까지 곱하면 정점위치의 w에 뷰스페이스 상의 z를 보관한다. == Out.vPosition이 반드시 float4이어야하는 이유. */
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

	/* 정점의 위치에 월드 뷰 투영행렬을 곱한다. 현재 정점은 ViewSpace에 존재하낟. */
	/* 투영행렬까지 곱하면 정점위치의 w에 뷰스페이스 상의 z를 보관한다. == Out.vPosition이 반드시 float4이어야하는 이유. */
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	float         fTexU = ((g_iFrame % g_iNumTexU) + In.vTexUV.x) / g_iNumTexU;
	float         fTexV = ((g_iFrame / g_iNumTexU) + In.vTexUV.y) / g_iNumTexV;
	Out.vTexUV = float2(fTexU, fTexV);

	return Out;
}

/* TriangleList로 그렸다면 정점 세개가 VS_MAIN의 수행을 모두 마친 이후. */

/* w나누기 연산을 수행하여 투영스페이스로 간다. */

/* !!!!!!!!!!!!!!!!잠깐. 투영영역을 벗어난 정점에 대해서는 이후 과정을 생략한다. */

/* 뷰포트스페이스(클라이언트영역 == 윈도우좌표)로의 변환을 수행한다. */
/* 래스터라이즈를 수행한다. == 정점의 정보를 기반으로하여 픽셀을 생성한다. VS_OUT == PS_IN이 같은 이유. */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
	float4		vNormal : SV_TARGET1;	//	외곽선용1
	float4		vDepth : SV_TARGET2;	//	외곽선용2
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

PS_OUT PS_OniSpecialBarMinus(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float4 DiffuseTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	float4 MaskTexture = g_MaskTexture.Sample(LinearSampler, In.vTexUV);

	if (g_fCurBar / 500.f < In.vTexUV.y)
	{
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	}
	else
	{
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
		if (MaskTexture.r > 0.f)
		{
			Out.vColor.r = 0.725f;
			Out.vColor.g = 0.f;
			Out.vColor.b = 0.f;
			Out.vColor.a = 1.f;
		}
		else
			Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	}

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

PS_OUT PS_WindowLeft(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float2 vNewUV = In.vTexUV;

	vNewUV.x += 1.f;
	vNewUV.x -= g_fUvMoveTime;

	float4 vDiffuseTexture = g_DiffuseTexture.Sample(CLAMPSampler, vNewUV);

	float4 vMaskTexture = g_MaskTexture.Sample(PointSampler, In.vTexUV);


	Out.vColor.rgba = vDiffuseTexture.rgba;

	if (vMaskTexture.r == 0)
		discard;

	return Out;
}
PS_OUT PS_WindowRight(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	float2 vNewUV = In.vTexUV;

	vNewUV.x -= 1.f;
	vNewUV.x += g_fUvMoveTime;

	float4 vDiffuseTexture = g_DiffuseTexture.Sample(CLAMPSampler, vNewUV);

	float4 vMaskTexture = g_MaskTexture.Sample(PointSampler, In.vTexUV);

	Out.vColor.rgba = vDiffuseTexture.rgba;

	if (vMaskTexture.r == 0)
		discard;

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

PS_OUT PS_Circle_Progress_Clock(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;
	float4 DiffuseTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	float duration = 60.f;

	float progress = g_fTime / duration;

	float innerRadius = 0.12f;
	float outerRadius = 0.18f;

	float middleRadius = 0.5f * (innerRadius + outerRadius);
	float halfWidth = 0.5f * (outerRadius - innerRadius);

	float2 pos = In.vTexUV.xy - 0.5f * g_WinXY.xy;
	//float2 pos = In.vTexUV.xy;
	float radius = length(pos.xy);

	float fr = halfWidth - abs(radius - middleRadius) + 1.f;
	/*if(fr < 0.0)
	discard;*/
	fr = saturate(fr);

	float angle = degrees(atan2(pos.x, pos.y) + 0.f) + 180.f;
	float fa = radians(angle - progress * 360.f) * radius + 1.f;

	fa = saturate(fa);
	if (fa != 1.f)
		discard;
	vector color = vector(0.f, 0.f, 0.f, 1);
	vector col = lerp(color, DiffuseTexture, fa);
	//   col.a *= fr;

	//col = col * col2;//DiffuseTexture;

	Out.vColor = col;

	return Out;
}

PS_OUT PS_Circle_Progress_CClock(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;
	float4 DiffuseTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	float duration = 500.f;

	float progress = g_fTime / duration;

	float innerRadius = 0.12f;
	float outerRadius = 0.18f;

	float middleRadius = 0.5f * (innerRadius + outerRadius);
	float halfWidth = 0.5f * (outerRadius - innerRadius);

	float2 pos = In.vTexUV.xy - 0.5f * g_WinXY.xy;
	//float2 pos = In.vTexUV.xy;
	float radius = length(pos.xy);

	float fr = halfWidth - abs(radius - middleRadius) + 1.f;
	/*if(fr < 0.0)
	discard;*/
	fr = saturate(fr);

	float angle = degrees(atan2(-pos.x, pos.y) + 0.f) + 180.f;
	float fa = radians(angle - progress * 360.f) * radius + 1.f;

	fa = saturate(fa);
	if (fa != 1.f)
		discard;
	vector color = vector(0.f, 0.f, 0.f, 1);
	vector col = lerp(color, DiffuseTexture, fa);
	//   col.a *= fr;

	//col = col * col2;//DiffuseTexture;

	Out.vColor = col;

	return Out;
}


PS_OUT PS_Combo_Progressbar(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;
	float4 DiffuseTexture = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	float4 MaskTexture = g_MaskTexture.Sample(LinearSampler, In.vTexUV);
	float duration = 60.f;

	if (MaskTexture.r == 0.f)
		discard;
	float progress = g_fTime;

	float innerRadius = 0.19f;
	float outerRadius = 0.2f;

	float middleRadius = 0.5f * (innerRadius + outerRadius);
	float halfWidth = 0.5f * (outerRadius - innerRadius);

	float2 pos = In.vTexUV.xy - 0.5f * g_WinXY.xy;

	float radius = length(pos.xy);

	float fr = halfWidth - abs(radius - middleRadius) + 1.f;

	if(fr < 0.f)
		discard;

	fr = saturate(fr);

	float angle = degrees(atan2(-pos.x, pos.y)) + 180.f;
	float fa = radians(angle - progress * 360.f) * radius + 1.f;
	fa = saturate(fa);

	if (fa != 1.f)
		discard;
	vector color = vector(0.f, 0.f, 0.f, 1);
	vector col = lerp(color, MaskTexture, fa);
	//	col.a *= fr;
	vector BarColor = vector(1.f, 0.855f, 0.f, 1.f);
	
	if (col.r != 0.f)
	{
		col.r = BarColor.r;
		col.g = BarColor.g;
		col.b = BarColor.b;
	}
	//col = col * col2;//DiffuseTexture;

	Out.vColor = col;

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

	pass CircleProgress_CL //17
	{
		SetRasterizerState(RS_UI);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Circle_Progress_Clock();
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

	pass WindowLeft //20
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_WindowLeft();
	}

	pass WindowRight //21
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_WindowRight();
	}

	pass Test //22
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Combo_Progressbar();
	}

	pass CircleProgress_CCL //23
	{
		SetRasterizerState(RS_UI);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Circle_Progress_CClock();
	}

	pass OniSpecialBar //24
	{
		SetRasterizerState(RS_UI);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_OniSpecialBarMinus();
	}
}