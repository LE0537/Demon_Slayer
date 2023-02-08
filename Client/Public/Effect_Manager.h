#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Characters.h"

BEGIN(Client)


class CEffect_Manager :
	public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

	//	RGK : ������
	//  TAN : ź����
	//  WATER : ���� ȣ��(���ڴ� ����)

public:
	enum EFFECT { EFF_HIT, EFF_HIT2, EFF_HIT3, EFF_HIT4, EFF_HIT5,																										// �⺻ ����Ʈ
		EFF_TANATTACK1, EFF_TANATTACK1_GROUND, EFF_TANATTACK2_1, EFF_TANATTACK2_2, EFF_TANATTACK2_GROUND, EFF_TANATTACK3, 			// ź���� ��Ÿ
		EFF_TANATTACK3_GROUND, EFF_TANATTACK4, EFF_TANATTACK4_GROUND,
		EFF_WATER2_1, EFF_WATER2_GROUND, EFF_WATER6_1, EFF_WATER8_1,																// ���� ȣ��
		EFF_RGKATK1, EFF_RGKATK2_1, EFF_RGKATK2_2, EFF_RGKATK3, EFF_RGKATK4,														// ������ ��Ÿ
		EFF_RGKSKL_3KIEN_START, EFF_RGKSKL_3KIEN_STARTFIRE, EFF_RGKSKL_3KIEN_MAIN,													// ������ ��ų
		EFF_RGKSKL_2NOBORI_1, EFF_RGKSKL_2NOBORI_2,
		EFF_RGKSKL_5TIGER_CHARGE, EFF_RGKSKL_5TIGER_MAIN, EFF_RGKSKL_COLL_5TIGER_PARTICLE,
		EFF_RGKSKL_JUMP_3KIEN_START, EFF_RGKSKL_JUMP_3KIEN_STARTFIRE, EFF_RGKSKL_JUMP_3KIEN_MAIN,
		EFF_RGKSKL_JUMP_5TIGER_CHARGE, EFF_RGKSKL_JUMP_5TIGER_MAIN, EFF_RGKSKL_JUMP_5TIGER_AFTER, EFF_RGKSKL_JUMP_5TIGER_END,
		EFF_RGKSKL_JUMPATK_MAIN,				//	Kyojuro JumpAtk
		EFF_RGKSKL_JUMPMOVEATK_NONFOLLOW,		//	Kyojuro JumpMoveAtk
		EFF_RUIATK1, EFF_RUIATK2_1, EFF_RUIATK2_2, EFF_RUIATK3_1, EFF_RUIATK3_2, EFF_RUIATK4,
		EFF_RUISKL_COLL_FRIENDCOM_START, EFF_RUISKL_COLL_FRIENDCOM_MAIN, EFF_RUISKL_COLL_FRIENDMOVE,	//	Rui Friend
		EFF_RUISKL_COLL_SHOTBALL_MAIN, EFF_RUI_SHOTBALL_START_1, EFF_RUI_SHOTBALL_START_2, EFF_RUI_SHOTBALL_START_3,	//	Rui Ball (Common)
		EFF_RUISKL_SHOTNET_STARTEFF, EFF_RUISKL_COLL_SHOTNET_MAIN,	//	Rui ShotNet
		EFF_RUISKL_JUMPBALL_START, EFF_RUISKL_COLL_JUMPBALL_MAIN,	//	Rui JumpBall (Common)
		EFF_RUISKL_JUMPSHOTNET_START, EFF_RUISKL_COLL_JUMPSHOTNET_MAIN,		//	Rui JumpNet
		EFF_RUIATK_JUMPMOVE_PULLMAIN, EFF_RUIATK_JUMPMOVE_PLAYERMAIN, EFF_RUIATK_JUMPMOVE_MAIN,	//	Rui JumpMoveAtk
		EFF_RUIATK_JUMP_MAIN,		//	Rui JumpAtk Main
		EFF_AKZATK_1, EFF_AKZATK_2, EFF_AKZATK_3, EFF_AKZATK_4,		//	Akaza Atk
		EFF_END };

public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	void Load_Effect(const _tchar* EffectName);
	void Create_Effect(_uint iEffectNum, void* pTarget);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	vector<const _tchar*>			m_Effect;

public:
	virtual void Free() override;
};

END