#pragma once

#include <unordered_map>

#define DIFFICULTY_EASY		("setting.difficulty.easy")
#define DIFFICULTY_MEDIUM	("setting.difficulty.medium")
#define DIFFICULTY_HARD		("setting.difficulty.hard")

#define SETTING_PLAYER_SPEED_MIN ("setting.player.speed.min")
#define SETTING_PLAYER_SPEED_MAX ("setting.player.speed.max")

#define SETTING_PLAYER_FUEL_CONSUMPTION_IDLE			("setting.player.fuel_consumption.idle")
#define SETTING_PLAYER_FUEL_CONSUMPTION_ACCELERATION	("setting.player.fuel_consumption.acceleration")

#define SETTING_PLATFORM_NORMAL_SPAWN_COUNT_MIN	("setting.platform.normal.spawn_count.min")
#define SETTING_PLATFORM_NORMAL_SPAWN_COUNT_MAX	("setting.platform.normal.spawn_count.max")

#define SETTING_PLATFORM_BAD_SPAWN_WEIGHT_END_GAME	("setting.platform.bad.spawn_weight.end_game")
#define SETTING_PLATFORM_BAD_SPAWN_WEIGHT_OVERDRIVE ("setting.platform.bad.spawn_weight.overdrive")
#define SETTING_PLATFORM_BAD_SPAWN_WEIGHT_LOSE_FUEL ("setting.platform.bad.spawn_weight.lose_fuel")
#define SETTING_PLATFORM_BAD_SPAWN_WEIGHT_NONE		("setting.platform.bad.spawn_weight.none")

#define SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_INVULNERABLE	("setting.platform.good.spawn_weight.invulnerable")
#define SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_GET_FUEL		("setting.platform.good.spawn_weight.get_fuel")
#define SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_NONE			("setting.platform.good.spawn_weight.none")

#define SETTING_OBSTACLE_SPAWN_WEIGHT_JUMP	("setting.obstacle.spawn_weight.jump")
#define SETTING_OBSTACLE_SPAWN_WEIGHT_DUCK	("setting.obstacle.spawn_weight.duck")
#define SETTING_OBSTACLE_SPAWN_WEIGHT_NONE	("setting.obstacle.spawn_weight.none")

class SettingsStore {

private:
	static SettingsStore* instance; // singleton

	std::unordered_map<std::string, std::unordered_map<std::string, bool>> boolSettings;
	std::unordered_map<std::string, std::unordered_map<std::string, int>> intSettings;
	std::unordered_map<std::string, std::unordered_map<std::string, float>> floatSettings;

