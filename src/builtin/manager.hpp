#include <memory>
#include <optional>
#include <unordered_map>

#include "builtin.hpp"
#include "logger.hpp"
#include "sound.hpp"
#include "backend.hpp"

namespace Soundhouse::Sounds
{
    /**
     * @brief SoundManager class. Every sound effect passes through here--builtin or not
     *
     */
    class SoundManager
    {
        public:
            explicit SoundManager(std::unique_ptr<Backends::IAudioBackend> backend, std::optional<Logging::Logger> logger = std::nullopt);

            Sound load(const std::string &path);

            void unload(Sound sound);

            void play(Sound sound);
            void stop(Sound sound);

            void set_volume(Sound sound, float volume);

            Sound get_builtin(BuiltinSound which);

        private:
            Sound create_builtin_sound(int ID);

            void load_all_builtin_sounds();

        private:
            std::unique_ptr<Backends::IAudioBackend> backend;

            std::unordered_map<int, std::string>    sounds;
            std::unordered_map<BuiltinSound, Sound> builtinSounds;

            std::optional<Logging::Logger> logger;

            int nextID = 0;
    };
} // namespace Soundhouse::Sounds