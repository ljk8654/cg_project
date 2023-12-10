#pragma once
#include "std.h"
#include "fmod.hpp"

enum class BGMSound { Normal = 0, _count };

enum class SoundChannel { Bgm = 0, Fire, Effect, Walk, _count };

// has Channel
constexpr GLuint NUM_SOUND_BGM = static_cast<GLuint>(BGMSound::_count);
constexpr GLuint NUM_SOUND_CHEENEL = static_cast<GLuint>(SoundChannel::_count);;

// Stop은 Play후에 사용할 것
class SoundManager {
private:
    typedef struct Sound {
        glm::vec3 position{};
        GLfloat volume = 0.0f;
    }Sound;

    FMOD_SYSTEM* soundSystem = nullptr;
    FMOD_SOUND* bgmSounds[NUM_SOUND_BGM]{};
    FMOD_CHANNEL* soundChannel[NUM_SOUND_CHEENEL]{};

public:
    SoundManager();
    GLvoid PlayBGMSound(const BGMSound& bgmSound, const GLfloat& volume = 1.0f, GLboolean isNewChannel = GL_FALSE);

};