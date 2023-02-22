
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

float3			g_vColor;

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

	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vWorld : TEXCOORD2;
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

	vector vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));

	/* 정점의 위치에 월드 뷰 투영행렬을 곱한다. 현재 정점은 ViewSpace에 존재하낟. */
	/* 투영행렬까지 곱하면 정점위치의 w에 뷰스페이스 상의 z를 보관한다. == Out.vPosition이 반드시 float4이어야하는 이유. */
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = vNormal;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	vector	vWorld = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vWorld = vWorld;

	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = cross(Out.vNormal, Out.vTangent);
	
	return Out;
}

struct VS_FLOWMAP_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float2		vTexCoord1 : TEXCOORD2;		//	플로우맵 정보를 저장합니다.
	float2		vWorld : TEXCOORD3;
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

	Out.vTexUV = In.vTexUV;
	Out.vTexCoord1.x = In.vTexUV.x + (frac(g_fCurrentTime) + 0.1f);
	Out.vTexCoord1.y = In.vTexUV.y + (0.1f);

	vector	vWorld = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vWorld = vWorld;


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
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
	float4		vGlow : SV_TARGET3;

	float4		vWorld : SV_TARGET5;
};
struct PS_OUT_SHADOW
{
	float4			vLightDepth :  SV_TARGET0;
};
/* 이렇게 만들어진 픽셀을 PS_MAIN함수의 인자로 던진다. */
/* 리턴하는 색은 Target0 == 장치에 0번째에 바인딩되어있는 렌더타겟(일반적으로 백버퍼)에 그린다. */
/* 그래서 백버퍼에 색이 그려진다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float3	vNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV).xyz;

	vNormal = vNormal * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, vNormal);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1800.f, 0.f, 0.f);
	Out.vGlow = g_GlowTexture.Sample(LinearSampler, In.vTexUV) * g_fGlowPower;
	Out.vWorld = In.vWorld / 1800.f;

	if (Out.vDiffuse.a <= 0.1f)
		discard;

	return Out;
}

PS_OUT_SHADOW PS_SHADOW(PS_IN In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjPos.w / 1800.f;

	Out.vLightDepth.a = 1.f;

	return Out;
}

PS_OUT PS_MAP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1800.f, 0.f, 0.f);
	Out.vWorld = In.vWorld / 1800.f;


	if (Out.vDiffuse.a <= 0.3f)
		discard;

	return Out;
}


struct PS_ALPHAOUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vGlow : SV_TARGET1;
};
PS_ALPHAOUT PS_ALPHABLEND(PS_IN In)
{
	PS_ALPHAOUT		Out = (PS_ALPHAOUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vGlow = g_GlowTexture.Sample(LinearSampler, In.vTexUV) * g_fGlowPower;

	if (Out.vDiffuse.a <= 0.03f)
		discard;

	return Out;
}
struct PS_FINAL
{
	float4		vDiffuse : SV_TARGET0;
};
PS_FINAL PS_FINAL2(PS_IN In)
{
	PS_FINAL		Out = (PS_FINAL)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (Out.vDiffuse.a <= 0.03f)
		discard;

	return Out;
}



PS_FINAL PS_CAMCLUSTER(PS_IN In)
{
	PS_FINAL		Out = (PS_FINAL)0;

	Out.vDiffuse.rgb = g_vColor;
	Out.vDiffuse.a = 1.f;

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

struct PS_EFF_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vGlow : SV_TARGET1;
	float4		vDrawEffect : SV_TARGET2;	//	Player, AnimMode 혹은 Effect 를 따로 그려둠.
};

PS_OUT PS_FLOWMAP(PS_FLOWMAP_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//	NoiseTexture의 UV = In.vTexCoord1
	float2 noise1 = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord1);
	float2 noise2 = g_NoiseTexture.Sample(LinearSampler, In.vTexCoord1 * 2.f);
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;

	float2 distortion1 = float2(g_fDistortionU, g_fDistortionV);
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion1.xy;

	float2	finalNoise = noise1.x + noise2.y;		//	Noise[0]는 X를, Noise[1]는 Y를 왜곡합니다.	
	float	perturb = (g_fDistortionScale)+g_fDistortionBias;

	float2		vNewUV = In.vTexUV;

	float2	noiseCoords = (finalNoise.xy * perturb) + vNewUV;
	noiseCoords.x += g_fMoveUV_U;
	noiseCoords.y += g_fMoveUV_V;
	//	텍스쳐 왜곡 끝


	float2	vTexUVMul = float2(g_iMulUV_U, g_iMulUV_V);

	vector		vTexture = g_DiffuseTexture.Sample(LinearSampler, noiseCoords);
	Out.vDiffuse = vTexture;

	vector		vMaskTexture = g_MaskTexture.Sample(LinearSampler, noiseCoords);

	if (Out.vDiffuse.a < 0.3f)
		discard;

	return Out;
}






technique11 DefaultTechnique
{
	pass Default //0
	{
		SetRasterizerState(RS_Effect);
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

	pass Map //2
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAP();
	}

	pass AlphaBlend		//	3
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHABLEND();
	}

	pass Flowmap		//	4
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_FLOWMAP();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FLOWMAP();
	}

	pass CamCluster		//	5
	{
		SetRasterizerState(RS_WireframeNonCull);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CAMCLUSTER();
	}
	pass AlphaBlend2		//	6
	{
		SetRasterizerState(RS_Effect);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FINAL2();
	}
}