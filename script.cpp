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
	time_t nowTimestamp = std::time(0);
	tm* now = localtime(&nowTimestamp);
	stringstream txt;

	file.open(LOG_FILE, std::ios_base::app);
	if (file.is_open())
	{
		txt << "[" << now->tm_mday << "/" << now->tm_mon << "/" << now->tm_year + 1900 << " "
			<< now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "] " << msg;
		file << txt.str().c_str() << "\n";
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

bool isPedFriendly(Ped ped)
{
	return PED::IS_PED_MODEL(ped, key("cs_abigailroberts"))
		|| PED::IS_PED_MODEL(ped, key("cs_beatenupcaptain"))
		|| PED::IS_PED_MODEL(ped, key("cs_billwilliamson"))
		|| PED::IS_PED_MODEL(ped, key("cs_brotherdorkins"))
		|| PED::IS_PED_MODEL(ped, key("cs_captainmonroe"))
		|| PED::IS_PED_MODEL(ped, key("cs_charlessmith"))
		|| PED::IS_PED_MODEL(ped, key("cs_clay"))
		|| PED::IS_PED_MODEL(ped, key("cs_cleet"))
		|| PED::IS_PED_MODEL(ped, key("cs_clive"))
		|| PED::IS_PED_MODEL(ped, key("cs_dutch"))
		|| PED::IS_PED_MODEL(ped, key("cs_eagleflies"))
		|| PED::IS_PED_MODEL(ped, key("cs_edithdown"))
		|| PED::IS_PED_MODEL(ped, key("cs_hamish"))
		|| PED::IS_PED_MODEL(ped, key("cs_hercule"))
		|| PED::IS_PED_MODEL(ped, key("cs_hoseamatthews"))
		|| PED::IS_PED_MODEL(ped, key("cs_jackmarston"))
		|| PED::IS_PED_MODEL(ped, key("cs_jackmarston_teen"))
		|| PED::IS_PED_MODEL(ped, key("cs_jamie"))
		|| PED::IS_PED_MODEL(ped, key("cs_javierescuella"))
		|| PED::IS_PED_MODEL(ped, key("cs_joe"))
		|| PED::IS_PED_MODEL(ped, key("cs_johnmarston"))
		|| PED::IS_PED_MODEL(ped, key("cs_josiahtrelawny"))
		|| PED::IS_PED_MODEL(ped, key("cs_jules"))
		|| PED::IS_PED_MODEL(ped, key("cs_karen"))
		|| PED::IS_PED_MODEL(ped, key("cs_lemiuxassistant"))
		|| PED::IS_PED_MODEL(ped, key("cs_lenny"))
		|| PED::IS_PED_MODEL(ped, key("cs_leon"))
		|| PED::IS_PED_MODEL(ped, key("cs_leostrauss"))
		|| PED::IS_PED_MODEL(ped, key("cs_magnifico"))
		|| PED::IS_PED_MODEL(ped, key("cs_marybeth"))
		|| PED::IS_PED_MODEL(ped, key("cs_micahbell"))
		|| PED::IS_PED_MODEL(ped, key("cs_mollyoshea"))
		|| PED::IS_PED_MODEL(ped, key("cs_mrpearson"))
		|| PED::IS_PED_MODEL(ped, key("cs_mrsadler"))
		|| PED::IS_PED_MODEL(ped, key("cs_paytah"))
		|| PED::IS_PED_MODEL(ped, key("cs_princessisabeau"))
		|| PED::IS_PED_MODEL(ped, key("cs_rainsfall"))
		|| PED::IS_PED_MODEL(ped, key("cs_revswanson"))
		|| PED::IS_PED_MODEL(ped, key("cs_sean"))
		|| PED::IS_PED_MODEL(ped, key("cs_sistercalderon"))
		|| PED::IS_PED_MODEL(ped, key("cs_susangrimshaw"))
		|| PED::IS_PED_MODEL(ped, key("cs_thomasdown"))
		|| PED::IS_PED_MODEL(ped, key("cs_tilly"))
		|| PED::IS_PED_MODEL(ped, key("cs_uncle"));
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

bool isPlayerCoreFilled()
{
	return (getPlayerCore(Core::Health) == 100 && getPlayerCore(Core::Stamina) == 100 && getPlayerCore(Core::DeadEye) == 100);
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
	else
		return 1.0f;
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
	return !PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) ? false : true;
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

bool isWeaponUnique(Hash weapon)
{
	const char* uniqueWeapons[] = {
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

	for each (const char* uniqueWeapon in uniqueWeapons)
	{
		return MISC::ARE_STRINGS_EQUAL(WEAPON::_GET_WEAPON_NAME(weapon), WEAPON::_GET_WEAPON_NAME(key(uniqueWeapon)));
	}
}

bool isWeaponThrowableOnly(Hash weapon) // weapon that is exclusively thrown only
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

	for each (const char* throwableWeapon in throwableWeapons)
	{
		return MISC::ARE_STRINGS_EQUAL(WEAPON::_GET_WEAPON_NAME(weapon), WEAPON::_GET_WEAPON_NAME(key(throwableWeapon)));
	}
}

bool isWeaponGroupMelee(Hash weapon)
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

	for each (const char* meleeWeapon in meleeWeapons)
	{
		return MISC::ARE_STRINGS_EQUAL(WEAPON::_GET_WEAPON_NAME(weapon), WEAPON::_GET_WEAPON_NAME(key(meleeWeapon)));
	}
}

bool isDeadEyeActivated()
{
	return PAD::IS_CONTROL_JUST_PRESSED(0, key("INPUT_SPECIAL_ABILITY")) || PAD::IS_CONTROL_PRESSED(0, key("INPUT_SPECIAL_ABILITY")) || PAD::IS_CONTROL_JUST_PRESSED(0, key("INPUT_SPECIAL_ABILITY_PC")) || PAD::IS_CONTROL_PRESSED(0, key("INPUT_SPECIAL_ABILITY_PC"));
}

bool isPlayerOutside()
{
	return (INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::PLAYER_PED_ID()) == 0) ? true : false;
}

