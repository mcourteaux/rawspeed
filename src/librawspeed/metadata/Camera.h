/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2009-2014 Klaus Post

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include "rawspeedconfig.h"
#include "adt/NotARational.h"
#include "adt/Point.h"
#include "metadata/BlackArea.h"
#include "metadata/CameraSensorInfo.h"
#include "metadata/ColorFilterArray.h"
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#ifdef HAVE_PUGIXML

namespace pugi {
class xml_node;
} // namespace pugi

#endif

namespace rawspeed {

template <typename T> inline void hint_type_not_parsed(T& val);

class Hints final {
  std::map<std::string, std::string, std::less<>> data;

public:
  void add(const std::string& key, const std::string& value) {
    data.try_emplace(key, value);
  }

  [[nodiscard]] bool contains(const std::string& key) const {
    return data.contains(key);
  }

  template <typename T>
  [[nodiscard]] T get(const std::string& key, T defaultValue) const {
    if (auto hint = data.find(key);
        hint != data.end() && !hint->second.empty()) {
      if constexpr (std::is_same_v<unsigned long long, T>) {
        defaultValue = std::stoull(hint->second);
      } else if constexpr (std::is_same_v<long long, T>) {
        defaultValue = std::stoll(hint->second);
      } else if constexpr (std::is_same_v<unsigned long, T>) {
        defaultValue = std::stoul(hint->second);
      } else if constexpr (std::is_same_v<long, T>) {
        defaultValue = std::stol(hint->second);
      } else if constexpr (std::is_same_v<unsigned int, T>) {
        defaultValue = (T) std::stoul(hint->second);
      } else if constexpr (std::is_same_v<int, T>) {
        defaultValue = std::stoi(hint->second);
      } else if constexpr (std::is_same_v<float, T>) {
        defaultValue = std::stof(hint->second);
      } else if constexpr (std::is_same_v<double, T>) {
        defaultValue = std::stod(hint->second);
      } else if constexpr (std::is_same_v<bool, T>) {
        defaultValue = hint->second == "true";
      } else if constexpr (std::is_same_v<std::string, T>) {
        defaultValue = hint->second;
      } else {
        hint_type_not_parsed<T>(defaultValue);
      }
    }
    return defaultValue;
  }
};

class Camera final {
public:
  enum class SupportStatus : uint8_t {
    SupportedNoSamples, // Tentatively supported, no RPU samples.
    Supported,          // Claimed as supported (explicitly).
    Unknown,            // Placeholder camera, support is unknown.
    UnknownCamera,      // Not found in database.
    UnknownNoSamples, // Placeholder camera, no RPU samples, support is unknown.
    Unsupported,      // Claimed as unsupported (explicitly).
  };

#ifdef HAVE_PUGIXML
  explicit Camera(const pugi::xml_node& camera);
#endif

  Camera(const Camera* camera, uint32_t alias_num);
  [[nodiscard]] const CameraSensorInfo* getSensorInfo(int iso) const;
  std::string make;
  std::string model;
  std::string mode;
  std::string canonical_make;
  std::string canonical_model;
  std::string canonical_alias;
  std::string canonical_id;
  std::vector<std::string> aliases;
  std::vector<std::string> canonical_aliases;
  ColorFilterArray cfa;
  SupportStatus supportStatus;
  iPoint2D cropSize;
  iPoint2D cropPos;
  std::vector<BlackArea> blackAreas;
  std::vector<CameraSensorInfo> sensorInfo;
  int decoderVersion;
  Hints hints;
  std::vector<NotARational<int>> color_matrix;
  /*
    Signals if there's a proper crop info available in the database entry.
    This flag can be used to decide whether to figure out the crop based on
    the camera vendor specs.
  */
  bool cropAvailable = false;

#ifdef HAVE_PUGIXML
  void parseColorRow(const pugi::xml_node& node);
  void parseColor(const pugi::xml_node& node);
  void parseCFA(const pugi::xml_node& node);
  void parseCrop(const pugi::xml_node& node);
  void parseBlackAreas(const pugi::xml_node& node);
  void parseAliases(const pugi::xml_node& node);
  void parseHints(const pugi::xml_node& node);
  void parseID(const pugi::xml_node& node);
  void parseSensor(const pugi::xml_node& node);
  void parseColorMatrix(const pugi::xml_node& node);
  void parseColorMatrices(const pugi::xml_node& node);

  void parseCameraChild(const pugi::xml_node& node);
#endif
};

} // namespace rawspeed
