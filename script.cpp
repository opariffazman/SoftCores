#include "script.h"
#include "keyboard.h"

#include <fstream>
#include <sstream>
#include <map>
#include <ctime>

using namespace std;

// logging functions
const char* const LOG_FILE = "SoftCores.log";

ofstream file;

void initializeLogger()
{
	file.open(LOG_FILE, std::ios_base::out);
	file.close();
}

bool isFileExists(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void writeLog(const char* msg)
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	stringstream text;

	file.open(LOG_FILE, ios_base::app);
	if (file.is_open())
	{
		text << "[" << newtime.tm_mday << "/" << newtime.tm_mon + 1 << "/" << newtime.tm_year + 1900 << " " << newtime.tm_hour << ":" << newtime.tm_min << ":" << newtime.tm_sec << "] " << msg;
		file << text.str().c_str() << "\n";
		file.close();
	}
}

void initialize()
{
	initializeLogger();
	writeLog("### SoftCores Mod by opsedar ###");
	(isFileExists(".\\SoftCores.ini")) ? writeLog("### SoftCores.ini found ###") : writeLog("### SoftCores.ini not found ###");
}

// math functions
int bRound(float x)
{
	return BUILTIN::ROUND(x);
}

int bCeil(float x)
{
	return BUILTIN::CEIL(x);
}

int bFloor(float x)
{
	return BUILTIN::FLOOR(x);
}

float toFloat(int x)
{
	return BUILTIN::TO_FLOAT(x);
}

Hash key(const char* key)
{
	return MISC::GET_HASH_KEY(key);
}

void showSubtitle(const char* text)
{
	UILOG::_UILOG_SET_CACHED_OBJECTIVE((const char*)MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", text)); //
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE(); // _UILOG_PRINT_CACHED_OBJECTIVE
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE(); //
}

// cores functions
enum class Core {
	Health,
	Stamina,
	DeadEye
};

int getCurrentHealthPercent(Entity entity)
{
	return bRound(ENTITY::GET_ENTITY_HEALTH(entity) * 100.0f / ENTITY::GET_ENTITY_MAX_HEALTH(entity, 0));
}

int getMaxPlayerPoint(Core coreIndex)
{
	return ATTRIBUTE::GET_MAX_ATTRIBUTE_POINTS(PLAYER::PLAYER_PED_ID(), static_cast<int>(coreIndex));
}

int getPlayerPoint(Core coreIndex)
{
	return ATTRIBUTE::GET_ATTRIBUTE_POINTS(PLAYER::PLAYER_PED_ID(), static_cast<int>(coreIndex));
}

void setPlayerPoint(Core coreIndex, int value)
{
	ATTRIBUTE::SET_ATTRIBUTE_POINTS(PLAYER::PLAYER_PED_ID(), static_cast<int>(coreIndex), value);
}

int getPlayerCore(Core coreIndex)
{
	return ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(PLAYER::PLAYER_PED_ID(), static_cast<int>(coreIndex));
}

void setPlayerCore(Core coreIndex, int coreValue)
{
	ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(PLAYER::PLAYER_PED_ID(), static_cast<int>(coreIndex), coreValue);
}

int getHorseCore(Core coreIndex)
{
	return ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID()), static_cast<int>(coreIndex));
}

void setHorseCore(Core coreIndex, int coreValue)
{
	ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID()), static_cast<int>(coreIndex), coreValue);
}

bool isPlayerCoreOverpowered(Core coreIndex)
{
	return ATTRIBUTE::_0x200373A8DF081F22(PLAYER::PLAYER_PED_ID(), static_cast<int>(coreIndex));
}

bool isPlayerPointOverpowered(Core coreIndex)
{
	return ATTRIBUTE::_IS_ATTRIBUTE_OVERPOWERED(PLAYER::PLAYER_PED_ID(), static_cast<int>(coreIndex));
}

bool isHorseCoreOverpowered(Core coreIndex)
{
	return ATTRIBUTE::_0x200373A8DF081F22(PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_PED_ID()), static_cast<int>(coreIndex));
}

float getTimeOfDayModifier()
{
	if (CLOCK::GET_CLOCK_HOURS() >= 5 && CLOCK::GET_CLOCK_HOURS() < 11)
		return 1.5f;
	else if (CLOCK::GET_CLOCK_HOURS() >= 11 && CLOCK::GET_CLOCK_HOURS() < 14)
		return 1.25f;
	else if (CLOCK::GET_CLOCK_HOURS() >= 14 && CLOCK::GET_CLOCK_HOURS() < 18)
		return 1.75f;
	else if (CLOCK::GET_CLOCK_HOURS() >= 18 && CLOCK::GET_CLOCK_HOURS() < 23)
		return 2.0f;
	else if (CLOCK::GET_CLOCK_HOURS() >= 23 && CLOCK::GET_CLOCK_HOURS() < 5)
		return 2.5f;
	else return 0.0f;
}

void setAIDamageModifer(float melee, float weapon)
{
	PED::SET_AI_MELEE_WEAPON_DAMAGE_MODIFIER(melee);
	PED::SET_AI_WEAPON_DAMAGE_MODIFIER(weapon);
}

void setPlayerDamageModifer(float melee, float weapon)
{
	PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_ID(), melee);
	PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_ID(), weapon);
}

void setPlayerHealthRegen(float multiplier)
{
	PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(PLAYER::PLAYER_ID(), multiplier);
}

enum class Clothes : uint {
	Hats = 0x9925C067,
	Shirts = 0x2026C46D,
	Vests = 0x485EE834,
	Pants = 0x1D4C528A,
	Boots = 0x777EC6EF,
	Cloaks = 0x3C1A74CD,
	Neckwear = 0x5FC29285,
	Neckties = 0x7A96FACA,
	Gloves = 0xEABE0032,
	Coats = 0xE06D30CE,
	Chaps = 0x3107499B
};

// conditions
bool isPlayerWearing(Clothes cloth)
{
	return PED::_IS_METAPED_USING_COMPONENT(PLAYER::PLAYER_PED_ID(), static_cast<uint>(cloth));
}

bool isPedFriendly(Ped ped)
{
	const char* friendlyPeds[] = {
		"CS_ABIGAILROBERTS",
		"CS_BEATENUPCAPTAIN",
		"CS_BILLWILLIAMSON",
		"CS_BROTHERDORKINS",
		"CS_CAPTAINMONROE",
		"CS_CHARLESSMITH",
		"CS_CLAY",
		"CS_CLEET",
		"CS_CLIVE",
		"CS_DUTCH",
		"CS_EAGLEFLIES",
		"CS_EDITHDOWN",
		"CS_HAMISH",
		"CS_HERCULE",
		"CS_HOSEAMATTHEWS",
		"CS_JACKMARSTON",
		"CS_JACKMARSTON_TEEN",
		"CS_JAMIE",
		"CS_JAVIERESCUELLA",
		"CS_JOE",
		"CS_JOHNMARSTON",
		"CS_JOSIAHTRELAWNY",
		"CS_JULES",
		"CS_KAREN",
		"CS_LEMIUXASSISTANT",
		"CS_LENNY",
		"CS_LEON",
		"CS_LEOSTRAUSS",
		"CS_MAGNIFICO",
		"CS_MARYBETH",
		"CS_MICAHBELL",
		"CS_MOLLYOSHEA",
		"CS_MRPEARSON",
		"CS_MRSADLER",
		"CS_PAYTAH",
		"CS_PRINCESSISABEAU",
		"CS_RAINSFALL",
		"CS_REVSWANSON",
		"CS_SEAN",
		"CS_SISTERCALDERON",
		"CS_SUSANGRIMSHAW",
		"CS_THOMASDOWN",
		"CS_TILLY",
		"CS_UNCLE"
	};

	for (const char* friendlyPed: friendlyPeds)
	{
		if (PED::IS_PED_MODEL(ped, key(friendlyPed)))
		{
			return true;
			break;
		}
	}
	return false;
}

bool isPlayerJustDied()
{
	return PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()) || ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID());
}

bool isPlayerPlaying()
{
	Player playerID = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	return PLAYER::IS_PLAYER_PLAYING(playerID) && ENTITY::DOES_ENTITY_EXIST(playerPed);
}

bool isPlayerInMission()
{
	return MISC::GET_MISSION_FLAG() || MISC::IS_MINIGAME_IN_PROGRESS() || GRAPHICS::ANIMPOSTFX_IS_RUNNING("MissionFail01");
}

bool isPlayerInControl()
{
	return PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID());
}

bool isPlayerPursued()
{
	return PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0;
}

bool isPlayerActiveInScenario()
{
	return TASK::IS_PED_ACTIVE_IN_SCENARIO(PLAYER::PLAYER_PED_ID(), 0);
}

bool isPlayerUsingAnyScenario()
{
	return PED::IS_PED_USING_ANY_SCENARIO(PLAYER::PLAYER_PED_ID());
}

bool isPlayerBathing()
{
	return MISC::ARE_STRINGS_EQUAL(TASK::GET_TASK_MOVE_NETWORK_STATE(PLAYER::PLAYER_PED_ID()), "Bathing");
}

bool isPlayerInCombat()
{
	return PED::IS_PED_IN_COMBAT(PLAYER::PLAYER_PED_ID(), NULL) || PED::IS_PED_IN_MELEE_COMBAT(PLAYER::PLAYER_PED_ID());
}

bool isHostileNearby(Ped ped)
{
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true);
	return PED::IS_ANY_HOSTILE_PED_NEAR_POINT(ped, playerPos.x, playerPos.y, playerPos.z, 100.0f);
}

bool isPlayerIdle()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	return TASK::IS_PED_STILL(playerPed) && !TASK::IS_PED_WALKING(playerPed) && !TASK::IS_PED_RUNNING(playerPed) && !TASK::IS_PED_SPRINTING(playerPed);
}

bool isPlayerOnMount()
{
	return PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID());
}

bool isPlayerInCover()
{
	return PED::IS_PED_IN_COVER(PLAYER::PLAYER_PED_ID(), true, true);
}

// items that is categorized as weapon but is actually not
bool isWeaponItem(Hash weapon)
{
	const char* itemWeapons[] = {
		"WEAPON_UNARMED",
		"WEAPON_MELEE_LANTERN",
		"WEAPON_MELEE_DAVY_LANTERN",
		"WEAPON_MELEE_LANTERN_ELECTRIC",
		"WEAPON_KIT_BINOCULARS",
		"WEAPON_KIT_CAMERA",
		"WEAPON_KIT_DETECTOR",
		"WEAPON_BOW_CHARLES",
		"WEAPON_BOW",
		"WEAPON_FISHINGROD",
		"WEAPON_LASSO",
		"WEAPON_KIT_CAMERA_ADVANCED",
		"WEAPON_BOW_IMPROVED",
		"WEAPON_LASSO_REINFORCED",
		"WEAPON_KIT_BINOCULARS_IMPROVED",
		"WEAPON_MOONSHINEJUG_MP"
	};

	for (const char* itemWeapon: itemWeapons)
	{
		if (MISC::ARE_STRINGS_EQUAL(WEAPON::_GET_WEAPON_NAME(weapon), itemWeapon))
		{
			return true;
			break;
		}
	}
	return false;
}