bool isSubmerged()
{
	return ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) || (ENTITY::GET_ENTITY_SUBMERGED_LEVEL(PLAYER::PLAYER_PED_ID()) > 0.0f);
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

int getGameTimer()
{
	return MISC::GET_GAME_TIMER();
}

// temperature mechanics
float getPlayerClothesPoint()
{
	float clothPoints{};
	if (isPlayerWearing(Clothes::Hats)) clothPoints += 0.5f;
	if (isPlayerWearing(Clothes::Shirts)) clothPoints += 1.0f;
	if (isPlayerWearing(Clothes::Vests)) clothPoints += 1.0f;
	if (isPlayerWearing(Clothes::Pants)) clothPoints += 1.5f;
	if (isPlayerWearing(Clothes::Boots)) clothPoints += 1.0f;
	if (isPlayerWearing(Clothes::Cloaks)) clothPoints += 2.5f;
	if (isPlayerWearing(Clothes::Neckwear)) clothPoints += 0.5f;
	if (isPlayerWearing(Clothes::Neckties)) clothPoints += 0.5f;
	if (isPlayerWearing(Clothes::Gloves)) clothPoints += 1.0f;
	if (isPlayerWearing(Clothes::Coats)) clothPoints += 2.5f;
	if (isPlayerWearing(Clothes::Chaps)) clothPoints += 1.5f;
	return clothPoints;
}

float celciusToFarenheit(float temperature)
{
	return ((temperature * 1.8f) + 32.0f);
}

float getSurroundingTemperature()
{
	float temperature;
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, true);
	temperature = MISC::_GET_TEMPERATURE_AT_COORDS(playerPos.x, playerPos.y, playerPos.z);

	if (!MISC::_SHOULD_USE_METRIC_TEMPERATURE())
	{
		temperature = celciusToFarenheit(temperature);
	}

	return temperature;
}

float getTemperaturePointsNeeded()
{
	if (!MISC::_SHOULD_USE_METRIC_TEMPERATURE())
	{
		if (getSurroundingTemperature() <= celciusToFarenheit(-20.0f))
			return 10.0f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(-20.0f) && getSurroundingTemperature() <= celciusToFarenheit (-16.0f))
			return 9.5f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(-16.0f) && getSurroundingTemperature() <= celciusToFarenheit(-12.0f))
			return 9.0f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(-12.0f) && getSurroundingTemperature() <= celciusToFarenheit(-8.0f))
			return 8.5f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(-8.0f) && getSurroundingTemperature() <= celciusToFarenheit(-4.0f))
			return 8.0f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(-4.0f) && getSurroundingTemperature() <= 0.0f)
			return 7.5f;
		else if (getSurroundingTemperature() >= 0.0f && getSurroundingTemperature() <= celciusToFarenheit(4.0f))
			return 7.0f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(4.0f) && getSurroundingTemperature() <= celciusToFarenheit(8.0f))
			return 6.5f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(8.0f) && getSurroundingTemperature() <= celciusToFarenheit(12.0f))
			return 6.0f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(12.0f) && getSurroundingTemperature() <= celciusToFarenheit(16.0f))
			return 6.0f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(16.0f) && getSurroundingTemperature() <= celciusToFarenheit(20.0f))
			return 5.5f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(20.0f) && getSurroundingTemperature() <= celciusToFarenheit(24.0f))
			return 5.0f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(24.0f) && getSurroundingTemperature() <= celciusToFarenheit(28.0f))
			return 4.5f;
		else if (getSurroundingTemperature() >= celciusToFarenheit(28.0f))
			return 4.0f;
	}
	else if (MISC::_SHOULD_USE_METRIC_TEMPERATURE())
	{
		if (getSurroundingTemperature() <= -20.0f)
			return 10.0f;
		else if (getSurroundingTemperature() >= -20.0f && getSurroundingTemperature() <= -16.0f)
			return 9.5f;
		else if (getSurroundingTemperature() >= -16.0f && getSurroundingTemperature() <= -12.0f)
			return 9.0f;
		else if (getSurroundingTemperature() >= -12.0f && getSurroundingTemperature() <= -8.0f)
			return 8.5f;
		else if (getSurroundingTemperature() >= -8.0f && getSurroundingTemperature() <= -4.0f)
			return 8.0f;
		else if (getSurroundingTemperature() >= -4.0f && getSurroundingTemperature() <= 0.0f)
			return 7.5f;
		else if (getSurroundingTemperature() >= 0.0f && getSurroundingTemperature() <= 4.0f)
			return 7.0f;
		else if (getSurroundingTemperature() >= 4.0f && getSurroundingTemperature() <= 8.0f)
			return 6.5f;
		else if (getSurroundingTemperature() >= 8.0f && getSurroundingTemperature() <= 12.0f)
			return 6.0f;
		else if (getSurroundingTemperature() >= 12.0f && getSurroundingTemperature() <= 16.0f)
			return 6.0f;
		else if (getSurroundingTemperature() >= 16.0f && getSurroundingTemperature() <= 20.0f)
			return 5.5f;
		else if (getSurroundingTemperature() >= 20.0f && getSurroundingTemperature() <= 24.0f)
			return 5.0f;
		else if (getSurroundingTemperature() >= 24.0f && getSurroundingTemperature() <= 28.0f)
			return 4.5f;
		else if (getSurroundingTemperature() >= 28.0f)
			return 4.0f;
	}
}

