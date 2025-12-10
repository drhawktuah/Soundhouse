#include "sound.hpp"

namespace Soundhouse::Sounds
{
    bool Sound::is_valid() const
    {
        return id >= 0;
    }

    bool Sound::is_builtin() const
    {
        return builtin;
    }

    int Sound::get_id() const
    {
        return id;
    }
} // namespace Soundhouse::Sounds