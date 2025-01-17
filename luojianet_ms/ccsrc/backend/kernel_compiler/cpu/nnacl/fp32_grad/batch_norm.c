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
#include <math.h>
#include <string.h>
#include "nnacl/fp32_grad/batch_norm.h"

void var2Invar(float *save_var, int size, float eps) {
  for (int i = 0; i < size; i++) {
    save_var[i] = 1.0f / sqrtf(save_var[i] + eps);
  }
}

#ifdef _MSC_VER
void backwardAll(const float *in, const float *yt, const float *mean, const float *invar, const float *scale, int size,
                 int ch, float *dbias, float *dscale, float *dx) {
#else
void backwardAll(const float *restrict in, const float *restrict yt, const float *restrict mean,
                 const float *restrict invar, const float *restrict scale, int size, int ch, float *restrict dbias,
                 float *restrict dscale, float *restrict dx) {
#endif
  NNACL_CHECK_ZERO_RETURN(size);
  float N = (float)size;
  for (int i = 0; i < size; i++) {
    for (int c = 0; c < ch; c++) {
      int ix = i * ch + c;
      dbias[c] += yt[ix];
      // dscale
      float x_hat = (in[ix] - mean[c]) * invar[c];
      dscale[c] += (yt[ix] * x_hat);
    }
  }
  for (int i = 0; i < size; i++) {
    for (int c = 0; c < ch; c++) {
      // dx_2
      int ix = i * ch + c;
      dx[ix] = yt[ix];
      dx[ix] -= dbias[c] / N + (in[ix] - mean[c]) * dscale[c] * invar[c] / N;
      dx[ix] *= scale[c] * invar[c];
    }
  }
}

#ifdef _MSC_VER
void backwardP1(const float *in, const float *yt, const float *mean, const float *invar, const float *scale, int size,
                int ch, float *dbias, float *dscale) {
#else
void backwardP1(const float *restrict in, const float *restrict yt, const float *restrict mean,
                const float *restrict invar, const float *restrict scale, int size, int ch, float *restrict dbias,
                float *restrict dscale) {
#endif
  for (int i = 0; i < size; i++) {
    for (int c = 0; c < ch; c++) {
      int ix = i * ch + c;
      dbias[c] += yt[ix];
      // dscale
      float x_hat = (in[ix] - mean[c]) * invar[c];
      dscale[c] += (yt[ix] * x_hat);
    }
  }
}

#ifdef _MSC_VER
void backwardP2(const float *in, const float *yt, const float *mean, const float *invar, const float *dscale,
                const float *dbias, const float *scale, int size, int total_size, int ch, float *dx) {
#else
void backwardP2(const float *restrict in, const float *restrict yt, const float *restrict mean,
                const float *restrict invar, const float *restrict dscale, const float *restrict dbias,
                const float *restrict scale, int size, int total_size, int ch, float *restrict dx) {
#endif
  NNACL_CHECK_ZERO_RETURN(total_size);
  const float N = (float)total_size;
  for (int i = 0; i < size; i++) {
    for (int c = 0; c < ch; c++) {
      // dx_2
      int ix = i * ch + c;
      dx[ix] = yt[ix];
      dx[ix] -= dbias[c] / N + (in[ix] - mean[c]) * dscale[c] * invar[c] / N;
      dx[ix] *= scale[c] * invar[c];
    }
  }
}
