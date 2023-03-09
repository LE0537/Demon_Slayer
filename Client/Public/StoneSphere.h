#pragma once
#include "Client_Defines.h"
#include "CollBox.h"

BEGIN(Engine)
class CTransform;
class CModel;
END

BEGIN(Client)

class CStoneSphere : public CCollBox
{
public:
	enum STONEPOS { STONEPOS_LEFT, STONEPOS_RIGHT, STONEPOS_CENTER, STONEPOS_END };
protected:
	CStoneSphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStoneSphere(const CStoneSphere& rhs);
	virtual ~CStoneSphere() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Stone(char* pFileName);
	_bool Move_Stone(STONEPOS _eStone,_float fTimeDelta);
	_bool Play_Stone(vector<_float4> vecPositions, vector<_float4> vecLookAts, vector<_float> vecUseTime, _float * pOut, _float fTimeDelta);
private:
	CModel*		m_pModelCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;
	class CCharacters*  m_pPlayer = nullptr;
	_bool	m_bShadowAlphaIncrease = false;
	_bool	m_bShadowAlphaDecrease = false;

	_matrix m_ShadowMatrix;

	_float		m_fCurrentCutSceneTime = 0.f;
	_float		m_fCullTime = 0.f;
	vector<vector<_float4>>		m_vecStoneEye;
	vector<vector<_float4>>		m_vecStoneAt;
	vector<vector<_float>>		m_vecStoneTime;
	_bool  m_bStone = true;
	STONEPOS m_eStone;
	_float		m_fTurnAngle = 0.f;
	_float		m_fStoneSmoke = 0.f;
public:
	static CStoneSphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
