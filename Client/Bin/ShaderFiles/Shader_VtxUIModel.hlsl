#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
texture2D		g_MaskTexture;

vector			g_vCamPosition;

float4			g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f); //���ǻ�
float4			g_vLightAmbient = float4(0.3f, 0.3f, 0.3f, 1.f); //���� �ּ� ���
float4			g_vBlackAmbient = float4(0.0f, 0.0f, 0.0f, 1.f); //���� �ּ� ���
float4			g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f); //���� ���̶���Ʈ (�����Ÿ��� ���)

float4			g_vLightDir = float4(1.f, -1.f, 0.f, 0.f); // ���ǹ��� //���⼺���� // ��

float4			g_vMtrlAmbient = float4(1.f, 1.f, 1.f, 1.f);  // ������ ������
float4			g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);  // ������ ���̶���Ʈ (�����Ÿ��´���)


/* �����鿡�� ���������� ���. */
/* �������� �޽ÿ��� �Ҽ�. �̶� ���������ϴ� ���� ��� == �� �޽ÿ� ������ �ִ� ����. */
/* �� ��ü�� ���ǵ� ����(xxxxx) */
matrix			g_BoneMatrices[630];

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
	float3		vNormal : NORMAL;
	float		fShade : COLOR0;
	float		fSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
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

	float		fW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix		BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices[In.vBlendIndex.w] * fW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);

	/* ������ ��ġ�� ���� �� ��������� ���Ѵ�. ���� ������ ViewSpace�� �����ϳ�. */
	/* ������ı��� ���ϸ� ������ġ�� w�� �佺���̽� ���� z�� �����Ѵ�. == Out.vPosition�� �ݵ�� float4�̾���ϴ� ����. */
	Out.vPosition = mul(vPosition, matWVP);

	Out.vNormal = In.vNormal;

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);

	Out.fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vWorldNormal)), 0.f);

	vector		vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	vector		vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));
	vector		vLook = vWorldPos - g_vCamPosition;

	Out.fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 20);

	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = vWorldPos;

	//	�ܰ����� Normal, Depth
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vNormal = vNormal;
	Out.vProjPos = Out.vPosition;


	return Out;
}
VS_OUT VS_BreakCar(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* ������ ��ġ�� ���� �� ��������� ���Ѵ�. ���� ������ ViewSpace�� �����ϳ�. */
	/* ������ı��� ���ϸ� ������ġ�� w�� �佺���̽� ���� z�� �����Ѵ�. == Out.vPosition�� �ݵ�� float4�̾���ϴ� ����. */
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	Out.vNormal = In.vNormal;

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);

	Out.fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vWorldNormal)), 0.f);


	vector		vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	vector		vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));
	vector		vLook = vWorldPos - g_vCamPosition;

	Out.fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 10);

	Out.vWorldPos = vWorldPos;

	return Out;
}
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float		fShade : COLOR0;
	float		fSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
	float4		vNormal : SV_TARGET1;	//	�ܰ�����1
	float4		vDepth : SV_TARGET2;	//	�ܰ�����2
};


/* �̷��� ������� �ȼ��� PS_MAIN�Լ��� ���ڷ� ������. */
/* �����ϴ� ���� Target0 == ��ġ�� 0��°�� ���ε��Ǿ��ִ� ����Ÿ��(�Ϲ������� �����)�� �׸���. */
/* �׷��� ����ۿ� ���� �׷�����. */

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	if (In.fShade < 0.05f)
	{
		Out.vColor = (g_vLightDiffuse * vDiffuse) * saturate(0.6f + g_vLightAmbient * g_vMtrlAmbient);
	}
	else
	{
		Out.vColor = (g_vLightDiffuse * vDiffuse) * saturate(0.4f + g_vLightAmbient * g_vMtrlAmbient);
	}

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.1f);

	//Out.vColor = (g_vLightDiffuse * vDiffuse) * saturate(In.fShade + g_vLightAmbient * g_vMtrlAmbient);
	//+(g_vLightSpecular * g_vMtrlSpecular) * In.fSpecular;

	return Out;
}

PS_OUT PS_MASK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vMask = g_MaskTexture.Sample(CLAMPSampler, In.vTexUV);

	Out.vColor = g_DiffuseTexture.Sample(CLAMPSampler, In.vTexUV);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.1f);

	if (vMask.r == 0.f)
		Out.vColor.rgb = 1.f;
	
	return Out;
}
PS_OUT PS_UI(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vDiffuse;

	return Out;
}
PS_OUT PS_UI_BLACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	if (In.fShade < 0.05f)
	{
		Out.vColor = (g_vLightDiffuse * vDiffuse) * saturate(0.6f + g_vBlackAmbient * g_vMtrlAmbient);
	}
	else
	{
		Out.vColor = (g_vLightDiffuse * vDiffuse) * saturate(0.4f + g_vBlackAmbient * g_vMtrlAmbient);
	}

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.1f);

	//Out.vColor = (g_vLightDiffuse * vDiffuse) * saturate(In.fShade + g_vLightAmbient * g_vMtrlAmbient);
	//+(g_vLightSpecular * g_vMtrlSpecular) * In.fSpecular;

	return Out;
}

PS_OUT PS_MASK_BLACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vMask = g_MaskTexture.Sample(CLAMPSampler, In.vTexUV);

	Out.vColor = g_DiffuseTexture.Sample(CLAMPSampler, In.vTexUV);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.1f);

	if (vMask.r == 0.f)
		Out.vColor.rgb = 0.7f;

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
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass MASK //1
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MASK();
	}
	pass UI //2
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UI();
	}
	pass MASK_BLACK //3
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MASK_BLACK();
	}
	pass UI_BLACK //4
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UI_BLACK();
	}
}