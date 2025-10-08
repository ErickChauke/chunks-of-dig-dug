#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <raylib-cpp.hpp>
#include <map>
#include <string>
#include <iostream>

/**
 * @file SoundManager.h
 * @brief Audio system for sound effects management
 */

/**
 * @enum SoundEffect
 * @brief Identifiers for game sound effects
 */
enum class SoundEffect {
    DIG,             ///< Digging through earth
    HARPOON_FIRE,    ///< Firing harpoon
    ENEMY_HIT,       ///< Enemy destroyed
    PLAYER_HIT,      ///< Player takes damage
    POWERUP_COLLECT, ///< Power-up collected
    ROCK_LAND,       ///< Rock hits ground
    LEVEL_COMPLETE   ///< Level finished
};

/**
 * @class SoundManager
 * @brief Manages audio loading, playback, and volume control
 * 
 * SoundManager provides centralized audio control:
 * - Load sound files from disk
 * - Play sounds by effect identifier
 * - Global volume control
 * - Enable/disable audio system
 * 
 * Sound file locations (if present):
 * - resources/sounds/dig.wav
 * - resources/sounds/shoot.wav
 * - resources/sounds/hit.wav
 * - resources/sounds/death.wav
 * - resources/sounds/powerup.wav
 * - resources/sounds/impact.wav
 * - resources/sounds/victory.wav
 * 
 * Usage pattern:
 * 1. Create: SoundManager soundMgr;
 * 2. Load: soundMgr.loadDefaultSounds();
 * 3. Play: soundMgr.playSound(SoundEffect::DIG);
 * 4. Control: soundMgr.setVolume(0.5f);
 * 
 * Design features:
 * - Graceful degradation (missing files don't crash)
 * - Map-based lookup (fast sound access)
 * - Volume control affects all sounds
 * - Enable/disable toggle for mute
 * 
 * Implementation note:
 * - Currently a framework (sounds not included)
 * - All loading attempts logged to console
 * - Missing files reported but not errors
 * 
 * @note Initializes raylib audio device in constructor
 */
class SoundManager {
private:
    std::map<SoundEffect, Sound> sounds;
    float volume;
    bool enabled;

public:
    SoundManager() : volume(0.5f), enabled(true) {
        InitAudioDevice();
    }
    
    ~SoundManager() {
        for (auto& pair : sounds) {
            UnloadSound(pair.second);
        }
        CloseAudioDevice();
    }
    
    /**
     * @brief Load sound file from disk
     * @param effect Sound effect identifier
     * @param filepath Path to audio file
     * @return true if loaded successfully
     */
    bool loadSound(SoundEffect effect, const std::string& filepath) {
        if (FileExists(filepath.c_str())) {
            Sound sound = LoadSound(filepath.c_str());
            SetSoundVolume(sound, volume);
            sounds[effect] = sound;
            return true;
        }
        return false;
    }
    
    /**
     * @brief Play sound effect
     * @param effect Sound to play
     * @note Does nothing if sound not loaded or disabled
     */
    void playSound(SoundEffect effect) {
        if (!enabled) return;
        
        auto it = sounds.find(effect);
        if (it != sounds.end()) {
            PlaySound(it->second);
        }
    }
    
    /**
     * @brief Set global volume level
     * @param vol Volume (0.0-1.0)
     */
    void setVolume(float vol) {
        volume = vol;
        for (auto& pair : sounds) {
            SetSoundVolume(pair.second, volume);
        }
    }
    
    /**
     * @brief Enable or disable audio
     * @param enable true to enable audio
     */
    void setEnabled(bool enable) {
        enabled = enable;
    }
    
    bool isEnabled() const {
        return enabled;
    }
    
    /**
     * @brief Load all default game sounds
     * @note Reports which sounds loaded successfully
     */
    void loadDefaultSounds() {
        std::cout << "Loading sounds..." << std::endl;
        
        loadSound(SoundEffect::DIG, "resources/sounds/dig.wav");
        loadSound(SoundEffect::HARPOON_FIRE, "resources/sounds/shoot.wav");
        loadSound(SoundEffect::ENEMY_HIT, "resources/sounds/hit.wav");
        loadSound(SoundEffect::PLAYER_HIT, "resources/sounds/death.wav");
        loadSound(SoundEffect::POWERUP_COLLECT, "resources/sounds/powerup.wav");
        loadSound(SoundEffect::ROCK_LAND, "resources/sounds/impact.wav");
        loadSound(SoundEffect::LEVEL_COMPLETE, "resources/sounds/victory.wav");
        
        std::cout << "Sound system initialized (" << sounds.size() 
                  << " sounds loaded)" << std::endl;
    }
};

#endif // SOUNDMANAGER_H
