#pragma once

#include "Client_Defines.h"
#include "Mini_Player.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CMini_Result final : public CGameObj
{
private:
	CMini_Result(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMini_Result(const CMini_Result& rhs);
	virtual ~CMini_Result() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void  Set_Target(CMini_Player* _pTarget) { m_pTarget = _pTarget; }
	void  Set_Start(_bool _bStart) { m_bStart = _bStart; }
	void  Set_PlayerDead(_bool _bPlayerDead) { m_bPlayerDead = _bPlayerDead; }
	void  Set_RankScore(_int _iScore) { m_iRankScore = _iScore; }
	_int  Get_RankScore() { return m_iRankScore; }
private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources();

private:
	CShader*				m_pShaderCom2 = nullptr;
	CTransform*				m_pTransformCom2 = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom2 = nullptr;

	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;

	CMini_Player*				m_pTarget = nullptr;
	class CLevel_Loading*	m_pLoading = nullptr;

	_bool					m_bStart = false;
	_bool					m_bPlayerDead = false;
	_int					m_iRankScore = 0;
public:
	static CMini_Result* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
