#include "move.h"
#include <stdexcept>
#include <set>

namespace cube {

Move GetMoveFromString(const std::string &move_string) {
  size_t start_pos = 0;
  size_t end_pos = 0;
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
  if (rotation_string == "" || rotation_string == "1") {
    rotation = 1;
  } else if (rotation_string == "'" || rotation_string == "3") {
    rotation = 3;
  } else if (rotation_string == "2") {
    rotation = 2;
  }
  return GetMove(rotation, face);
}

std::vector<Move> GetMovesFromString(const std::string &moves_string) {
  const std::set<char> face_chars = {'U', 'D', 'F', 'B', 'L', 'R'};
  std::vector<Move> result;
  size_t i = 0;
  while (i < moves_string.size()) {
    std::string word;
    word.push_back(moves_string[i]);
    if (i + 1 < moves_string.size() && !face_chars.count(moves_string[i + 1])) {
      word.push_back(moves_string[i + 1]);
    }
    result.push_back(GetMoveFromString(word));
    i += word.size();
  }
  return result;
}

std::string MoveToString(Move move) {
  std::string result;
  switch (GetFace(move))
  {
  case Face::U:
    result += "U";
    break;
  case Face::D:
    result += "D";
    break;
  case Face::F:
    result += "F";
    break;
  case Face::B:
    result += "B";
    break;
  case Face::L:
    result += "L";
    break;
  case Face::R:
    result += "R";
    break;
  default:
    throw std::logic_error("Unsupported face type");
  }
  switch (getRotation(move))
  {
  case 1:
    break;
  case 2:
    result += "2";
    break;
  case 3:
    result += "'";
    break;
  default:
    throw std::logic_error("Unsupported rotation type = " + std::to_string(getRotation(move)));
  }
  return result;
}

std::string MovesToString(const std::vector<Move>& moves) {
  std::string result;
  for (auto move: moves) {
    result += MoveToString(move);
  }
  return result;
}

} // namespace cube