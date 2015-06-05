#include "stdafx.h"
#include "Drawing.h"
#include "CSGOInternal.h"

clock_t timeClock;

static const char *Ranks[] =
{
	"-",
	"Silver I",
	"Silver II",
	"Silver III",
	"Silver IV",
	"Silver Elite",
	"Silver Elite Master",

	"Gold Nova I",
	"Gold Nova II",
	"Gold Nova III",
	"Gold Nova Master",
	"Master Guardian I",
	"Master Guardian II",

	"Master Guardian Elite",
	"Distinguished Master Guardian",
	"Legendary Eagle",
	"Legendary Eagle Master",
	"Supreme Master First Class",
	"The Global Elite"
};

static const char *keyboardChars[] =
{
	"Undefined",
	"Left Mouse Button",
	"Right Mouse Button",
	"Break",
	"Middle Mouse Button",
	"Mouse X1",
	"Mouse X2",
	"Undefined",
	"Backspace",
	"Tab",
	"Reserved",
	"Reserved",
	"Clear",
	"Enter",
	"Undefined",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps Lock",
	"Undefined", //0x15
	"Undefined", //0x16
	"Undefined", //0x17
	"Undefined", //0x18
	"Undefined", //0x19
	"Undefined", //0x1A
	"Escape", //0x1B
	"Undefined", //0x1C
	"Undefined", //0x1D
	"Undefined", //0x1E
	"Undefined", //0x1F
	"Spacebar", //0x20
	"Page up", //0x21
	"Page down", //0x22
	"End", //0x23
	"Home", //0x24
	"Left Arrow", //0x25
	"Up Arrow", //0x26
	"Right Arrow", //0x27
	"Down Arrow", //0x28
	"Select", //0x29
	"Print", //0x2A
	"Execute" //0x2B
	"Print Screen", //0x2C
	"Insert", //0x2D
	"Delete", //0x2E
	"Help", //0x2F
	"0", //0x30
	"1", //0x31
	"2", //0x32
	"3", //0x33
	"4", //0x34
	"5", //0x35
	"6", //0x36
	"7", //0x37
	"8", //0x38
	"9", //0x39
	"Undefined", //0x3A
	"Undefined", //0x3B
	"Undefined", //0x3C
	"Undefined", //0x3D
	"Undefined", //0x3E
	"Undefined", //0x3F
	"Undefined", //0x40
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"Left Windows Key",
	"Right Windows Key"
};

void Drawing::DrawESP(int i, IClientEntity* pLocal, IClientEntity* pEntity)
{
	Vector vHead, vEyePos;

	player_info_t pInfo;

	if (!pEngine->GetPlayerInfo(pEntity->Index(), &pInfo))
		return;

	char playerName[32];
	sprintf(playerName, "%s", pInfo.name);

	if (!cEng.GetBonePosition(pEntity, vHead, 10))
		return;

	/* Alternate way get headposition

	bool bDucking = pEntity->GetFlags() &(1<<1);

	if( bDucking )
	vHead = pEntity->GetAbsOrigin() + Vector( 0, 0, 52 );
	else
	vHead = pEntity->GetAbsOrigin() + Vector( 0, 0, 72 );

	*/
	//bool bIsVisible = cEng.IsVisible(pLocal, pEntity, cEng.GetEyePosition(pLocal), vHead);

	Vector vScreenHead, vScreenLeg, vScreenView;

	if (cMath.WorldToScreen(vHead, vScreenHead) && cMath.WorldToScreen(pEntity->GetOrigin(), vScreenLeg))
	{
		float Height = abs(vScreenLeg.Y - vScreenHead.Y);
		float Width = Height / 2.0f;

		cDraw.OutlinedRect(vScreenHead.X - Width / 2, vScreenHead.Y, Width, Height, Color(150, 255, 0, 255));
		
		cDraw.FillRGBA(vScreenHead.X + Width / 2, vScreenHead.Y, 3, (Height / 100) * pEntity->GetHealth(), Color(255, 0, 0, 255));
		
		cDraw.DrawString(espfont, false, vScreenHead.X + Width / 2, vScreenHead.Y, Color(55, 100, 255, 255), playerName);
		
		PCHAR pszWeapon = NULL;
		C_BaseCombatWeapon * pActiveCombatWeapon = pEntity->GetActiveWeapon(pClientEntList);
		if (pActiveCombatWeapon)
		{
			pszWeapon = pActiveCombatWeapon->GetName();
		}
		
		cDraw.DrawString(espfont, false, vScreenHead.X + Width / 2, vScreenHead.Y + 15, Color(150, 50, 75, 255), pszWeapon);

	}

}