	SettingsStore() {

		// PLAYER
		AddSetting(SETTING_PLAYER_SPEED_MIN, DIFFICULTY_EASY, 20.f);
		AddSetting(SETTING_PLAYER_SPEED_MIN, DIFFICULTY_MEDIUM, 30.f);
		AddSetting(SETTING_PLAYER_SPEED_MIN, DIFFICULTY_HARD, 40.f);

		AddSetting(SETTING_PLAYER_SPEED_MAX, DIFFICULTY_EASY, 40.f);
		AddSetting(SETTING_PLAYER_SPEED_MAX, DIFFICULTY_MEDIUM, 60.f);
		AddSetting(SETTING_PLAYER_SPEED_MAX, DIFFICULTY_HARD, 80.f);

		AddSetting(SETTING_PLAYER_FUEL_CONSUMPTION_IDLE, DIFFICULTY_EASY, 0.5f);
		AddSetting(SETTING_PLAYER_FUEL_CONSUMPTION_IDLE, DIFFICULTY_MEDIUM, 1.25f);
		AddSetting(SETTING_PLAYER_FUEL_CONSUMPTION_IDLE, DIFFICULTY_HARD, 2.5f);

		AddSetting(SETTING_PLAYER_FUEL_CONSUMPTION_ACCELERATION, DIFFICULTY_EASY, 5.f);
		AddSetting(SETTING_PLAYER_FUEL_CONSUMPTION_ACCELERATION, DIFFICULTY_MEDIUM, 12.5f);
		AddSetting(SETTING_PLAYER_FUEL_CONSUMPTION_ACCELERATION, DIFFICULTY_HARD, 25.f);

		// NORMAL PLATFORM
		AddSetting(SETTING_PLATFORM_NORMAL_SPAWN_COUNT_MIN, DIFFICULTY_EASY, 2);
		AddSetting(SETTING_PLATFORM_NORMAL_SPAWN_COUNT_MIN, DIFFICULTY_MEDIUM, 2);
		AddSetting(SETTING_PLATFORM_NORMAL_SPAWN_COUNT_MIN, DIFFICULTY_HARD, 1);

		AddSetting(SETTING_PLATFORM_NORMAL_SPAWN_COUNT_MAX, DIFFICULTY_EASY, 4);
		AddSetting(SETTING_PLATFORM_NORMAL_SPAWN_COUNT_MAX, DIFFICULTY_MEDIUM, 3);
		AddSetting(SETTING_PLATFORM_NORMAL_SPAWN_COUNT_MAX, DIFFICULTY_HARD, 2);

		// BAD PLATFORM
		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_END_GAME, DIFFICULTY_EASY, 1.f);
		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_END_GAME, DIFFICULTY_MEDIUM, 1.f);
		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_END_GAME, DIFFICULTY_HARD, 2.f);

		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_OVERDRIVE, DIFFICULTY_EASY, 2.f);
		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_OVERDRIVE, DIFFICULTY_MEDIUM, 2.f);
		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_OVERDRIVE, DIFFICULTY_HARD, 3.f);

		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_LOSE_FUEL, DIFFICULTY_EASY, 2.f);
		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_LOSE_FUEL, DIFFICULTY_MEDIUM, 3.f);
		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_LOSE_FUEL, DIFFICULTY_HARD, 4.f);

		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_NONE, DIFFICULTY_EASY, 8.f);
		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_NONE, DIFFICULTY_MEDIUM, 6.f);
		AddSetting(SETTING_PLATFORM_BAD_SPAWN_WEIGHT_NONE, DIFFICULTY_HARD, 4.f);

		// GOOD PLATFORM
		AddSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_INVULNERABLE, DIFFICULTY_EASY, 0.f);
		AddSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_INVULNERABLE, DIFFICULTY_MEDIUM, 1.f);
		AddSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_INVULNERABLE, DIFFICULTY_HARD, 1.f);

		AddSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_GET_FUEL, DIFFICULTY_EASY, 1.f);
		AddSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_GET_FUEL, DIFFICULTY_MEDIUM, 2.f);
		AddSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_GET_FUEL, DIFFICULTY_HARD, 3.f);

		AddSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_NONE, DIFFICULTY_EASY, 8.f);
		AddSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_NONE, DIFFICULTY_MEDIUM, 8.f);
		AddSetting(SETTING_PLATFORM_GOOD_SPAWN_WEIGHT_NONE, DIFFICULTY_HARD, 8.f);

		// OBSTACLES
		AddSetting(SETTING_OBSTACLE_SPAWN_WEIGHT_JUMP, DIFFICULTY_EASY, 0.f);
		AddSetting(SETTING_OBSTACLE_SPAWN_WEIGHT_JUMP, DIFFICULTY_MEDIUM, 1.f);
		AddSetting(SETTING_OBSTACLE_SPAWN_WEIGHT_JUMP, DIFFICULTY_HARD, 2.f);

		AddSetting(SETTING_OBSTACLE_SPAWN_WEIGHT_DUCK, DIFFICULTY_EASY, 0.f);
		AddSetting(SETTING_OBSTACLE_SPAWN_WEIGHT_DUCK, DIFFICULTY_MEDIUM, 1.f);
		AddSetting(SETTING_OBSTACLE_SPAWN_WEIGHT_DUCK, DIFFICULTY_HARD, 2.f);

		AddSetting(SETTING_OBSTACLE_SPAWN_WEIGHT_NONE, DIFFICULTY_EASY, 1.f);
		AddSetting(SETTING_OBSTACLE_SPAWN_WEIGHT_NONE, DIFFICULTY_MEDIUM, 1.f);
		AddSetting(SETTING_OBSTACLE_SPAWN_WEIGHT_NONE, DIFFICULTY_HARD, 1.f);

	}

	~SettingsStore() {}

public:
	static SettingsStore* GetInstance() {
		if (!instance)
			instance = new SettingsStore();

		return instance;
	}

	void AddSetting(std::string name, std::string difficulty, bool value) {
		boolSettings[name][difficulty] = value;
	}

	void AddSetting(std::string name, std::string difficulty, int value) {
		intSettings[name][difficulty] = value;
	}

	void AddSetting(std::string name, std::string difficulty, float value) {
		floatSettings[name][difficulty] = value;
	}

	bool GetBoolSetting(std::string name, std::string difficulty) {
		return boolSettings.at(name).at(difficulty);
	}

	int GetIntSetting(std::string name, std::string difficulty) {
		return intSettings.at(name).at(difficulty);
	}

	float GetFloatSetting(std::string name, std::string difficulty) {
		return floatSettings.at(name).at(difficulty);
	}
};