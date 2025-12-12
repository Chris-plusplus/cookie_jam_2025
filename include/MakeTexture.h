#pragma once

#include <archimedes/gfx/Texture.h>

using namespace arch;

namespace vs {

// creates texture from path relative to current working directory
Ref<gfx::Texture> makeTexture(std::string_view filename);

}