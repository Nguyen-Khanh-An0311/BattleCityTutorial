#ifndef AUDIOMANAGER_H_INCLUDED
#define AUDIOMANAGER_H_INCLUDED

#include <SDL_mixer.h>
#include <bits/stdc++.h>
using namespace std;

class AudioManager {
public:
    void Init(){  // Load âm thanh
        //sounds["background"] = Mix_LoadWAV("assets/background.wav");
        sounds["fireboss"] = Mix_LoadWAV("Sound//bossSound.wav");
        sounds["shoot"] = Mix_LoadWAV("Sound//fireSound.wav");
        sounds["explosion"] = Mix_LoadWAV("Sound//explosionSound.wav");
    }

    void CleanUp(){
        for (auto& pair : sounds) {
            Mix_FreeChunk(pair.second);
        }
        sounds.clear();
    }      // Xoá âm thanh

    void AudioManager::PlaySound(string id, int loop) {
        Mix_PlayChannel(-1, sounds[id], loop);
    } // phát âm

    void StopSound(string id){}               // dừng âm
    static void SetVolume(string id, int volume);    // chỉnh volume

private:
    static unordered_map<string, Mix_Chunk*> sounds;
};


#endif // AUDIOMANAGER_H_INCLUDED
