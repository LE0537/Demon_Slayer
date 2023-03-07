#pragma once

#include "Client_Defines.h"
#include "GameObj.h"
#include "Mini_Tail.h"
BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CMini_Player final : public CGameObj
{
private:
	CMini_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMini_Player(const CMini_Player& rhs);
	virtual ~CMini_Player() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources();
	void Key_Input(_float fTimeDelta);
	void Create_Tail();
	void Save_Pos(_float fTimeDelta);
	void Set_TailPos();
	void Check_Coll();
	void	ConvertToViewPort();
public:
	void Set_Eat() { m_bEat = true; }
	void Set_PlayerDead() { m_bPlayerDead = true; }
	void Set_Score(_int _iScore) { m_iScore += _iScore; }
	_int Get_Score() { return m_iScore; }
	_bool Get_PlayerDead() { return m_bPlayerDead; }
	list<CMini_Tail*> Get_Tail() { return m_listTail; }
private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;

	class CMini_Result*		m_pResult = nullptr;
	list<CMini_Tail*>		m_listTail;
	list<_vector>			m_listPos;
	_bool					m_bEat = false;
	_int					m_iTailIndex = 0;
	_float					m_fSaveTime = 0.f;
	_bool					m_bPlayerDead = false;

	_float4					m_vPlayerPos;
	_bool					m_bResultDead = false;

	_int					m_iScore = 0;
public:
	static CMini_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
