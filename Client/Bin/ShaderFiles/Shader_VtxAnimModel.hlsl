#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
texture2D		g_NormalTexture;
texture2D		g_MaskTexture;

matrix			g_BoneMatrices[630];

float		g_fFar;

texture2D		g_DissolveTexture;
vector			g_vDissolveColor;
float			g_fDeadTimeRatio;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float4		vWorld : TEXCOORD2;
};
struct VS_NORMALOUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float4		vWorld : TEXCOORD2;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
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

	float		fW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix		BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices[In.vBlendIndex.w] * fW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vNormal = normalize(mul(vNormal, g_WorldMatrix));

	/* 정점의 위치에 월드 뷰 투영행렬을 곱한다. 현재 정점은 ViewSpace에 존재하낟. */
	/* 투영행렬까지 곱하면 정점위치의 w에 뷰스페이스 상의 z를 보관한다. == Out.vPosition이 반드시 float4이어야하는 이유. */
	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = vNormal;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	vector vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vWorld = vWorldPos;

	return Out;
}
VS_NORMALOUT VS_NORMAL(VS_IN In)
{
	VS_NORMALOUT		Out = (VS_NORMALOUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float		fW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix		BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices[In.vBlendIndex.w] * fW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vNormal = normalize(mul(vNormal, g_WorldMatrix));

	/* 정점의 위치에 월드 뷰 투영행렬을 곱한다. 현재 정점은 ViewSpace에 존재하낟. */
	/* 투영행렬까지 곱하면 정점위치의 w에 뷰스페이스 상의 z를 보관한다. == Out.vPosition이 반드시 float4이어야하는 이유. */
	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = vNormal;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	vector vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vWorld = vWorldPos;

	vector		vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);
	vTangent = normalize(mul(vTangent, g_WorldMatrix));
	Out.vTangent = vTangent;
	Out.vBinormal = cross(Out.vNormal, Out.vTangent);

	return Out;
}
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float4		vWorld : TEXCOORD2;

};
struct PS_NORMALIN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float4		vWorld : TEXCOORD2;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};
struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;

	float4		vDrawPlayer : SV_TARGET4;	//	Player, AnimMode 혹은 Effect 를 따로 그려둠.
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

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.1f);
	Out.vDrawPlayer = Out.vDiffuse;
	Out.vWorld = In.vWorld/* / g_fFar*/;

	if (Out.vDiffuse.a <= 0.3f)
		discard;

	return Out;
}
PS_OUT PS_RUI(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	vector vColor = { 0.f,0.f,0.f,1.f };
	vColor.r = 1.f;
	Out.vDiffuse = vColor;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.1f);
	Out.vDrawPlayer = Out.vDiffuse;
	Out.vWorld = In.vWorld/* / g_fFar*/;

	if (Out.vDiffuse.a <= 0.3f)
		discard;

	return Out;
}
PS_OUT PS_NORMAL(PS_NORMALIN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float3	vNormal = g_NormalTexture.Sample(LinearSampler, In.vTexUV).xyz;

	vNormal = vNormal * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = mul(vNormal, WorldMatrix);

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.1f);
	Out.vDrawPlayer = Out.vDiffuse;
	Out.vWorld = In.vWorld/* / g_fFar*/;

	if (Out.vDiffuse.a <= 0.3f)
		discard;

	return Out;
}
PS_OUT_SHADOW PS_Shadow(PS_IN In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.x = In.vProjPos.w / g_fFar;

	Out.vLightDepth.a = 1.f;

	return Out;
}
PS_OUT PS_MASK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float4 vMask = g_MaskTexture.Sample(CLAMPSampler, In.vTexUV);

	Out.vDiffuse = g_DiffuseTexture.Sample(CLAMPSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.1f);
	Out.vDrawPlayer = Out.vDiffuse;
	Out.vWorld = In.vWorld /*/ g_fFar*/;

	if (vMask.r == 0.f)
		Out.vDiffuse.rgb = 1.f;
	
	return Out;
}

struct PS_GLOWOUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
	float4		vGlow : SV_TARGET3;
	float4		vDrawPlayer : SV_TARGET4;	//	Player, AnimMode 혹은 Effect 를 따로 그려둠.
	float4		vWorld : SV_TARGET5;
};

PS_GLOWOUT PS_ONI_DISSOLVE(PS_IN In)
{
	PS_GLOWOUT		Out = (PS_OUT)0;
	
	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.1f);
	Out.vDrawPlayer = Out.vDiffuse;
	Out.vWorld = In.vWorld /*/ g_fFar*/;

	Out.vDiffuse.a = (g_DissolveTexture.Sample(LinearSampler, In.vTexUV)).r - g_fDeadTimeRatio;
	float	fDissolvingValue = Out.vDiffuse.a + 0.1f;
	
	if (Out.vDiffuse.a < fDissolvingValue)
	{
		Out.vDiffuse.rgb = float3(1.f, 0.3f, 0.f);
		Out.vGlow = float3(1.f, 1.f, 0.f);
	}

	if (Out.vDiffuse.a <= 0.01f)
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

	pass Shadow //1
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Shadow, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Shadow();
	}
	pass MASK //2
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MASK();
	}
	pass Normal //3
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_NORMAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NORMAL();
	}
	pass RuiHand //4
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_RUI();
	}
	pass Oni_Dissolve	//	5
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ONI_DISSOLVE();
	}
}