void Drawing::DrawBone(Color color, int boneID1, int boneID2, IClientEntity* Entity, IClientEntity* localEntity)
{
	Vector bone1XYZ, bone2XYZ, bone1XY, bone2XY;
	if (cEng.GetBonePosition(Entity, bone1XYZ, boneID1))
	{
		if (cEng.GetBonePosition(Entity, bone2XYZ, boneID2))
		{
			if (sqrt(
				pow(double(bone1XYZ.X - bone2XYZ.X), 2.0) +
				pow(double(bone1XYZ.Y - bone2XYZ.Y), 2.0) +
				pow(double(bone1XYZ.Z - bone2XYZ.Z), 2.0)) < 250)
			{
				if (cMath.WorldToScreen(bone1XYZ, bone1XY))
				{
					if (cMath.WorldToScreen(bone2XYZ, bone2XY))
					{
						bool bDucking = localEntity->GetFlags() &(1 << 1);
						Vector renderOriginDucking = localEntity->GetOrigin();
						renderOriginDucking.Z += 66;
						if (bDucking)
						{
							renderOriginDucking.Z -= 20;
						}

						if (cEng.IsVisible(localEntity, renderOriginDucking, bone1XYZ) ||
							cEng.IsVisible(localEntity, renderOriginDucking, bone2XYZ) && Entity->GetTeamNum() != localEntity->GetTeamNum())
						{
							cDraw.DrawLine(bone1XY.X, bone1XY.Y, bone2XY.X, bone2XY.Y, Color(10, 255, 10, 255));
						}
						else
						{
							cDraw.DrawLine(bone1XY.X, bone1XY.Y, bone2XY.X, bone2XY.Y, color);
						}
					}
				}
			}
		}
	}
}

void Drawing::DrawBoneESP(Color color, IClientEntity* Entity, IClientEntity* localEntity)
{
	//Crotch to neck
	Drawing::DrawBone(color, 0, 5, Entity, localEntity);
	//Left Arm
	Drawing::DrawBone(color, 5, 6, Entity, localEntity);
	Drawing::DrawBone(color, 6, 36, Entity, localEntity);
	Drawing::DrawBone(color, 36, 31, Entity, localEntity);
	Drawing::DrawBone(color, 31, 21, Entity, localEntity);
	//Right Arm
	Drawing::DrawBone(color, 5, 12, Entity, localEntity);
	Drawing::DrawBone(color, 12, 38, Entity, localEntity);
	Drawing::DrawBone(color, 38, 15, Entity, localEntity);
	//Left Leg
	Drawing::DrawBone(color, 0, 32, Entity, localEntity);
	Drawing::DrawBone(color, 27, 28, Entity, localEntity);
	//Right Leg
	Drawing::DrawBone(color, 0, 33, Entity, localEntity);
	Drawing::DrawBone(color, 33, 25, Entity, localEntity);

	if (!boxESP)
	{
		player_info_t pInfo;

		if (!pEngine->GetPlayerInfo(Entity->Index(), &pInfo))
			return;

		char playerName[64];
		sprintf(playerName, "%s - %d", pInfo.name, Entity->GetHealth());
	
		Vector onScreenPos;
		if (cMath.WorldToScreen(Entity->GetOrigin(), onScreenPos))
		{
			cDraw.DrawString(espfont, true, onScreenPos.X, onScreenPos.Y, Color(55, 100, 255, 255), playerName);
		}
	}

}

