#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CEffect_Manager :
	public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

public:
	enum EFFECT { EFFECT_HIT, EFFECT_END };

public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Load_Effect();

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

public:
	virtual void Free() override;
};

END