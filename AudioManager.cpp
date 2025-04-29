#include "AudioManager.h"

unordered_map<string , Mix_Chunk*> AudioManager::sounds;

void AudioManager::Init() {
        sounds["background"] = Mix_LoadWAV("Sound//background.wav"); //0
        sounds["fireboss"] = Mix_LoadWAV("Sound//bossSound.wav"); //1
        sounds["shoot"] = Mix_LoadWAV("Sound//fireSound.wav"); //3
        sounds["explosion"] = Mix_LoadWAV("Sound//explosionSound.wav"); //4
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
