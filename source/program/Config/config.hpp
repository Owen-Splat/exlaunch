#pragma once
#include "lib.hpp"
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

enum MovementMode {
    Standard,
    Extra,
    DPad
};

struct PatchConfig {
    bool initialized = false;

    struct {
        bool enabled;
    } debug_mode;

    struct {
        MovementMode movement;
    } control_scheme;

    struct {
        bool enabled;
        bool bombs;
        bool hookshot;
        bool rod;
        bool sword;
    } nice_items;

    struct {
        bool enabled;
    } blur_removal;

    struct {
        bool enabled;
    } speed_hack;

    struct {
        bool enabled;
    } randomizer_compatible;

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
            else if (current_section == "control_scheme") {
                parseControlScheme(key, value);
            }
            else if (current_section == "nice_items") {
                parseNiceItems(key, value);
            }
            else if (current_section == "blur_removal") {
                parseBlurRemoval(key, value);
            }
            else if (current_section == "speed_hack") {
                parseSpeedHack(key, value);
            }
            else if (current_section == "randomizer_compatible") {
                parseRandomizerCompatible(key, value);
            }
        }

        initialized = true;
    }

    void parseDebugMode(std::string key, std::string value) {
        if (key == "enabled" && value == "true") {
            debug_mode.enabled = true;
        }
    }

    void parseControlScheme(std::string key, std::string value) {
        Logging.Log(key + " | " + value);
        if (key == "movement") {
            if (value == "extra") {
                control_scheme.movement = MovementMode::Extra;
            }
            else if (value == "dpad") {
                control_scheme.movement = MovementMode::DPad;
            }
            else {
                control_scheme.movement = MovementMode::Standard;
            }
        }
    }

    void parseNiceItems(std::string key, std::string value) {
        if (key == "bombs" && value == "true") {
            nice_items.bombs = true;
            nice_items.enabled = true;
        }
        else if (key == "hookshot" && value == "true") {
            nice_items.hookshot = true;
            nice_items.enabled = true;
        }
        else if (key == "rod" && value == "true") {
            nice_items.rod = true;
            nice_items.enabled = true;
        }
        else if (key == "sword" && value == "true") {
            nice_items.sword = true;
            nice_items.enabled = true;
        }
    }

    void parseBlurRemoval(std::string key, std::string value) {
        if (key == "enabled" && value == "true") {
            blur_removal.enabled = true;
        }
    }

    void parseSpeedHack(std::string key, std::string value) {
        if (key == "enabled" && value == "true") {
            speed_hack.enabled = true;
        }
    }

    void parseRandomizerCompatible(std::string key, std::string value) {
        if (key == "enabled" && value == "true") {
            randomizer_compatible.enabled = true;
        }
    }
};

extern PatchConfig global_config;