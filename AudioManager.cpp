#include "AudioManager.h"

unordered_map<string , Mix_Chunk*> AudioManager::sounds;

void AudioManager::Init() {
        sounds["background"] = Mix_LoadWAV("Sound//background.wav"); //0
        if (sounds["background"] == nullptr) {
            cout << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
        }
        sounds["fireboss"] = Mix_LoadWAV("Sound//bossSound.wav"); //1
        if (sounds["fireboss"] == nullptr) {
            cout << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
        }
        sounds["shoot"] = Mix_LoadWAV("Sound//fireSound.wav"); //3
        if (sounds["shoot"] == nullptr) {
            cout << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
        }
        sounds["explosion"] = Mix_LoadWAV("Sound//explosionSound.wav"); //4
        if (sounds["explosion"] == nullptr) {
            cout << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
        }
}

void AudioManager::CleanUp(){
            for (auto& pair : sounds) {
            Mix_FreeChunk(pair.second);
        }
        sounds.clear();
}

void AudioManager::PlaySound(int channel, string id, int loop){
    Mix_PlayChannel(channel, sounds[id], loop);
    if (Mix_PlayChannel(channel, sounds[id], loop) == -1) {
        cout << "Failed to play sound! SDL_mixer Error: " << Mix_GetError() << endl;
    }
}
