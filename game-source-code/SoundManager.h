#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <raylib-cpp.hpp>
#include <map>
#include <string>
#include <iostream>

enum class SoundEffect {
    DIG,
    HARPOON_FIRE,
    ENEMY_HIT,
    PLAYER_HIT,
    POWERUP_COLLECT,
    ROCK_LAND,
    LEVEL_COMPLETE
};

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
    
    bool loadSound(SoundEffect effect, const std::string& filepath) {
        if (FileExists(filepath.c_str())) {
            Sound sound = LoadSound(filepath.c_str());
            SetSoundVolume(sound, volume);
            sounds[effect] = sound;
            return true;
        }
        return false;
    }
    
    void playSound(SoundEffect effect) {
        if (!enabled) return;
        
        auto it = sounds.find(effect);
        if (it != sounds.end()) {
            PlaySound(it->second);
        }
    }
    
    void setVolume(float vol) {
        volume = vol;
        for (auto& pair : sounds) {
            SetSoundVolume(pair.second, volume);
        }
    }
    
    void setEnabled(bool enable) {
        enabled = enable;
    }
    
    bool isEnabled() const {
        return enabled;
    }
    
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
