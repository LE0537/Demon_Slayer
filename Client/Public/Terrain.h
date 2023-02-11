#pragma once

#include "Client_Defines.h"
#include "GameObj.h"
#include "VIBuffer_NewTerrain.h"

BEGIN(Engine)
class CVIBuffer_NewTerrain;
END


BEGIN(Client)

class CTerrain final : public CGameObj
{
public:
	typedef struct tagNewTerrainDesc
	{
		CVIBuffer_NewTerrain::TERRAINDESC tTerrainDesc;

	}NEWTERRAINDESC;
public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_BRUSH, TYPE_FILTER, TYPE_NORMAL, TYPE_END };
	enum OPTION { OP_HEIGHT, OP_FILTER, OP_END };
	enum FILTER_RGB { FILTER_A, FILTER_R, FILTER_G, FILTER_B, FILTER_END };

private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CTexture*					m_pTerrain_TextureCom[TYPE_END] = { nullptr };
	CVIBuffer_NewTerrain*		m_pNew_VIBufferCom = nullptr;


private:
	_uint			m_iPassIndex = 0;
	_bool			m_bRenderWireFrame = false;

	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;

	OPTION			m_eOption = OP_END;

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT SetUp_ShaderResources();

private:/* Filter */
	D3D11_TEXTURE2D_DESC		m_tTextureDesc;
	_uint						m_iTerrainSRVNum[4] = { 0,0,0,0 };
	CTexture*					m_pVTXColor_TextureCom = nullptr;
	ID3D11ShaderResourceView*	m_pFilterTexture = nullptr;
	ID3D11Texture2D*			m_pTexture2D = nullptr;

	FILTER_RGB				m_eFilterRGB = FILTER_END;
	_uint					m_iFilterIndex = 0;
	_uint*					m_pPixel = nullptr;

private:
	HRESULT	Load_FilterTexture(char* strName);
	HRESULT	Load_FilterTexNumber(char* strName);


public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END