bool isWeaponExotic(Hash weapon)
{
	const char* exoticWeapons[] = {
		"WEAPON_REVOLVER_DOUBLEACTION_GAMBLER",
		"WEAPON_REVOLVER_CATTLEMAN_MEXICAN",
		"WEAPON_REVOLVER_CATTLEMAN_PIG",
		"WEAPON_REVOLVER_DOUBLEACTION_EXOTIC",
		"WEAPON_REVOLVER_SCHOFIELD_GOLDEN",
		"WEAPON_PISTOL_MAUSER_DRUNK",
		"WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC",
		"WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC",
		"WEAPON_REVOLVER_CATTLEMAN_JOHN",
		"WEAPON_REVOLVER_DOUBLEACTION_MICAH",
		"WEAPON_REVOLVER_SCHOFIELD_CALLOWAY"
	};

	for (const char* exoticWeapon : exoticWeapons)
	{
		if (MISC::ARE_STRINGS_EQUAL(WEAPON::_GET_WEAPON_NAME(weapon), exoticWeapon))
		{
			return true;
			break;
		}
	}
	return false;
}

// weapon that is exclusively thrown only
bool isWeaponThrowableOnly(Hash weapon) 
{
	const char* throwableWeapons[] = {
		"WEAPON_THROWN_DYNAMITE",
		"WEAPON_THROWN_MOLOTOV",
		"WEAPON_THROWN_THROWING_KNIVES_JAVIER",
		"WEAPON_THROWN_THROWING_KNIVES",
		"WEAPON_THROWN_TOMAHAWK",
		"WEAPON_THROWN_TOMAHAWK_ANCIENT",
		"WEAPON_THROWN_BOLAS",
		"WEAPON_THROWN_POISONBOTTLE"
	};

	for (const char* throwableWeapon: throwableWeapons)
	{
		if (MISC::ARE_STRINGS_EQUAL(WEAPON::_GET_WEAPON_NAME(weapon), throwableWeapon))
		{
			return true;
			break;
		}
	}
	return false;
}

// weapon that can be used as melee or can be thrown
bool isWeaponMelee(Hash weapon)
{
	const char* meleeWeapons[] = {
		"WEAPON_MELEE_HATCHET_MELEEONLY",
		"WEAPON_MELEE_KNIFE",
		"WEAPON_MELEE_KNIFE_CIVIL_WAR",
		"WEAPON_MELEE_BROKEN_SWORD",
		"WEAPON_MELEE_HATCHET",
		"WEAPON_MELEE_HATCHET_HEWING",
		"WEAPON_MELEE_ANCIENT_HATCHET",
		"WEAPON_MELEE_HATCHET_HUNTER",
		"WEAPON_MELEE_HATCHET_VIKING",
		"WEAPON_MELEE_HATCHET_DOUBLE_BIT_RUSTED",
		"WEAPON_MELEE_HATCHET_DOUBLE_BIT",
		"WEAPON_MELEE_HATCHET_HUNTER_RUSTED",
		"WEAPON_MELEE_CLEAVER"
	};

	for (const char* meleeWeapon: meleeWeapons)
	{
		if (MISC::ARE_STRINGS_EQUAL(WEAPON::_GET_WEAPON_NAME(weapon), meleeWeapon))
		{
			return true;
			break;
		}
	}
	return false;
}

bool isDeadEyeActivated()
{
	return PAD::IS_CONTROL_JUST_PRESSED(0, key("INPUT_SPECIAL_ABILITY")) 
		|| PAD::IS_CONTROL_PRESSED(0, key("INPUT_SPECIAL_ABILITY")) 
		|| PAD::IS_CONTROL_JUST_PRESSED(0, key("INPUT_SPECIAL_ABILITY_PC")) 
		|| PAD::IS_CONTROL_PRESSED(0, key("INPUT_SPECIAL_ABILITY_PC"));
}

bool isPlayerIndoor()
{
	return (INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::PLAYER_PED_ID()) != 0) ? true : false;
}

bool isSubmerged()
{
	return ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) 
		|| ENTITY::GET_ENTITY_SUBMERGED_LEVEL(PLAYER::PLAYER_PED_ID()) > 0.0f;
}

bool isRaining()
{
	return MISC::GET_RAIN_LEVEL() > 0.0f;
}

bool isSnowing()
{
	return MISC::GET_SNOW_LEVEL() > 0.0f;
}

bool isPlayerMoving()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	return isPlayerInControl()
		&& (TASK::IS_PED_WALKING(playerPed)
		|| TASK::IS_PED_RUNNING(playerPed)
		|| TASK::IS_PED_SPRINTING(playerPed)
		|| TASK::IS_PED_GETTING_UP(playerPed)
		|| !TASK::IS_PED_STILL(playerPed)
		|| PED::IS_PED_GETTING_INTO_A_VEHICLE(playerPed)
		|| PED::_IS_PED_GETTING_INTO_A_MOUNT_SEAT(playerPed, true));
}

bool isPlayerStartedCampScenario()
{
	const char* campScenarios[] = {
		"WORLD_PLAYER_CAMP_FIRE_KNEEL1",
		"WORLD_PLAYER_CAMP_FIRE_KNEEL2",
		"WORLD_PLAYER_CAMP_FIRE_KNEEL3",
		"WORLD_PLAYER_CAMP_FIRE_KNEEL4",
		"WORLD_PLAYER_CAMP_FIRE_SIT",
		"WORLD_PLAYER_CAMP_FIRE_SQUAT",
		"WORLD_PLAYER_DYNAMIC_KNEEL_KNIFE",
		"WORLD_PLAYER_CAMP_FIRE_SQUAT_MALE_A",
		"WORLD_PLAYER_CAMP_FIRE_SIT_MALE_A",
		"WORLD_PLAYER_DYNAMIC_CAMP_FIRE_KNEEL_ARTHUR",
		"PROP_PLAYER_SLEEP_TENT_A_FRAME",
		"PROP_PLAYER_SEAT_CHAIR_PLAYER_CAMP",
		"PROP_PLAYER_SEAT_CHAIR_DYNAMIC",
		"PROP_PLAYER_SEAT_CHAIR_GENERIC",
		"PROP_PLAYER_SEAT_CHAIR_GENERIC_CA"
	};

	for (const char* campScenario : campScenarios)
	{
		if (PED::_IS_PED_USING_SCENARIO_HASH(PLAYER::PLAYER_PED_ID(), key(campScenario)) && TASK::_GET_SCENARIO_POINT_PED_IS_USING(PLAYER::PLAYER_PED_ID(), 1) == -1)
		{
			return true;
			break;
		}
	}
	return false;
}

bool isPlayerStartedSleepScenario()
{
	const char* sleepScenarios[] = {
		"WORLD_PLAYER_SLEEP_BEDROLL",
		"WORLD_PLAYER_SLEEP_BEDROLL_ARTHUR",
		"WORLD_PLAYER_SLEEP_GROUND",
		"PROP_PLAYER_SLEEP_BED",
		"PROP_PLAYER_SLEEP_BED_ARTHUR",
		"PROP_PLAYER_SLEEP_TENT_A_FRAME",
		"PROP_PLAYER_SLEEP_TENT_A_FRAME_ARTHUR",
		"PROP_PLAYER_SLEEP_TENT_MALE_A",
		"PROP_PLAYER_SLEEP_TENT_MALE_A_ARTHUR",
		"PROP_PLAYER_SLEEP_A_FRAME_TENT_PLAYER_CAMPS",
		"PROP_PLAYER_SLEEP_A_FRAME_TENT_PLAYER_CAMPS_ARTHUR"
	};

	for (const char* sleepScenario : sleepScenarios)
	{
		if (PED::_IS_PED_USING_SCENARIO_HASH(PLAYER::PLAYER_PED_ID(), key(sleepScenario)) && TASK::_GET_SCENARIO_POINT_PED_IS_USING(PLAYER::PLAYER_PED_ID(), 1) == -1)
		{
			return true;
			break;
		}
	}
	return false;
}

bool isStoryFXPlaying()
{
	const char* storyPostFXs[] = {
				"ODR3_Injured01Loop",
				"ODR3_Injured02Loop",
				"ODR3_Injured03Loop",
				"PlayerHealthPoorGuarma",
				"PlayerSickDoctorsOpinion",
				"PlayerSickDoctorsOpinionOutBad",
				"PlayerSickDoctorsOpinionOutGood",
				"PlayerWakeUpInterrogation"
	};

	for (const char* storyPostFX : storyPostFXs)
	{
		if (GRAPHICS::ANIMPOSTFX_IS_RUNNING(storyPostFX))
		{
			return true;
			break;
		}
	}
	return false;
}

int getGameTimer()
{
	return MISC::GET_GAME_TIMER();
}

// temperature mechanics
// arbitrary accumulated points for clothing player is wearing
float getPlayerClothesPoint()
{
	float clothPoints{};
	if (isPlayerWearing(Clothes::Hats)) clothPoints += 0.5f;
	if (isPlayerWearing(Clothes::Shirts)) clothPoints += 1.0f; // assured points
	if (isPlayerWearing(Clothes::Vests)) clothPoints += 1.0f;
	if (isPlayerWearing(Clothes::Pants)) clothPoints += 1.5f; // assured points
	if (isPlayerWearing(Clothes::Boots)) clothPoints += 1.0f;
	if (isPlayerWearing(Clothes::Cloaks)) clothPoints += 2.5f;
	if (isPlayerWearing(Clothes::Neckwear)) clothPoints += 0.5f;
	if (isPlayerWearing(Clothes::Neckties)) clothPoints += 0.5f;
	if (isPlayerWearing(Clothes::Gloves)) clothPoints += 1.0f;
	if (isPlayerWearing(Clothes::Coats)) clothPoints += 2.5f;
	if (isPlayerWearing(Clothes::Chaps)) clothPoints += 1.5f;
	return clothPoints;
}

float celciusToFarenheit(float temperature) // using exactly the same formula based on the decompiled scripts
{
	return ((temperature * 1.8f) + 32.0f);
}

float getSurroundingTemperature()
{
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true);
	float temperature = MISC::_GET_TEMPERATURE_AT_COORDS(playerPos.x, playerPos.y, playerPos.z); // this function return in celcius

	return (MISC::_SHOULD_USE_METRIC_TEMPERATURE()) ? temperature: celciusToFarenheit(temperature);
}

