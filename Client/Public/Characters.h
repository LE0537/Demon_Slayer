#pragma once
#include "Client_Defines.h"
#include "GameObj.h"


BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;

END


BEGIN(Client)

class CCharacters : public CGameObj
{
protected:
	CCharacters(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacters(const CCharacters& rhs);
	virtual ~CCharacters() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

protected:
	typedef struct tagInfo {
		wstring		strName; //ĳ���� �̸�;
		_bool		bOni;    //true = ���� , false = �ΰ�;
		_int		iMaxHp;  
		_int		iHp;
		_int		iSkMaxBar; //�ƽ���ų������
		_int		iSkBar;    //��ų������
		_int		iUnicMaxBar; //�ƽ��ʻ�������
		_int		iUnicBar;    //�ʻ�������
		_int		iDmg;      //��Ÿ ������
		_int		iCombo;
		_float		fComboTime;
		_bool		bPowerUp;
		_float		fPowerUpTime;
		_int		iFriendMaxBar; //�ƽ�ģ��������
		_int		iFriendBar;    //ģ��������

	}PLAYERINFO;

public:
	PLAYERINFO Get_PlayerInfo() { return m_tInfo; };

protected:
	PLAYERINFO		m_tInfo;

public:
	static CCharacters* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
