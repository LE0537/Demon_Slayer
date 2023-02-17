#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "Component.h"
#include "PipeLine.h"
#include "GameInstance.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::Initialize_Prototype()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_fValue[VALUE_FOGCOLOR_R] = 0.15f;
	m_fValue[VALUE_FOGCOLOR_G] = 0.15f;
	m_fValue[VALUE_FOGCOLOR_B] = 0.4f;
	m_fValue[VALUE_FOGDISTANCE] = 40.f;
	m_fValue[VALUE_FOGRANGE] = 450.f;

	m_fValue[VALUE_AO] = 1.36f;
	m_fValue[VALUE_AORADIUS] = 0.4f;
	m_fValue[VALUE_GLOWBLURCOUNT] = 1.f;
	m_fValue[VALUE_DISTORTION] = 20.f;
	m_fValue[VALUE_OUTLINE] = 300.f;
	m_fValue[VALUE_INNERLINE] = 0.05f;
	m_fValue[VALUE_ENVLIGHT] = 0.9f;
	m_fValue[VALUE_LIGHTSHAFT] = 0.5f;
	m_fValue[VALUE_LIGHTPOWER] = 1.f;
	m_fValue[VALUE_SHADOWTESTLENGTH] = 0.5f;
	m_fValue[VALUE_MAPGRAYSCALETIME] = 15.f;

	m_bRenderAO = true;
	m_bMapGrayScale = false;

	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	m_fFar = 1500.f;//ViewportDesc.MaxDepth;

	/* 렌더타겟들을 추가한다. */

	_uint		iShadowMapCX = (_uint)ViewportDesc.Width * 5;
	_uint		iShadowMapCY = (_uint)ViewportDesc.Height * 5;

	// For.Target_ShadowDepth
	if (FAILED(m_pTarget_Manager->Ready_ShadowDepthStencilRenderTargetView(m_pDevice, iShadowMapCX, iShadowMapCY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth"), iShadowMapCX, iShadowMapCY,
		DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	// For.Target_Static_LightDepth
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Static_LightDepth"), iShadowMapCX, iShadowMapCY,
		DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(1.f, 1.f, 1.f, 0.f)))) return E_FAIL;
	/* For.Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, &_float4(1.f, 1.f, 1.f, 1.f)))) return E_FAIL;
	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;
	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.f, 0.f, 0.f, 1.f)))) return E_FAIL;
	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Specular"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;

	/* For.Target_Glow */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Glow"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;
	/* For.Target_AlphaGlow */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_AlphaGlow"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;
	/* For.Target_GlowAll */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_GlowAll"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;

	/* For.Target_Blur */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;

	_float		fGlowSizeX = ViewportDesc.Width * 1.f / 5.f;
	_float		fGlowSizeY = ViewportDesc.Height * 1.f / 5.f;

	if (FAILED(Ready_GlowDSV(fGlowSizeX, fGlowSizeY)))
		return E_FAIL;
	/* For.Target_GlowX */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_GlowX"), (_uint)fGlowSizeX, (_uint)fGlowSizeY, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;
	/* For.Target_GlowXY */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_GlowXY"), (_uint)fGlowSizeX, (_uint)fGlowSizeY, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;

	/* For.Target_BlurX */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BlurX"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;
	/* For.Target_BlurXY */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BlurXY"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;

	/* For.Target_GrayScale */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_GrayScale"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;
	/* For.Target_Distortion */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Distortion"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;
	/* For.Target_LightShaft */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_LightShaft"), (_uint)ViewportDesc.Width / 5, (_uint)ViewportDesc.Height / 5, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;

	/* For.Target_AO */	//	Ambient Occlusion
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_AO"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(1.f, 1.f, 1.f, 1.f)))) return E_FAIL;

	lstrcpy(m_strPPS_RTName_1, L"Target_PostProcessing_1");
	lstrcpy(m_strPPS_RTName_2, L"Target_PostProcessing_2");
	/* For.Target_PostProcessing_1 */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, m_strPPS_RTName_1, (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.f)))) return E_FAIL;
	/* For.Target_PostProcessing_2 */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, m_strPPS_RTName_2, (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.f)))) return E_FAIL;
	/* For.Target_Master */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Master"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.f)))) return E_FAIL;


	/* For.Target_UINormal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_UINormal"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, &_float4(1.f, 1.f, 1.f, 1.f)))) return E_FAIL;
	/* For.Target_UIDepth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_UIDepth"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;

	/* For.Target_Player */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Player"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;
	/* For.Target_Effect */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f)))) return E_FAIL;


	/* For.MRT_Deferred */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Glow"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Player"))))
		return E_FAIL;

	/* For.MRT_LightAcc */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	// For.MRT_ShadowDepth(Shadow)
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightDepth"), TEXT("Target_ShadowDepth"))))
		return E_FAIL;

	// For.MRT_Static_LightDepth(StaticObj_Shadow)
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Static_LightDepth"), TEXT("Target_Static_LightDepth"))))
		return E_FAIL;

	/* For.MRT_NonLight */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_NonLight"), TEXT("Target_Master"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_NonLight"), TEXT("Target_Glow"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_NonLight"), TEXT("Target_Effect"))))
		return E_FAIL;

	/* For.MRT_AlphaDeferred */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_AlphaDeferred"), TEXT("Target_Master"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_AlphaDeferred"), TEXT("Target_AlphaGlow"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_AlphaDeferred"), TEXT("Target_Effect"))))
		return E_FAIL;

	/* For.MRT_GlowAll */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GlowAll"), TEXT("Target_GlowAll")))) return E_FAIL;
	/* For.MRT_GlowX */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GlowX"), TEXT("Target_GlowX")))) return E_FAIL;
	/* For.MRT_GlowXY */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GlowXY"), TEXT("Target_GlowXY")))) return E_FAIL;

	/* For.MRT_GrayScale */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GrayScale"), TEXT("Target_GrayScale"))))
		return E_FAIL;

	/* For.MRT_Blur */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur"), TEXT("Target_Blur")))) return E_FAIL;
	/* For.MRT_BlurX */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlurX"), TEXT("Target_BlurX")))) return E_FAIL;
	/* For.MRT_BlurXY */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlurXY"), TEXT("Target_BlurXY")))) return E_FAIL;

	/* For.MRT_Distortion*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Distortion"), TEXT("Target_Distortion"))))
		return E_FAIL;
	/* For.MRT_LightShaft */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightShaft"), TEXT("Target_LightShaft"))))
		return E_FAIL;

	/* For.MRT_AO*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_AO"), TEXT("Target_AO"))))
		return E_FAIL;

	lstrcpy(m_strPPS_MRTName_1, L"MRT_PostProcessing_1");
	lstrcpy(m_strPPS_MRTName_2, L"MRT_PostProcessing_2");
	/* For.MRT_PostProcessing_1 */
	if (FAILED(m_pTarget_Manager->Add_MRT(m_strPPS_MRTName_1, m_strPPS_RTName_1)))
		return E_FAIL;
	/* For.MRT_PostProcessing_2 */
	if (FAILED(m_pTarget_Manager->Add_MRT(m_strPPS_MRTName_2, m_strPPS_RTName_2)))
		return E_FAIL;
	/* For.MRT_Master */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Master"), TEXT("Target_Master"))))
		return E_FAIL;

	/* For.MRT_UIMaster */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_UIMaster"), TEXT("Target_Master"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_UIMaster"), TEXT("Target_UINormal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_UIMaster"), TEXT("Target_UIDepth"))))
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f)));

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_WorldMatrix._11 = ViewportDesc.Width;
	m_WorldMatrix._22 = ViewportDesc.Height;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));


	_float		fVIBufferRadius = 120.f;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 0.5f * fVIBufferRadius, 0.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 0.5f * fVIBufferRadius, 1.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 0.5f * fVIBufferRadius, 2.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 0.5f * fVIBufferRadius, 3.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), 0.5f * fVIBufferRadius, 4.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Glow"), 1.5f * fVIBufferRadius, 0.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_AlphaGlow"), 1.5f * fVIBufferRadius, 1.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_GlowAll"), 1.5f * fVIBufferRadius, 2.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_GlowX"), 1.5f * fVIBufferRadius, 3.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_GlowXY"), 1.5f * fVIBufferRadius, 4.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BlurXY"), 1.5f * fVIBufferRadius, 5.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_GrayScale"), 2.5f * fVIBufferRadius, 0.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Distortion"), 2.5f * fVIBufferRadius, 1.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_AO"), 2.5f * fVIBufferRadius, 2.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_LightShaft"), 2.5f * fVIBufferRadius, 3.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Player"), 2.5f * fVIBufferRadius, 4.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect"), 2.5f * fVIBufferRadius, 5.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Master"), ViewportDesc.Width - (0.5f * 2.f * fVIBufferRadius), 0.5f * 2.f * fVIBufferRadius, 2.f * fVIBufferRadius, 2.f * fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowDepth"), ViewportDesc.Width - (0.5f * fVIBufferRadius), 2.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Static_LightDepth"), ViewportDesc.Width - (0.5f * fVIBufferRadius), 3.5f * fVIBufferRadius, fVIBufferRadius, fVIBufferRadius)))
		return E_FAIL;


	//	렌더타겟 초기화
	if (FAILED(m_pTarget_Manager->Begin_ShadowMRT(m_pContext, TEXT("MRT_Static_LightDepth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup_Front(RENDERGROUP eRenderGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects[eRenderGroup].push_front(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Render_GameObjects(_float fTimeDelta, _bool _bDebug, _int _iLevel)
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_StaticShadowDepth()))
		return E_FAIL;
	if (FAILED(Render_ShadowDepth()))
		return E_FAIL;

	//	광원 효과를 받는 객체를 Deferred에 그립니다.
	if (FAILED(Render_NonAlphaBlend()))
		return E_FAIL;
	if (FAILED(Render_Lights()))
		return E_FAIL;

	if (FAILED(Render_AO()))
		return E_FAIL;
	//	그려진 객체들을 아름답게 섞습니다.
	if (FAILED(Render_Blend(_iLevel)))
		return E_FAIL;

	m_fMapGrayScaleTime += fTimeDelta;
	if (10.f < m_fMapGrayScaleTime)
	{
		m_bMapGrayScale = false;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (true == pGameInstance->Key_Down(DIK_Y))
	{
		if (false == m_bMapGrayScale)
		{
			m_bMapGrayScale = !m_bMapGrayScale;
			m_fMapGrayScaleTime = 0.f;

			m_fMapGrayScalePower = 1.f;
		}
	}

	if (false == m_bMapGrayScale)
		m_fMapGrayScalePower /= (fTimeDelta * 65.f);	//	(1.2f)

	RELEASE_INSTANCE(CGameInstance);


	if (FAILED(Render_OutLine()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;
	if (FAILED(Render_AlphaBlend()))
		return E_FAIL;


	if (FAILED(Render_Effect()))
		return E_FAIL;

	//	NonAlpha의 Glow, Alpha의 Glow를 취합합니다.
	if (FAILED(Ready_GlowTexture()))
		return E_FAIL;

	//	포스트 프로세싱 : 외곽선을 포함한 " 전체 " 에 효과를 먹입니다.
	//	postprocessing_1 = master
	if (FAILED(Ready_PostProcessing()))
		return E_FAIL;

	_int	iIndex = 0;
	_tchar	pRTName[MAX_PATH] = L"";
	_tchar	pMRTName[MAX_PATH] = L"";
	while (true)
	{
		if (0 == iIndex % 2)
		{
			lstrcpy(pRTName, m_strPPS_RTName_1);
			lstrcpy(pMRTName, m_strPPS_MRTName_2);
		}
		else
		{
			lstrcpy(pRTName, m_strPPS_RTName_2);
			lstrcpy(pMRTName, m_strPPS_MRTName_1);
		}
		if (ORDER_END <= iIndex)
			break;

		switch (iIndex)
		{
		case ORDER_GLOW:
			if (FAILED(Render_Glow(pRTName, pMRTName))) return E_FAIL;
			break;
		case ORDER_MAPGRAYSCALE:
			if (FAILED(Render_MapGrayScale(pRTName, pMRTName))) return E_FAIL;
			break;
		case ORDER_GRAYSCALE:
			if (FAILED(Render_GrayScale(pRTName, pMRTName))) return E_FAIL;
			break;
		case ORDER_BLUR:
			if (FAILED(Render_Blur(pRTName, pMRTName))) return E_FAIL;
			break;
		case ORDER_LIGHTSHAFT:
			if (FAILED(Render_LightShaft(pRTName, pMRTName))) return E_FAIL;
			break;
		case ORDER_DISTORTION:
			if (FAILED(Render_Distortion(pRTName, pMRTName))) return E_FAIL;
			break;
		default: break;
		}

		++iIndex;
	}

	//	객체 Render가 끝나면 PostProcessing_n 를 Master Target에 그립니다.
	if (FAILED(Render_Master(pRTName)))
		return E_FAIL;




	if (FAILED(Render_UI()))
		return E_FAIL;
	if (FAILED(Render_UIPOKE()))
		return E_FAIL;

	if (FAILED(Render_UIMaster()))
		return E_FAIL;



	if (FAILED(Render_Debug(_bDebug)))
		return E_FAIL;

	//	Clear MRT_Master
	if (FAILED(m_pTarget_Manager->MRT_Clear(m_pContext, TEXT("MRT_Master"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->MRT_Clear(m_pContext, TEXT("MRT_AlphaDeferred"))))
		return E_FAIL;

	return S_OK;
}
HRESULT CRenderer::Add_Debug(CComponent* pDebugCom)
{
	m_DebugComponents.push_back(pDebugCom);

	Safe_AddRef(pDebugCom);

	return S_OK;
}
HRESULT CRenderer::Ready_GlowDSV(_float fWinCX, _float fWinCY)
{
	ID3D11Texture2D*		pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = (_uint)fWinCX;
	m_fGlowWinCX = fWinCX;
	TextureDesc.Height = (_uint)fWinCY;
	m_fGlowWinCY = fWinCY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;


	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pGlowDSV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}
HRESULT CRenderer::Set_Viewport(_float fWinCX, _float fWinCY)
{
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = fWinCX;
	ViewPortDesc.Height = fWinCY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	return S_OK;
}
HRESULT CRenderer::Render_Priority()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Deferred"))))
		return E_FAIL;

	for (auto& pGameObject : m_GameObjects[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_PRIORITY].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_StaticShadowDepth()
{
	//	사이즈가 바뀌는 경우는 레벨이 바뀌고 오브젝트가 생성되어 Initialize에서 Static_ShadowDepth렌더그룹에 추가할 떄 입니다.
	//	한 번 그리고 다시는 안그릴 것이기 때문에 타겟을 그대로 보존합니다.
	_int iIndex = m_GameObjects[RENDER_STATIC_SHADOWDEPTH].size();
	if (0 == iIndex)
		return S_OK;

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);
	m_FirstProjmatrix = pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	RELEASE_INSTANCE(CPipeLine);

	if (FAILED(m_pTarget_Manager->Begin_ShadowMRT(m_pContext, TEXT("MRT_Static_LightDepth"))))
		return E_FAIL;
	for (auto& pGameObject : m_GameObjects[RENDER_STATIC_SHADOWDEPTH])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_ShadowDepth();
		Safe_Release(pGameObject);
	}
	m_GameObjects[RENDER_STATIC_SHADOWDEPTH].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_ShadowDepth()
{
	if (FAILED(m_pTarget_Manager->Begin_ShadowMRT(m_pContext, TEXT("MRT_LightDepth"))))
		return E_FAIL;

	for (auto& pGameObject : m_GameObjects[RENDER_SHADOWDEPTH])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_ShadowDepth();
		Safe_Release(pGameObject);
	}

	m_GameObjects[RENDER_SHADOWDEPTH].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonAlphaBlend()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* 현재까지는 백버퍼가 셋팅되어있었지만.
	빛연산ㅇ에 필요한 정보를 받아오기위해 MRT_Deferred타겟들을 바인딩한다. */
	/* Target_Diffuse, Target_Normal에 그린다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_Deferred"))))
		return E_FAIL;

	for (auto& pGameObject : m_GameObjects[RENDER_NONALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_NONALPHABLEND].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}
HRESULT CRenderer::Render_Lights()
{
	if (nullptr == m_pTarget_Manager ||
		nullptr == m_pLight_Manager)
		return E_FAIL;

	/* Target_Shader에 그린다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_LightAcc"))))
		return E_FAIL;

	/* 노말 렌더타겟의 SRV를 셰이더에 바인딩 한다. */
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	CPipeLine*				pPipeLine = GET_INSTANCE(CPipeLine);

	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CPipeLine);

	//	캐릭터에 영향이 가지 않는 빛의 세기
	if (FAILED(m_pShader->Set_RawValue("g_fEnvLightValue", &m_fValue[VALUE_ENVLIGHT], sizeof(_float))))
		return E_FAIL;
	//	얘는 감
	if (FAILED(m_pShader->Set_RawValue("g_fLightPower", &m_fValue[VALUE_LIGHTPOWER], sizeof(_float))))
		return E_FAIL;

	//	그림자가 검수되는 길이
	if (FAILED(m_pShader->Set_RawValue("g_fShadowTestLength", &m_fValue[VALUE_SHADOWTESTLENGTH], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pLight_Manager->Render_Lights(m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Render_AO()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_AO"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_fFar", &m_fFar, sizeof(_float))))
		return E_FAIL;
	_float	fRadius = 0.001f * m_fValue[VALUE_AORADIUS];
	if (FAILED(m_pShader->Set_RawValue("g_fSSAORadius", &fRadius, sizeof(_float))))
		return E_FAIL;
	_float	fAOValue = 0.0002f * m_fValue[VALUE_AO];
	if (FAILED(m_pShader->Set_RawValue("g_fAOValue", &fAOValue, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_bRenderAO", &m_bRenderAO, sizeof(_bool))))
		return E_FAIL;

	m_pShader->Begin(9);
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blend(_int _iLevel)
{
	/* 백버퍼에 그린다. */
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_AO"), m_pShader, "g_AOTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_ShadowDepth"), m_pShader, "g_ShadowDepthTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Static_LightDepth"), m_pShader, "g_ShadowDepthTexture_Once")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;


	_float3			vFogColor = _float3(m_fValue[VALUE_FOGCOLOR_R], m_fValue[VALUE_FOGCOLOR_G], m_fValue[VALUE_FOGCOLOR_B]);
	if (FAILED(m_pShader->Set_RawValue("g_vFogColor", &vFogColor, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_fFogDistance", &m_fValue[VALUE_FOGDISTANCE], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_fFogRange", &m_fValue[VALUE_FOGRANGE], sizeof(_float))))
		return E_FAIL;

	CLevel_Manager*		pLevelMgr = GET_INSTANCE(CLevel_Manager);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);

	_vector		vLightEye, vLightAt, vLightUp;
	_matrix		matLightView;
	const LIGHTDESC* pLightDesc = nullptr;

	if (_iLevel == 1)
		pLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_FIELDSHADOW);
	else if (_iLevel == 2)
		pLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_RUISHADOW);

	if (nullptr != pLightDesc)
	{
		vLightEye = XMLoadFloat4(&pLightDesc->vDirection);
		vLightAt = XMLoadFloat4(&pLightDesc->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

		if (FAILED(m_pShader->Set_RawValue("g_matLightView", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShader->Set_RawValue("g_matLightProj", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShader->Set_RawValue("g_ProjMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShader->Set_RawValue("g_ViewMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
			return E_FAIL;


	}

	//	StaticObjs
	if (nullptr != pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW))
	{
		vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW)->vDirection);
		vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW)->vDiffuse);
		vLightUp = { 0.f, 1.f, 0.f ,0.f };
		matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);

		if (FAILED(m_pShader->Set_RawValue("g_StaticShadowProj", &XMMatrixTranspose(XMLoadFloat4x4(&m_FirstProjmatrix)), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShader->Set_RawValue("g_matLightView_Static", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
			return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CPipeLine);


	RELEASE_INSTANCE(CLevel_Manager);

	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_Master"))))
		return E_FAIL;

	m_pShader->Begin(3);
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_OutLine()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;


	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_fOutLineValue", &m_fValue[VALUE_OUTLINE], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_fInnerLineValue", &m_fValue[VALUE_INNERLINE], sizeof(_float))))
		return E_FAIL;
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);
	if (FAILED(m_pShader->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	RELEASE_INSTANCE(CPipeLine);


	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_Master"))))
		return E_FAIL;

	//	OutLine
	m_pShader->Begin(4);
	m_pVIBuffer->Render();

	//	InnerLine
	m_pShader->Begin(5);
	m_pVIBuffer->Render();


	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_NonLight"))))
		return E_FAIL;

	for (auto& pGameObject : m_GameObjects[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_NONLIGHT].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_AlphaBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_AlphaDeferred"))))
		return E_FAIL;

	m_GameObjects[RENDER_ALPHABLEND].sort([](CGameObject* pSour, CGameObject* pDest)
	{
		return pSour->Get_CamDistance() > pDest->Get_CamDistance();
	});

	for (auto& pGameObject : m_GameObjects[RENDER_ALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_ALPHABLEND].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Effect()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_AlphaDeferred"))))
		return E_FAIL;

	for (auto& pGameObject : m_GameObjects[RENDER_EFFECT])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}
	m_GameObjects[RENDER_EFFECT].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Ready_GlowTexture()
{
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Glow"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_AlphaGlow"), m_pShader, "g_AddTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;




	_float fAddValue = 1.f;
	if (FAILED(m_pShader->Set_RawValue("g_fAddValue", &fAddValue, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GlowAll"))))
		return E_FAIL;
	m_pShader->Begin(13);
	m_pVIBuffer->Render();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Ready_PostProcessing()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_PostProcessing_1"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Master"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	m_pShader->Begin(0);		//	Default
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Render_LightShaft(const _tchar * pTexName, const _tchar * pMRTName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	const LIGHTDESC* pLightDesc = pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW);
	if (nullptr == pLightDesc)
	{
		RELEASE_INSTANCE(CGameInstance);
		return S_OK;
	}

	_vector	vLightDir = XMLoadFloat4(&pLightDesc->vDiffuse);



	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Static_LightDepth"), m_pShader, "g_ShadowDepthTexture")))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_vLightDir", &vLightDir, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	_vector			vLightEye = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW)->vDirection);
	_vector			vLightAt = XMLoadFloat4(&pGameInstance->Get_ShadowLightDesc(LIGHTDESC::TYPE_BATTLESHADOW)->vDiffuse);
	_vector			vLightUp = { 0.f, 1.f, 0.f ,0.f };
	_matrix			matLightView = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	if (FAILED(m_pShader->Set_RawValue("g_vLightPos", &vLightEye, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_matLightView", &XMMatrixTranspose(matLightView), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_matLightProj", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;



	RELEASE_INSTANCE(CPipeLine);
	RELEASE_INSTANCE(CGameInstance);

	Set_Viewport(m_fGlowWinCX, m_fGlowWinCY);

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_LightShaft"), m_pGlowDSV)))
		return E_FAIL;

	m_pShader->Begin(14);		//	LightShaft
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	Set_Viewport(1280.f, 720.f);



	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(pTexName, m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_LightShaft"), m_pShader, "g_AddTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_fAddValue", &m_fValue[VALUE_LIGHTSHAFT], sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, pMRTName)))
		return E_FAIL;

	//m_pShader->Begin(0);
	m_pShader->Begin(13);
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Glow(const _tchar * pTexName, const _tchar * pMRTName)
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_GlowAll"), m_pShader, "g_GlowTexture")))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;



	Set_Viewport(m_fGlowWinCX, m_fGlowWinCY);

	//	GlowX
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GlowX"), m_pGlowDSV)))
		return E_FAIL;

	m_pContext->ClearDepthStencilView(m_pGlowDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	m_pShader->Begin(6);
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	Set_Viewport(1280.f, 720.f);



	Set_Viewport(m_fGlowWinCX, m_fGlowWinCY);

	//	GlowXY
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_GlowX"), m_pShader, "g_GlowTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GlowXY"), m_pGlowDSV)))
		return E_FAIL;

	m_pShader->Begin(7);
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	Set_Viewport(1280.f, 720.f);




	//	Render to MRT
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(pTexName, m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_GlowXY"), m_pShader, "g_AddTexture")))
		return E_FAIL;
	_float fAddValue = 1.f;
	if (FAILED(m_pShader->Set_RawValue("g_fAddValue", &fAddValue, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, pMRTName)))
		return E_FAIL;
	m_pShader->Begin(13);
	m_pVIBuffer->Render();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	return S_OK;

}

HRESULT CRenderer::Render_Blur(const _tchar* pTexName, const _tchar* pMRTName)
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blur"))))
		return E_FAIL;
	for (auto& pGameObject : m_GameObjects[RENDER_BLUR])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}
	m_GameObjects[RENDER_BLUR].clear();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(pTexName, m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Blur"), m_pShader, "g_BlurTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	//	BlurX
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BlurX"))))
		return E_FAIL;
	m_pShader->Begin(10);
	m_pVIBuffer->Render();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	//	BlurXY
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_BlurX"), m_pShader, "g_BlurTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BlurXY"))))
		return E_FAIL;
	m_pShader->Begin(11);
	m_pVIBuffer->Render();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	//	Master
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(pTexName, m_pShader, "g_BlurTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, pMRTName)))
		return E_FAIL;
	m_pShader->Begin(0);
	m_pVIBuffer->Render();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Render_MapGrayScale(const _tchar * pTexName, const _tchar * pMRTName)
{
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Player"), m_pShader, "g_PlayerTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Effect"), m_pShader, "g_EffectTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	_float	fGrayScaleTimeRatio = min(m_fMapGrayScaleTime / m_fValue[VALUE_MAPGRAYSCALETIME], 1.f);
	if (FAILED(m_pShader->Set_RawValue("g_fMapGrayScaleTimeRatio", &fGrayScaleTimeRatio, sizeof(_float))))
		return E_FAIL;
	_float	fGrayScaleFogRange = 10.f;
	if (FAILED(m_pShader->Set_RawValue("g_fMapGrayScaleFogRange", &fGrayScaleFogRange, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_fMapGrayScalePower", &m_fMapGrayScalePower, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(pTexName, m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, pMRTName)))
		return E_FAIL;
	m_pShader->Begin(15);
	m_pVIBuffer->Render();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_GrayScale(const _tchar * pTexName, const _tchar * pMRTName)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GrayScale"))))
		return E_FAIL;

	for (auto& pGameObject : m_GameObjects[RENDER_GRAYSCALE])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_GRAYSCALE].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;




	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(pTexName, m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_GrayScale"), m_pShader, "g_GrayScaleTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, pMRTName)))
		return E_FAIL;

	m_pShader->Begin(12);
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Distortion(const _tchar* pTexName, const _tchar* pMRTName)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Distortion"))))
		return E_FAIL;
	for (auto& pGameObject : m_GameObjects[RENDER_DISTORTION])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}
	m_GameObjects[RENDER_DISTORTION].clear();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(pTexName, m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Distortion"), m_pShader, "g_DistortionTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_fDistortionValue", &m_fValue[VALUE_DISTORTION], sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, pMRTName)))
		return E_FAIL;

	m_pShader->Begin(8);
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Master(const _tchar* pTexName)
{
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(pTexName, m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_Master"))))
		return E_FAIL;
	m_pShader->Begin(0);
	m_pVIBuffer->Render();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_UIMaster"))))
		return E_FAIL;
	for (auto& pGameObject : m_GameObjects[RENDER_UI])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}
	m_GameObjects[RENDER_UI].clear();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;




	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_UIDepth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_UINormal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrixInv", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_float		fOutLineValue = 10;
	if (FAILED(m_pShader->Set_RawValue("g_fOutLineValue", &m_fValue[VALUE_OUTLINE], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_fInnerLineValue", &m_fValue[VALUE_INNERLINE], sizeof(_float))))
		return E_FAIL;



	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_Master"))))
		return E_FAIL;

	//	OutLine
	m_pShader->Begin(4);
	m_pVIBuffer->Render();

	//	InnerLine
	m_pShader->Begin(5);
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_UIPOKE()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_UIMaster"))))
		return E_FAIL;
	for (auto& pGameObject : m_GameObjects[RENDER_UIPOKE])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}
	m_GameObjects[RENDER_UIPOKE].clear();
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;



	return S_OK;
}

HRESULT CRenderer::Render_UIMaster()
{
	/*
	if (FAILED(m_pTarget_Manager->Begin_MRT_NonClear(m_pContext, TEXT("MRT_PostProcessing_2"))))
		return E_FAIL;
	*/

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(L"Target_Master", m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	m_pShader->Begin(0);
	m_pVIBuffer->Render();
	/*
		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;
	*/

	return S_OK;
}

HRESULT CRenderer::Render_Debug(_bool _bDebug)
{
	if (nullptr == m_pShader ||
		nullptr == m_pVIBuffer)
		return E_FAIL;
	if (_bDebug)
	{
		if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
			return E_FAIL;
	}
	for (auto& pComponent : m_DebugComponents)
	{
		if (nullptr != pComponent)
		{
			if (_bDebug)
				pComponent->Render();
		}
		Safe_Release(pComponent);
	}

	m_DebugComponents.clear();

	if (_bDebug)
	{
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_LightDepth"), m_pShader, m_pVIBuffer)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_GrayScale"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Distortion"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_AO"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Master"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_BlurXY"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_LightShaft"), m_pShader, m_pVIBuffer)))
			return E_FAIL;

		//	if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_Glow"), m_pShader, m_pVIBuffer)))
		//	return E_FAIL;		by MRT_Deferred
		if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_AlphaGlow"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_GlowX"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_GlowXY"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_GlowAll"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_Static_LightDepth"), m_pShader, m_pVIBuffer)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_Player"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_SoloTarget_Debug(TEXT("Target_Effect"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
	}
	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
	Safe_Release(m_pGlowDSV);

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);
}