// arbitrary values for temperature points needed for player clothing to match
float getTemperaturePointsNeeded()
{
	float temperaturePoints[] = { -20.0f, -16.0f, -12.0f, -8.0f, -4.0f, 0.0f, 4.0f, 8.0f, 12.0f, 16.0f, 20.0f, 24.0f, 28.0f }; // based on a climate/temperature map by hopper on reddit https://i.redd.it/p6f6etiw7by11.jpg in celcius
	int size = sizeof(temperaturePoints) / sizeof(temperaturePoints[0]);

	if (!MISC::_SHOULD_USE_METRIC_TEMPERATURE())
	{
		for (int i = 0; i < size; i++)
		{
			temperaturePoints[i] = celciusToFarenheit(temperaturePoints[i]);
		}
	}

	if (getSurroundingTemperature() < temperaturePoints[0]) // coldest
		return 10.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[0] && getSurroundingTemperature() < temperaturePoints[1])
		return 9.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[1] && getSurroundingTemperature() < temperaturePoints[2])
		return 9.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[2] && getSurroundingTemperature() < temperaturePoints[3])
		return 8.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[3] && getSurroundingTemperature() < temperaturePoints[4])
		return 8.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[4] && getSurroundingTemperature() < temperaturePoints[5])
		return 7.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[5] && getSurroundingTemperature() < temperaturePoints[6])
		return 7.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[6] && getSurroundingTemperature() < temperaturePoints[7])
		return 6.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[7] && getSurroundingTemperature() < temperaturePoints[8])
		return 6.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[8] && getSurroundingTemperature() < temperaturePoints[9])
		return 5.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[9] && getSurroundingTemperature() < temperaturePoints[10])
		return 5.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[10] && getSurroundingTemperature() < temperaturePoints[11])
		return 4.5f;
	else if (getSurroundingTemperature() >= temperaturePoints[11] && getSurroundingTemperature() < temperaturePoints[12])
		return 4.0f;
	else if (getSurroundingTemperature() >= temperaturePoints[12]) // hottest
		return 3.5f;
	else return 0.0f;
}

