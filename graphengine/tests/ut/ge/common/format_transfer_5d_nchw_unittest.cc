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

#include <gtest/gtest.h>

#include "common/formats/format_transfers/format_transfer_nc1hwc0_nchw.h"
//#include "common/formats/format_transfers/format_transfer.h"
#include "common/fp16_t.h"
#include "register/register_format_transfer.h"
#include "framework/common/ge_inner_error_codes.h"

namespace ge {
namespace formats {
class UTEST_FormatTransferNc1hwc0ToNchw : public testing::Test {
 protected:
  void SetUp() {}
  void TearDown() {}
};

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, nc1hwc0_to_nchw_uint8) {
  uint8_t data_5d[1 * 1 * 4 * 4 * 32] = {
      1,  101, 201, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      2,  102, 202, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      3,  103, 203, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      4,  104, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      5,  105, 205, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      6,  106, 206, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      7,  107, 207, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      8,  108, 208, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      9,  109, 209, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      10, 110, 210, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      11, 111, 211, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      12, 112, 212, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      13, 113, 213, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      14, 114, 214, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      15, 115, 215, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      16, 116, 216, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };
  uint8_t data[1 * 3 * 4 * 4] = {1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,
                                 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
                                 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216};

  TransArgs args{data_5d, FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 4, 4, 32}, {1, 3, 4, 4}, DT_UINT8};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), SUCCESS);
  EXPECT_EQ(result.length, sizeof(data));
  for (int i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
    EXPECT_EQ(*(result.data.get() + i), data[i]);
  }
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, nc1hwc0_to_nchw_uint8_32c) {
  uint8_t data_5d[1 * 1 * 4 * 4 * 32] = {
      0,   16,  32,  48,  64,  80,  96,  112, 128, 144, 160, 176, 192, 208, 224, 240, 1,   17,  33,  49,  65,  81,  97,
      113, 129, 145, 161, 177, 193, 209, 225, 241, 1,   17,  33,  49,  65,  81,  97,  113, 129, 145, 161, 177, 193, 209,
      225, 241, 2,   18,  34,  50,  66,  82,  98,  114, 130, 146, 162, 178, 194, 210, 226, 242, 2,   18,  34,  50,  66,
      82,  98,  114, 130, 146, 162, 178, 194, 210, 226, 242, 3,   19,  35,  51,  67,  83,  99,  115, 131, 147, 163, 179,
      195, 211, 227, 243, 3,   19,  35,  51,  67,  83,  99,  115, 131, 147, 163, 179, 195, 211, 227, 243, 4,   20,  36,
      52,  68,  84,  100, 116, 132, 148, 164, 180, 196, 212, 228, 244, 4,   20,  36,  52,  68,  84,  100, 116, 132, 148,
      164, 180, 196, 212, 228, 244, 5,   21,  37,  53,  69,  85,  101, 117, 133, 149, 165, 181, 197, 213, 229, 245, 5,
      21,  37,  53,  69,  85,  101, 117, 133, 149, 165, 181, 197, 213, 229, 245, 6,   22,  38,  54,  70,  86,  102, 118,
      134, 150, 166, 182, 198, 214, 230, 246, 6,   22,  38,  54,  70,  86,  102, 118, 134, 150, 166, 182, 198, 214, 230,
      246, 7,   23,  39,  55,  71,  87,  103, 119, 135, 151, 167, 183, 199, 215, 231, 247, 7,   23,  39,  55,  71,  87,
      103, 119, 135, 151, 167, 183, 199, 215, 231, 247, 8,   24,  40,  56,  72,  88,  104, 120, 136, 152, 168, 184, 200,
      216, 232, 248, 8,   24,  40,  56,  72,  88,  104, 120, 136, 152, 168, 184, 200, 216, 232, 248, 9,   25,  41,  57,
      73,  89,  105, 121, 137, 153, 169, 185, 201, 217, 233, 249, 9,   25,  41,  57,  73,  89,  105, 121, 137, 153, 169,
      185, 201, 217, 233, 249, 10,  26,  42,  58,  74,  90,  106, 122, 138, 154, 170, 186, 202, 218, 234, 250, 10,  26,
      42,  58,  74,  90,  106, 122, 138, 154, 170, 186, 202, 218, 234, 250, 11,  27,  43,  59,  75,  91,  107, 123, 139,
      155, 171, 187, 203, 219, 235, 251, 11,  27,  43,  59,  75,  91,  107, 123, 139, 155, 171, 187, 203, 219, 235, 251,
      12,  28,  44,  60,  76,  92,  108, 124, 140, 156, 172, 188, 204, 220, 236, 252, 12,  28,  44,  60,  76,  92,  108,
      124, 140, 156, 172, 188, 204, 220, 236, 252, 13,  29,  45,  61,  77,  93,  109, 125, 141, 157, 173, 189, 205, 221,
      237, 253, 13,  29,  45,  61,  77,  93,  109, 125, 141, 157, 173, 189, 205, 221, 237, 253, 14,  30,  46,  62,  78,
      94,  110, 126, 142, 158, 174, 190, 206, 222, 238, 254, 14,  30,  46,  62,  78,  94,  110, 126, 142, 158, 174, 190,
      206, 222, 238, 254, 15,  31,  47,  63,  79,  95,  111, 127, 143, 159, 175, 191, 207, 223, 239, 0,   15,  31,  47,
      63,  79,  95,  111, 127, 143, 159, 175, 191, 207, 223, 239, 0,   16,  32,  48,  64,  80,  96,  112, 128, 144, 160,
      176, 192, 208, 224, 240, 1,
  };
  uint8_t data[1 * 32 * 4 * 4] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,
      23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,
      46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,
      69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,
      92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
      115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137,
      138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
      161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183,
      184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206,
      207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
      230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
      253, 254, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,
      21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,
      44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,
      67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
      90,  91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
      113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
      136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
      159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
      182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
      205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
      228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250,
      251, 252, 253, 254, 0,   1,
  };

  TransArgs args{data_5d, FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 4, 4, 32}, {1, 32, 4, 4}, DT_UINT8};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), SUCCESS);
  EXPECT_EQ(result.length, sizeof(data));
  for (int i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
    EXPECT_EQ(*(result.data.get() + i), data[i]);
  }
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, nc1hwc0_to_nchwfp16_single) {
  uint16_t data[1 * 1 * 1 * 1] = {13425};
  uint16_t data_5d[1 * 1 * 1 * 1 * 16] = {
      13425, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };

  TransArgs args{
      reinterpret_cast<uint8_t *>(data_5d), FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 1, 1, 16}, {1, 1, 1, 1}, DT_FLOAT16};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), SUCCESS);
  EXPECT_EQ(result.length, sizeof(data));
  for (int i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
    EXPECT_EQ((reinterpret_cast<uint16_t *>(result.data.get()))[i], data[i]);
  }
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, nc1hwc0_to_nchw_fp16) {
  uint16_t data_5d[1 * 1 * 4 * 4 * 16] = {
      14862, 14023, 14816, 11390, 14779, 11715, 14638, 14509, 14441, 13503, 14814, 13795, 13847, 14745, 15059, 13746,
      8282,  15213, 14495, 14221, 15299, 14126, 14322, 11435, 14238, 15287, 15273, 14807, 15163, 14883, 14912, 12849,
      14921, 14896, 15259, 14167, 13653, 14609, 12054, 13925, 14673, 14744, 10492, 14718, 12709, 14964, 14952, 15255,
      15123, 14600, 14627, 12936, 14972, 11265, 13908, 13264, 13523, 14115, 13541, 13446, 8391,  10700, 14445, 11586,
      15088, 12353, 15138, 15162, 15290, 14850, 14862, 14131, 10827, 14038, 12047, 13469, 13654, 15009, 15189, 12226,
      15295, 15216, 15254, 14722, 15216, 14427, 12508, 14553, 15158, 11562, 15171, 14775, 15161, 13439, 15097, 8566,
      14780, 14590, 12232, 14709, 12227, 14429, 15354, 12318, 13589, 14488, 14560, 15145, 14640, 14740, 14165, 12610,
      15230, 15069, 15161, 14221, 14997, 14818, 11639, 13424, 15310, 11455, 13646, 12862, 12989, 15032, 13677, 14551,
      14510, 15157, 14759, 15108, 12693, 14636, 14575, 13679, 12659, 15217, 11509, 10551, 14286, 13520, 13470, 12282,
      14734, 14821, 14667, 12783, 11479, 13083, 12017, 10590, 12052, 15280, 12302, 14675, 14546, 14962, 12830, 13908,
      12601, 12343, 11266, 14534, 14482, 12896, 12191, 14423, 15219, 14576, 10054, 15120, 15021, 14918, 13288, 14529,
      14616, 14758, 15067, 12250, 14251, 14158, 14423, 13791, 15224, 14959, 14637, 12702, 15359, 15334, 15144, 9603,
      14632, 14955, 15207, 11988, 12695, 15015, 15123, 14705, 14942, 14763, 13985, 14995, 13719, 14939, 14033, 14696,
      14174, 15183, 14922, 11460, 14856, 14838, 14643, 14625, 15080, 14732, 15128, 15241, 13484, 14453, 14983, 13360,
      13613, 13815, 14349, 12802, 14386, 14675, 13697, 13069, 12385, 14200, 13553, 14937, 14979, 14747, 14663, 14843,
      13120, 14373, 13953, 15234, 13729, 13999, 13505, 13100, 14788, 15024, 14270, 13430, 15140, 15104, 14409, 14096,
  };
  uint16_t data[1 * 16 * 4 * 4] = {
      14862, 8282,  14921, 15123, 15088, 15295, 14780, 15230, 14510, 14734, 12601, 14616, 14632, 14174, 13613, 13120,
      14023, 15213, 14896, 14600, 12353, 15216, 14590, 15069, 15157, 14821, 12343, 14758, 14955, 15183, 13815, 14373,
      14816, 14495, 15259, 14627, 15138, 15254, 12232, 15161, 14759, 14667, 11266, 15067, 15207, 14922, 14349, 13953,
      11390, 14221, 14167, 12936, 15162, 14722, 14709, 14221, 15108, 12783, 14534, 12250, 11988, 11460, 12802, 15234,
      14779, 15299, 13653, 14972, 15290, 15216, 12227, 14997, 12693, 11479, 14482, 14251, 12695, 14856, 14386, 13729,
      11715, 14126, 14609, 11265, 14850, 14427, 14429, 14818, 14636, 13083, 12896, 14158, 15015, 14838, 14675, 13999,
      14638, 14322, 12054, 13908, 14862, 12508, 15354, 11639, 14575, 12017, 12191, 14423, 15123, 14643, 13697, 13505,
      14509, 11435, 13925, 13264, 14131, 14553, 12318, 13424, 13679, 10590, 14423, 13791, 14705, 14625, 13069, 13100,
      14441, 14238, 14673, 13523, 10827, 15158, 13589, 15310, 12659, 12052, 15219, 15224, 14942, 15080, 12385, 14788,
      13503, 15287, 14744, 14115, 14038, 11562, 14488, 11455, 15217, 15280, 14576, 14959, 14763, 14732, 14200, 15024,
      14814, 15273, 10492, 13541, 12047, 15171, 14560, 13646, 11509, 12302, 10054, 14637, 13985, 15128, 13553, 14270,
      13795, 14807, 14718, 13446, 13469, 14775, 15145, 12862, 10551, 14675, 15120, 12702, 14995, 15241, 14937, 13430,
      13847, 15163, 12709, 8391,  13654, 15161, 14640, 12989, 14286, 14546, 15021, 15359, 13719, 13484, 14979, 15140,
      14745, 14883, 14964, 10700, 15009, 13439, 14740, 15032, 13520, 14962, 14918, 15334, 14939, 14453, 14747, 15104,
      15059, 14912, 14952, 14445, 15189, 15097, 14165, 13677, 13470, 12830, 13288, 15144, 14033, 14983, 14663, 14409,
      13746, 12849, 15255, 11586, 12226, 8566,  12610, 14551, 12282, 13908, 14529, 9603,  14696, 13360, 14843, 14096,
  };

  TransArgs args{
      reinterpret_cast<uint8_t *>(data_5d), FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 4, 4, 16}, {1, 16, 4, 4}, DT_FLOAT16};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), SUCCESS);
  EXPECT_EQ(result.length, sizeof(data));
  for (int i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
    EXPECT_EQ((reinterpret_cast<uint16_t *>(result.data.get()))[i], data[i]);
  }
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, nc1hwc0_to_nchw_fp16_17c) {
  uint16_t data_5d[1 * 2 * 4 * 4 * 16] = {
      14568, 14421, 14797, 14544, 13105, 13370, 15172, 14381, 15270, 12195, 13233, 15350, 13571, 10961, 10330, 13865,
      13864, 11756, 13374, 14495, 11969, 11541, 13723, 14035, 12632, 12958, 15051, 13749, 15124, 13845, 13742, 14891,
      14728, 15305, 13959, 14165, 13934, 14044, 14807, 14168, 14771, 13262, 14305, 14341, 14705, 11336, 13744, 12774,
      9931,  15085, 14748, 14941, 15255, 13546, 9992,  15150, 14654, 15272, 14954, 14247, 14786, 14715, 14736, 14596,
      13909, 15000, 14607, 13415, 15009, 12345, 14431, 11364, 12769, 14173, 15184, 12405, 14950, 14596, 14995, 14367,
      15238, 12103, 15218, 13477, 14363, 15129, 13895, 14014, 14424, 12544, 15284, 14554, 14972, 14031, 12372, 14881,
      13252, 13647, 14843, 13552, 12019, 13836, 14410, 14987, 13967, 15002, 14597, 13275, 15273, 15332, 13185, 13735,
      14643, 10549, 14527, 14460, 14840, 13478, 14703, 14563, 10958, 14177, 15050, 15096, 14875, 14397, 14143, 13434,
      14995, 12539, 15308, 14687, 14654, 14183, 12403, 14639, 14824, 12740, 7525,  14283, 14554, 14766, 15104, 15021,
      14363, 7084,  12950, 14904, 15248, 11753, 14283, 15168, 14818, 13238, 15014, 12556, 12828, 14049, 14558, 15137,
      15298, 14873, 14293, 13047, 14263, 13515, 14198, 13906, 14489, 9541,  13617, 13656, 13406, 14513, 13360, 13604,
      14911, 12182, 14215, 14713, 12221, 9609,  15221, 15010, 11716, 12863, 15349, 14575, 13794, 15164, 14754, 11357,
      15106, 9793,  11630, 10997, 14930, 14946, 15178, 12638, 14766, 14440, 14660, 13576, 14684, 15073, 12315, 15039,
      14522, 11816, 13598, 14531, 14892, 14832, 12603, 11430, 13780, 14985, 14740, 14461, 10439, 11816, 14360, 14929,
      12107, 12592, 14456, 13482, 14755, 15340, 15225, 14599, 14681, 14962, 12123, 10863, 15160, 13471, 12378, 15048,
      14647, 15209, 15114, 13465, 14893, 14925, 14931, 12569, 12727, 15193, 13636, 15240, 14501, 14656, 13539, 14598,
      14699, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      15057, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      13683, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14368, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      8654,  0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11915, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      15315, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14933, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14449, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      15148, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      13736, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14541, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      12031, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      15255, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14550, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14823, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
  };
  uint16_t data[1 * 17 * 4 * 4] = {
      14568, 13864, 14728, 9931,  13909, 15238, 13252, 14643, 14995, 14363, 15298, 14911, 15106, 14522, 12107, 14647,
      14421, 11756, 15305, 15085, 15000, 12103, 13647, 10549, 12539, 7084,  14873, 12182, 9793,  11816, 12592, 15209,
      14797, 13374, 13959, 14748, 14607, 15218, 14843, 14527, 15308, 12950, 14293, 14215, 11630, 13598, 14456, 15114,
      14544, 14495, 14165, 14941, 13415, 13477, 13552, 14460, 14687, 14904, 13047, 14713, 10997, 14531, 13482, 13465,
      13105, 11969, 13934, 15255, 15009, 14363, 12019, 14840, 14654, 15248, 14263, 12221, 14930, 14892, 14755, 14893,
      13370, 11541, 14044, 13546, 12345, 15129, 13836, 13478, 14183, 11753, 13515, 9609,  14946, 14832, 15340, 14925,
      15172, 13723, 14807, 9992,  14431, 13895, 14410, 14703, 12403, 14283, 14198, 15221, 15178, 12603, 15225, 14931,
      14381, 14035, 14168, 15150, 11364, 14014, 14987, 14563, 14639, 15168, 13906, 15010, 12638, 11430, 14599, 12569,
      15270, 12632, 14771, 14654, 12769, 14424, 13967, 10958, 14824, 14818, 14489, 11716, 14766, 13780, 14681, 12727,
      12195, 12958, 13262, 15272, 14173, 12544, 15002, 14177, 12740, 13238, 9541,  12863, 14440, 14985, 14962, 15193,
      13233, 15051, 14305, 14954, 15184, 15284, 14597, 15050, 7525,  15014, 13617, 15349, 14660, 14740, 12123, 13636,
      15350, 13749, 14341, 14247, 12405, 14554, 13275, 15096, 14283, 12556, 13656, 14575, 13576, 14461, 10863, 15240,
      13571, 15124, 14705, 14786, 14950, 14972, 15273, 14875, 14554, 12828, 13406, 13794, 14684, 10439, 15160, 14501,
      10961, 13845, 11336, 14715, 14596, 14031, 15332, 14397, 14766, 14049, 14513, 15164, 15073, 11816, 13471, 14656,
      10330, 13742, 13744, 14736, 14995, 12372, 13185, 14143, 15104, 14558, 13360, 14754, 12315, 14360, 12378, 13539,
      13865, 14891, 12774, 14596, 14367, 14881, 13735, 13434, 15021, 15137, 13604, 11357, 15039, 14929, 15048, 14598,
      14699, 15057, 13683, 14368, 8654,  11915, 15315, 14933, 14449, 15148, 13736, 14541, 12031, 15255, 14550, 14823,
  };

  TransArgs args{
      reinterpret_cast<uint8_t *>(data_5d), FORMAT_NC1HWC0, FORMAT_NCHW, {1, 2, 4, 4, 16}, {1, 17, 4, 4}, DT_FLOAT16};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), SUCCESS);
  EXPECT_EQ(result.length, sizeof(data));
  for (int i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
    EXPECT_EQ((reinterpret_cast<uint16_t *>(result.data.get()))[i], data[i]);
  }
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, nc1hwc0_to_nchw_float) {
  float data[1 * 8 * 4 * 4] = {
      0.5892849569036265,   0.6803315204121938,  0.5177982416755607,  0.12129040093083365, 0.04237103024867994,
      0.7428342506399291,   0.5359802823824235,  0.04499999698629653, 0.7610018014391726,  0.38621323898777005,
      0.4360751167195973,   0.3834964892197801,  0.5888008090373326,  0.09836678109117547, 0.9950749943600654,
      0.9635155267139188,   0.5378606253569519,  0.2383735299723022,  0.5049400994471483,  0.2967628815119744,
      0.9172822428045107,   0.5429433505121389,  0.38499549462545024, 0.23619965842338686, 0.720664799527641,
      0.02313921408863151,  0.6488943229526023,  0.3390551602851408,  0.728417105824467,   0.9053225912171141,
      0.32494694533300583,  0.9119093270624166,  0.9647657094436359,  0.7219930950678662,  0.36167953499559,
      0.5984012357524195,   0.9544874847178995,  0.02306924612189265, 0.8026403495895027,  0.22551907272533667,
      0.36263992795411604,  0.58886941262115,    0.5735986398876265,  0.5252128788659909,  0.0827150730694497,
      0.17498225712307047,  0.4845825388200229,  0.40605108821850533, 0.9274359210940875,  0.7147299778467197,
      0.32288439175726646,  0.4065504767493492,  0.6286803275241362,  0.20374542713340105, 0.7445032000224268,
      0.9674821461856206,   0.909400577299532,   0.40363134678641066, 0.9627522330737276,  0.6933785292758723,
      0.9641353478602301,   0.7754020225695061,  0.620702777688872,   0.11214574817054179, 0.894884208921027,
      0.7101293717077931,   0.36970203638442056, 0.9356214764169016,  0.8339204066613951,  0.516307604153244,
      0.7030058401326411,   0.3991170380257899,  0.691216036157706,   0.7414799310134091,  0.22811510970169568,
      0.5609880702374889,   0.22477373948238633, 0.12881731266251306, 0.4573255943473218,  0.17517491298262455,
      0.4664711535236884,   0.36304572216421005, 0.04147865556156949, 0.321799545851576,   0.3134658252359267,
      0.9168822528697251,   0.2070779910124293,  0.4370570617679451,  0.5458639932730591,  0.34286569365484054,
      0.4939443382175456,   0.3412383781775876,  0.299800764913397,   0.3458667905629188,  0.5037122283316625,
      0.13792096399324794,  0.5296944120355204,  0.6593970795972891,  0.672741074468844,   0.5297456647001881,
      0.006066715407170187, 0.8287877066716592,  0.18033462634801634, 0.5000576732820233,  0.8853254925542572,
      0.38219052838295775,  0.17776888090118503, 0.2556143927933693,  0.46146366919906867, 0.4037875054768396,
      0.062043324444360226, 0.4479202861693887,  0.25183795798980213, 0.6102048134444441,  0.9471408150891643,
      0.8789211226767781,   0.5987926543415545,  0.08687291331362201, 0.09890376596175199, 0.39921593538893263,
      0.8463226026274682,   0.5365747044508772,  0.762082525622205,   0.6515229727575028,  0.10831064130367352,
      0.4628228725538879,   0.820619798511191,   0.5779888725124475,
  };
  float data_5d[1 * 1 * 4 * 4 * 16] = {
      0.5892849569036265,
      0.5378606253569519,
      0.9647657094436359,
      0.9274359210940875,
      0.894884208921027,
      0.4664711535236884,
      0.5296944120355204,
      0.25183795798980213,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.6803315204121938,
      0.2383735299723022,
      0.7219930950678662,
      0.7147299778467197,
      0.7101293717077931,
      0.36304572216421005,
      0.6593970795972891,
      0.6102048134444441,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.5177982416755607,
      0.5049400994471483,
      0.36167953499559,
      0.32288439175726646,
      0.36970203638442056,
      0.04147865556156949,
      0.672741074468844,
      0.9471408150891643,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.12129040093083365,
      0.2967628815119744,
      0.5984012357524195,
      0.4065504767493492,
      0.9356214764169016,
      0.321799545851576,
      0.5297456647001881,
      0.8789211226767781,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.04237103024867994,
      0.9172822428045107,
      0.9544874847178995,
      0.6286803275241362,
      0.8339204066613951,
      0.3134658252359267,
      0.006066715407170187,
      0.5987926543415545,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.7428342506399291,
      0.5429433505121389,
      0.02306924612189265,
      0.20374542713340105,
      0.516307604153244,
      0.9168822528697251,
      0.8287877066716592,
      0.08687291331362201,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.5359802823824235,
      0.38499549462545024,
      0.8026403495895027,
      0.7445032000224268,
      0.7030058401326411,
      0.2070779910124293,
      0.18033462634801634,
      0.09890376596175199,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.04499999698629653,
      0.23619965842338686,
      0.22551907272533667,
      0.9674821461856206,
      0.3991170380257899,
      0.4370570617679451,
      0.5000576732820233,
      0.39921593538893263,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.7610018014391726,
      0.720664799527641,
      0.36263992795411604,
      0.909400577299532,
      0.691216036157706,
      0.5458639932730591,
      0.8853254925542572,
      0.8463226026274682,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.38621323898777005,
      0.02313921408863151,
      0.58886941262115,
      0.40363134678641066,
      0.7414799310134091,
      0.34286569365484054,
      0.38219052838295775,
      0.5365747044508772,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.4360751167195973,
      0.6488943229526023,
      0.5735986398876265,
      0.9627522330737276,
      0.22811510970169568,
      0.4939443382175456,
      0.17776888090118503,
      0.762082525622205,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.3834964892197801,
      0.3390551602851408,
      0.5252128788659909,
      0.6933785292758723,
      0.5609880702374889,
      0.3412383781775876,
      0.2556143927933693,
      0.6515229727575028,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.5888008090373326,
      0.728417105824467,
      0.0827150730694497,
      0.9641353478602301,
      0.22477373948238633,
      0.299800764913397,
      0.46146366919906867,
      0.10831064130367352,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.09836678109117547,
      0.9053225912171141,
      0.17498225712307047,
      0.7754020225695061,
      0.12881731266251306,
      0.3458667905629188,
      0.4037875054768396,
      0.4628228725538879,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.9950749943600654,
      0.32494694533300583,
      0.4845825388200229,
      0.620702777688872,
      0.4573255943473218,
      0.5037122283316625,
      0.062043324444360226,
      0.820619798511191,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.9635155267139188,
      0.9119093270624166,
      0.40605108821850533,
      0.11214574817054179,
      0.17517491298262455,
      0.13792096399324794,
      0.4479202861693887,
      0.5779888725124475,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
  };

  TransArgs args{
      reinterpret_cast<uint8_t *>(data_5d), FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 4, 4, 16}, {1, 8, 4, 4}, DT_FLOAT};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), SUCCESS);
  EXPECT_EQ(result.length, sizeof(data));
  for (int i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
    EXPECT_FLOAT_EQ((reinterpret_cast<float *>(result.data.get()))[i], data[i]);
  }
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, invalid_src_shape1) {
  uint16_t data[1 * 1 * 4 * 4 * 16] = {0};
  TransArgs args{
      reinterpret_cast<uint8_t *>(data), FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 4, 16}, {1, 0, 4, 4}, DT_FLOAT16};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), ACL_ERROR_GE_SHAPE_INVALID);
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, invalid_src_shape2) {
  uint16_t data[1 * 1 * 4 * 4 * 16] = {0};
  TransArgs args{
      reinterpret_cast<uint8_t *>(data), FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 4, 16, 0}, {1, 0, 4, 4}, DT_FLOAT16};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), ACL_ERROR_GE_SHAPE_INVALID);
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, invalid_dst_shape1) {
  uint16_t data[1 * 1 * 4 * 4 * 16] = {0};
  TransArgs args{reinterpret_cast<uint8_t *>(data), FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 4, 4, 16}, {4, 4}, DT_FLOAT16};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), ACL_ERROR_GE_SHAPE_INVALID);
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, invalid_dst_shape2) {
  uint16_t data[1 * 1 * 4 * 4 * 16] = {0};
  TransArgs args{
      reinterpret_cast<uint8_t *>(data), FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 4, 4, 16}, {1, 0, 4, 4}, DT_FLOAT16};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), ACL_ERROR_GE_SHAPE_INVALID);
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, invalid_src_dst_shape_relation) {
  uint16_t data[1 * 1 * 4 * 4 * 16] = {0};
  TransArgs args{
      reinterpret_cast<uint8_t *>(data), FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 4, 4, 16}, {1, 17, 4, 4}, DT_FLOAT16};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), ACL_ERROR_GE_SHAPE_INVALID);
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, invalid_src_format) {
  uint16_t data[1 * 1 * 4 * 4 * 16] = {0};
  TransArgs args{
      reinterpret_cast<uint8_t *>(data), FORMAT_RESERVED, FORMAT_NCHW, {1, 1, 4, 4, 16}, {1, 1, 4, 4}, DT_FLOAT16};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), ACL_ERROR_GE_FORMAT_INVALID);
  Status status =
      transfer.TransShape(args.src_format, args.src_shape, args.src_data_type, args.dst_format, args.dst_shape);
  EXPECT_EQ(status, ACL_ERROR_GE_FORMAT_INVALID);
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, invalid_dst_format) {
  uint16_t data[1 * 1 * 4 * 4 * 16] = {0};
  TransArgs args{
      reinterpret_cast<uint8_t *>(data), FORMAT_NC1HWC0, FORMAT_NHWC, {1, 1, 4, 4, 16}, {1, 1, 4, 4}, DT_FLOAT16};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), ACL_ERROR_GE_FORMAT_INVALID);
}

TEST_F(UTEST_FormatTransferNc1hwc0ToNchw, invalid_src_data_type) {
  uint16_t data[1 * 1 * 4 * 4 * 16] = {0};

  TransArgs args{
      reinterpret_cast<uint8_t *>(data), FORMAT_NC1HWC0, FORMAT_NCHW, {1, 1, 4, 4, 16}, {1, 1, 1, 4, 4}, DT_UNDEFINED};
  TransResult result;

  FormatTransferNc1hwc0Nchw transfer;
  EXPECT_EQ(transfer.TransFormat(args, result), ACL_ERROR_GE_DATATYPE_INVALID);
}
}  // namespace formats
}  // namespace ge
