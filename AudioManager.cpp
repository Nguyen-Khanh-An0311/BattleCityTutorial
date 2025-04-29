#include "AudioManager.h"

unordered_map<string , Mix_Chunk*> AudioManager::sounds;

void AudioManager::Init() {
        //sounds["background"] = Mix_LoadWAV("assets/background.wav");
        sounds["fireboss"] = Mix_LoadWAV("Sound//bossSound.wav");
        sounds["shoot"] = Mix_LoadWAV("Sound//fireSound.wav");
        sounds["explosion"] = Mix_LoadWAV("Sound//explosionSound.wav");
}

void AudioManager::CleanUp(){
            for (auto& pair : sounds) {
            Mix_FreeChunk(pair.second);
        }
        sounds.clear();
}

void AudioManager::PlaySound(int channel, string id, int loop){
    Mix_PlayChannel(channel, sounds[id], loop);
}
