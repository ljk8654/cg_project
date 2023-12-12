#include "std.h"
#include "Sound.h"

#define EFFECT_MAX_DISTANCE 600

inline constexpr GLvoid CheckResult(const FMOD_RESULT& result)
{
    if (result != FMOD_OK)
    {
        assert(0);
    }
}
SoundManager::SoundManager()
{
    FMOD_System_Create(&soundSystem, FMOD_VERSION);
    FMOD_System_Init(soundSystem, 32, FMOD_INIT_NORMAL, NULL);

    FMOD_RESULT result = FMOD_OK;

    // bgmSound
    result = FMOD_System_CreateSound(soundSystem, "Sounds\\bgm.mp3", FMOD_LOOP_NORMAL, 0, &bgmSounds[static_cast<GLint>(BGMSound::Normal)]);
    CheckResult(result);


    for (GLint i = 0; i < NUM_SOUND_CHEENEL; ++i)
    {
        soundChannel[i] = 0;
    }
}


GLvoid SoundManager::PlayBGMSound(const BGMSound& bgmSound, const GLfloat& volume, GLboolean isNewChannel)
{
    FMOD_System_Update(soundSystem);
    if (isNewChannel == GL_TRUE)
    {
        FMOD_System_PlaySound(soundSystem, bgmSounds[static_cast<GLint>(bgmSound)], 0, GL_FALSE, &soundChannel[static_cast<GLint>(SoundChannel::Bgm)]);
        FMOD_Channel_SetVolume(soundChannel[static_cast<GLint>(SoundChannel::Bgm)], volume);
    }
}