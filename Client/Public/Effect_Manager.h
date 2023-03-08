#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Characters.h"

BEGIN(Client)


class CEffect_Manager :
	public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

	//	RGK : 렌고쿠
	//  TAN : 탄지로
	//  WATER : 물의 호흡(숫자는 몇형)

public:
	enum EFFECT {
		EFF_HIT, EFF_HIT2, EFF_HIT3, EFF_HIT4, EFF_HIT5,																										// 공용 히트
		EFF_PCHANGE_DOWN, EFF_PCHANGE_UP, EFF_GUARD1, EFF_GUARD2, EFF_GUARD3, EFF_GUARD3_BROKEN, EFF_JUMP_DOWN, EFF_JUMP_UP, EFF_RUN,							// 공용 이펙트
		EFF_RUSH_START, EFF_RUSH_MOVE, EFF_RUSH_HIT, EFF_POWER_CHARGING, EFF_POWER_UP, EFF_POWER_UP_PLAYER, EFF_POWER_UP_PLAYER_PERFACT,
		EFF_GAMESTART,
		EFF_DASH_TAN_MOVE, EFF_DASH_TAN_STOP, EFF_DASH_TAN_MOVEFB, EFF_DASH_TAN_STOPFB,																			// 대쉬 이펙트
		EFF_TANATTACK1, EFF_TANATTACK1_GROUND, EFF_TANATTACK2_1, EFF_TANATTACK2_2, EFF_TANATTACK2_GROUND, EFF_TANATTACK3, 										// 탄지로 평타
		EFF_TANATTACK3_GROUND, EFF_TANATTACK4, EFF_TANATTACK4_GROUND, EFF_TANJUMPATT, EFF_TANJUMPMOVEATT1, EFF_TANJUMPMOVEATT2,
		EFF_HINOATT1, EFF_HINOATT2_1, EFF_HINOATT2_2, EFF_HINOATT3_1,																							// 히노카미 카구라 평타
		EFF_HINOATT3_2, EFF_HINOATT4_1, EFF_HINOATT4_2, EFF_HINOJUMPATT, EFF_HINOMOVEJUMPATT2,
		EFF_WATER2_1, EFF_WATER2_GROUND, EFF_WATER6_1, EFF_WATER6_1_GROUND, EFF_WATER8_1,																		// 물의 호흡
		EFF_HINOCOMMON1, EFF_HINOCOMMON2, EFF_HINOMOVE1, EFF_HINOMOVE2, EFF_HINOMOVE3, EFF_HINOMOVE4, EFF_HINOMOVE5,											// 히노카미 카구라 스킬
		EFF_HINOSPHERE_MAIN1, EFF_HINOSPHERE_MAIN2, EFF_HINOSPERE_START, EFF_HINOSPHERE_ATC1, EFF_HINOSPHERE_ATC2,
		EFF_RGKATK1, EFF_RGKATK2_1, EFF_RGKATK2_2, EFF_RGKATK3, EFF_RGKATK4,																					// 렌고쿠 평타
		EFF_RGKSKL_3KIEN_START, EFF_RGKSKL_3KIEN_STARTFIRE, EFF_RGKSKL_3KIEN_MAIN,																				// 렌고쿠 스킬
		EFF_RGKSKL_2NOBORI_1, EFF_RGKSKL_2NOBORI_2,
		EFF_RGKSKL_5TIGER_CHARGE, EFF_RGKSKL_5TIGER_MAIN, EFF_RGKSKL_COLL_5TIGER_PARTICLE,
		EFF_RGKSKL_JUMP_3KIEN_START, EFF_RGKSKL_JUMP_3KIEN_STARTFIRE, EFF_RGKSKL_JUMP_3KIEN_MAIN,
		EFF_RGKSKL_JUMP_5TIGER_CHARGE, EFF_RGKSKL_JUMP_5TIGER_MAIN, EFF_RGKSKL_JUMP_5TIGER_AFTER, EFF_RGKSKL_JUMP_5TIGER_END,
		EFF_RGKSKL_JUMPATK_MAIN,																																//	Kyojuro JumpAtk
		EFF_RGKSKL_JUMPMOVEATK_NONFOLLOW,																														//	Kyojuro JumpMoveAtk
		EFF_RUIATK1, EFF_RUIATK2_1, EFF_RUIATK2_2, EFF_RUIATK3_1, EFF_RUIATK3_2, EFF_RUIATK4,
		EFF_RUISKL_COLL_FRIENDCOM_START, EFF_RUISKL_COLL_FRIENDCOM_MAIN, EFF_RUISKL_COLL_FRIENDMOVE,															//	Rui Friend
		EFF_RUISKL_COLL_SHOTBALL_MAIN, EFF_RUI_SHOTBALL_START_1, EFF_RUI_SHOTBALL_START_2, EFF_RUI_SHOTBALL_START_3,											//	Rui Ball (Common)
		EFF_RUISKL_SHOTNET_STARTEFF, EFF_RUISKL_COLL_SHOTNET_MAIN, EFF_RUISKL_COLL_SHOTNET_COLEVERYTIME,																							//	Rui ShotNet
		EFF_RUISKL_JUMPBALL_START, EFF_RUISKL_COLL_JUMPBALL_MAIN,																								//	Rui JumpBall (Common)
		EFF_RUISKL_JUMPSHOTNET_START, EFF_RUISKL_COLL_JUMPSHOTNET_MAIN,																							//	Rui JumpNet
		EFF_RUIATK_JUMPMOVE_PULLMAIN, EFF_RUIATK_JUMPMOVE_PLAYERMAIN, EFF_RUIATK_JUMPMOVE_MAIN,																	//	Rui JumpMoveAtk
		EFF_RUIATK_JUMP_MAIN,																																	//	Rui JumpAtk Main
		EFF_AKZATK_1, EFF_AKZATK_2, EFF_AKZATK_3, EFF_AKZATK_4,																									//	Akaza Atk
		EFF_AKZATK_JUMP_MAIN,
		EFF_AKZATK_JUMPMOVE_NONFOLLOW, EFF_AKZATK_JUMPMOVE_FOLLOW, EFF_AKZATK_JUMPMOVE_END,
		EFF_AKZSKL_COM_START, EFF_AKZSKL_COM_HAND_1, EFF_AKZSKL_COM_HAND_2, EFF_AKZSKL_COM_HAND_3, EFF_AKZSKL_COM_HAND_4, EFF_AKZSKL_COM_HAND_5, EFF_AKZSKL_COLL_COM_MAIN,
		EFF_AKZSKL_MOVE_STARTFOLLOWPLC, EFF_AKZSKL_MOVE_RUNSMOKE, EFF_AKZSKL_MOVE_MAIN, EFF_AKZSKL_MOVE_FINAL,
		EFF_AKZSKL_JUMPCOM_STARTEFF,																															//	Akaza Jump Common
		EFF_AKZSKL_JUMPMOVE_STARTEFF, EFF_AKZSKL_JUMPMOVE_STARTFOLLOW, EFF_AKZSKL_JUMPMOVE_MAIN,																//	Akaza Jump Move
		EFF_AKZSKL_SPECIALGROUND,																																//	Akaza Special Ground
		EFF_AKZSKL_DESTROY_MAIN, EFF_AKZSKL_DESTROY_SUCCESSS_FINAL, EFF_AKZSKL_DESTROY_FAILEDFINAL,
		EFF_AKZSKL_DASH,
		EFF_AKZSKL_FRIEND_COM_START, EFF_AKZSKL_FRIEND_COM_STARTFOLLOW, EFF_AKZSKL_FRIEND_COM_INGFOLLOW, EFF_AKZSKL_FRIEND_COM_MAIN,
		EFF_AKZSKL_FRIEND_MOVE_STARTFOL, EFF_AKZSKL_FRIEND_MOVE_STARTNONFOL, EFF_AKZSKL_FRIEND_MOVE_MAIN,
		EFF_SHINOBU_ATK_1, EFF_SHINOBU_ATK_2_1, EFF_SHINOBU_ATK_2_2, EFF_SHINOBU_ATK_3_1, EFF_SHINOBU_ATK_4_1, EFF_SHINOBU_ATK_4_2,
		EFF_SHINOBU_ATK_JUMP_COM_MAIN, EFF_SHINOBU_ATK_JUMP_MOVE_FOL, EFF_SHINOBU_ATK_JUMP_MOVE_NONFOL,
		EFF_SHINOBU_SKL_COM_1, EFF_SHINOBU_SKL_COM_2, EFF_SHINOBU_SKL_COM_3, EFF_SHINOBU_SKL_COM_4, EFF_SHINOBU_SKL_COM_5, EFF_SHINOBU_SKL_COM_FINAL_FOL, EFF_SHINOBU_SKL_COM_FINAL_NONFOL,
		EFF_SHINOBU_SKL_MOVE_START, EFF_SHINOBU_SKL_MAIN_1R, EFF_SHINOBU_SKL_MAIN_2L, EFF_SHINOBU_SKL_FINAL_NONFOL,
		EFF_SHINOBU_SKL_JUMPCOM_START_1, EFF_SHINOBU_SKL_JUMPCOM_START_2, EFF_SHINOBU_SKL_JUMPCOM_START_3, EFF_SHINOBU_SKL_JUMPCOM_START_4,
		EFF_SHINOBU_SKL_JUMPCOM_LOOP_FOL, EFF_SHINOBU_SKL_JUMPCOM_LOOP_NONFOL, EFF_SHINOBU_SKL_JUMPCOM_LOOPFIN_YZERO,
		EFF_SHINOBU_SKL_JUMPMOVE_1LOOP_FOL, EFF_SHINOBU_SKL_JUMPMOVE_1LOOP_NONFOL, EFF_SHINOBU_SKL_JUMPMOVE_1LOOPEND_MAIN,
		EFF_SHINOBU_SKL_SPECIAL_NONFOL, EFF_SHINOBU_SKL_SPECIAL_FOL,
		EFF_NEZUKO_ATK1, EFF_NEZUKO_ATK2, EFF_NEZUKO_ATK3_1, EFF_NEZUKO_ATK3_2, EFF_NEZUKO_ATK3_3, EFF_NEZUKO_ATK4_1, EFF_NEZUKO_ATK4_2,						// 네즈코 평타
		EFF_NEZUKO_JUMPATK1, EFF_NEZUKO_JUMPATK2, EFF_NEZUKO_JUMPMOVEATK1, EFF_NEZUKO_JUMPMOVEATK2,																// 네즈코 점프 평타
		EFF_NEZUKO_JUMP_CLAW1, EFF_NEZUKO_JUMP_CLAW2, EFF_NEZUKO_JUMP_CLAW3, EFF_NEZUKO_JUMP_CLAW4, EFF_NEZUKO_JUMP_CLAW5,
		EFF_NEZUKO_JUMP_KICK_GROUND, EFF_NEZUKO_JUMP_KICK,
		EFF_NEZUKO_CLAW1, EFF_NEZUKO_CLAW2, EFF_NEZUKO_CLAW3, EFF_NEZUKO_CLAW4, EFF_NEZUKO_CLAW5, EFF_NEZUKO_CLAW_GROUND,
		EFF_NEZUKO_KICKDOWN_FOOTFIRE, EFF_NEZUKO_KICKDOWN_GROUND, EFF_NEZUKO_KICKDOWN_TURN, EFF_NEZUKO_KICKDOWN_UPFIRE, EFF_NEZUKO_KICKDOWN_WIND,
		EFF_RUIDAD_THROWING_BIGSTONE, EFF_RUIDAD_THROWING_SMALLSTONE, EFF_RUIDAD_THROWING_GROUND1, EFF_RUIDAD_THROWING_GROUND2, EFF_RUIDAD_THROWING_GROUND3, EFF_RUIDAD_THROWING_GROUND4,
		EFF_RUIDAD_ANGRY, EFF_RUIDAD_ATK1_1, EFF_RUIDAD_ATK1_2, EFF_RUIDAD_ATK2_GROUND, EFF_RUIDAD_ATK2_SLASH, EFF_RUIDAD_ATK3_END, EFF_RUIDAD_ATK3_SLASH, EFF_RUIDAD_ATK3_START,
		EFF_RUIDAD_DASH_ENDGROUND, EFF_RUIDAD_DASH_GROUND, EFF_RUIDAD_DASH_PLAYER, EFF_RUIDAD_DASH_STARTGROUND, EFF_RUIDAD_FREE,
		EFF_RUIDAD_JUMP_ENDGROUND, EFF_RUIDAD_JUMP_GROUNDMARK, EFF_RUIDAD_JUMP_PLAYERDOWN, EFF_RUIDAD_JUMP_PLAYERUP, EFF_RUIDAD_JUMP_UP,
		EFF_RUIDAD_SWING_GROUND, EFF_RUIDAD_SWING_SLASH,
		EFF_SPL_HINO_START, EFF_SPL_HINO_STARTSLASH, EFF_SPL_HINO_ENDGROUND, EFF_SPL_HINO_ENDPLAYER,
		EFF_SPL_HINO_MO1_SLASH1, EFF_SPL_HINO_MO1_SLASH2, EFF_SPL_HINO_MO1_SWORD,
		EFF_SPL_HINO_MO2_BOOM1, EFF_SPL_HINO_MO2_BOOM2, EFF_SPL_HINO_MO2_BOOM3, EFF_SPL_HINO_MO2_BOOM4, EFF_SPL_HINO_MO2_BOOM5, EFF_SPL_HINO_MO2_BOOM6,
		EFF_SPL_HINO_MO2_PROJ1, EFF_SPL_HINO_MO2_PROJ2, EFF_SPL_HINO_MO2_PROJ3, EFF_SPL_HINO_MO2_PROJ4, EFF_SPL_HINO_MO2_PROJ5,
		EFF_SPL_HINO_MO2_SLASH1, EFF_SPL_HINO_MO2_SLASH2, EFF_SPL_HINO_MO2_SLASH3, EFF_SPL_HINO_MO2_SLASH4, EFF_SPL_HINO_MO2_SLASH5, EFF_SPL_HINO_MO2_SLASH6,
		EFF_SPL_HINO_MO2_SWORD, EFF_SPL_HINO_MO3_PROJ1, EFF_SPL_HINO_MO3_SLASH1, EFF_SPL_HINO_MO3_SLASH2, EFF_SPL_HINO_MO4_PROJ1, EFF_SPL_HINO_MO4_SLASH1,
		EFF_SPL_HINO_MO5_PROJ1, EFF_SPL_HINO_MO5_PROJ2, EFF_SPL_HINO_MO5_GOUND, EFF_SPL_HINO_MO5_SLASH1, EFF_SPL_HINO_MO5_SLASH2, EFF_SPL_HINO_MO5_SLASH3,
		EFF_SPL_RUI_MO1_WEB1, EFF_SPL_RUI_MO2_PROJ1, EFF_SPL_RUI_MO2_REDWEB1, EFF_SPL_RUI_MO2_TREADL1, EFF_SPL_RUI_MO2_TREADR1,
		EFF_SPL_RUI_MO3_REDWEB1, EFF_SPL_RUI_MO3_TREADL1, EFF_SPL_RUI_MO3_TREADR1, EFF_SPL_RUI_MO3_WEB1, 
		EFF_SPL_RUI_MO4_PROJ1, EFF_SPL_RUI_MO4_REDWEB1, EFF_SPL_RUI_MO4_REDWEB2, EFF_SPL_RUI_MO4_TREADL1, EFF_SPL_RUI_MO4_TREADR1,
		EFF_ENMU_ATK1, EFF_ENMU_ATK2_1, EFF_ENMU_ATK2_2, EFF_ENMU_ATK3_1, EFF_ENMU_ATK3_2, EFF_ENMU_ATK3_3, EFF_ENMU_BALL, EFF_ENMU_SKILL_BALLSTART,
		EFF_SPL_REN_MO1_SWORD1, EFF_SPL_REN_MO2_BG, EFF_SPL_REN_MO2_SWORD1, EFF_SPL_REN_MO2_AURA1, EFF_SPL_REN_MO2_AURA2, EFF_SPL_REN_MO3_GROUND1,
		EFF_SPL_REN_MO4_BG1, EFF_SPL_REN_MO4_BG2, EFF_SPL_REN_MO4_PLAYER, EFF_SPL_REN_MO5_BG1, EFF_SPL_REN_MO5_FIRE1,
		EFF_SPL_REN_MO6_BG, EFF_SPL_REN_MO6_HIT, EFF_SPL_REN_MO6_HITBG, EFF_SPL_REN_MO7_BG1, EFF_SPL_REN_MO7_FIRE1, EFF_SPL_REN_MO7_FIRE2,
		EFF_SPL_REN_MO9_BOOM, EFF_SPL_REN_MO9_GROUND, EFF_SPL_REN_MO9_PROJ1, EFF_SPL_REN_MO9_PORJ2, EFF_SPL_REN_MO9_TONEIDO, EFF_SPL_REN_MO10_FLASH,
		EFF_ENMUBOSS_PAT1_FLASH, EFF_ENMUBOSS_PAT2_CHOK, EFF_ENMUBOSS_PAT2_FLASH, EFF_ENMUBOSS_PAT2_FLASH2, EFF_ENMUBOSS_PAT2_GROUND, EFF_ENMUBOSS_PAT3_FLASH, EFF_ENMUBOSS_PAT3_FLASH2, EFF_ENMUBOSS_PAT3_GROUND,
		EFF_ENMUBOSS_PAT4_FLASH, EFF_ENMUBOSS_PAT4_FLASH2, EFF_ENMUBOSS_PAT4_GROUND, EFF_ENMUBOSS_PAT4_HAND,
		EFF_ENMUBOSS_PAT5_GROUND, EFF_ENMUBOSS_PAT5_GROUND2, EFF_ENMUBOSS_PAT5_HAND, EFF_ENMUBOSS_PAT5_WIND, EFF_ENMUBOSS_PAT5_FLASH,
		EFF_SPL_AKA_MO1_HIT, EFF_SPL_AKA_MO2_GROUND, EFF_SPL_AKA_MO3_GROUND, EFF_SPL_AKA_MO4_DIST,
		EFF_SPL_AKA_MO5_DASH1, EFF_SPL_AKA_MO5_HAND1, EFF_SPL_AKA_MO5_HIT1, EFF_SPL_AKA_MO5_HIT2, EFF_SPL_AKA_MO5_KICK1, EFF_SPL_AKA_MO5_RUN1,
		EFF_SPL_AKA_MO6_PROJ1, EFF_SPL_AKA_MO6_SLASH1, EFF_SPL_AKA_MO6_SLASH2, 
		EFF_SPL_AKA_MO7_BACKLIGHT, EFF_SPL_AKA_MO7_BG1, EFF_SPL_AKA_MO7_BG2, EFF_SPL_AKA_MO7_HAND, EFF_SPL_AKA_MO7_HAND2, EFF_SPL_AKA_MO7_HAND3, EFF_SPL_AKA_MO7_PROJ1,
		EFF_SPL_AKA_MO8_BG1, EFF_SPL_AKA_MO8_HAND1, EFF_SPL_AKA_MO8_PROJ1, EFF_SPL_AKA_MO9_AURA1, EFF_SPL_AKA_MO9_BG1, EFF_SPL_AKA_MO9_PROJ1, EFF_SPL_AKA_MO9_BOOM1,
		EFF_BOOM1, EFF_TRAIN_SMOKE, EFF_TRAIN_WIND, EFF_FADE, EFF_END };

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