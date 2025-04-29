#ifndef AUDIOMANAGER_H_INCLUDED
#define AUDIOMANAGER_H_INCLUDED

#include <SDL_mixer.h>
#include <bits/stdc++.h>
using namespace std;

class AudioManager {
public:
    static void Init();// Load âm thanh


    static void CleanUp();// Xoá âm thanh

    static void PlaySound(int channel, string id, int loop);// phát âm

    static void StopSound(string id){}               // dừng âm
    static void SetVolume(string id, int volume);    // chỉnh volume

//private:
    static unordered_map<string, Mix_Chunk*> sounds;
};


#endif // AUDIOMANAGER_H_INCLUDED
