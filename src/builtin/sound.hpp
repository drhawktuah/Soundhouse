#pragma once

namespace Soundhouse::Sounds
{
    /**
     * @brief The sound class. Dedicated for encapsulating playback files for Soundhouse
     *
     */
    class Sound
    {
        public:
            Sound() : id(-1), builtin(false)
            {
            }

            explicit Sound(int id) : id(id), builtin(false)
            {
            }

            bool is_valid() const;
            bool is_builtin() const;
            int  get_id() const;

        protected:
            struct builtin_t
            {
            };

            Sound(builtin_t, int builtinID) : id(builtinID), builtin(true)
            {
            }

            friend class SoundManager;

        private:
            int  id;
            bool builtin;
    };
} // namespace Soundhouse::Sounds