#include "stdafx.h"
#include "Drawing.h"
CEngine cEng;

bool CEngine::IsVisible(IClientEntity* pLocalBaseEntity, Vector vStart, Vector vEnd)
{	
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = pLocalBaseEntity;

	ray.Init(vStart, vEnd);
	pEngineTrace->TraceRay(ray, (MASK_OPAQUE_AND_NPCS | CONTENTS_IGNORE_NODRAW_OPAQUE | CONTENTS_HITBOX | MASK_VISIBLE)/*0x4600400B*/, &filter, &tr);

	

	IClientEntity* pLocalEntity = pLocalBaseEntity;
	IClientEntity* TouchedEntity;
	if (tr.m_pEnt)
	{
		
		TouchedEntity = tr.m_pEnt;

		if (TouchedEntity->Index() == pLocalEntity->Index())
			return false;

		if (TouchedEntity->GetTeamNum() == pLocalEntity->GetTeamNum())
			return false;

	}
	if (tr.allSolid || tr.startSolid)
		return true;
	
	if (tr.hitgroup > 0 && tr.hitgroup <= 10 && tr.m_pEnt && (tr.m_pEnt->Index() != 0))
		return true;

	if (tr.fraction > 0.99f)
		return true;

	return false;
}
Vector CEngine::GetEyePosition(IClientEntity* pEntity)
{
	Vector vecViewOffset = *reinterpret_cast< Vector* >(reinterpret_cast< int >( pEntity ) + 0x104 );

	return pEntity->GetAbsOrigin( ) + vecViewOffset;
}

bool CEngine::GetBonePosition(IClientEntity* pPlayer, Vector& Hitbox, int Bone)
{
	matrix3x4_t MatrixArray[128];

	if(!pPlayer->SetupBones( MatrixArray, 128, 0x00000100, pEngine->Time() ) )
		return FALSE;

	matrix3x4_t HitboxMatrix = MatrixArray[Bone];

	Hitbox = Vector(HitboxMatrix[0][3], HitboxMatrix[1][3], HitboxMatrix[2][3]);

	return TRUE;
}

int CEngine::GetHealth(IClientEntity* pEntity)
{
	return *reinterpret_cast< int* >(reinterpret_cast< int >( pEntity ) + 0xFC);
}

bool CEngine::IsAlive(IClientEntity* pEntity)
{
	char szLifeState = *(char*)((DWORD)pEntity + 0x25B);
	return (szLifeState == 0);
}

LPCSTR CEngine::GetWeaponName(int WeaponID)
{
	switch (WeaponID)
	{
	case 0:
		return "WEAPON_NULL";
	case 1:
		return "WEAPON_AK47";
	case 2:
		return "WEAPON_AUG";
	case 3:
		return "WEAPON_AWP";
	case 4:
		return "WEAPON_DEAGLE";
	case 5:
		return "WEAPON_ELITES";
	case 6:
		return "WEAPON_FAMAS";
	case 7:
		return "WEAPON_FIVESEVEN";
	case 8:
		return "WEAPON_G3SG1";
	case 9:
		return "WEAPON_GALIL";
	case 10:
		return "WEAPON_GLOCK18";
	case 11:
		return "WEAPON_M249";
	case 12:
		return "WEAPON_M3";
	case 13:
		return "WEAPON_M4A1";
	case 14:
		return "WEAPON_MAC10";
	case 15:
		return "WEAPON_MP5";
	case 16:
		return "WEAPON_P228";
	case 17:
		return "WEAPON_P90";
	case 18:
		return "WEAPON_SCOUT";
	case 19:
		return "WEAPON_SG550";
	case 20:
		return "WEAPON_SG552";
	case 21:
		return "WEAPON_TMP";
	case 22:
		return "WEAPON_UMP45";
	case 23:
		return "WEAPON_USP45";
	case 24:
		return "WEAPON_XM1014";
	case 25:
		return "WEAPON_KNIFE";
	case 26:
		return "WEAPON_FLASHBANG";
	case 27:
		return "WEAPON_HE_GRENADE";
	case 28:
		return "WEAPON_SMOKE_GRENADE";
	case 29:
		return "WEAPON_C4";
	default:
		return "UNKNOWN";
	}
}