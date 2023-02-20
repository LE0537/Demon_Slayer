#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Line final : public CVIBuffer
{
protected:
	CVIBuffer_Line(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Line(const CVIBuffer_Line& rhs);
	virtual ~CVIBuffer_Line() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	void	Update(_float3 vPos1, _float3 vPos2);

private:
	_float3 m_vPos1;
	_float3 m_vPos2;

public:
	static CVIBuffer_Line* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
