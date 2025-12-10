#include <memory>
#include <utility>

#include "manager.hpp"
#include "builtin.hpp"
#include "sound.hpp"

namespace Soundhouse::Sounds
{
    SoundManager::SoundManager(std::unique_ptr<Backends::IAudioBackend> backend, std::optional<Logging::Logger> logger) : backend(std::move(backend)), logger(logger)
    {
        load_all_builtin_sounds();
    }

    Sound SoundManager::load(const std::string &path)
    {
        int backendID = backend->load_sound(path);
        int handleID  = nextID++;

        sounds[handleID] = path;

        return Sound(handleID);
    }

    void SoundManager::unload(Sound sound)
    {
        if (!sound.is_valid() || sound.is_builtin())
        {
            return;
        }

        int id = sound.get_id();

        auto iterator = sounds.find(id);
        if (iterator != sounds.end())
        {
            backend->unload_sound(id);
            sounds.erase(iterator);
        }
    }

    void SoundManager::play(Sound sound)
    {
        if (sound.is_valid())
        {
            backend->play(sound.get_id());
        }
    }

    void SoundManager::stop(Sound sound)
    {
        if (sound.is_valid())
        {
            backend->stop(sound.get_id());
        }
    }

    void SoundManager::set_volume(Sound sound, float volume)
    {
        if (sound.is_valid())
        {
            backend->set_volume(sound.get_id(), volume);
        }
    }

    Sound SoundManager::create_builtin_sound(int id)
    {
        return Sound(Sound::builtin_t{}, id);
    }

    void SoundManager::load_all_builtin_sounds()
    {
        builtinSounds[BuiltinSound::Fart]      = create_builtin_sound(backend->load_sound("assets/fart.wav"));
        builtinSounds[BuiltinSound::MenuClick] = create_builtin_sound(backend->load_sound("assets/menu_click.wav"));
        builtinSounds[BuiltinSound::MenuHover] = create_builtin_sound(backend->load_sound("assets/menu_hover.wav"));
        builtinSounds[BuiltinSound::ErrorBeep] = create_builtin_sound(backend->load_sound("assets/error_beep.wav"));
        builtinSounds[BuiltinSound::ClownHorn] = create_builtin_sound(backend->load_sound("assets/clown_horn.wav"));
    }

    Sound SoundManager::get_builtin(BuiltinSound which)
    {
        return builtinSounds[which];
    }
} // namespace Soundhouse::Sounds