struct MenuItems
{
	char text[128];
};

int selectedItem = 1;
const int maxItems = 11;
bool doOnce = true;

MenuItems menuItems[maxItems];

void Drawing::Menu()
{
	if (doOnce)
	{
		timeClock = clock();
		
		doOnce = false;
	}

	sprintf(menuItems[0].text, "%s", "Hack Menu");
	sprintf(menuItems[1].text, "%s %i", "Box ESP", boxESP);
	sprintf(menuItems[2].text, "%s %i", "Bone ESP", boneESP);
	sprintf(menuItems[3].text, "%s %i", "Friendly boneESP", friendlyBoneESP);
	sprintf(menuItems[4].text, "%s %f", "aimbotFOV:", aimbotFOV);
	if (aimbotKey > 0x0 && aimbotKey < 0x5D)
	{
		sprintf(menuItems[5].text, "%s %02x - %s", "aimbotKey:", aimbotKey, keyboardChars[aimbotKey]);
	}
	else
	{
		sprintf(menuItems[5].text, "%s %02x", "aimbotKey:", aimbotKey);
	}	
	sprintf(menuItems[6].text, "%s %i", "Free for all", ffa);
	sprintf(menuItems[7].text, "%s %i", "Triggerbot", triggerbot);
	sprintf(menuItems[8].text, "%s %f", "Triggerbot delay per shot:", triggerbotD);
	sprintf(menuItems[9].text, "%s %f", "Aimbot smoothing:", aimbotMov);
	sprintf(menuItems[10].text, "%s %i", "Only shoot when zoomed:", shootOnZoom);

	for (int i = 0; i < maxItems; i++)
	{
		if (selectedItem == i)
		{
			cDraw.DrawString(espfont, false, 0, 400 + (i * 10), Color(255, 255, 100, 255), menuItems[i].text);
		}
		else
		{
			cDraw.DrawString(espfont, false, 0, 400 + (i * 10), Color(0, 255, 100, 255), menuItems[i].text);
		}
	}

	if (GetAsyncKeyState(VK_DOWN) && clock() - timeClock > 200.0f)
	{
		timeClock = clock();
		if (selectedItem + 1 < maxItems)
		{
			selectedItem++;
		}
	}

	if (GetAsyncKeyState(VK_UP) && clock() - timeClock > 200.0f)
	{
		timeClock = clock();
		if (selectedItem - 1 != 0)
		{
			selectedItem--;
		}
	}

	if (GetAsyncKeyState(VK_RIGHT) && clock() - timeClock > 125.0f)
	{
		timeClock = clock();
		switch (selectedItem)
		{
		case 1:
			boxESP = true;
			break;
		case 2:
			boneESP = true;
			break;
		case 3:
			friendlyBoneESP = true;
			break;
		case 4:
			aimbotFOV += 2.0f;
			break;
		case 5:
			aimbotKey += 1;
			break;
		case 6:
			ffa = true;
			break;
		case 7:
			triggerbot = true;
			break;
		case 8:
			triggerbotD += 5.0f;
			break;
		case 9:
			aimbotMov += 0.1f;
			break;
		case 10:
			shootOnZoom = true;
			break;
		default:
			break;
		}
	}

	if (GetAsyncKeyState(VK_LEFT) && clock() - timeClock > 125.0f)
	{

		timeClock = clock();
		switch (selectedItem)
		{
		case 1:
			boxESP = false;
			break;
		case 2:
			boneESP = false;
			break;
		case 3:
			friendlyBoneESP = false;
			break;
		case 4:
			aimbotFOV -= 2.0f;
			break;
		case 5:
			aimbotKey -= 1;
			break;
		case 6:
			ffa = false;
			break;
		case 7:
			triggerbot = false;
			break;
		case 8:
			triggerbotD -= 5.0f;
			break;
		case 9:
			if (aimbotMov - 0.1f >= 1.0f)
				aimbotMov -= 0.1f;
			break;
		case 10:
			shootOnZoom = false;
			break;
		default:
			break;
		}
	}
}