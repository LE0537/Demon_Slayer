#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Characters.h"

BEGIN(Client)


class CEffect_Manager :
	public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

	//	RGK : ·»°íÄí
	//  TAN : ÅºÁö·Î
	//  WATER : ¹°ÀÇ È£Èí(¼ýÀÚ´Â ¸îÇü)

public:
	enum EFFECT { EFF_HIT,																											// ±âº» ÀÌÆåÆ®
		EFF_TANATTACK1, EFF_TANATTACK1_GROUND, EFF_TANATTACK2_1, EFF_TANATTACK2_2, EFF_TANATTACK2_GROUND, EFF_TANATTACK3, 			// ÅºÁö·Î ÆòÅ¸
		EFF_TANATTACK3_GROUND, EFF_TANATTACK4, EFF_TANATTACK4_GROUND,
		EFF_WATER2_1, EFF_WATER6_1, EFF_WATER8_1,																					// ¹°ÀÇ È£Èí
		EFF_RGKATK1, EFF_RGKATK2_1, EFF_RGKATK2_2, EFF_RGKATK3, EFF_RGKATK4,														// ·»°íÄí ÆòÅ¸
		EFF_RGKSKL_3KIEN_START, EFF_RGKSKL_3KIEN_STARTFIRE, EFF_RGKSKL_3KIEN_MAIN,													// ·»°íÄí ½ºÅ³
		EFF_RGKSKL_2NOBORI_1, EFF_RGKSKL_2NOBORI_2,
		EFF_RGKSKL_5TIGER_CHARGE, EFF_RGKSKL_5TIGER_MAIN, EFF_RGKSKL_COLL_5TIGER_PARTICLE,
		EFF_RGKSKL_JUMP_3KIEN_START, EFF_RGKSKL_JUMP_3KIEN_STARTFIRE, EFF_RGKSKL_JUMP_3KIEN_MAIN,
		EFF_RGKSKL_JUMP_5TIGER_CHARGE, EFF_RGKSKL_JUMP_5TIGER_MAIN, EFF_RGKSKL_JUMP_5TIGER_AFTER, EFF_RGKSKL_JUMP_5TIGER_END,
		EFF_RUIATK1, EFF_RUIATK2_1, EFF_RUIATK2_2, EFF_RUIATK3_1, EFF_RUIATK3_2, EFF_RUIATK4,
		EFF_RUISKL_COLL_FRIENDCOM_START, EFF_RUISKL_COLL_FRIENDCOM_MAIN, EFF_RUISKL_COLL_FRIENDMOVE,
		EFF_RUISKL_COLL_SHOTBALL_MAIN, EFF_RUI_SHOTBALL_START_1, EFF_RUI_SHOTBALL_START_2, EFF_RUI_SHOTBALL_START_3,
		EFF_RUISKL_COLL_SHOTNET_MAIN, EFF_RUISKL_SHOTNET_STARTEFF,
		EFF_END };

public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	void Load_Effect(const _tchar* EffectName);
	void Create_Effect(_uint iEffectNum, CGameObj* pTarget);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	vector<const _tchar*>			m_Effect;

public:
	virtual void Free() override;
};

END