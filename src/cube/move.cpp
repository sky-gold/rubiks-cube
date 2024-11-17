#include "move.h"
#include <stdexcept>

namespace cube {

Move GetMoveFromString(const std::string &move_string) {
  size_t start_pos = 0;
  size_t end_pos = 0;
  while (end_pos < move_string.size() && isdigit(move_string[end_pos])) {
    ++end_pos;
  }
  uint8_t move_index = 0;
  if (start_pos != end_pos) {
    move_index = std::stoi(move_string.substr(start_pos, end_pos));
  }
  start_pos = end_pos;
  while (end_pos < move_string.size() && isalpha(move_string[end_pos])) {
    ++end_pos;
  }
  const auto face_string = move_string.substr(start_pos, end_pos);
  uint8_t face;
  if (face_string == "U") {
    face = static_cast<uint8_t>(Face::U);
  } else if (face_string == "D") {
    face = static_cast<uint8_t>(Face::D);
  } else if (face_string == "F") {
    face = static_cast<uint8_t>(Face::F);
  } else if (face_string == "B") {
    face = static_cast<uint8_t>(Face::B);
  } else if (face_string == "L") {
    face = static_cast<uint8_t>(Face::L);
  } else if (face_string == "R") {
    face = static_cast<uint8_t>(Face::R);
  } else {
    throw std::logic_error("Unsupported face name = " + face_string);
  }
  start_pos = end_pos;
  end_pos = move_string.size() - 1;
  const auto rotation_string = move_string.substr(start_pos, end_pos);
  uint8_t rotation;
  if (rotation_string == "") {
    rotation = 1;
  } else if (rotation_string == "'") {
    rotation = 3;
  } else if (rotation_string == "2") {
    rotation = 2;
  }
  return GetMove(move_index, rotation, face);
}

std::vector<Move> GetMovesFromString(const std::string &moves_string) {
  std::vector<Move> result;
  std::string word;
  for (auto c : moves_string) {
    if (c == ' ') {
      if (!word.empty()) {
        result.push_back(GetMoveFromString(word));
      }
      word.clear();
    } else {
      word.push_back(c);
    }
  }
  if (!word.empty()) {
    result.push_back(GetMoveFromString(word));
  }
  return result;
}

} // namespace cube