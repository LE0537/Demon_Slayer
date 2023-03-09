#pragma once
#include "Client_Defines.h"
#include "Characters.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CModel;
class CCollider;
class CNavigation;

END


BEGIN(Client)

class CItemBox : public CCharacters
{
public:
	enum BOMB { BOMB_1, BOMB_2, BOMB_3, BOMB_4, BOMB_5, BOMB_6, BOMB_7, BOMB_8, BOMB_9, BOMB_END };

public:
	CItemBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemBox(const CItemBox& rhs);
	virtual ~CItemBox() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();


public:
	//	CTransform* Get_Transfrom() const { return m_pTransformCom; }
	virtual CModel* Get_Model() const { return m_pModelCom; }



private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();
	void Check_Event(_float fTimeDelta);



	void Play_Bomb(_float fTimeDelta);
	void Pattern_Bomb1(_float fTimeDelta);
	void Pattern_Bomb2(_float fTimeDelta);
	void Pattern_Bomb3(_float fTimeDelta);
	void Pattern_Bomb4(_float fTimeDelta);
	void Pattern_Bomb5(_float fTimeDelta);
	void Pattern_Bomb6(_float fTimeDelta);
	void Pattern_Bomb7(_float fTimeDelta);
	void Pattern_Bomb8(_float fTimeDelta);
	void Pattern_Bomb9(_float fTimeDelta);
	void EventCheck();
	void EventCheck2();

public:
	virtual	void  Take_Damage(_float _fPow, _bool _bJumpHit = 0);
	virtual	void  Get_GuardHit(_int eType);
	virtual void  Player_TakeDown(_float _fPow, _bool _bJump = 0);
	virtual void  Player_UpperDown(HIT_TYPE eHitType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower);
	virtual void  Play_Scene();
private:
	CModel*					m_pModelCom = nullptr;

	_float					m_fEffectTime = 0.f;
	_bool					m_bQuestStop = false;
	_bool					m_MsgReset = false;
	_bool					m_bMsgStart = false;
	_bool					m_bMsgEnd = false;
	_bool					m_bSoundCheck = false;
	_bool					m_bIsPlaying = false;
	//_bool					m_bIsPlaying = false;
	_float					m_fMsgTime = 0.f;

	_vector					m_vPosition;


	BOMB m_eBomb = BOMB_END;

	_bool m_bPlayAnim = false;
	_bool m_bOpen = false;
	_bool  m_bSound = false;

	_bool m_bPattern = false;

	_bool m_bBomb[BOMB_END] = { false };
	_float m_fBombTime[BOMB_END] = { 0.f };
	_bool m_bCreate[BOMB_END] = { false };


	_bool m_bTopView = false;
	_bool m_bSound2 = false;
	vector<_vector> m_vBombPosition;

public:
	static CItemBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;


};

END
