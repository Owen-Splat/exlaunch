#pragma once
#include "lib.hpp"
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

struct PatchConfig {
    bool initialized = false;

    struct {
        bool enabled;
    } debug_mode;

    struct {
        bool full_direction;
        float speed;
    } movement;

    struct {
        bool bombs;
        bool hookshot;
        bool rod;
    } nice_items;

    struct {
        bool enabled;
    } blur_removal;

    struct {
        std::string mode;
    } damage;

    struct {
        bool enabled;
        bool free_book;
        std::string stealing;
        bool enemies;
        bool enemy_sizes;
    } randomizer;

    void parse(std::string config_str) {
        std::stringstream ss(config_str);
        std::string line;
        std::string current_section;

        while (std::getline(ss, line)) {
            // ignore comments
            if (line.starts_with(';') || line.starts_with('#')) {
                continue;
            }

            // read section
            if (line.starts_with('[') && line.length() > 2) {
                current_section = line.substr(1, line.length() - 3);
                Logging.Log(current_section);
            }

            // remove section if line is blank
            if (line.empty()) {
                current_section = "";
            }

            // ignore if there is no section
            if (current_section.empty()) {
                continue;
            }

            // get index of = within string, ignore if not found
            int pos = (int)line.find('=');
            if (pos < 0) {
                continue;
            }

            // get key:value in lowercase with whitespace removed
            std::string key = line.substr(0, pos);
            std::transform(key.begin(), key.end(), key.begin(),
                [](unsigned char c){ return std::tolower(c); });
            key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());

            std::string value = line.substr(pos + 1);
            std::transform(value.begin(), value.end(), value.begin(),
                [](unsigned char c){ return std::tolower(c); });
            value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());

            // set config properties based on section and key:value
            if (current_section == "debug_mode") {
                parseDebugMode(key, value);
            }
            else if (current_section == "movement") {
                parseMovement(key, value);
            }
            else if (current_section == "nice_items") {
                parseNiceItems(key, value);
            }
            else if (current_section == "blur_removal") {
                parseBlurRemoval(key, value);
            }
            else if (current_section == "damage") {
                parseDamage(key, value);
            }
            else if (current_section == "randomizer") {
                parseRandomizer(key, value);
            }
        }

        initialized = true;
    }

    void parseDebugMode(std::string key, std::string value) {
        if (key == "enabled" && value == "true") {
            debug_mode.enabled = true;
        }
    }

    void parseMovement(std::string key, std::string value) {
        Logging.Log(key + " | " + value);
        if (key == "full_direction") {
            if (value == "true") {
                movement.full_direction = true;
            }
        }
        if (key == "speed") {
            movement.speed = std::stof(value);
        }
    }

    void parseNiceItems(std::string key, std::string value) {
        if (key == "bombs" && value == "true") {
            nice_items.bombs = true;
        }
        else if (key == "hookshot" && value == "true") {
            nice_items.hookshot = true;
        }
        else if (key == "rod" && value == "true") {
            nice_items.rod = true;
        }
    }

    void parseBlurRemoval(std::string key, std::string value) {
        if (key == "enabled" && value == "true") {
            blur_removal.enabled = true;
        }
    }

    void parseDamage(std::string key, std::string value) {
        if (key == "mode") {
            damage.mode = value;
        }
    }

    void parseRandomizer(std::string key, std::string value) {
        if (key == "enabled") {
            if (value == "true") {
                randomizer.enabled = true;
            }
            else {
                return;
            }
        }
        else if (key == "free_book" && value == "true") {
            randomizer.free_book = true;
        }
        else if (key == "stealing") {
            randomizer.stealing = value;
        }
        else if (key == "enemies" && value == "true") {
            randomizer.enemies = true;
        }
        else if (key == "enemy_sizes" && value == "true") {
            randomizer.enemy_sizes = true;
        }
    }
};

extern PatchConfig global_config;