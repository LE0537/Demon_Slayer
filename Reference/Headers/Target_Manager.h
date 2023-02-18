#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)

private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTargetTag,
		_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4* pColor);

	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag, ID3D11DepthStencilView* pDSV);

	HRESULT Begin_MRT_NonClear(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT Begin_MRT_NonClear(ID3D11DeviceContext* pContext, const _tchar* pMRTTag, ID3D11DepthStencilView* pDSV);
	HRESULT Begin_ShadowMRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag, const _tchar* pDSVTag, _uint iWinCX, _uint iWinCY);
	HRESULT Begin_ShadowMRT_NonClear(ID3D11DeviceContext* pContext, const _tchar* pMRTTag, const _tchar* pDSVTag, _uint iWinCX, _uint iWinCY);
	HRESULT End_MRT(ID3D11DeviceContext* pContext);

	HRESULT	MRT_Clear(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);

public:
	HRESULT Bind_ShaderResource(const _tchar* pTargetTag, class CShader* pShader, const char* pConstantName);
	HRESULT Ready_ShadowDepthStencilRenderTargetView(ID3D11Device * pDevice, const _tchar* pDSVTag, _uint iWinCX, _uint iWinCY);

public:
	HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _tchar* pMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Render_SoloTarget_Debug(const _tchar* pRenderTargetTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);


private:
	map<const _tchar*, class CRenderTarget*>			m_RenderTargets;
	typedef map<const _tchar*, class CRenderTarget*>	RENDERTARGETS;

private:
	map<const _tchar*, list<class CRenderTarget*>>			m_MRTs;
	typedef map<const _tchar*, list<class CRenderTarget*>>	MRTS;

private:
	map<const _tchar*, ID3D11DepthStencilView*>			m_DSVs;
	typedef map<const _tchar*, ID3D11DepthStencilView*>	DSVS;

private:
	ID3D11RenderTargetView*				m_pOldRTV = nullptr;
	ID3D11DepthStencilView*				m_pOldDSV = nullptr;




private:
	class CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);
	ID3D11DepthStencilView* Find_DSV(const _tchar* pDSVTag);

public:
	virtual void Free() override;
};

END