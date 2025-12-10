#include "builtin/builtin.hpp"
#include "builtin/logger.hpp"
#include "builtin/sound.hpp"
#include "builtin/manager.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>

class TestingBackend : public Soundhouse::Sounds::Backends::IAudioBackend
{
    public:
        TestingBackend() : Soundhouse::Sounds::Backends::IAudioBackend("TestingBackend")
        {
        }

        int load_sound(const std::string &path) override
        {
            auto full = std::filesystem::absolute(path);

            logger.info("Checking sound path: %s", full.c_str());

            if (!std::filesystem::exists(path))
            {
                logger.warn("Builtin sound effect %s does not exist", path.c_str());
                return -1;
            }

            logger.info("Loaded sound: %s", path.c_str());
            return nextID++;
        }

        void unload_sound(int id) override
        {
            logger.info("Unloaded sound: %i", id);
        }

        void play(int id) override
        {
            logger.info("Playing sound: %i", id);
        }

        void stop(int id) override
        {
            logger.info("Stopped sound: %i", id);
        }

        void set_volume(int id, float volume) override
        {
            logger.info("Increased voume of sound %i to %f", id, volume);
        }

    private:
        int nextID = 0;
};

int main()
{
    auto backend = std::make_unique<Soundhouse::Sounds::Backends::SDL2Backend>("SDL2Backend");
    Soundhouse::Sounds::SoundManager manager(std::move(backend));

    auto fart = manager.get_builtin(Soundhouse::Sounds::BuiltinSound::Fart);

    manager.play(fart);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    manager.stop(fart);

    return 0;
}