void main()
{
	bool enableLogging = GetPrivateProfileInt("DEBUG", "ENABLE_LOGGING", 0, ".\\SoftCores.ini");
	if(enableLogging) initialize();

	// immersion
	bool immersionFX = GetPrivateProfileInt("IMMERSION", "IMMERSION_FX", 1, ".\\SoftCores.ini");
	bool emptyHealthFX = GetPrivateProfileInt("IMMERSION", "EMPTY_HEALTH_FX", 1, ".\\SoftCores.ini");
	bool emptyStaminaFX = GetPrivateProfileInt("IMMERSION", "EMPTY_STAMINA_FX", 1, ".\\SoftCores.ini");
	bool emptyDeadEyeFX = GetPrivateProfileInt("IMMERSION", "EMPTY_DEADEYE_FX", 1, ".\\SoftCores.ini");
	bool overpowerFX = GetPrivateProfileInt("IMMERSION", "OVERPOWER_FX", 1, ".\\SoftCores.ini");
	bool duelFX = GetPrivateProfileInt("IMMERSION", "DUEL_FX", 1, ".\\SoftCores.ini");
	bool deadEyeFX = GetPrivateProfileInt("IMMERSION", "DEADEYE_FX", 1, ".\\SoftCores.ini");
	bool eagleEyeFX = GetPrivateProfileInt("IMMERSION", "EAGLEEYE_FX", 1, ".\\SoftCores.ini");
	bool hudWheelFX = GetPrivateProfileInt("IMMERSION", "HUDWHEEL_FX", 0, ".\\SoftCores.ini");
	bool killCamFX = GetPrivateProfileInt("IMMERSION", "KILLCAM_FX", 0, ".\\SoftCores.ini");
	bool pedKillFX = GetPrivateProfileInt("IMMERSION", "PEDKILL_FX", 0, ".\\SoftCores.ini");
	bool deathFX = GetPrivateProfileInt("IMMERSION", "DEATH_FX", 0, ".\\SoftCores.ini");
	bool failFX = GetPrivateProfileInt("IMMERSION", "FAIL_FX", 0, ".\\SoftCores.ini");

	bool pickupsGlow = GetPrivateProfileInt("IMMERSION", "PICKUPS_GLOW", 0, ".\\SoftCores.ini");
	bool objectsGlow = GetPrivateProfileInt("IMMERSION", "OBJECTS_GLOW", 0, ".\\SoftCores.ini");
	bool hostileBlip = GetPrivateProfileInt("IMMERSION", "HOSTILE_BLIP", 0, ".\\SoftCores.ini");
	bool deadEyeReload = GetPrivateProfileInt("IMMERSION", "DEADEYE_RELOAD", 0, ".\\SoftCores.ini");
	bool sleepStaminaOnly = GetPrivateProfileInt("IMMERSION", "SLEEP_STAMINA_ONLY", 1, ".\\SoftCores.ini");
	bool bathDeadEyeOnly = GetPrivateProfileInt("IMMERSION", "BATH_DEADEYE_ONLY", 1, ".\\SoftCores.ini");
	bool headshotImmunity = GetPrivateProfileInt("IMMERSION", "HEADSHOT_IMMUNITY", 0, ".\\SoftCores.ini");

	// dead eye no reload usage
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Hash primaryWeapon;
	Hash secondaryWeapon;

	WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &primaryWeapon, false, 0, true);
	WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &secondaryWeapon, false, 1, true);

	int primaryAmmo{};
	int secondaryAmmo{};
	int* currPrimaryAmmo = &primaryAmmo;
	int* currSecondaryAmmo = &secondaryAmmo;
	int primaryClipSize = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, primaryWeapon, true);
	int secondaryClipSize = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, secondaryWeapon, true);
	WEAPON::GET_AMMO_IN_CLIP(playerPed, currPrimaryAmmo, primaryWeapon);
	WEAPON::GET_AMMO_IN_CLIP(playerPed, currSecondaryAmmo, secondaryWeapon);
	int primaryAmmoUsed = primaryClipSize - *currPrimaryAmmo;
	int secondaryAmmoUsed = secondaryClipSize - *currSecondaryAmmo;
	int primaryAmmoLeft = primaryClipSize - primaryAmmoUsed;
	int secondaryAmmoLeft = primaryClipSize - secondaryAmmoUsed;

	// timers
	int depletionMs = GetPrivateProfileInt("TIMERS", "CORE_DEPLETION", 120000, ".\\SoftCores.ini");
	int healthMs = GetPrivateProfileInt("TIMERS", "HEALTH_PENALTY", 8000, ".\\SoftCores.ini");
	int temperatureMs = GetPrivateProfileInt("TIMERS", "TEMPERATURE_PENALTY", 8000, ".\\SoftCores.ini");
	int aiHealMs = GetPrivateProfileInt("TIMERS", "AI_HEALTH_REGEN", 5000, ".\\SoftCores.ini");
	int aimMs = GetPrivateProfileInt("TIMERS", "AIMING_DEPLETION", 500, ".\\SoftCores.ini");

	int depletionTimer = getGameTimer() + depletionMs;
	int healthTimer = getGameTimer() + healthMs;
	int temperatureTimer = getGameTimer() + temperatureMs;
	int aimTimer = getGameTimer() + aimMs;

	// core modifiers
	bool isPlaying{};
	bool temperatureCore = GetPrivateProfileInt("TEMPERATURE_MODIFIER", "TEMPERATURE_CORE", 1, ".\\SoftCores.ini");
	bool temperatureCoreFx = GetPrivateProfileInt("TEMPERATURE_MODIFIER", "TEMPERATURE_CORE_FX", 1, ".\\SoftCores.ini");
	bool temperatureCoreSprite = GetPrivateProfileInt("TEMPERATURE_MODIFIER", "TEMPERATURE_CORE_SPRITE", 1, ".\\SoftCores.ini");
	bool aimPenalty = GetPrivateProfileInt("CORE_MODIFIER", "AIM_PENALTY", 1, ".\\SoftCores.ini");

	int playerHpModifier = GetPrivateProfileInt("CORE_MODIFIER", "PLAYER_HEALTH_CORE", 4, ".\\SoftCores.ini");
	int playerStModifier = GetPrivateProfileInt("CORE_MODIFIER", "PLAYER_STAMINA_CORE", 12, ".\\SoftCores.ini");
	int playerDeModifier = GetPrivateProfileInt("CORE_MODIFIER", "PLAYER_DEADEYE_CORE", 8, ".\\SoftCores.ini");
	int horseHpModifier = GetPrivateProfileInt("CORE_MODIFIER", "HORSE_HEALTH_CORE", 4, ".\\SoftCores.ini");
	int horseStModifier = GetPrivateProfileInt("CORE_MODIFIER", "HORSE_STAMINA_CORE", 8, ".\\SoftCores.ini");

	// death stuffs
	bool isInMission{};
	bool penaltyOnDeath = GetPrivateProfileInt("DEATH_PENALTY", "PENALTY_ON_DEATH", 1, ".\\SoftCores.ini");
	bool loseHandWeapon = GetPrivateProfileInt("DEATH_PENALTY", "LOSE_HAND_WEAPON", 1, ".\\SoftCores.ini");
	bool loseBodyWeapon = GetPrivateProfileInt("DEATH_PENALTY", "LOSE_BODY_WEAPON", 1, ".\\SoftCores.ini");
	bool loseExoticWeapon = GetPrivateProfileInt("DEATH_PENALTY", "LOSE_EXOTIC_WEAPON", 1, ".\\SoftCores.ini");

	// lose randomized % of money on death
	bool loseMoney = GetPrivateProfileInt("DEATH_PENALTY", "LOSE_MONEY", 1, ".\\SoftCores.ini");
	int lowestMoneyLose = GetPrivateProfileInt("DEATH_PENALTY", "LOWEST_MONEY_LOSE", 40, ".\\SoftCores.ini");
	int highestMoneyLose = GetPrivateProfileInt("DEATH_PENALTY", "HIGHEST_MONEY_LOSE", 80, ".\\SoftCores.ini");

	// used for restoring only stamina after sleep and deadeye after bath
	int lastHealthCore{};
	int lastStaminaCore{};
	int lastDeadEyeCore{};

	// damage modifier
	bool damageTweaks = GetPrivateProfileInt("DAMAGE_MODIFIER", "DAMAGE_TWEAKS", 1, ".\\SoftCores.ini");
	float aiDamageHighest = (float)GetPrivateProfileInt("DAMAGE_MODIFIER", "HIGHEST_AI_DAMAGE", 500, ".\\SoftCores.ini") / 100;
	float aiDamageHigh = (float)GetPrivateProfileInt("DAMAGE_MODIFIER", "HIGH_AI_DAMAGE", 400, ".\\SoftCores.ini") / 100;
	float aiDamageMedium = (float)GetPrivateProfileInt("DAMAGE_MODIFIER", "MEDIUM_AI_DAMAGE", 300, ".\\SoftCores.ini") / 100;
	float aiDamageLow = (float)GetPrivateProfileInt("DAMAGE_MODIFIER", "LOW_AI_DAMAGE", 200, ".\\SoftCores.ini") / 100;
	float aiDamageLowest = (float)GetPrivateProfileInt("DAMAGE_MODIFIER", "LOWEST_AI_DAMAGE", 100, ".\\SoftCores.ini") / 100;

	float playerDamageHighest = (float)GetPrivateProfileInt("DAMAGE_MODIFIER", "HIGHEST_PLAYER_DAMAGE", 120, ".\\SoftCores.ini") / 100;
	float playerDamageHigh = (float)GetPrivateProfileInt("DAMAGE_MODIFIER", "HIGH_PLAYER_DAMAGE", 100, ".\\SoftCores.ini") / 100;
	float playerDamageMedium = (float)GetPrivateProfileInt("DAMAGE_MODIFIER", "MEDIUM_PLAYER_DAMAGE", 80, ".\\SoftCores.ini") / 100;
	float playerDamageLow = (float)GetPrivateProfileInt("DAMAGE_MODIFIER", "LOW_PLAYER_DAMAGE", 60, ".\\SoftCores.ini") / 100;
	float playerDamageLowest = (float)GetPrivateProfileInt("DAMAGE_MODIFIER", "LOWEST_PLAYER_DAMAGE", 40, ".\\SoftCores.ini") / 100;

	// ai regen and heal map
	std::map<Ped, int> aiRegenMap;
	std::map<Ped, int> aiHealMap;

	// hostile blip map
	std::map<Ped, Blip> hostileBlipMap;

	// ai combat modifier variables
	bool aiTweaks = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "AI_TWEAKS", 1, ".\\SoftCores.ini");
	bool aiRegen = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "AI_REGEN", 1, ".\\SoftCores.ini");
	int aiHighestRegen = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "HIGHEST_HEALTH_REGEN", 80, ".\\SoftCores.ini");
	int aiHighRegen = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "HIGH_HEALTH_REGEN", 60, ".\\SoftCores.ini");
	int aiMediumRegen = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "MEDIUM_HEALTH_REGEN", 40, ".\\SoftCores.ini");
	int aiLowRegen = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "LOW_HEALTH_REGEN", 20, ".\\SoftCores.ini");
	int aiLowestRegen = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "LOWEST_HEALTH_REGEN", 10, ".\\SoftCores.ini");

	bool aiAccuracy = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "AI_ACCURACY", 1, ".\\SoftCores.ini");
	int aiHighestAccuracy = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "HIGHEST_ACCURACY", 100, ".\\SoftCores.ini");
	int aiHighAccuracy = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "HIGH_ACCURACY", 90, ".\\SoftCores.ini");
	int aiMediumAccuracy = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "MEDIUM_ACCURACY", 80, ".\\SoftCores.ini");
	int aiLowAccuracy = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "LOW_ACCURACY", 70, ".\\SoftCores.ini");
	int aiLowestAccuracy = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "LOWEST_ACCURACY", 60, ".\\SoftCores.ini");

	bool aiShootRate = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "AI_SHOOT_RATE", 1, ".\\SoftCores.ini");
	int aiHighestShootRate = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "HIGHEST_SHOOTRATE", 1000, ".\\SoftCores.ini");
	int aiHighShootRate = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "HIGH_SHOOTRATE", 900, ".\\SoftCores.ini");
	int aiMediumShootRate = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "MEDIUM_SHOOTRATE", 800, ".\\SoftCores.ini");
	int aiLowShootRate = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "LOW_SHOOTRATE", 700, ".\\SoftCores.ini");
	int aiLowestShootRate = GetPrivateProfileInt("AI_COMBAT_MODIFIER", "LOWEST_SHOOTRATE", 600, ".\\SoftCores.ini");

	// mod range
	int pedsRange = GetPrivateProfileInt("MOD_RANGE", "PEDS", 1024, ".\\SoftCores.ini");

	while (true)
	{
		// variables that need constantly updated
		Player playerID = PLAYER::PLAYER_ID();
		playerPed = PLAYER::PLAYER_PED_ID();
		Ped horsePed = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(playerID);

		// IMMERSION PART =======================================================================================================================

		// START IMMERSION FX +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (immersionFX)
		{
			if (!emptyHealthFX) GRAPHICS::ANIMPOSTFX_STOP("PlayerRPGEmptyCoreHealth");
			if (!emptyStaminaFX) GRAPHICS::ANIMPOSTFX_STOP("PlayerRPGEmptyCoreStamina");
			if (!emptyDeadEyeFX) GRAPHICS::ANIMPOSTFX_STOP("PlayerRPGEmptyCoreDeadEye");
			if (!overpowerFX) GRAPHICS::ANIMPOSTFX_STOP("PlayerOverpower");
			if (!duelFX) GRAPHICS::ANIMPOSTFX_STOP("Duel");
			if (!deadEyeFX) GRAPHICS::ANIMPOSTFX_STOP("deadeye");
			if (!deadEyeFX) GRAPHICS::ANIMPOSTFX_STOP("DeadEyeEmpty");
			if (!eagleEyeFX) GRAPHICS::ANIMPOSTFX_STOP("EagleEye");
			if (!hudWheelFX) GRAPHICS::ANIMPOSTFX_STOP("WheelHUDIn");
			if (!killCamFX) GRAPHICS::ANIMPOSTFX_STOP("killCam");
			if (!killCamFX) GRAPHICS::ANIMPOSTFX_STOP("KillCamHonorChange");
			if (!pedKillFX) GRAPHICS::ANIMPOSTFX_STOP("PedKill");
			if (!deathFX) GRAPHICS::ANIMPOSTFX_STOP("DeathFailMP01");
			if (!failFX) GRAPHICS::ANIMPOSTFX_STOP("MissionFail01");
		}
		// END IMMERSION FX +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		if (!headshotImmunity) PED::SET_PED_CONFIG_FLAG(playerPed, 263, false); // disable playerPed headshot immunity

		// START HOSTILE BLIP +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (!hostileBlip)
		{
			Ped hostilePed[1024];
			int hostileRange = 1024;
			int count = worldGetAllPeds(hostilePed, hostileRange);

			for (int i = 0; i < count; i++)
			{
				if (hostileBlipMap.find(hostilePed[i]) == hostileBlipMap.end()) hostileBlipMap[hostilePed[i]] = 0; // initialize map
			}

			for (int i = 0; i < count; i++)
			{
				if (isHostileNearby(hostilePed[i]) || isPlayerInCombat() || isPlayerPursued() || isPlayerInMission()) // returns TRUE whenever hostile is nearby/player in combat, being pursued/wanted or in mission (hopefully works for stealth missions)
				{
					if (hostilePed[i] != playerPed && hostilePed[i] != horsePed && !isPedFriendly(hostilePed[i]) && ENTITY::IS_ENTITY_A_PED(hostilePed[i]) && !PED::IS_PED_DEAD_OR_DYING(hostilePed[i], true)) // not playerPed, horsePed, friendlyPed, a ped & not dead
					{
						PED::REQUEST_PED_VISIBILITY_TRACKING(ENTITY::GET_PED_INDEX_FROM_ENTITY_INDEX(hostilePed[i])); // track these hostile peds if all is true
						hostileBlipMap[hostilePed[i]] = MAP::GET_BLIP_FROM_ENTITY(hostilePed[i]); // save em in a Ped <-> Blip Map

						if (PED::IS_TRACKED_PED_VISIBLE(ENTITY::GET_PED_INDEX_FROM_ENTITY_INDEX(hostilePed[i]))) // within player fov
						{
							MAP::_BLIP_SET_MODIFIER(hostileBlipMap[hostilePed[i]], key("BLIP_MODIFIER_FADE_IN"));
							MAP::_SET_BLIP_FLASH_STYLE(hostileBlipMap[hostilePed[i]], key("BLIP_MODIFIER_FADE"));
						}
						else if (!PED::IS_TRACKED_PED_VISIBLE(ENTITY::GET_PED_INDEX_FROM_ENTITY_INDEX(hostilePed[i]))) // not within player fov
						{
							MAP::_BLIP_SET_MODIFIER(hostileBlipMap[hostilePed[i]], key("BLIP_MODIFIER_FADE"));
							MAP::_SET_BLIP_FLASH_STYLE(hostileBlipMap[hostilePed[i]], key("BLIP_MODIFIER_FADE_OUT_SLOW"));
						}
					}
				}
			}
		}
		// END HOSTILE BLIP +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// START OF SLEEP STAMINA ONLY PART +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (sleepStaminaOnly)
		{
			bool isNotinControl;
			bool isSleeping;

			if (!isPlayerInControl() && !isNotinControl) // get last health and deadeye core values when player is no longer in control (when using campfire from wheel or start of most scenario)
			{
				isNotinControl = true;
				lastHealthCore = getPlayerCore(Core::Health);
				lastDeadEyeCore = getPlayerCore(Core::DeadEye);
			}
			else if (isPlayerInControl() && isNotinControl) // set back to false when player regain control
			{
				isNotinControl = false;
			}

			if (isNotinControl && isPlayerStartedSleepScenario() && !isSleeping) // once hooked the first entry point of scenario which is -1 while not in control, stop hooking at all
			{
				isSleeping = true;
				stringstream text;
				text << "hooked player is sleeping while not in control, lastHealthCore: " << lastHealthCore << " lastDeadEyeCore: " << lastDeadEyeCore;
				writeLog(text.str().c_str());
			}
			else if (!isNotinControl && isPlayerStartedSleepScenario() && !isSleeping) // once hooked the first entry point of scenario which is -1 while in control, stop hooking at all
			{
				isSleeping = true;
				isNotinControl = true;
				lastHealthCore = getPlayerCore(Core::Health);
				lastDeadEyeCore = getPlayerCore(Core::DeadEye);
				stringstream text;
				text << "hooked player is sleeping while in control, lastHealthCore: " << lastHealthCore << " lastDeadEyeCore: " << lastDeadEyeCore;
				writeLog(text.str().c_str());
			}
			else if (isRaining() && PED::_IS_PED_USING_SCENARIO_HASH(playerPed, key("PROP_PLAYER_SLEEP_TENT_A_FRAME")) && !isSleeping) // if player sets camp when its raining, will go directly to tent, hence not initiating -1 scenario point i reckon
			{
				isSleeping = true;
				lastHealthCore = getPlayerCore(Core::Health);
				lastDeadEyeCore = getPlayerCore(Core::DeadEye);
				stringstream text;
				text << "hooked player is sleeping while raining, lastHealthCore: " << lastHealthCore << " lastDeadEyeCore: " << lastDeadEyeCore;
				writeLog(text.str().c_str());
			}

			if (isSleeping) // keep setting player last health, deadeye until player starts moving or using campfire scenario
			{
				setPlayerCore(Core::Health, lastHealthCore);
				setPlayerCore(Core::DeadEye, lastDeadEyeCore);

				isSleeping = (isPlayerMoving() || isPlayerStartedCampScenario()) ? false : true;
			}
		}
		// END OF SLEEP STAMINA ONLY PART +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// START OF BATH DEADEYE ONLY PART ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (bathDeadEyeOnly)
		{
			bool isBathing;

			if (isPlayerBathing() && !isBathing) // hook bath here & get last health, stamina core
			{
				lastHealthCore = getPlayerCore(Core::Health);
				lastStaminaCore = getPlayerCore(Core::Stamina);
				isBathing = true;
				stringstream text;
				text << "hooked player is bathing, lastHealthCore: " << lastHealthCore << " lastStaminaCore: " << lastStaminaCore;
				writeLog(text.str().c_str());
			}

			if (isBathing) // keep setting player last health, stamina until player starts moving
			{
				setPlayerCore(Core::Health, lastHealthCore);
				setPlayerCore(Core::Stamina, lastStaminaCore);

				isBathing = (isPlayerMoving()) ? false : true;
			}
		}
		// END OF BATH DEADEYE ONLY PART ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// START DISABLE GLOW PART ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (!pickupsGlow)
		{
			Pickup pickups[512];
			int pickupsRange = 512;
			int pCount = worldGetAllPickups(pickups, pickupsRange); // disabling weapon pickups glow
			for (int i = 0; i < pCount; i++) GRAPHICS::_0x50C14328119E1DD1(pickups[i], true); // yep this native below directly disables the glow
		}

		if (!objectsGlow)
		{
			Object objects[512];
			int objectsRange = 512;
			int oCount = worldGetAllObjects(objects, objectsRange); // disabling lootable object such as apple, carrot and such
			for (int i = 0; i < oCount; i++) GRAPHICS::_0x50C14328119E1DD1(objects[i], true);
		}
		// END DISABLE GLOW PART ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// START OF DEADEYE NO RELOAD PART ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (!deadEyeReload)
		{
			if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &primaryWeapon, false, 0, true)) // primary hand and ammo
			{
				primaryClipSize = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, primaryWeapon, true);
				WEAPON::GET_AMMO_IN_CLIP(playerPed, currPrimaryAmmo, primaryWeapon);
			}

			if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &secondaryWeapon, false, 1, true)) // secondary hand weapon and ammo
			{
				secondaryClipSize = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, secondaryWeapon, true);
				WEAPON::GET_AMMO_IN_CLIP(playerPed, currSecondaryAmmo, secondaryWeapon);
			}

			if (PED::IS_PED_SHOOTING(playerPed) || PED::GET_PED_RESET_FLAG(playerPed, 332) || PED::IS_PED_RELOADING(playerPed)) // updates the value ammoUsed everytime player shoots, switched weapon or reloading
			{
				primaryAmmoUsed = primaryClipSize - *currPrimaryAmmo;
				secondaryAmmoUsed = secondaryClipSize - *currSecondaryAmmo;
				primaryAmmoLeft = primaryClipSize - primaryAmmoUsed;
				secondaryAmmoLeft = secondaryClipSize - secondaryAmmoUsed;
			}

			if (isDeadEyeActivated() && !TASK::_IS_PED_DUELLING(playerPed)) // hook deadEye and sets ammo accordingly and let game handle dueling
			{
				WEAPON::SET_AMMO_IN_CLIP(playerPed, primaryWeapon, primaryAmmoLeft);
				WEAPON::SET_AMMO_IN_CLIP(playerPed, secondaryWeapon, secondaryAmmoLeft);
			}
		}
		// END OF DEADEYE NO RELOAD PART ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// END OF IMMERSION PART ================================================================================================================

		// CORE PART ============================================================================================================================
		if (isPlayerPlaying() && !isPlaying)
		{
			isPlaying = true;
			writeLog("hooked player is playing");
		}
		else if (!isPlayerPlaying() && isPlaying)
		{
			isPlaying = false;
			writeLog("hooked player is not playing");
		}
		
		bool isStoryPostFX;

		if (isStoryFXPlaying() && !isStoryPostFX)
		{
			isStoryPostFX = true;
			writeLog("hooked isStoryPostFX running");
		}
		else if (!isStoryFXPlaying() && isStoryPostFX)
		{
			isStoryPostFX = false;
			writeLog("hooked isStoryPostFX stopped");
		}

		if ((isPlaying && !isStoryPostFX) || (ENTITY::DOES_ENTITY_EXIST(horsePed) && !ENTITY::IS_ENTITY_DEAD(horsePed))) // do this first else will crash on new game, also disable the core part of the mod while in storyPostFX
		{
			float playerHealthRegen = (float)GetPrivateProfileInt("CORE_MODIFIER", "PLAYER_HEALTH_REGEN", 50, ".\\SoftCores.ini") / 100.0f;
			int playerHpPercentageDrain = GetPrivateProfileInt("CORE_MODIFIER", "PLAYER_HEALTH_PENALTY", 4, ".\\SoftCores.ini") * ENTITY::GET_ENTITY_MAX_HEALTH(playerPed, 0) / 100;
			int horseHpPercentageDrain = GetPrivateProfileInt("CORE_MODIFIER", "HORSE_HEALTH_PENALTY", 4, ".\\SoftCores.ini") * ENTITY::GET_ENTITY_MAX_HEALTH(horsePed, 0) / 100;
			
			// START TEMPERATURE CORE +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			float pointsDifferences;
			int outfitModifier;

			if (temperatureCore) // this on to be called every tick
			{
				float pointsModifier = 0.0f; // default no points modifier, just uses surrounding temperature as base value to determine clothes points needed
				
				// isNearFireModifier ***********************************************************************************************************

				bool isNearFireModifier;
				Vector3 nearestFire;
				Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, true);

				float fireModifier = (float)GetPrivateProfileInt("TEMPERATURE_MODIFIER", "FIRE_MODIFIER", 50, ".\\SoftCores.ini") / 100.0f;

				if (FIRE::GET_CLOSEST_FIRE_POS(&nearestFire, playerPos.x, playerPos.y, playerPos.z))
				{
					if (MISC::GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, nearestFire.x, nearestFire.y, nearestFire.z, true) < 4.0f && !isNearFireModifier)
					{
						isNearFireModifier = true;
						stringstream text;
						text << "hooked player near fire, fireModifer: " << fireModifier << " positive points (hotness)";
						writeLog(text.str().c_str());
					}
					else if (MISC::GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, nearestFire.x, nearestFire.y, nearestFire.z, true) > 4.0f && isNearFireModifier)
					{
						isNearFireModifier = false;
						writeLog("hooked player far from fire");
					}
				}

				if (isNearFireModifier) pointsModifier = pointsModifier + fireModifier; // positive point value for hotness

				// isCampfireModifier ***********************************************************************************************************

				bool isCampfireModifier;

				float campFireModifier = (float)GetPrivateProfileInt("TEMPERATURE_MODIFIER", "CAMPFIRE_MODIFIER", 50, ".\\SoftCores.ini") / 100.0f;

				if (isPlayerInControl() && isPlayerStartedCampScenario() && !isCampfireModifier)
				{
					isCampfireModifier = true;
					stringstream text;
					text << "hooked player started campfire scenario, campFireModifier: " << campFireModifier << " positive points (hotness)";
					writeLog(text.str().c_str());
				}
				else if (isPlayerInControl() && isPlayerMoving() && !isPlayerStartedCampScenario() && isCampfireModifier)
				{
					isCampfireModifier = false;
					writeLog("hooked player stopped campfire scenario");
				}

				if (isCampfireModifier) pointsModifier = pointsModifier + campFireModifier; // positive point value for hotness

				// isNotOutsideModifier ***********************************************************************************************************

				bool isIndoorModifier;
				
				float indoorModifier = (float)GetPrivateProfileInt("TEMPERATURE_MODIFIER", "INDOOR_MODIFIER", 100, ".\\SoftCores.ini") / 100.0f;

				if (isPlayerIndoor() && !isIndoorModifier)
				{
					isIndoorModifier = true;

					stringstream text;
					text << "hooked player is indoor, indoorModifier: " << indoorModifier << " positive points (hotness)";
					writeLog(text.str().c_str());
				}
				else if (!isPlayerIndoor() && isIndoorModifier)
				{
					isIndoorModifier = false;
					writeLog("hooked player is outdoor");
				}

				if (isIndoorModifier) pointsModifier = pointsModifier + indoorModifier; // positive point value for hotness

				// isSubmergedModifier ***********************************************************************************************************

				bool isSubmergedModifier;

				float submergedModifier = (float)GetPrivateProfileInt("TEMPERATURE_MODIFIER", "SUBMERGED_MODIFIER", 100, ".\\SoftCores.ini") / 100.0f;

				if (isSubmerged() && !isSubmergedModifier)
				{
					isSubmergedModifier = true;
					submergedModifier = submergedModifier + ENTITY::GET_ENTITY_SUBMERGED_LEVEL(playerPed);
					stringstream text;
					text << "hooked player is submerged, submergedModifier: " << submergedModifier << " negative points (coldness)";
					writeLog(text.str().c_str());
				}
				else if (!isSubmerged() && isSubmergedModifier)
				{
					isSubmergedModifier = false;
					writeLog("hooked player is no longer submerged");
				}

				if (isSubmergedModifier) pointsModifier = pointsModifier - submergedModifier; // negative point value for coldness

				if (!isIndoorModifier) // only apply raining & snowing modifier when player is not indoor
				{
					// isRainingModifier ***********************************************************************************************************

					bool isRainingModifier;
					
					float rainingModifier = (float)GetPrivateProfileInt("TEMPERATURE_MODIFIER", "RAINING_MODIFIER", 50, ".\\SoftCores.ini") / 100.0f;

					if (isRaining() && !isRainingModifier)
					{
						isRainingModifier = true;
						rainingModifier = rainingModifier + MISC::GET_RAIN_LEVEL();
						stringstream text;
						text << "hooked is raining, rainingModifier: " << rainingModifier << " negative points (coldness)";
						writeLog(text.str().c_str());
					}
					else if (!isRaining() && isRainingModifier)
					{
						isRainingModifier = false;
						writeLog("hooked is no longer raining");
					}

					if (isRainingModifier) pointsModifier = pointsModifier - rainingModifier; // negative point value for coldness

					// isSnowingModifier ***********************************************************************************************************

					bool isSnowingModifier;

					float snowingModifier = (float)GetPrivateProfileInt("TEMPERATURE_MODIFIER", "SNOWING_MODIFIER", 100, ".\\SoftCores.ini") / 100.0f;

					if (isSnowing() && !isSnowingModifier)
					{
						isSnowingModifier = true;
						snowingModifier = snowingModifier + MISC::GET_SNOW_LEVEL();
						stringstream text;
						text << "hooked is snowing, snowingModifier: " << snowingModifier << " negative points (coldness)";
						writeLog(text.str().c_str());
					}
					else if (!isSnowing() && isSnowingModifier)
					{
						isSnowingModifier = false;
						writeLog("hooked is no longer snowing");
					}

					if (isSnowingModifier) pointsModifier = pointsModifier - snowingModifier;  // negative point value for coldness
				}

				pointsDifferences = getPlayerClothesPoint() - getTemperaturePointsNeeded() + pointsModifier; // stack with pointsModifer which accumulates +ve points for hotness and -ve points for coldness

				const char* spriteModifier;

				if (pointsDifferences < -2.5f) // simulate cold if pointDifferences is less than -2.5, say player is wearing 7.0 points of clothing vs 10.0 points of temperature, outfitModifier is cold
				{
					outfitModifier = 0;
					spriteModifier = "RPG_COLD";
				}
				else if (pointsDifferences >= -2.5f && pointsDifferences < 4.5f) // simulate warm, ideal condition
				{
					outfitModifier = 1;
					spriteModifier = "RPG_WARM";
				}
				else if (pointsDifferences >= 4.5f) // simulate hot if pointDifferences is more than 4.5, say player is wearing 10.0 points of clothing vs 5.5 points of temperature, outfitModifier is hot
				{
					outfitModifier = 2;
					spriteModifier = "RPG_HOT";
				}

				bool isBathing;

				if (isPlayerBathing() && !isBathing) isBathing = true; // this can only be entered during inital bathing moment
				else if (!isPlayerBathing() && isBathing) isBathing = (isPlayerMoving()) ? false : true; // set to false when player starts moving

				if (temperatureCoreSprite) // only show sprite when true on ini configuration file
				{
					int drawTimer;
					bool drawSprite;

					if (PAD::IS_CONTROL_JUST_PRESSED(0, key("INPUT_REVEAL_HUD")) && !PAD::IS_CONTROL_PRESSED(0, key("INPUT_OPEN_WHEEL_MENU")) && !HUD::IS_HUD_HIDDEN() && !isPlayerActiveInScenario() && !isBathing && !drawSprite)
					{
						drawTimer = getGameTimer();
						drawSprite = true;

						stringstream text;
						text << "hooked player is showing sprite, spriteModifier: " << spriteModifier << " overall clothing points: " << getPlayerClothesPoint() << " needs: " << getTemperaturePointsNeeded() + pointsModifier;
						writeLog(text.str().c_str());
					}

					if (drawSprite)
					{
						if (!TXD::_HAS_STREAMED_TXD_LOADED(key("RPG_TEXTURES")))
						{
							TXD::REQUEST_STREAMED_TEXTURE_DICT("RPG_TEXTURES", false);
						}
						else if (TXD::_HAS_STREAMED_TXD_LOADED(key("RPG_TEXTURES")) && spriteModifier != "RPG_WARM")
						{
							GRAPHICS::DRAW_SPRITE("RPG_TEXTURES", spriteModifier, 0.25f, 0.9f, 0.045f, 0.07f, 0.0f, 240, 240, 240, 180, false);
						}
					}

					if (getGameTimer() - drawTimer > 3000) drawSprite = false; // only show for 3 seconds
				}

				(outfitModifier == 2) ? PED::SET_PED_RESET_FLAG(playerPed, 139, true) : PED::SET_PED_RESET_FLAG(playerPed, 139, false); // if outfit is hot, no stamina regen
				
				bool knockedOut;

				temperatureMs = (outfitModifier == 2 || knockedOut) ? (GetPrivateProfileInt("TIMERS", "TEMPERATURE_PENALTY", 8000, ".\\SoftCores.ini") * 2) : GetPrivateProfileInt("TIMERS", "TEMPERATURE_PENALTY", 8000, ".\\SoftCores.ini"); // if outfit is hot/knockedOut previously, temperatureMs is doubled 

				if (isPlayerInControl() && !isBathing) // returns TRUE if player is in control && not bathing
				{
					if (getGameTimer() > temperatureTimer) // penalty timer for player outfit in accordance to surrounding temperature, called every temperatureTimer
					{
						stringstream text;
						int temperatureHpPercentageDrain = bCeil(fabsf(pointsDifferences)) * ENTITY::GET_ENTITY_MAX_HEALTH(playerPed, 0) / 100; // calculater hp drain, convert the pointsDifferences to positive first

						switch (outfitModifier)
						{
						case 0: // cold
							if(temperatureCoreFx) GRAPHICS::ANIMPOSTFX_PLAY("PlayerHonorLevelBad"); // grayish tint, seems suitable enough to show player is cold
							(ENTITY::GET_ENTITY_HEALTH(playerPed) - temperatureHpPercentageDrain <= 1) ? ENTITY::_SET_ENTITY_HEALTH(playerPed, 1, 1) : ENTITY::_SET_ENTITY_HEALTH(playerPed, (ENTITY::GET_ENTITY_HEALTH(playerPed) - temperatureHpPercentageDrain), 0); // hp outer core drain
							if (getPlayerCore(Core::Health) < 10 && getCurrentHealthPercent(playerPed) < 10 && !isPlayerCoreOverpowered(Core::Health) && !isPlayerPointOverpowered(Core::Health) && !knockedOut) // when reached this threshold, knock player out
							{
								TASK::TASK_KNOCKED_OUT(playerPed, 0.0f, false);
								knockedOut = true;
								text << "hooked cold effect with health less that 10%, knockedOut, next occurence after " << temperatureMs * 2 << " ms";
								writeLog(text.str().c_str());
							}
							else
							{
								text << "hooked cold effect, next occurence after " << temperatureMs << " ms";
								writeLog(text.str().c_str());
								knockedOut = false;
							}
							break;
						case 1: // warm
							break;
						case 2: // hot
							if (temperatureCoreFx) GRAPHICS::ANIMPOSTFX_PLAY("PlayerHonorLevelGood"); // reddish tint, seems suitable enough to show player is hot
							text << "hooked hot effect, next occurence after " << temperatureMs * 2 << " ms";
							writeLog(text.str().c_str());
							break;
						default:
							break;
						}

						temperatureMs = (outfitModifier == 2 || knockedOut) ? (GetPrivateProfileInt("TIMERS", "TEMPERATURE_PENALTY", 8000, ".\\SoftCores.ini") * 2) : GetPrivateProfileInt("TIMERS", "TEMPERATURE_PENALTY", 8000, ".\\SoftCores.ini"); // if outfit is hot/knockedOut previously, temperatureMs is doubled 
						temperatureTimer = getGameTimer() + temperatureMs; // if knocked out previously, the next timer will be longer to allow recovery period
					}
				}
			}
			// END TEMPERATURE CORE +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			// START AIMING PENALTY +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (aimPenalty)
			{
				int stAimingPenalty = GetPrivateProfileInt("CORE_MODIFIER", "STAMINA_AIMING_PENALTY", 4, ".\\SoftCores.ini") * getMaxPlayerPoint(Core::Stamina) / 100;
				int deAimingPenalty = GetPrivateProfileInt("CORE_MODIFIER", "DEADEYE_AIMING_PENALTY", 8, ".\\SoftCores.ini") * getMaxPlayerPoint(Core::DeadEye) / 100;
				bool isAimingAir = false;

				if (PAD::IS_CONTROL_JUST_PRESSED(0, key("INPUT_AIM_IN_AIR")) || PAD::IS_CONTROL_PRESSED(0, key("INPUT_AIM_IN_AIR"))) isAimingAir = !isAimingAir;
				
				if (getGameTimer() > aimTimer)
				{
					stAimingPenalty = (isPlayerPointOverpowered(Core::Stamina)) ? 0 : stAimingPenalty;
					deAimingPenalty = (isPlayerPointOverpowered(Core::DeadEye)) ? 0 : deAimingPenalty;

					if (PLAYER::IS_PLAYER_FREE_AIMING(playerID) && !isAimingAir)
					{
						Hash aimedWeapon;
						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &aimedWeapon, false, 0, true))
						{
							if (!isWeaponItem(aimedWeapon) && !isWeaponThrowableOnly(aimedWeapon) && !isWeaponMelee(aimedWeapon)) // if not unique and not throwable
							{
								PED::SET_PED_RESET_FLAG(playerPed, 139, true); // disable stamina outer core regen

								if (!isPlayerPointOverpowered(Core::Stamina)) // only occurs if not overpowered by potions, let game handles that instead
								{
									(getPlayerPoint(Core::Stamina) - stAimingPenalty <= 0) ? setPlayerPoint(Core::Stamina, 0) : setPlayerPoint(Core::Stamina, getPlayerPoint(Core::Stamina) - stAimingPenalty); // drain outer stamina core until empty
								}

								if (!isPlayerPointOverpowered(Core::DeadEye)) // only occurs if not overpowered by potions, let game handles that instead
								{
									(getPlayerPoint(Core::DeadEye) - deAimingPenalty <= 0) ? setPlayerPoint(Core::DeadEye, 0) : setPlayerPoint(Core::DeadEye, getPlayerPoint(Core::DeadEye) - deAimingPenalty); // drain outer deadeye core until empty
								}

								if (getPlayerPoint(Core::Stamina) == 0) // if stamina outer core is empty 
								{
									if (!isPlayerCoreOverpowered(Core::Stamina)) // and main core is not overpowered
									{
										(getPlayerCore(Core::Stamina) - 1 <= 0) ? setPlayerCore(Core::Stamina, 0) : setPlayerCore(Core::Stamina, getPlayerCore(Core::Stamina) - 1); // drains main stamina core
									}
								}

								if (getPlayerPoint(Core::DeadEye) == 0) // if stamina outer core is empty 
								{
									if (!isPlayerCoreOverpowered(Core::DeadEye)) // and main core is not overpowered
									{
										(getPlayerCore(Core::DeadEye) - 1 <= 0) ? setPlayerCore(Core::DeadEye, 0) : setPlayerCore(Core::DeadEye, getPlayerCore(Core::DeadEye) - 1); // drains main deadeye core
									}
								}
							}
						}
					}
					else if (!PLAYER::IS_PLAYER_FREE_AIMING(playerID) || isAimingAir)
					{
						(getPlayerPoint(Core::Stamina) + stAimingPenalty < getMaxPlayerPoint(Core::Stamina)) ? setPlayerPoint(Core::Stamina, getPlayerPoint(Core::Stamina) + stAimingPenalty) : setPlayerPoint(Core::Stamina, getMaxPlayerPoint(Core::Stamina)); // restore outer core stamina until max
						(getPlayerPoint(Core::DeadEye) + deAimingPenalty < getMaxPlayerPoint(Core::DeadEye)) ? setPlayerPoint(Core::DeadEye, getPlayerPoint(Core::DeadEye) + deAimingPenalty) : setPlayerPoint(Core::DeadEye, getMaxPlayerPoint(Core::DeadEye)); // restore outer core  deadeye until max

						PED::SET_PED_RESET_FLAG(playerPed, 139, false); // enable stamina outer core regen
					}

					aimTimer = getGameTimer() + aimMs;
				}
			}
			// END AIMING PENALTY +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			// START CORE DRAIN +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (getGameTimer() > depletionTimer) // every time game timer has pass the timeMs mark, set main core drain values accordingly, called every depletionTimer
			{
				// dynamic drain values according to time of day
				float drainModifier = getTimeOfDayModifier();

				if (!isPlayerIdle() && isPlayerInControl() && (!isPlayerActiveInScenario() || !isPlayerUsingAnyScenario())) // returns TRUE if player is not idle and in control and not active in scenario or using any
				{
					// PLAYER Cores
					int playerHpDrain = (isPlayerCoreOverpowered(Core::Health)) ? 0 : bCeil(drainModifier * playerHpModifier);
					int playerStDrain = (isPlayerCoreOverpowered(Core::Stamina)) ? 0 : bCeil(drainModifier * playerStModifier);
					int playerDeDrain = (isPlayerCoreOverpowered(Core::DeadEye)) ? 0 : bCeil(drainModifier * playerDeModifier);

					// HORSE Cores
					int horseHpDrain = (isHorseCoreOverpowered(Core::Health)) ? 0 : bCeil(drainModifier * horseHpModifier);
					int horseStDrain = (isHorseCoreOverpowered(Core::Stamina)) ? 0 : bCeil(drainModifier * horseStModifier);

					// if player on mount, all core drain slower by calculated above else, horse core drain slower
					if (isPlayerOnMount())
					{
						playerHpDrain = bFloor(playerHpDrain * 0.5f);
						playerStDrain = bFloor(playerStDrain * 0.5f);
						playerDeDrain = bFloor(playerDeDrain * 0.5f);
					}
					else
					{
						horseHpDrain = bFloor(horseHpDrain * 0.5f);
						horseStDrain = bFloor(horseStDrain * 0.5f);
					}

					if (temperatureCore) // if temperatureCore feature enabled in .ini configuration, apply temperature modifier to base main core drain effects
					{
						float temperatureModifier;
						stringstream text;

						switch (outfitModifier)
						{
						case 0: // cold
							playerHealthRegen = 0.0f; // no hp regen at all
							temperatureModifier = 1.0f + fabsf(pointsDifferences);
							playerHpDrain = bCeil(playerHpDrain * temperatureModifier);
							depletionMs = GetPrivateProfileInt("TIMERS", "CORE_DEPLETION", 120000, ".\\SoftCores.ini") / 2; // if outfit is not warm, core depletionMs is faster by 50%
							text << "hooked cold effect, next core drain occurence after " << depletionMs << " ms playerHpDrain: " << playerHpDrain << " playerStDrain: " << playerStDrain << " playerDeDrain: " << playerDeDrain;
							writeLog(text.str().c_str());
							break;
						case 1: // warm, all core drain slower by 10 %
							playerHealthRegen = (isPlayerCoreOverpowered(Core::Health)) ? 1.0f : playerHealthRegen;
							temperatureModifier = 0.9f;
							playerHpDrain = bFloor(playerHpDrain * temperatureModifier);
							playerStDrain = bFloor(playerStDrain * temperatureModifier);
							playerDeDrain = bFloor(playerDeDrain * temperatureModifier);
							depletionMs = GetPrivateProfileInt("TIMERS", "CORE_DEPLETION", 120000, ".\\SoftCores.ini"); // if outfit warm, core depletionMs is as set in .ini configuration file
							text << "hooked warm effect, next core drain occurence after " << depletionMs << " ms playerHpDrain: " << playerHpDrain << " playerStDrain: " << playerStDrain << " playerDeDrain: " << playerDeDrain;
							writeLog(text.str().c_str());
							break;
						case 2: // hot
							playerHealthRegen = playerHealthRegen * 0.2f; // slowed health outer core regen by 80%
							temperatureModifier = 0.5f + pointsDifferences;
							playerStDrain = bCeil(playerStDrain * temperatureModifier);
							playerDeDrain = bCeil(playerDeDrain * temperatureModifier);
							depletionMs = GetPrivateProfileInt("TIMERS", "CORE_DEPLETION", 120000, ".\\SoftCores.ini") / 2; // if outfit is not warm, core depletionMs is faster by 50%
							text << "hooked hot effect, next core drain occurence after " << depletionMs << " ms playerHpDrain: " << playerHpDrain << " playerStDrain: " << playerStDrain << " playerDeDrain: " << playerDeDrain;
							writeLog(text.str().c_str());
							break;
						default:
							break;
						}
					}
					else if (!temperatureCore)
					{
						stringstream text;
						text << "hooked core drain effect, next core drain occurence after " << depletionMs << " ms playerHpDrain: " << playerHpDrain << " playerStDrain: " << playerStDrain << " playerDeDrain: " << playerDeDrain;
						writeLog(text.str().c_str());
						playerHealthRegen = (isPlayerCoreOverpowered(Core::Health)) ? 1.0f : playerHealthRegen;
					}

					// calculate both PLAYER and HORSE core drain accordingly
					(getPlayerCore(Core::Health) - playerHpDrain <= 0) ? setPlayerCore(Core::Health, 0) : setPlayerCore(Core::Health, getPlayerCore(Core::Health) - playerHpDrain);
					(getPlayerCore(Core::Stamina) - playerStDrain <= 0) ? setPlayerCore(Core::Stamina, 0) : setPlayerCore(Core::Stamina, getPlayerCore(Core::Stamina) - playerStDrain);
					(getPlayerCore(Core::DeadEye) - playerDeDrain <= 0) ? setPlayerCore(Core::DeadEye, 0) : setPlayerCore(Core::DeadEye, getPlayerCore(Core::DeadEye) - playerDeDrain);

					(getHorseCore(Core::Health) - horseHpDrain <= 0) ? setHorseCore(Core::Health, 0) : setHorseCore(Core::Health, getHorseCore(Core::Health) - horseHpDrain);
					(getHorseCore(Core::Stamina) - horseStDrain <= 0) ? setHorseCore(Core::Stamina, 0) : setHorseCore(Core::Stamina, getHorseCore(Core::Stamina) - horseStDrain);
				}

				depletionTimer = getGameTimer() + depletionMs;
			}

			if (getGameTimer() > healthTimer) // every time game timer has pass the healthMs mark, set outer core drain values accordingly, called every healthTimer
			{
				if (!isPlayerIdle() && isPlayerInControl() && (!isPlayerActiveInScenario() || !isPlayerUsingAnyScenario())) // returns TRUE if player is not idle and in control and not active in scenario or using any
				{
					if (getPlayerCore(Core::Health) <= 0 && !isPlayerCoreOverpowered(Core::Health))
					{
						playerHealthRegen = 0.0f;
						(ENTITY::GET_ENTITY_HEALTH(playerPed) - playerHpPercentageDrain <= 1) ? ENTITY::_SET_ENTITY_HEALTH(playerPed, 1, 1) : ENTITY::_SET_ENTITY_HEALTH(playerPed, (ENTITY::GET_ENTITY_HEALTH(playerPed) - playerHpPercentageDrain), 0); // health outer core penalty for main core being empty
					}
					 
					if (isPlayerOnMount() && getHorseCore(Core::Health) <= 0 && !isHorseCoreOverpowered(Core::Health)) // only if player is on mount
					{
						(ENTITY::GET_ENTITY_HEALTH(horsePed) - horseHpPercentageDrain <= 1) ? ENTITY::_SET_ENTITY_HEALTH(horsePed, 1, 1) : ENTITY::_SET_ENTITY_HEALTH(horsePed, (ENTITY::GET_ENTITY_HEALTH(horsePed) - horseHpPercentageDrain), 0);
					}
				}
				healthTimer = getGameTimer() + healthMs;
			}

			(getPlayerCore(Core::Stamina) <= 0) ? PED::SET_PED_RESET_FLAG(playerPed, 139, true) : PED::SET_PED_RESET_FLAG(playerPed, 139, false); // if main stamina core is empty, no stamina regen

			(isPlayerCoreOverpowered(Core::DeadEye) || isPlayerPointOverpowered(Core::DeadEye)) ? PED::SET_PED_ACCURACY(playerPed, 100) : PED::SET_PED_ACCURACY(playerPed, getPlayerCore(Core::DeadEye)); // if deadeye main or outer core is overpowered, accuracy is 100%, else, ties to deadeye main core value
			
			(isPlayerInCombat() && (!isPlayerOnMount() || !isPlayerInCover())) ? setPlayerHealthRegen(0.0f) : setPlayerHealthRegen(playerHealthRegen); // no regen in combat unless on mount or in cover
			// END CORE DRAIN +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			// START OF DEATH PENALTY +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (isPlayerInMission() && !isInMission)
			{
				writeLog("hooked player is in mission, penalty on death disabled");
				isInMission = true;
			}
			else if (!isPlayerInMission() && isInMission)
			{
				isInMission = false;
				writeLog("hooked player is not in mission, penalty on death enabled");
			}
			
			if (penaltyOnDeath && !isInMission)
			{
				bool coresPenalty;
				bool deathPenalty;
				int deathTime;

				if (!isPlayerMoving() && isPlayerJustDied() && !isPlayerPlaying() && !coresPenalty)
				{
					deathTime = PED::GET_PED_TIME_OF_DEATH(playerPed);
					coresPenalty = true;
					writeLog("hooked player is not moving, just died, not playing and !coresPenalty");
				}
				
				if (coresPenalty && !deathPenalty)
				{
					if (loseHandWeapon) // lose weapon on hand
					{
						const int weaponAttachPoints[] = { 0, 1 }; // weapon attachment points for both hands

						for (const int& weaponAttachPoint : weaponAttachPoints)
						{
							Hash weaponHash;
							if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponHash, false, weaponAttachPoint, true))
							{
								if (!isWeaponItem(weaponHash) && !isWeaponMelee(weaponHash)) // check first for weapon is item or melee
								{
									if (!loseExoticWeapon)
									{
										if (!isWeaponExotic(weaponHash)) // check for exotic if loseExoticWeapon is set to false
										{
											stringstream text;
											text << WEAPON::_GET_WEAPON_NAME(weaponHash) << " removed with ammo: " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash);
											writeLog(text.str().c_str());
											WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, weaponHash), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash), 0x2188E0A3);
											WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, weaponHash, true, NULL);
										}
									}
									else
									{
										stringstream text;
										text << WEAPON::_GET_WEAPON_NAME(weaponHash) << " removed with ammo: " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash);
										writeLog(text.str().c_str());
										WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, weaponHash), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash), 0x2188E0A3);
										WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, weaponHash, true, NULL);
									}
								}
							}
						}
					}

					if (loseBodyWeapon) // lose weapon on body
					{
						const int weaponAttachPoints[] = { 2, 3, 9, 10, 6 }; // all weapon attachment points on body

						for (const int& weaponAttachPoint : weaponAttachPoints)
						{
							Hash weaponHash;
							if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponHash, false, weaponAttachPoint, true))
							{
								if (!isWeaponItem(weaponHash) && !isWeaponMelee(weaponHash)) // check first for weapon is item or melee
								{
									if (weaponAttachPoint == 6)
									{
										if (isWeaponThrowableOnly(weaponHash)) // check thrower slot and ensure throwably exclusive only
										{
											if (!loseExoticWeapon)
											{
												if (!isWeaponExotic(weaponHash)) // check for exotic if loseExoticWeapon is set to false
												{
													stringstream text;
													text << WEAPON::_GET_WEAPON_NAME(weaponHash) << " removed with ammo: " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash);
													writeLog(text.str().c_str());
													WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, weaponHash), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash), 0x2188E0A3);
													WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, weaponHash, true, NULL);
												}
											}
											else
											{
												stringstream text;
												text << WEAPON::_GET_WEAPON_NAME(weaponHash) << " removed with ammo: " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash);
												writeLog(text.str().c_str());
												WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, weaponHash), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash), 0x2188E0A3);
												WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, weaponHash, true, NULL);
											}
										}
									}
									else
									{
										if (!loseExoticWeapon)
										{
											if (!isWeaponExotic(weaponHash)) // check for exotic if loseExoticWeapon is set to false
											{
												stringstream text;
												text << WEAPON::_GET_WEAPON_NAME(weaponHash) << " removed with ammo " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash);
												writeLog(text.str().c_str());
												WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, weaponHash), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash), 0x2188E0A3);
												WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, weaponHash, true, NULL);
											}
										}
										else
										{
											stringstream text;
											text << WEAPON::_GET_WEAPON_NAME(weaponHash) << " removed with ammo " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash);
											writeLog(text.str().c_str());
											WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, weaponHash), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, weaponHash), 0x2188E0A3);
											WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, weaponHash, true, NULL);
										}
									}
								}
							}
						}
					}

					if (loseMoney)
					{
						int rngInt = MISC::GET_RANDOM_INT_IN_RANGE(lowestMoneyLose, highestMoneyLose);
						int finalMoney = (MONEY::_MONEY_GET_CASH_BALANCE() * rngInt / 100 <= 0) ? 0 : MONEY::_MONEY_GET_CASH_BALANCE() * rngInt / 100;
						MONEY::_MONEY_DECREMENT_CASH_BALANCE(finalMoney);
						stringstream text;
						text << "percentage of money to lose: " << rngInt << " % finalMoney: " << finalMoney;
						writeLog(text.str().c_str());
					}
					writeLog("hooked death penalty to player weapons, ammo and money applied");
					deathPenalty = true;
				}

				if (coresPenalty)
				{
					setPlayerCore(Core::Health, 0);
					setPlayerCore(Core::Stamina, 0);
					setPlayerCore(Core::DeadEye, 0);

					ENTITY::_SET_ENTITY_HEALTH(playerPed, 1, 0);
					setPlayerPoint(Core::Stamina, 1);
					setPlayerPoint(Core::DeadEye, 1);
					coresPenalty = (getGameTimer() - deathTime > 15000) ? false : true;
				}

				if (isPlayerMoving() && !isPlayerJustDied() && isPlayerPlaying() && !coresPenalty && deathPenalty)
				{
					setPlayerPoint(Core::Stamina, getMaxPlayerPoint(Core::Stamina)); // restore outer core stamina until max
					setPlayerPoint(Core::DeadEye, getMaxPlayerPoint(Core::DeadEye)); // restore outer core  deadeye until max
					deathPenalty = false;
					writeLog("hooked stopped applying death penalty to player cores");
				}
			}
			// END OF DEATH PENALTY +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		}
		// END OF CORE PART =====================================================================================================================

		// START DAMAGE AND NPC MODIFIER ========================================================================================================
		if (damageTweaks) // set damage modifier base on player health, basically the more health you got, the higher damager you take, the lower damage you give
		{
			if (getCurrentHealthPercent(playerPed) > 80 && getCurrentHealthPercent(playerPed) <= 100)
			{
				setAIDamageModifer(aiDamageHighest * 0.5f, aiDamageHighest);
				setPlayerDamageModifer(playerDamageLowest * 0.5f, playerDamageLowest);
			}
			else if (getCurrentHealthPercent(playerPed) > 60 && getCurrentHealthPercent(playerPed) <= 80)
			{
				setAIDamageModifer(aiDamageHigh * 0.5f, aiDamageHigh);
				setPlayerDamageModifer(playerDamageLow * 0.5f, playerDamageLow);
			}
			else if (getCurrentHealthPercent(playerPed) > 40 && getCurrentHealthPercent(playerPed) <= 60)
			{
				setAIDamageModifer(aiDamageMedium * 0.5f, aiDamageMedium);
				setPlayerDamageModifer(playerDamageMedium * 0.5f, playerDamageMedium);
			}
			else if (getCurrentHealthPercent(playerPed) > 20 && getCurrentHealthPercent(playerPed) <= 40)
			{
				setAIDamageModifer(aiDamageLow * 0.5f, aiDamageLow);
				setPlayerDamageModifer(playerDamageHigh * 0.5f, playerDamageHigh);
			}
			else if (getCurrentHealthPercent(playerPed) > 0 && getCurrentHealthPercent(playerPed) <= 20)
			{
				setAIDamageModifer(aiDamageLowest * 0.5f, aiDamageLowest);
				setPlayerDamageModifer(playerDamageHighest * 0.5f, playerDamageHighest);
			}
		}

		if (aiTweaks)
		{
			Ped peds[1024];
			int count = worldGetAllPeds(peds, pedsRange); // get all peds within range and do stuffs

			if (aiRegen)
			{
				for (int i = 0; i < count; i++)
				{
					if (aiRegenMap.find(peds[i]) == aiRegenMap.end()) aiRegenMap[peds[i]] = 0;
					if (aiHealMap.find(peds[i]) == aiHealMap.end()) aiHealMap[peds[i]] = 0;
				}
			}

			for (int i = 0; i < count; i++)
			{
				if (peds[i] != playerPed && PED::IS_PED_HUMAN(peds[i]) && !PED::IS_PED_DEAD_OR_DYING(peds[i], true) && ENTITY::DOES_ENTITY_EXIST(peds[i]))
				{
					if (getCurrentHealthPercent(peds[i]) > 80 && getCurrentHealthPercent(peds[i]) <= 100)
					{
						if (aiAccuracy) PED::SET_PED_ACCURACY(peds[i], aiHighestAccuracy); // accuracy
						if (aiShootRate) PED::SET_PED_SHOOT_RATE(peds[i], aiHighestShootRate); // shoot rate
						if (aiRegen) aiRegenMap[peds[i]] = aiLowestRegen * 100 / ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], 0);
					}
					else if (getCurrentHealthPercent(peds[i]) > 60 && getCurrentHealthPercent(peds[i]) <= 80)
					{
						if (aiAccuracy) PED::SET_PED_ACCURACY(peds[i], aiHighAccuracy);
						if (aiShootRate) PED::SET_PED_SHOOT_RATE(peds[i], aiHighShootRate);
						if (aiRegen) aiRegenMap[peds[i]] = aiLowRegen * 100 / ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], 0);
					}
					else if (getCurrentHealthPercent(peds[i]) > 40 && getCurrentHealthPercent(peds[i]) <= 60)
					{
						if (aiAccuracy) PED::SET_PED_ACCURACY(peds[i], aiMediumAccuracy);
						if (aiShootRate) PED::SET_PED_SHOOT_RATE(peds[i], aiMediumShootRate);
						if (aiRegen) aiRegenMap[peds[i]] = aiMediumRegen * 100 / ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], 0);
					}
					else if (getCurrentHealthPercent(peds[i]) > 20 && getCurrentHealthPercent(peds[i]) <= 40)
					{
						if (aiAccuracy) PED::SET_PED_ACCURACY(peds[i], aiLowAccuracy);
						if (aiShootRate) PED::SET_PED_SHOOT_RATE(peds[i], aiLowShootRate);
						if (aiRegen) aiRegenMap[peds[i]] = aiHighRegen * 100 / ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], 0);
					}
					else if (getCurrentHealthPercent(peds[i]) > 0 && getCurrentHealthPercent(peds[i]) <= 20)
					{
						if (aiAccuracy) PED::SET_PED_ACCURACY(peds[i], aiLowestAccuracy);
						if (aiShootRate) PED::SET_PED_SHOOT_RATE(peds[i], aiLowestShootRate);
						if (aiRegen) aiRegenMap[peds[i]] = aiLowestRegen * 100 / ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], 0);
					}

					if (aiRegen)
					{
						if (ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(peds[i]) || ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_OBJECT(peds[i]))
						{
							aiHealMap[peds[i]] = getGameTimer() + aiHealMs; // time ai is damaged

							if (getGameTimer() - aiHealMap[peds[i]] >= aiHealMs) // current game time minus time ai is damaged more than aiHealMs
							{
								if (PED::IS_PED_IN_MELEE_COMBAT(peds[i])) // melee combat check so they can actually be taken down, still heal tho
								{
									if ((ENTITY::GET_ENTITY_HEALTH(peds[i]) + (aiRegenMap[peds[i]] * 0.1f)) < ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], 0))
									{
										ENTITY::_SET_ENTITY_HEALTH(peds[i], bFloor(ENTITY::GET_ENTITY_HEALTH(peds[i]) + (aiRegenMap[peds[i]] * 0.1f)), 0); // slowed regen for melee combat
									}
									else
									{
										ENTITY::_SET_ENTITY_HEALTH(peds[i], (ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], 0)), 0); // set max health
									}
								}
								else if (!PED::IS_PED_IN_MELEE_COMBAT(peds[i]))
								{
									if ((ENTITY::GET_ENTITY_HEALTH(peds[i]) + aiRegenMap[peds[i]]) < ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], 0))
									{
										ENTITY::_SET_ENTITY_HEALTH(peds[i], (ENTITY::GET_ENTITY_HEALTH(peds[i]) + aiRegenMap[peds[i]]), 0); // normal regen
									}
									else
									{
										ENTITY::_SET_ENTITY_HEALTH(peds[i], (ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], 0)), 0); // set max health
										PED::CLEAR_PED_BLOOD_DAMAGE(peds[i]);
										for (int j = 0; j < 10; j++)
										{
											PED::_CLEAR_PED_BLOOD_DAMAGE_FACIAL(peds[i], j);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		// END DAMAGE AND NPC MODIFIER ==========================================================================================================

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(static_cast<int>(GetTickCount64()));
	main();
}