void main()
{
	initialize();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

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

	// dead eye no reload usage
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
	bool temperatureCore = GetPrivateProfileInt("CORE_MODIFIER", "TEMPERATURE_CORE", 1, ".\\SoftCores.ini");
	bool temperatureCoreFx = GetPrivateProfileInt("CORE_MODIFIER", "TEMPERATURE_CORE_FX", 1, ".\\SoftCores.ini");
	bool aimPenalty = GetPrivateProfileInt("CORE_MODIFIER", "AIM_PENALTY", 1, ".\\SoftCores.ini");

	int playerHpModifier = GetPrivateProfileInt("CORE_MODIFIER", "PLAYER_HEALTH_CORE", 4, ".\\SoftCores.ini");
	int playerStModifier = GetPrivateProfileInt("CORE_MODIFIER", "PLAYER_STAMINA_CORE", 12, ".\\SoftCores.ini");
	int playerDeModifier = GetPrivateProfileInt("CORE_MODIFIER", "PLAYER_DEADEYE_CORE", 8, ".\\SoftCores.ini");
	int horseHpModifier = GetPrivateProfileInt("CORE_MODIFIER", "HORSE_HEALTH_CORE", 4, ".\\SoftCores.ini");
	int horseStModifier = GetPrivateProfileInt("CORE_MODIFIER", "HORSE_STAMINA_CORE", 8, ".\\SoftCores.ini");

	// death stuffs
	bool penaltyOnDeath = GetPrivateProfileInt("DEATH_PENALTY", "PENALTY_ON_DEATH", 1, ".\\SoftCores.ini");
	bool loseHandWeapon = GetPrivateProfileInt("DEATH_PENALTY", "LOSE_HAND_WEAPON", 1, ".\\SoftCores.ini");
	bool loseBodyWeapon = GetPrivateProfileInt("DEATH_PENALTY", "LOSE_BODY_WEAPON", 1, ".\\SoftCores.ini");

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
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Ped horsePed = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(playerID);

		// START WEATHER PART +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*int f5{ VK_F5 };
		if (IsKeyJustUp(f5))
		{
			Hash weatherType1;
			Hash weatherType2;
			float transition;
			MISC::_GET_WEATHER_TYPE_TRANSITION(&weatherType1, &weatherType2, &transition);
			MISC::_SET_WEATHER_TYPE_TRANSITION(weatherType1, 839715153, transition, 1);
			MISC::_SET_WEATHER_TYPE(839715153, true, true, true, 1.0f, true);
			writeLog("set weather type transition 839715153");
		}*/
		// END WEATHER PART +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

			if (!isPlayerInControl() && !isNotinControl) // get last health and deadeye core values when player is no longer in control (when using campfire from wheel or start of most scenario)
			{
				isNotinControl = true;
				lastHealthCore = getPlayerCore(Core::Health);
				lastDeadEyeCore = getPlayerCore(Core::DeadEye);
				stringstream text;
				text << "hooked player is not in control, lastHealthCore: " << lastHealthCore << " lastDeadEyeCore: " << lastDeadEyeCore;
				writeLog(text.str().c_str());
			}
			else if (isPlayerInControl() && isNotinControl) // set back to false when player regain control
			{
				isNotinControl = false;
				writeLog("hooked player is in control");
			}

			for each (const char* sleepScenario in sleepScenarios) // tries hooking via each sleeping scenario available as of now
			{
				if (isNotinControl && PED::_IS_PED_USING_SCENARIO_HASH(playerPed, key(sleepScenario)) && TASK::_GET_SCENARIO_POINT_PED_IS_USING(playerPed, 1) == -1 && !isSleeping) // once hooked the first entry point of scenario which is -1 while not in control, stop hooking at all
				{
					isSleeping = true;
					stringstream text;
					text << "hooked entry point of sleep scenario: " << sleepScenario << " while isNotinControl = true, lastHealthCore: " << lastHealthCore << " lastDeadEyeCore: " << lastDeadEyeCore;
					writeLog(text.str().c_str());
				}
				else if (!isNotinControl && PED::_IS_PED_USING_SCENARIO_HASH(playerPed, key(sleepScenario)) && TASK::_GET_SCENARIO_POINT_PED_IS_USING(playerPed, 1) == -1 && !isSleeping) // once hooked the first entry point of scenario which is -1 while in control, stop hooking at all
				{
					isSleeping = true;
					isNotinControl = true;
					lastHealthCore = getPlayerCore(Core::Health);
					lastDeadEyeCore = getPlayerCore(Core::DeadEye);
					stringstream text;
					text << "hooked entry point of sleep scenario: " << sleepScenario << " while isNotinControl = false, lastHealthCore: " << lastHealthCore << " lastDeadEyeCore: " << lastDeadEyeCore;
					writeLog(text.str().c_str());
				}
			}

			if (isSleeping) // keep setting player last health, deadeye until player starts moving or using campfire scenario
			{
				setPlayerCore(Core::Health, lastHealthCore);
				setPlayerCore(Core::DeadEye, lastDeadEyeCore);

				isSleeping = (isPlayerMoving() || PED::_IS_PED_USING_SCENARIO_HASH(playerPed, key("WORLD_PLAYER_CAMP_FIRE_KNEEL1"))) ? false : true;
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
				text << "hooked task move network state: " << TASK::GET_TASK_MOVE_NETWORK_STATE(playerPed) << " lastHealthCore: " << lastHealthCore << " lastStaminaCore: " << lastStaminaCore;
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
			Pickup pickups[1024];
			int pickupsRange = 1024;
			int pCount = worldGetAllPickups(pickups, pickupsRange); // disabling weapon pickups glow
			for (int i = 0; i < pCount; i++)
			{
				GRAPHICS::_0x50C14328119E1DD1(pickups[i], true); // yep this native below directly disables the glow
			}
		}

		if (!objectsGlow)
		{
			Object objects[1024];
			int objectsRange = 1024;
			int oCount = worldGetAllObjects(objects, objectsRange); // disabling lootable object such as apple, carrot and such
			for (int i = 0; i < oCount; i++)
			{
				GRAPHICS::_0x50C14328119E1DD1(objects[i], true);
			}
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
		bool isPlaying;
		if (isPlayerPlaying() && !isPlaying)
		{
			isPlaying = true;
			writeLog("isPlaying = true");
		}
		else if (!isPlayerPlaying() && isPlaying)
		{
			isPlaying = false;
			writeLog("isPlaying = false");
		}
		
		bool isStoryPostFX;

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

		for each (const char* storyPostFX in storyPostFXs)
		{
			if (GRAPHICS::ANIMPOSTFX_IS_RUNNING(storyPostFX) && !isStoryPostFX)
			{
				isStoryPostFX = true;
				stringstream text;
				text << "hooked isStoryPostFX running: " << storyPostFX;
				writeLog(text.str().c_str());
			}
			else if (!GRAPHICS::ANIMPOSTFX_IS_RUNNING(storyPostFX) && isStoryPostFX)
			{
				isStoryPostFX = false;
				stringstream text;
				writeLog("hooked isStoryPostFX stopped");
			}
		}

		if ((isPlayerPlaying() && !isStoryPostFX) || (ENTITY::DOES_ENTITY_EXIST(horsePed) && !ENTITY::IS_ENTITY_DEAD(horsePed))) // do this first else will crash on new game, also disable the core part of the mod while in storyPostFX
		{
			float playerHealthRegen = (float)GetPrivateProfileInt("CORE_MODIFIER", "PLAYER_HEALTH_REGEN", 50, ".\\SoftCores.ini") / 100;
			int playerHpPercentageDrain = GetPrivateProfileInt("CORE_MODIFIER", "PLAYER_HEALTH_PENALTY", 4, ".\\SoftCores.ini") * ENTITY::GET_ENTITY_MAX_HEALTH(playerPed, 0) / 100;
			int horseHpPercentageDrain = GetPrivateProfileInt("CORE_MODIFIER", "HORSE_HEALTH_PENALTY", 4, ".\\SoftCores.ini") * ENTITY::GET_ENTITY_MAX_HEALTH(horsePed, 0) / 100;
			
			// START TEMPERATURE CORE +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (temperatureCore)
			{
				float pointsModifier = 0.0f;
				
				// isNearFireModifier ***********************************************************************************************************

				bool isNearFireModifier;
				Vector3 nearestFire;
				Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, true);

				if (FIRE::GET_CLOSEST_FIRE_POS(&nearestFire, playerPos.x, playerPos.y, playerPos.z))
				{
					if (MISC::GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, nearestFire.x, nearestFire.y, nearestFire.z, true) < 4.0f && !isNearFireModifier)
					{
						isNearFireModifier = true;
						stringstream text;
						text << "hooked player close to fire, pointsModifier: " << pointsModifier - 1.0f;
						writeLog(text.str().c_str());
					}
					else if (MISC::GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, nearestFire.x, nearestFire.y, nearestFire.z, true) > 4.0f && isNearFireModifier)
					{
						isNearFireModifier = false;
						stringstream text;
						writeLog("hooked player far from fire");
					}
				}

				if (isNearFireModifier)
				{
					pointsModifier = pointsModifier - 1.0f; // minus pointsModifier value here to subtract to overall value later
				}

				// isCampfireModifier ***********************************************************************************************************

				bool isCampfireModifier;
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

				for each (const char* campScenario in campScenarios)
				{
					if (isPlayerInControl() && PED::_IS_PED_USING_SCENARIO_HASH(playerPed, key(campScenario)) && TASK::_GET_SCENARIO_POINT_PED_IS_USING(playerPed, 1) == -1 && !isCampfireModifier)
					{
						isCampfireModifier = true;

						stringstream text;
						text << "hooked first entry point of campfire scenario: " << campScenario << " pointsModifier: " << pointsModifier - 1.0f;
						writeLog(text.str().c_str());
					}

					if (isCampfireModifier)
					{
						pointsModifier = pointsModifier - 1.0f; // minus pointsModifier value here to subtract to overall value later
						isCampfireModifier = (isPlayerMoving() && !PED::_IS_PED_USING_SCENARIO_HASH(playerPed, key(campScenario))) ? false : true;
					}

				}

				// isNotOutsideModifier ***********************************************************************************************************

				bool isNotOutsideModifier;
				
				if (!isPlayerOutside() && !isNotOutsideModifier)
				{
					isNotOutsideModifier = true;

					stringstream text;
					text << "hooked player is inside interior: " << INTERIOR::GET_INTERIOR_FROM_ENTITY(playerPed) << " pointsModifier: " << pointsModifier - 2.0f;
					writeLog(text.str().c_str());
				}
				else if (isPlayerOutside() && isNotOutsideModifier)
				{
					isNotOutsideModifier = false;
					writeLog("hooked player outside interior");
				}

				if (isNotOutsideModifier)
				{
					pointsModifier = pointsModifier - 2.0f; // minus pointsModifier value here to subtract to overall value later
				}

				// isSubmergedModifier ***********************************************************************************************************

				bool isSubmergedModifier;

				if (isSubmerged() && !isSubmergedModifier)
				{
					isSubmergedModifier = true;

					stringstream text;
					text << "hooked player submerged, submergedLevel: " << 0.5f + ENTITY::GET_ENTITY_SUBMERGED_LEVEL(playerPed) << " pointsModifier: " << pointsModifier + 0.5f + ENTITY::GET_ENTITY_SUBMERGED_LEVEL(playerPed);
					writeLog(text.str().c_str());
				}
				else if (!isSubmerged() && isSubmergedModifier)
				{
					isSubmergedModifier = false;
					writeLog("hooked player no longer submerged");
				}

				if (isSubmergedModifier)
				{
					pointsModifier = pointsModifier + 0.5f + ENTITY::GET_ENTITY_SUBMERGED_LEVEL(playerPed);
				}

				// isRainingModifier ***********************************************************************************************************

				bool isRainingModifier;

				if (isRaining() && !isRainingModifier)
				{
					isRainingModifier = true;
					
					stringstream text;
					text << "hooked is raining, rainLevel: " << 1.0f + MISC::GET_RAIN_LEVEL() << " pointsModifier: " << pointsModifier + 1.0f + MISC::GET_RAIN_LEVEL();
					writeLog(text.str().c_str());
				}
				else if (!isRaining() && isRainingModifier)
				{
					isRainingModifier = false;
					writeLog("hooked is no longer raining");
				}

				if (isRainingModifier)
				{
					pointsModifier = pointsModifier + 1.0f + MISC::GET_RAIN_LEVEL();
				}

				// isSnowingModifier ***********************************************************************************************************

				bool isSnowingModifier;

				if (isSnowing() && !isSnowingModifier)
				{
					isSnowingModifier = true;

					stringstream text;
					text << "hooked is snowing, snowLevel: " << 1.5f + MISC::GET_SNOW_LEVEL() << " pointsModifier: " << pointsModifier + 1.5f + MISC::GET_SNOW_LEVEL();
					writeLog(text.str().c_str());
				}
				else if (!isSnowing() && isSnowingModifier)
				{
					isSnowingModifier = false;
					writeLog("hooked is no longer snowing");
				}

				if (isSnowingModifier)
				{
					pointsModifier = pointsModifier + 1.5f + MISC::GET_SNOW_LEVEL();
				}

				float pointsDifferences = (pointsModifier != 0.0f) ? getPlayerClothesPoint() - getTemperaturePointsNeeded() - pointsModifier : getPlayerClothesPoint() - getTemperaturePointsNeeded(); // stack value above if pointsModifier != 0.0f, else calculate clothes only

				int outfitModifier;

				if (pointsDifferences <= -2.0f) // simulate cold
					outfitModifier = 0;
				else if (pointsDifferences > -2.0f && pointsDifferences <= 4.0f) // simulate warm
					outfitModifier = 1;
				else if (pointsDifferences > 4.0f) // simulate hot
					outfitModifier = 2;

				(outfitModifier == 2) ? PED::SET_PED_RESET_FLAG(playerPed, 139, true) : PED::SET_PED_RESET_FLAG(playerPed, 139, false); // if outfit is hot, no stamina regen
				depletionMs = (outfitModifier == 1) ? GetPrivateProfileInt("TIMERS", "CORE_DEPLETION", 120000, ".\\SoftCores.ini") : (GetPrivateProfileInt("TIMERS", "CORE_DEPLETION", 120000, ".\\SoftCores.ini") / 2); // if outfit is not warm, core depletionMs is halved 
				
				bool knockedOut;

				temperatureMs = (outfitModifier == 2 || knockedOut) ? (GetPrivateProfileInt("TIMERS", "TEMPERATURE_PENALTY", 8000, ".\\SoftCores.ini") * 2) : GetPrivateProfileInt("TIMERS", "TEMPERATURE_PENALTY", 8000, ".\\SoftCores.ini"); // if outfit is hot/knockedOut previously, temperatureMs is doubled 

				if (isPlayerInControl()) // returns TRUE if player is in control
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

							if (getPlayerCore(Core::Health) <= 20 && !isPlayerCoreOverpowered(Core::Health) && !isPlayerPointOverpowered(Core::Health) && !knockedOut) // when reached this threshold, knock player out
							{
								TASK::TASK_KNOCKED_OUT(playerPed, 0.0f, false);
								knockedOut = true;
								text << "hooked cold effect on health less than 20%, knockedOut = true, next occurence after " << temperatureMs << " ms pointsDifferences: " << pointsDifferences << " raw: " << getPlayerClothesPoint() - getTemperaturePointsNeeded();
								writeLog(text.str().c_str());
							}
							else
							{
								text << "hooked cold effect, next occurence after " << temperatureMs << " ms pointsDifferences: " << pointsDifferences << " raw: " << getPlayerClothesPoint() - getTemperaturePointsNeeded();
								writeLog(text.str().c_str());
								knockedOut = false;
							}

							break;
						case 1: // warm
							break;
						case 2: // hot
							if (temperatureCoreFx) GRAPHICS::ANIMPOSTFX_PLAY("PlayerHonorLevelGood"); // reddish tint, seems suitable enough to show player is hot
							text << "hooked hot effect, next occurence after " << temperatureMs << " ms pointsDifferences: " << pointsDifferences << " raw: " << getPlayerClothesPoint() - getTemperaturePointsNeeded();
							writeLog(text.str().c_str());
							break;
						default:
							break;
						}

						pointsModifier = 0.0f; // reset pointsModifier to recalculate above

						temperatureTimer = getGameTimer() + temperatureMs; // if knocked out previously, the next timer will be longer to allow recovery period
					}
				}
			}
			// END TEMPERATURE CORE +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			// START AIMING PENALTY +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (aimPenalty)
			{
				Hash aimedWeapon;
				int stAimingPenalty = GetPrivateProfileInt("CORE_MODIFIER", "STAMINA_AIMING_PENALTY", 4, ".\\SoftCores.ini") * getMaxPlayerPoint(Core::Stamina) / 100;
				int deAimingPenalty = GetPrivateProfileInt("CORE_MODIFIER", "DEADEYE_AIMING_PENALTY", 8, ".\\SoftCores.ini") * getMaxPlayerPoint(Core::DeadEye) / 100;

				if (getGameTimer() > aimTimer)
				{
					stAimingPenalty = (isPlayerPointOverpowered(Core::Stamina)) ? 0 : stAimingPenalty;
					deAimingPenalty = (isPlayerPointOverpowered(Core::DeadEye)) ? 0 : deAimingPenalty;

					if (PLAYER::IS_PLAYER_FREE_AIMING(playerID) && WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &aimedWeapon, false, 0, true))
					{
						if (!isWeaponUnique(aimedWeapon) && !isWeaponThrowableOnly(aimedWeapon)) // if not unique and not throwable
						{
							PED::SET_PED_RESET_FLAG(playerPed, 139, true); // disable stamina outer core regen

							(getPlayerPoint(Core::Stamina) - stAimingPenalty > 0) ? setPlayerPoint(Core::Stamina, getPlayerPoint(Core::Stamina) - stAimingPenalty) : setPlayerPoint(Core::Stamina, -1); // drain outer core stamina until empty
							(getPlayerPoint(Core::DeadEye) - deAimingPenalty > 0) ? setPlayerPoint(Core::DeadEye, getPlayerPoint(Core::DeadEye) - deAimingPenalty) : setPlayerPoint(Core::DeadEye, -1); // drain outer core deadeye until empty

							if (getPlayerPoint(Core::Stamina) <= 0 && !isPlayerCoreOverpowered(Core::Stamina)) // if stamina outer core is empty and main core is not overpowered
							{
								(getPlayerCore(Core::Stamina) > 0) ? setPlayerCore(Core::Stamina, getPlayerCore(Core::Stamina) - 1) : setPlayerCore(Core::Stamina, 0); // if main stamina core more than 0, drain until equals to 0
							}

							if (getPlayerPoint(Core::DeadEye) <= 0 && !isPlayerCoreOverpowered(Core::DeadEye))
							{
								(getPlayerCore(Core::DeadEye) > 0) ? setPlayerCore(Core::DeadEye, getPlayerCore(Core::DeadEye) - 1) : setPlayerCore(Core::DeadEye, 0); // if main deadeye core more than 0, drain until equals to 0
							}
						}
					}
					else
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

					stringstream text;
					text << "hooked main core drain effect, next core drain occurence after " << depletionMs << " ms playerHpDrain: " << playerHpDrain << " playerStDrain: " << playerStDrain << " playerDeDrain: " << playerDeDrain;
					writeLog(text.str().c_str());

					if (temperatureCore)
					{
						float temperatureModifier;
						float pointsDifferences = getPlayerClothesPoint() - getTemperaturePointsNeeded();
						int outfitModifier;

						if (pointsDifferences <= -2.0f) // simulate cold
							outfitModifier = 0;
						else if (pointsDifferences > -2.0f && pointsDifferences <= 4.0f) // simulate warm
							outfitModifier = 1;
						else if (pointsDifferences > 4.0f) // simulate hot
							outfitModifier = 2;

						stringstream text;

						switch (outfitModifier)
						{
						case 0: // cold
							playerHealthRegen = 0.0f; // no hp regen at all
							temperatureModifier = 1.0f + fabsf(pointsDifferences);
							playerHpDrain = bCeil(playerHpDrain * temperatureModifier);
							text << "hooked cold effect, next core drain occurence after " << depletionMs << " ms playerHpDrain: " << playerHpDrain << " playerStDrain: " << playerStDrain << " playerDeDrain: " << playerDeDrain;
							writeLog(text.str().c_str());
							break;
						case 1: // warm, all core drain slower by 10 %
							playerHealthRegen = (isPlayerCoreOverpowered(Core::Health)) ? 1.0f : playerHealthRegen;
							temperatureModifier = 0.9f;
							playerHpDrain = bFloor(playerHpDrain * temperatureModifier);
							playerStDrain = bFloor(playerStDrain * temperatureModifier);
							playerDeDrain = bFloor(playerDeDrain * temperatureModifier);
							text << "hooked warm effect, next core drain occurence after " << depletionMs << " ms playerHpDrain: " << playerHpDrain << " playerStDrain: " << playerStDrain << " playerDeDrain: " << playerDeDrain;
							writeLog(text.str().c_str());
							break;
						case 2: // hot
							playerHealthRegen = playerHealthRegen * 0.2f; // slowed health outer core regen by 80%
							temperatureModifier = 0.5f + pointsDifferences;
							playerStDrain = bCeil(playerStDrain * temperatureModifier);
							playerDeDrain = bCeil(playerDeDrain * temperatureModifier);
							text << "hooked hot effect, next core drain occurence after " << depletionMs << " ms playerHpDrain: " << playerHpDrain << " playerStDrain: " << playerStDrain << " playerDeDrain: " << playerDeDrain;
							writeLog(text.str().c_str());
							break;
						default:
							break;
						}
					}
					else
					{
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
			if (penaltyOnDeath)
			{
				bool isInMission;
				bool coresPenalty;
				bool deathPenalty;

				if (!isPlayerInMission() && isInMission)
				{
					writeLog("hooked player is not in mission, penalty on death enabled");
				}

				if (isPlayerInMission() && !isInMission)
				{
					writeLog("hooked player is in mission, penalty on death disabled");
					isInMission = true;
				}
				else if (!isPlayerInMission())
				{
					isInMission = false;
				}

				if (!isInMission)
				{
					if (!isPlayerMoving() && isPlayerJustDied() && !isPlayerPlaying() && !coresPenalty)
					{
						coresPenalty = true;
						writeLog("coresPenalty = true");
					}
				}

				if (coresPenalty && !deathPenalty)
				{
					writeLog("hooked applying death penalty to player weapons, ammo and money");

					if (loseHandWeapon) // lose weapon on hand
					{
						Hash primaryHand;
						Hash secondaryHand;

						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &primaryHand, false, 0, true)) // WEAPON_ATTACH_POINT_HAND_PRIMARY
						{
							if (!isWeaponUnique(primaryHand) && !isWeaponGroupMelee(primaryHand)) // if not, melee weapon that can be thrown will also be removed
							{
								stringstream text;
								text << "WEAPON_ATTACH_POINT_HAND_PRIMARY: " << WEAPON::_GET_WEAPON_NAME(primaryHand) << " ammo: " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, primaryHand) << " removed";
								writeLog(text.str().c_str());
								WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, primaryHand), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, primaryHand), 0x2188E0A3);
								WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, primaryHand, true, NULL);
							}
						}

						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &secondaryHand, false, 1, true)) // WEAPON_ATTACH_POINT_HAND_SECONDARY 
						{
							if (!isWeaponUnique(secondaryHand) && !isWeaponGroupMelee(secondaryHand))
							{
								stringstream text;
								text << "WEAPON_ATTACH_POINT_HAND_SECONDARY: " << WEAPON::_GET_WEAPON_NAME(secondaryHand) << " ammo: " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, secondaryHand) << " removed";
								writeLog(text.str().c_str());
								WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, secondaryHand), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, primaryHand), 0x2188E0A3);
								WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, secondaryHand, true, NULL);
							}
						}
					}

					if (loseBodyWeapon) // lose weapon on body
					{
						Hash pistolR;
						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &pistolR, false, 2, true)) // WEAPON_ATTACH_POINT_PISTOL_R
						{
							if (!isWeaponUnique(pistolR) && !isWeaponGroupMelee(pistolR))
							{
								stringstream text;
								text << "WEAPON_ATTACH_POINT_PISTOL_R: " << WEAPON::_GET_WEAPON_NAME(pistolR) << " ammo: " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, pistolR) << " removed";
								writeLog(text.str().c_str());
								WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, pistolR), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, pistolR), 0x2188E0A3);
								WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, pistolR, true, NULL);
							}
						}

						Hash pistolL;
						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &pistolL, false, 3, true)) // WEAPON_ATTACH_POINT_PISTOL_L
						{
							if(!isWeaponUnique(pistolL) && !isWeaponGroupMelee(pistolL))
							{
								stringstream text;
								text << "WEAPON_ATTACH_POINT_PISTOL_L: " << WEAPON::_GET_WEAPON_NAME(pistolL) << " ammo: " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, pistolL) << " removed";
								writeLog(text.str().c_str());
								WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, pistolL), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, pistolL), 0x2188E0A3);
								WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, pistolL, true, NULL);
							}
						}

						Hash rifle;
						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &rifle, false, 9, true)) // WEAPON_ATTACH_POINT_RIFLE
						{
							if (!isWeaponUnique(rifle) && !isWeaponGroupMelee(rifle))
							{
								stringstream text;
								text << "WEAPON_ATTACH_POINT_RIFLE: " << WEAPON::_GET_WEAPON_NAME(rifle) << " ammo: " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, rifle) << " removed";
								writeLog(text.str().c_str());
								WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, rifle), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, rifle), 0x2188E0A3);
								WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, rifle, true, NULL);
							}
						}

						Hash rifleAlt;
						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &rifleAlt, false, 10, true)) // WEAPON_ATTACH_POINT_RIFLE_ALTERNATE
						{
							if (!isWeaponUnique(rifleAlt) && !isWeaponGroupMelee(rifleAlt))
							{
								stringstream text;
								text << "WEAPON_ATTACH_POINT_RIFLE_ALTERNATE: " << WEAPON::_GET_WEAPON_NAME(rifleAlt) << " ammo: " << WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, rifleAlt) << " removed";
								writeLog(text.str().c_str());
								WEAPON::_REMOVE_AMMO_FROM_PED_BY_TYPE(playerPed, WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, rifleAlt), WEAPON::GET_AMMO_IN_PED_WEAPON(playerPed, rifleAlt), 0x2188E0A3);
								WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, rifleAlt, true, NULL);
							}
						}

						Hash throwable;
						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &throwable, false, 6, true)) // WEAPON_ATTACH_POINT_THROWER
						{
							if (!isWeaponUnique(throwable) && !isWeaponGroupMelee(throwable) && isWeaponThrowableOnly(throwable)) // if not, melee weapon that can be thrown such as hewit will also be removed
							{
								WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, throwable, true, NULL);
								stringstream text;
								text << "WEAPON_ATTACH_POINT_THROWER: " << WEAPON::_GET_WEAPON_NAME(throwable) << " removed";
								writeLog(text.str().c_str());
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
				}

				if (isPlayerMoving() && !isPlayerJustDied() && isPlayerPlaying() && coresPenalty && deathPenalty)
				{
					coresPenalty = false;
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

			PED::SET_PED_CONFIG_FLAG(playerPed, 263, false); // disable playerPed headshot immunity

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
