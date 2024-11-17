#pragma once

#include "../cube/cube.h"

namespace presentation {

std::string FaceToColor(uint8_t face);

void PrintCube(const cube::CubeLayout &cube_layout);

} // namespace presentation