#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CChangeBaseDeco final : public CUI
{
private:
	CChangeBaseDeco(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChangeBaseDeco(const CChangeBaseDeco& rhs);
	virtual ~CChangeBaseDeco() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */


public:
	static CChangeBaseDeco* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
