#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector			g_vCamPosition;

float3			g_vColor;
float3			g_vSplinePos1;
float3			g_vSplinePos2;
float3			g_vSplinePos3;
float3			g_vSplinePos4;
float3			g_vSplinePos5;
float3			g_vSplinePos6;
float3			g_vSplinePos7;

struct VS_IN
{
	float3		vPosition : POSITION;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Out.vPosition = vector(In.vPosition, 1.f);	//	들어 올 때 Local = World. (World = 항등)

	return Out;
}


//==========================================================================================
//=======================================  GS  =============================================
//==========================================================================================
struct GS_IN
{
	float4		vPosition : SV_POSITION;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
};

[maxvertexcount(20)]
void GS_MAIN(line GS_IN In[2], inout LineStream<GS_OUT> DataStream)
{
	GS_OUT			Out[9];

	matrix			matVP;
	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	Out[0].vPosition = In[0].vPosition;
	Out[0].vPosition = mul(Out[0].vPosition, matVP);

	Out[1].vPosition = float4(g_vSplinePos1, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);

	Out[2].vPosition = float4(g_vSplinePos2, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);

	Out[3].vPosition = float4(g_vSplinePos3, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);

	Out[4].vPosition = float4(g_vSplinePos4, 1.f);
	Out[4].vPosition = mul(Out[4].vPosition, matVP);

	Out[5].vPosition = float4(g_vSplinePos5, 1.f);
	Out[5].vPosition = mul(Out[5].vPosition, matVP);

	Out[6].vPosition = float4(g_vSplinePos6, 1.f);
	Out[6].vPosition = mul(Out[6].vPosition, matVP);

	Out[7].vPosition = float4(g_vSplinePos7, 1.f);
	Out[7].vPosition = mul(Out[7].vPosition, matVP);

	Out[8].vPosition = In[1].vPosition;
	Out[8].vPosition = mul(Out[8].vPosition, matVP);

	DataStream.Append(Out[0]);
	DataStream.Append(Out[1]);
	DataStream.RestartStrip();

	DataStream.Append(Out[1]);
	DataStream.Append(Out[2]);
	DataStream.RestartStrip();

	DataStream.Append(Out[2]);
	DataStream.Append(Out[3]);
	DataStream.RestartStrip();

	DataStream.Append(Out[3]);
	DataStream.Append(Out[4]);
	DataStream.RestartStrip();

	DataStream.Append(Out[4]);
	DataStream.Append(Out[5]);
	DataStream.RestartStrip();

	DataStream.Append(Out[5]);
	DataStream.Append(Out[6]);
	DataStream.RestartStrip();

	DataStream.Append(Out[6]);
	DataStream.Append(Out[7]);
	DataStream.RestartStrip();

	DataStream.Append(Out[7]);
	DataStream.Append(Out[8]);
	DataStream.RestartStrip();
}




//==========================================================================================
//=======================================  PS  =============================================
//==========================================================================================
struct PS_IN
{
	float4		vPosition : SV_POSITION;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor.rgb = g_vColor.rgb;
	Out.vColor.a = 1.f;

	return Out;
}

technique11 DefaultTechnique
{
	pass Default //	0
	{ 
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}