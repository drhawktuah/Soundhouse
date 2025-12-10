#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <filesystem>
#include <stdexcept>

#include "backend.hpp"

namespace Soundhouse::Sounds::Backends
{
    IAudioBackend::IAudioBackend(const char *backendName) : logger(backendName, Logging::LoggerLevel::Info, Logging::LoggerTimeResolution::Milliseconds)
    {
    }

    SDL2Backend::SDL2Backend(const char *name) : Soundhouse::Sounds::Backends::IAudioBackend(name)
    {
        logger.info("Initializing SDL audio");

        if (SDL_Init(SDL_INIT_AUDIO) != 0)
        {
            logger.critical("Failed to initialize SDL2 %s", SDL_GetError());
            throw std::runtime_error("SDL_init failed");
        }
    }

    SDL2Backend::~SDL2Backend()
    {
        for (auto &[id, sd] : m_sounds)
        {
            SDL_CloseAudioDevice(sd.device);
            SDL_FreeWAV(sd.buffer);
        }

        SDL_Quit();
    }

    int SDL2Backend::load_sound(const std::string &path)
    {
        if (!std::filesystem::exists(path))
        {
            logger.warn("Sound file %s does not exist", path.c_str());
            return -1;
        }

        SoundData sd{};
        if (SDL_LoadWAV(path.c_str(), &sd.spec, &sd.buffer, &sd.length) == nullptr)
        {
            logger.error("Failed to load WAV %s: %s", path.c_str(), SDL_GetError());
            return -1;
        }

        SDL_AudioSpec obtained{};
        sd.device = SDL_OpenAudioDevice(nullptr, 0, &sd.spec, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);
        if (sd.device == 0)
        {
            logger.error("Failed to open audio device: %s", SDL_GetError());
            SDL_FreeWAV(sd.buffer);
            return -1;
        }

        SDL_PauseAudioDevice(sd.device, 0);

        int id       = m_nextId++;
        m_sounds[id] = sd;
        logger.info("Loaded sound: %s (freq=%d, channels=%d, format=%d)", path.c_str(), sd.spec.freq, sd.spec.channels, sd.spec.format);
        return id;
    }

    void SDL2Backend::unload_sound(int id)
    {
        auto it = m_sounds.find(id);
        if (it != m_sounds.end())
        {
            SDL_ClearQueuedAudio(it->second.device);
            SDL_CloseAudioDevice(it->second.device);
            SDL_FreeWAV(it->second.buffer);
            m_sounds.erase(it);
            logger.info("Unloaded sound: %d", id);
        }
    }

    void SDL2Backend::play(int id)
    {
        auto it = m_sounds.find(id);
        if (it != m_sounds.end())
        {
            SDL_ClearQueuedAudio(it->second.device);
            SDL_QueueAudio(it->second.device, it->second.buffer, it->second.length);
            SDL_PauseAudioDevice(it->second.device, 0);
            logger.info("Playing sound: %d", id);
        }
    }

    void SDL2Backend::stop(int id)
    {
        auto it = m_sounds.find(id);
        if (it != m_sounds.end())
        {
            SDL_ClearQueuedAudio(it->second.device);
            SDL_PauseAudioDevice(it->second.device, 1); // pause device
            logger.info("Stopped sound: %d", id);
        }
    }

    void SDL2Backend::set_volume(int id, float volume)
    {
        logger.warn("Not implemented yet");
    }
} // namespace Soundhouse::Sounds::Backends