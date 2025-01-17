/**
 * Copyright 2021, 2022 LuoJiaNET Research and Development Group, Wuhan University
 * Copyright 2021, 2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LUOJIANET_MS_CORE_OPS_AUDIO_SPECTROGRAM_H_
#define LUOJIANET_MS_CORE_OPS_AUDIO_SPECTROGRAM_H_
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "ops/primitive_c.h"
#include "abstract/abstract_value.h"
#include "utils/check_convert_utils.h"

namespace luojianet_ms {
namespace ops {
constexpr auto kNameAudioSpectrogram = "AudioSpectrogram";
/// \brief AudioSpectrogram defined AudioSpectrogram operator prototype.
class MS_CORE_API AudioSpectrogram : public PrimitiveC {
 public:
  /// \brief Constructor.
  AudioSpectrogram() : PrimitiveC(kNameAudioSpectrogram) {}

  /// \brief Destructor.
  ~AudioSpectrogram() = default;

  MS_DECLARE_PARENT(AudioSpectrogram, PrimitiveC);

  /// \brief Method to init the op's attributes.
  ///
  /// \param[in] window_size Define the size of window.
  /// \param[in] stride Define the step size of window to move.
  /// \param[in] mag_square Define a boolean value to indicate the output is the magnitude or the square of magnitude.
  void Init(const int64_t window_size, const int64_t stride, const bool mag_square);

  /// \brief Method to set window_size attribute.
  ///
  /// \param[in] window_size Define the size of window.
  void set_window_size(const int64_t window_size);

  /// \brief Method to set stride attribute.
  ///
  /// \param[in] stride Define the step size of window to move.
  void set_stride(const int64_t stride);

  /// \brief Method to set mag_square attribute.
  ///
  /// \param[in] mag_square Define a boolean to indicate the output is the magnitude or the square of magnitude.
  void set_mag_square(const bool mag_square);

  /// \brief Method to get  window_size attribute.
  ///
  /// \return the size of window.
  int64_t get_window_size() const;

  /// \brief Method to get stride attribute.
  ///
  /// \return the step size.
  int64_t get_stride() const;

  /// \brief Method to get mag_square attribute.
  ///
  /// \return a boolean value.
  bool get_mag_square() const;
};
AbstractBasePtr AudioSpectrogramInfer(const abstract::AnalysisEnginePtr &, const PrimitivePtr &primitive,
                                      const std::vector<AbstractBasePtr> &input_args);
}  // namespace ops
}  // namespace luojianet_ms

#endif  // LUOJIANET_MS_CORE_OPS_AUDIO_SPECTROGRAM_H_
