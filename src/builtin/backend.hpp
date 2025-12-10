#include <map>
#include <string>

#include <SDL2/SDL.h>

#include "logger.hpp"

namespace Soundhouse::Sounds::Backends
{
    /**
     * @brief Interface for audio backends
     * 
     */
    class IAudioBackend
    {
        public:
            virtual ~IAudioBackend() = default;

            virtual int  load_sound(const std::string &path) = 0;
            virtual void unload_sound(int id)                = 0;

            virtual void play(int id) = 0;
            virtual void stop(int id) = 0;

            virtual void set_volume(int id, float volume) = 0;

        protected:
            Logging::Logger logger;

            explicit IAudioBackend(const char *backendName);
    };

    /**
     * @brief Struct that carries output and the file's data
     * 
     */
    struct SoundData
    {
        SDL_AudioSpec spec;
        Uint8* buffer = nullptr;
        Uint32 length = 0;
        SDL_AudioDeviceID device = 0;
    };

    /**
     * @brief Support for SDL2 backend
     * 
     */
    class SDL2Backend : public Soundhouse::Sounds::Backends::IAudioBackend
    {
        public:
            SDL2Backend(const char *name = "SDL2Backend");
            ~SDL2Backend() override;

            int  load_sound(const std::string &path) override;
            void unload_sound(int id) override;

            void play(int id) override;
            void stop(int id) override;

            void set_volume(int id, float volume) override;

        private:
            std::map<int, SoundData> m_sounds;
            int                      m_nextId = 0;
    };
}; // namespace Soundhouse::Sounds::Backends