#pragma once

#include "cube.h"

namespace cube {

class SimpleFlatCube : public Cube {
public:
  SimpleFlatCube(size_t n);

  bool IsSolved() const override;

  void Turn(Move move) override;

  const CubeLayout &GetCubeLayout() const override;

  size_t GetHash() const override;

  bool operator==(const SimpleFlatCube &other) const;

private:
  CubeLayout cube_;
};

} // namespace cube

namespace std {
template <> struct hash<cube::SimpleFlatCube> {
  size_t operator()(const cube::SimpleFlatCube &cube) const {
    return cube.GetHash();
  }
};
} // namespace std