// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "gandiva/gdv_function_stubs.h"

#include <gandiva/precompiled/testing.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "arrow/util/logging.h"
#include "gandiva/execution_context.h"

namespace gandiva {

TEST(TestGdvFnStubs, TestCrc32) {
  gandiva::ExecutionContext ctx;
  auto ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  EXPECT_EQ(gdv_fn_crc_32_utf8(ctx_ptr, "ABC", 3), 2743272264);
  EXPECT_EQ(gdv_fn_crc_32_utf8(ctx_ptr, "Hello", 5), 4157704578);
  EXPECT_EQ(gdv_fn_crc_32_utf8(ctx_ptr, "hive", 4), 3698179064);
  EXPECT_EQ(gdv_fn_crc_32_utf8(ctx_ptr, "372189372123", 12), 2607335846);
  EXPECT_EQ(gdv_fn_crc_32_utf8(ctx_ptr, "", 0), 0);

  EXPECT_EQ(gdv_fn_crc_32_utf8(ctx_ptr, "-5", -5), 0);
  EXPECT_THAT(ctx.get_error(), ::testing::HasSubstr("Input length can't be negative"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastVarbinaryNumeric) {
  gandiva::ExecutionContext ctx;

  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  // tests for integer values as input
  const char* out_str = gdv_fn_castVARBINARY_int32_int64(ctx_ptr, -46, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "-46");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARBINARY_int32_int64(ctx_ptr, 2147483647, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "2147483647");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARBINARY_int32_int64(ctx_ptr, -2147483647 - 1, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "-2147483648");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARBINARY_int32_int64(ctx_ptr, 0, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "0");
  EXPECT_FALSE(ctx.has_error());

  // test with required length less than actual buffer length
  out_str = gdv_fn_castVARBINARY_int32_int64(ctx_ptr, 34567, 3, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "345");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARBINARY_int32_int64(ctx_ptr, 347, 0, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_FALSE(ctx.has_error());

  gdv_fn_castVARBINARY_int32_int64(ctx_ptr, 347, -1, &out_len);
  EXPECT_THAT(ctx.get_error(), ::testing::HasSubstr("Buffer length can not be negative"));
  ctx.Reset();

  // tests for big integer values as input
  out_str =
      gdv_fn_castVARBINARY_int64_int64(ctx_ptr, 9223372036854775807LL, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "9223372036854775807");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARBINARY_int64_int64(ctx_ptr, -9223372036854775807LL - 1, 100,
                                             &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "-9223372036854775808");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARBINARY_int64_int64(ctx_ptr, 0, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "0");
  EXPECT_FALSE(ctx.has_error());

  // test with required length less than actual buffer length
  out_str = gdv_fn_castVARBINARY_int64_int64(ctx_ptr, 12345, 3, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "123");
  EXPECT_FALSE(ctx.has_error());
}

TEST(TestGdvFnStubs, TestBase64Encode) {
  gandiva::ExecutionContext ctx;

  auto ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  auto value = gdv_fn_base64_encode_binary(ctx_ptr, "hello", 5, &out_len);
  std::string out_value = std::string(value, out_len);
  EXPECT_EQ(out_value, "aGVsbG8=");

  value = gdv_fn_base64_encode_binary(ctx_ptr, "test", 4, &out_len);
  out_value = std::string(value, out_len);
  EXPECT_EQ(out_value, "dGVzdA==");

  value = gdv_fn_base64_encode_binary(ctx_ptr, "hive", 4, &out_len);
  out_value = std::string(value, out_len);
  EXPECT_EQ(out_value, "aGl2ZQ==");

  value = gdv_fn_base64_encode_binary(ctx_ptr, "", 0, &out_len);
  out_value = std::string(value, out_len);
  EXPECT_EQ(out_value, "");

  value = gdv_fn_base64_encode_binary(ctx_ptr, "test", -5, &out_len);
  out_value = std::string(value, out_len);
  EXPECT_EQ(out_value, "");
  EXPECT_THAT(ctx.get_error(), ::testing::HasSubstr("Buffer length can not be negative"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestBase64Decode) {
  gandiva::ExecutionContext ctx;

  auto ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  auto value = gdv_fn_base64_decode_utf8(ctx_ptr, "aGVsbG8=", 8, &out_len);
  std::string out_value = std::string(value, out_len);
  EXPECT_EQ(out_value, "hello");

  value = gdv_fn_base64_decode_utf8(ctx_ptr, "dGVzdA==", 8, &out_len);
  out_value = std::string(value, out_len);
  EXPECT_EQ(out_value, "test");

  value = gdv_fn_base64_decode_utf8(ctx_ptr, "aGl2ZQ==", 8, &out_len);
  out_value = std::string(value, out_len);
  EXPECT_EQ(out_value, "hive");

  value = gdv_fn_base64_decode_utf8(ctx_ptr, "", 0, &out_len);
  out_value = std::string(value, out_len);
  EXPECT_EQ(out_value, "");

  value = gdv_fn_base64_decode_utf8(ctx_ptr, "test", -5, &out_len);
  out_value = std::string(value, out_len);
  EXPECT_EQ(out_value, "");
  EXPECT_THAT(ctx.get_error(), ::testing::HasSubstr("Buffer length can not be negative"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastINT) {
  gandiva::ExecutionContext ctx;

  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  EXPECT_EQ(gdv_fn_castINT_utf8(ctx_ptr, "-45", 3), -45);
  EXPECT_EQ(gdv_fn_castINT_utf8(ctx_ptr, "0", 1), 0);
  EXPECT_EQ(gdv_fn_castINT_utf8(ctx_ptr, "2147483647", 10), 2147483647);
  EXPECT_EQ(gdv_fn_castINT_utf8(ctx_ptr, "02147483647", 11), 2147483647);
  EXPECT_EQ(gdv_fn_castINT_utf8(ctx_ptr, "-2147483648", 11), -2147483648LL);
  EXPECT_EQ(gdv_fn_castINT_utf8(ctx_ptr, "-02147483648", 12), -2147483648LL);
  EXPECT_EQ(gdv_fn_castINT_utf8(ctx_ptr, " 12 ", 4), 12);
  EXPECT_EQ(gdv_fn_castINT_utf8(ctx_ptr, "12", 2), 12);

  gdv_fn_castINT_utf8(ctx_ptr, "2147483648", 10);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string 2147483648 to int32"));
  ctx.Reset();

  gdv_fn_castINT_utf8(ctx_ptr, "-2147483649", 11);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string -2147483649 to int32"));
  ctx.Reset();

  gdv_fn_castINT_utf8(ctx_ptr, "12.34", 5);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string 12.34 to int32"));
  ctx.Reset();

  gdv_fn_castINT_utf8(ctx_ptr, "abc", 3);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string abc to int32"));
  ctx.Reset();

  gdv_fn_castINT_utf8(ctx_ptr, "", 0);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string  to int32"));
  ctx.Reset();

  gdv_fn_castINT_utf8(ctx_ptr, "-", 1);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string - to int32"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastBIGINT) {
  gandiva::ExecutionContext ctx;

  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  EXPECT_EQ(gdv_fn_castBIGINT_utf8(ctx_ptr, "-45", 3), -45);
  EXPECT_EQ(gdv_fn_castBIGINT_utf8(ctx_ptr, "0", 1), 0);
  EXPECT_EQ(gdv_fn_castBIGINT_utf8(ctx_ptr, "9223372036854775807", 19),
            9223372036854775807LL);
  EXPECT_EQ(gdv_fn_castBIGINT_utf8(ctx_ptr, "09223372036854775807", 20),
            9223372036854775807LL);
  EXPECT_EQ(gdv_fn_castBIGINT_utf8(ctx_ptr, "-9223372036854775808", 20),
            -9223372036854775807LL - 1);
  EXPECT_EQ(gdv_fn_castBIGINT_utf8(ctx_ptr, "-009223372036854775808", 22),
            -9223372036854775807LL - 1);
  EXPECT_EQ(gdv_fn_castBIGINT_utf8(ctx_ptr, " 12 ", 4), 12);

  gdv_fn_castBIGINT_utf8(ctx_ptr, "9223372036854775808", 19);
  EXPECT_THAT(
      ctx.get_error(),
      ::testing::HasSubstr("Failed to cast the string 9223372036854775808 to int64"));
  ctx.Reset();

  gdv_fn_castBIGINT_utf8(ctx_ptr, "-9223372036854775809", 20);
  EXPECT_THAT(
      ctx.get_error(),
      ::testing::HasSubstr("Failed to cast the string -9223372036854775809 to int64"));
  ctx.Reset();

  gdv_fn_castBIGINT_utf8(ctx_ptr, "12.34", 5);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string 12.34 to int64"));
  ctx.Reset();

  gdv_fn_castBIGINT_utf8(ctx_ptr, "abc", 3);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string abc to int64"));
  ctx.Reset();

  gdv_fn_castBIGINT_utf8(ctx_ptr, "", 0);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string  to int64"));
  ctx.Reset();

  gdv_fn_castBIGINT_utf8(ctx_ptr, "-", 1);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string - to int64"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastFloat4) {
  gandiva::ExecutionContext ctx;

  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  EXPECT_EQ(gdv_fn_castFLOAT4_utf8(ctx_ptr, "-45.34", 6), -45.34f);
  EXPECT_EQ(gdv_fn_castFLOAT4_utf8(ctx_ptr, "0", 1), 0.0f);
  EXPECT_EQ(gdv_fn_castFLOAT4_utf8(ctx_ptr, "5", 1), 5.0f);
  EXPECT_EQ(gdv_fn_castFLOAT4_utf8(ctx_ptr, " 3.4 ", 5), 3.4f);

  gdv_fn_castFLOAT4_utf8(ctx_ptr, "", 0);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string  to float"));
  ctx.Reset();

  gdv_fn_castFLOAT4_utf8(ctx_ptr, "e", 1);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string e to float"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastFloat8) {
  gandiva::ExecutionContext ctx;

  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  EXPECT_EQ(gdv_fn_castFLOAT8_utf8(ctx_ptr, "-45.34", 6), -45.34);
  EXPECT_EQ(gdv_fn_castFLOAT8_utf8(ctx_ptr, "0", 1), 0.0);
  EXPECT_EQ(gdv_fn_castFLOAT8_utf8(ctx_ptr, "5", 1), 5.0);
  EXPECT_EQ(gdv_fn_castFLOAT8_utf8(ctx_ptr, " 3.4 ", 5), 3.4);

  gdv_fn_castFLOAT8_utf8(ctx_ptr, "", 0);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string  to double"));
  ctx.Reset();

  gdv_fn_castFLOAT8_utf8(ctx_ptr, "e", 1);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string e to double"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastVARCHARFromInt32) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  const char* out_str = gdv_fn_castVARCHAR_int32_int64(ctx_ptr, -46, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "-46");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_int32_int64(ctx_ptr, 2147483647, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "2147483647");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_int32_int64(ctx_ptr, -2147483647 - 1, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "-2147483648");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_int32_int64(ctx_ptr, 0, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "0");
  EXPECT_FALSE(ctx.has_error());

  // test with required length less than actual buffer length
  out_str = gdv_fn_castVARCHAR_int32_int64(ctx_ptr, 34567, 3, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "345");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_int32_int64(ctx_ptr, 347, 0, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_int32_int64(ctx_ptr, 347, -1, &out_len);
  EXPECT_THAT(ctx.get_error(), ::testing::HasSubstr("Buffer length can not be negative"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastVARCHARFromInt64) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  const char* out_str =
      gdv_fn_castVARCHAR_int64_int64(ctx_ptr, 9223372036854775807LL, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "9223372036854775807");
  EXPECT_FALSE(ctx.has_error());

  out_str =
      gdv_fn_castVARCHAR_int64_int64(ctx_ptr, -9223372036854775807LL - 1, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "-9223372036854775808");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_int64_int64(ctx_ptr, 0, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "0");
  EXPECT_FALSE(ctx.has_error());

  // test with required length less than actual buffer length
  out_str = gdv_fn_castVARCHAR_int64_int64(ctx_ptr, 12345, 3, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "123");
  EXPECT_FALSE(ctx.has_error());
}

TEST(TestGdvFnStubs, TestCastVARCHARFromMilliseconds) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  gdv_date64 ts = StringToTimestamp("2021-04-23 10:20:33");
  const char* out_str = gdv_fn_castVARCHAR_date64_int64(ctx_ptr, ts, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "2021-04-23");
  EXPECT_FALSE(ctx.has_error());

  ts = StringToTimestamp("2008-08-20 10:20:33");
  out_str = gdv_fn_castVARCHAR_date64_int64(ctx_ptr, ts, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "2008-08-20");
  EXPECT_FALSE(ctx.has_error());

  ts = StringToTimestamp("2011-09-28 10:20:33");
  out_str = gdv_fn_castVARCHAR_date64_int64(ctx_ptr, ts, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "2011-09-28");
  EXPECT_FALSE(ctx.has_error());

  ts = StringToTimestamp("2021-04-21 10:20:33");
  out_str = gdv_fn_castVARCHAR_date64_int64(ctx_ptr, ts, 7, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "2021-04");
  EXPECT_FALSE(ctx.has_error());

  ts = StringToTimestamp("2008-04-21 10:20:33");
  out_str = gdv_fn_castVARCHAR_date64_int64(ctx_ptr, ts, 4, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "2008");
  EXPECT_FALSE(ctx.has_error());
}

TEST(TestGdvFnStubs, TestCastVARCHARFromFloat) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  const char* out_str = gdv_fn_castVARCHAR_float32_int64(ctx_ptr, 4.567f, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "4.567");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_float32_int64(ctx_ptr, -3.4567f, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "-3.4567");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_float32_int64(ctx_ptr, 0.00001f, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "1.0E-5");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_float32_int64(ctx_ptr, 0.00099999f, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "9.9999E-4");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_float32_int64(ctx_ptr, 0.0f, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "0.0");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_float32_int64(ctx_ptr, 10.00000f, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "10.0");
  EXPECT_FALSE(ctx.has_error());

  // test with required length less than actual buffer length
  out_str = gdv_fn_castVARCHAR_float32_int64(ctx_ptr, 1.2345f, 3, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "1.2");
  EXPECT_FALSE(ctx.has_error());
}

TEST(TestGdvFnStubs, TestCastVARCHARFromDouble) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  const char* out_str = gdv_fn_castVARCHAR_float64_int64(ctx_ptr, 4.567, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "4.567");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_float64_int64(ctx_ptr, -3.4567, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "-3.4567");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_float64_int64(ctx_ptr, 0.00001, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "1.0E-5");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_float32_int64(ctx_ptr, 0.00099999f, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "9.9999E-4");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_float64_int64(ctx_ptr, 0.0, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "0.0");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_castVARCHAR_float64_int64(ctx_ptr, 10.0000000000, 100, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "10.0");
  EXPECT_FALSE(ctx.has_error());

  // test with required length less than actual buffer length
  out_str = gdv_fn_castVARCHAR_float64_int64(ctx_ptr, 1.2345, 3, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "1.2");
  EXPECT_FALSE(ctx.has_error());
}

TEST(TestGdvFnStubs, TestSubstringIndex) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<gdv_int64>(&ctx);
  gdv_int32 out_len = 0;

  const char* out_str =
      gdv_fn_substring_index(ctx_ptr, "Abc.DE.fGh", 10, ".", 1, 2, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "Abc.DE");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "Abc.DE.fGh", 10, ".", 1, -2, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "fGh");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "S;DCGS;JO!L", 11, ";", 1, 1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "S");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "S;DCGS;JO!L", 11, ";", 1, -1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "DCGS;JO!L");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "www.mysql.com", 13, "Q", 1, 1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "www.mysql.com");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "www||mysql||com", 15, "||", 2, 2, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "www||mysql");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "", 0, ".", 1, 1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len).size(), 0);
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "www||mysql||com", 15, "", 0, 1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len).size(), 0);
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "www||mysql||com", 15, "||", 2, 0, &out_len);
  EXPECT_EQ(std::string(out_str, out_len).size(), 0);
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "www||mysql||com", 15, "||", 2, -2, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "com");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "M??NCHEN", 8, "??", 2, 1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "M");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "M??NCHEN", 8, "??", 2, -1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "NCHEN");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "citro??n", 8, "??", 2, -1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "n");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "citro??n", 8, "??", 2, 1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "citro");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "??????\\L", 8, "\\", 1, 1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "??????");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_substring_index(ctx_ptr, "??????\\L", 8, "\\", 1, -1, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "L");
  EXPECT_FALSE(ctx.has_error());
}

TEST(TestGdvFnStubs, TestUpper) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<gdv_int64>(&ctx);
  gdv_int32 out_len = 0;

  const char* out_str = gdv_fn_upper_utf8(ctx_ptr, "AbcDEfGh", 8, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "ABCDEFGH");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_upper_utf8(ctx_ptr, "asdfj", 5, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "ASDFJ");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_upper_utf8(ctx_ptr, "s;dcGS,jO!l", 11, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "S;DCGS,JO!L");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_upper_utf8(ctx_ptr, "m??nchen", 8, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "M??NCHEN");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_upper_utf8(ctx_ptr, "CITRO??N", 8, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "CITRO??N");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_upper_utf8(ctx_ptr, "??B??D??FGH", 11, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "??B??D??FGH");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_upper_utf8(ctx_ptr, "??hpqR??v??", 11, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "??HPQR??V??");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_upper_utf8(ctx_ptr, "M??beltr??gerf????e", 19, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "M??BELTR??GERF?????E");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_upper_utf8(ctx_ptr, "{??hp,PQ??v}??+", 15, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "{??HP,PQ??V}??+");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_upper_utf8(ctx_ptr, "", 0, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_FALSE(ctx.has_error());

  std::string d("AbOJj??o??\xc3");
  out_str = gdv_fn_upper_utf8(ctx_ptr, d.data(), static_cast<int>(d.length()), &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr(
                  "unexpected byte \\c3 encountered while decoding utf8 string"));
  ctx.Reset();

  std::string e(
      "??b??g\xe0\xa0"
      "??BU??");
  out_str = gdv_fn_upper_utf8(ctx_ptr, e.data(), static_cast<int>(e.length()), &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr(
                  "unexpected byte \\e0 encountered while decoding utf8 string"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestLower) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<gdv_int64>(&ctx);
  gdv_int32 out_len = 0;

  const char* out_str = gdv_fn_lower_utf8(ctx_ptr, "AbcDEfGh", 8, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "abcdefgh");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_lower_utf8(ctx_ptr, "asdfj", 5, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "asdfj");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_lower_utf8(ctx_ptr, "S;DCgs,Jo!L", 11, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "s;dcgs,jo!l");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_lower_utf8(ctx_ptr, "M??NCHEN", 8, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "m??nchen");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_lower_utf8(ctx_ptr, "citro??n", 8, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "citro??n");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_lower_utf8(ctx_ptr, "??b??D??Fgh", 11, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "??b??d??fgh");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_lower_utf8(ctx_ptr, "??HPQr??v??", 11, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "??hpqr??v??");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_lower_utf8(ctx_ptr, "M??BELTR??GERF?????E", 20, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "m??beltr??gerf????e");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_lower_utf8(ctx_ptr, "{??HP,pq??v}??+", 15, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "{??hp,pq??v}??+");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_lower_utf8(ctx_ptr, "", 0, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_FALSE(ctx.has_error());

  std::string d("AbOJj??o??\xc3");
  out_str = gdv_fn_lower_utf8(ctx_ptr, d.data(), static_cast<int>(d.length()), &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr(
                  "unexpected byte \\c3 encountered while decoding utf8 string"));
  ctx.Reset();

  std::string e(
      "??b??g\xe0\xa0"
      "??BU??");
  out_str = gdv_fn_lower_utf8(ctx_ptr, e.data(), static_cast<int>(e.length()), &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr(
                  "unexpected byte \\e0 encountered while decoding utf8 string"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestInitCap) {
  gandiva::ExecutionContext ctx;
  uint64_t ctx_ptr = reinterpret_cast<gdv_int64>(&ctx);
  gdv_int32 out_len = 0;

  const char* out_str = gdv_fn_initcap_utf8(ctx_ptr, "test string", 11, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "Test String");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "asdfj\nhlqf", 10, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "Asdfj\nHlqf");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "s;DCgs,Jo!l", 11, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "S;Dcgs,Jo!L");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, " m??NCHEN", 9, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), " M??nchen");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "citro??n CaR", 12, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "Citro??n Car");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "??b??D??Fgh\n??ll", 16, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "??b??d??fgh\n??ll");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "  ??hpqr??v??  \n\n", 17, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "  ??hpqr??v??  \n\n");
  EXPECT_FALSE(ctx.has_error());

  out_str =
      gdv_fn_initcap_utf8(ctx_ptr, "m??beltr??gerf????e   \nm??beltr??gerf????e", 42, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "M??beltr??gerf????e   \nM??beltr??gerf????e");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "{??HP,pq??v}??+", 15, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "{??hp,Pq??v}??+");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "s??asasds??sd\"sdsd??", 19, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "S??asasds??sd\"Sdsd??");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "mysuperscipt@number??isfine", 27, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "Mysuperscipt@Number??Isfine");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "??<t??as???????????vIYw??", 25, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "??<T??as???????????Viyw??");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "???check???numberisspace", 24, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "???check???numberisspace");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "testing ???Title???case", 23, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "Testing ???title???case");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "??Testing m??odified", 20, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "??Testing M??Odified");
  EXPECT_FALSE(ctx.has_error());

  out_str = gdv_fn_initcap_utf8(ctx_ptr, "", 0, &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_FALSE(ctx.has_error());

  std::string d("AbOJj??o??\xc3");
  out_str =
      gdv_fn_initcap_utf8(ctx_ptr, d.data(), static_cast<int>(d.length()), &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr(
                  "unexpected byte \\c3 encountered while decoding utf8 string"));
  ctx.Reset();

  std::string e(
      "??b??g\xe0\xa0"
      "??BU??");
  out_str =
      gdv_fn_initcap_utf8(ctx_ptr, e.data(), static_cast<int>(e.length()), &out_len);
  EXPECT_EQ(std::string(out_str, out_len), "");
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr(
                  "unexpected byte \\e0 encountered while decoding utf8 string"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastVarbinaryINT) {
  gandiva::ExecutionContext ctx;

  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  EXPECT_EQ(gdv_fn_castINT_varbinary(ctx_ptr, "-45", 3), -45);
  EXPECT_EQ(gdv_fn_castINT_varbinary(ctx_ptr, "0", 1), 0);
  EXPECT_EQ(gdv_fn_castINT_varbinary(ctx_ptr, "2147483647", 10), 2147483647);
  EXPECT_EQ(gdv_fn_castINT_varbinary(ctx_ptr, "\x32\x33", 2), 23);
  EXPECT_EQ(gdv_fn_castINT_varbinary(ctx_ptr, "02147483647", 11), 2147483647);
  EXPECT_EQ(gdv_fn_castINT_varbinary(ctx_ptr, "-2147483648", 11), -2147483648LL);
  EXPECT_EQ(gdv_fn_castINT_varbinary(ctx_ptr, "-02147483648", 12), -2147483648LL);
  EXPECT_EQ(gdv_fn_castINT_varbinary(ctx_ptr, " 12 ", 4), 12);

  gdv_fn_castINT_varbinary(ctx_ptr, "2147483648", 10);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string 2147483648 to int32"));
  ctx.Reset();

  gdv_fn_castINT_varbinary(ctx_ptr, "-2147483649", 11);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string -2147483649 to int32"));
  ctx.Reset();

  gdv_fn_castINT_varbinary(ctx_ptr, "12.34", 5);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string 12.34 to int32"));
  ctx.Reset();

  gdv_fn_castINT_varbinary(ctx_ptr, "abc", 3);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string abc to int32"));
  ctx.Reset();

  gdv_fn_castINT_varbinary(ctx_ptr, "", 0);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string  to int32"));
  ctx.Reset();

  gdv_fn_castINT_varbinary(ctx_ptr, "-", 1);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string - to int32"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastVarbinaryBIGINT) {
  gandiva::ExecutionContext ctx;

  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  EXPECT_EQ(gdv_fn_castBIGINT_varbinary(ctx_ptr, "-45", 3), -45);
  EXPECT_EQ(gdv_fn_castBIGINT_varbinary(ctx_ptr, "0", 1), 0);
  EXPECT_EQ(gdv_fn_castBIGINT_varbinary(ctx_ptr, "9223372036854775807", 19),
            9223372036854775807LL);
  EXPECT_EQ(gdv_fn_castBIGINT_varbinary(ctx_ptr, "09223372036854775807", 20),
            9223372036854775807LL);
  EXPECT_EQ(gdv_fn_castBIGINT_varbinary(ctx_ptr, "-9223372036854775808", 20),
            -9223372036854775807LL - 1);
  EXPECT_EQ(gdv_fn_castBIGINT_varbinary(ctx_ptr, "-009223372036854775808", 22),
            -9223372036854775807LL - 1);
  EXPECT_EQ(gdv_fn_castBIGINT_varbinary(ctx_ptr, " 12 ", 4), 12);

  EXPECT_EQ(gdv_fn_castBIGINT_varbinary(ctx_ptr,
                                        "\x39\x39\x39\x39\x39\x39\x39\x39\x39\x39", 10),
            9999999999LL);

  gdv_fn_castBIGINT_varbinary(ctx_ptr, "9223372036854775808", 19);
  EXPECT_THAT(
      ctx.get_error(),
      ::testing::HasSubstr("Failed to cast the string 9223372036854775808 to int64"));
  ctx.Reset();

  gdv_fn_castBIGINT_varbinary(ctx_ptr, "-9223372036854775809", 20);
  EXPECT_THAT(
      ctx.get_error(),
      ::testing::HasSubstr("Failed to cast the string -9223372036854775809 to int64"));
  ctx.Reset();

  gdv_fn_castBIGINT_varbinary(ctx_ptr, "12.34", 5);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string 12.34 to int64"));
  ctx.Reset();

  gdv_fn_castBIGINT_varbinary(ctx_ptr, "abc", 3);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string abc to int64"));
  ctx.Reset();

  gdv_fn_castBIGINT_varbinary(ctx_ptr, "", 0);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string  to int64"));
  ctx.Reset();

  gdv_fn_castBIGINT_varbinary(ctx_ptr, "-", 1);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string - to int64"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastVarbinaryFloat4) {
  gandiva::ExecutionContext ctx;

  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  EXPECT_EQ(gdv_fn_castFLOAT4_varbinary(ctx_ptr, "-45.34", 6), -45.34f);
  EXPECT_EQ(gdv_fn_castFLOAT4_varbinary(ctx_ptr, "0", 1), 0.0f);
  EXPECT_EQ(gdv_fn_castFLOAT4_varbinary(ctx_ptr, "5", 1), 5.0f);
  EXPECT_EQ(gdv_fn_castFLOAT4_varbinary(ctx_ptr, " 3.4 ", 5), 3.4f);
  EXPECT_EQ(gdv_fn_castFLOAT4_varbinary(ctx_ptr, " \x33\x2E\x34 ", 5), 3.4f);

  gdv_fn_castFLOAT4_varbinary(ctx_ptr, "", 0);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string  to float"));
  ctx.Reset();

  gdv_fn_castFLOAT4_varbinary(ctx_ptr, "e", 1);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string e to float"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestCastVarbinaryFloat8) {
  gandiva::ExecutionContext ctx;

  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);

  EXPECT_EQ(gdv_fn_castFLOAT8_varbinary(ctx_ptr, "-45.34", 6), -45.34);
  EXPECT_EQ(gdv_fn_castFLOAT8_varbinary(ctx_ptr, "0", 1), 0.0);
  EXPECT_EQ(gdv_fn_castFLOAT8_varbinary(ctx_ptr, "5", 1), 5.0);
  EXPECT_EQ(gdv_fn_castFLOAT8_varbinary(ctx_ptr, " \x33\x2E\x34 ", 5), 3.4);

  gdv_fn_castFLOAT8_varbinary(ctx_ptr, "", 0);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string  to double"));
  ctx.Reset();

  gdv_fn_castFLOAT8_varbinary(ctx_ptr, "e", 1);
  EXPECT_THAT(ctx.get_error(),
              ::testing::HasSubstr("Failed to cast the string e to double"));
  ctx.Reset();
}

TEST(TestGdvFnStubs, TestMaskFirstN) {
  gandiva::ExecutionContext ctx;
  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  std::string data = "a???????&";
  auto data_len = static_cast<int32_t>(data.length());
  std::string expected = "x???Xx&";
  const char* result =
      gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 4, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "?????????";
  data_len = static_cast<int32_t>(data.length());
  expected = "?????????";
  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 4, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "a6??????";
  data_len = static_cast<int32_t>(data.length());
  expected = "xnXx??";
  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 4, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "0123456789";
  data_len = static_cast<int32_t>(data.length());
  expected = "nnnnnnnnnn";
  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 10, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  data_len = static_cast<int32_t>(data.length());
  expected = "XXXXXXXXXXXXXXXXXXXXXXXXXX";
  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 26, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "abcdefghijklmnopqrstuvwxyz";
  expected = "xxxxxxxxxxxxxxxxxxxxxxxxxx";
  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 26, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "aB-6";
  data_len = static_cast<int32_t>(data.length());
  expected = "xX-6";
  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 3, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "xX-n";
  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 5, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "aB-6";
  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, -3, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 0, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "ABcd-123456";
  data_len = static_cast<int32_t>(data.length());
  expected = "XXxx-n23456";
  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 6, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "";
  data_len = 0;
  expected = "";
  result = gdv_mask_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 6, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));
}

TEST(TestGdvFnStubs, TestMaskLastN) {
  gandiva::ExecutionContext ctx;
  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  std::string data = "a???????&";
  int32_t data_len = static_cast<int32_t>(data.length());
  std::string expected = "a???Xx&";
  const char* result =
      gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 4, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "ab??????";
  data_len = static_cast<int32_t>(data.length());
  expected = "axXxx";
  result = gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 4, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "0123456789";
  data_len = static_cast<int32_t>(data.length());
  expected = "nnnnnnnnnn";
  result = gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 10, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  data_len = static_cast<int32_t>(data.length());
  expected = "XXXXXXXXXXXXXXXXXXXXXXXXXX";
  result = gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 26, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "abcdefghijklmnopqrstuvwxyz";
  expected = "xxxxxxxxxxxxxxxxxxxxxxxxxx";
  result = gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 26, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "aB-6";
  data_len = static_cast<int32_t>(data.length());
  expected = "aX-n";
  result = gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 3, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "xX-n";
  result = gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 5, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "aB-6";
  result = gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, -3, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  result = gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 0, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "ABcd-123456";
  data_len = static_cast<int32_t>(data.length());
  expected = "ABcd-nnnnnn";
  result = gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 6, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "";
  data_len = 0;
  expected = "";
  result = gdv_mask_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 6, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));
}

TEST(TestGdvFnStubs, TestTranslate) {
  gandiva::ExecutionContext ctx;
  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  std::string expected = "ACACACA";
  const char* result =
      translate_utf8_utf8_utf8(ctx_ptr, "ABABABA", 7, "B", 1, "C", 1, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "acde";
  result = translate_utf8_utf8_utf8(ctx_ptr, "a b c d e", 9, " b", 2, "", 0, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "h3110, h0w ar3 y0u/";
  result = translate_utf8_utf8_utf8(ctx_ptr, "hello, how are you?", 19, "elo?", 4, "310/",
                                    4, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "1b9ef";
  result = translate_utf8_utf8_utf8(ctx_ptr, "abcdef", 6, "adc", 3, "19", 2, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "abcd";
  result = translate_utf8_utf8_utf8(ctx_ptr, "a b c d", 7, " ", 1, "", 0, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "1b9c9e1f";
  result =
      translate_utf8_utf8_utf8(ctx_ptr, "abdcdeaf", 8, "adad", 4, "192", 3, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "012345678";
  result = translate_utf8_utf8_utf8(ctx_ptr, "123456789", 9, "987654321", 9, "0123456789",
                                    10, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "012345678";
  result = translate_utf8_utf8_utf8(ctx_ptr, "987654321", 9, "123456789", 9, "0123456789",
                                    10, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));
}

TEST(TestGdvFnStubs, TestToUtcTimezone) {
  gandiva::ExecutionContext context;
  auto context_ptr = reinterpret_cast<int64_t>(&context);
  auto len_ist = static_cast<gdv_int32>(strlen("Asia/Kolkata"));
  auto len_pst = static_cast<gdv_int32>(strlen("America/Los_Angeles"));

  // ts: 2012-02-28 15:30:00
  // ts2:2012-02-28 10:00:00
  gdv_timestamp ts = 1330443000000;
  gdv_timestamp ts2 = to_utc_timezone_timestamp(context_ptr, ts, "Asia/Kolkata", len_ist);
  EXPECT_EQ(1330423200000, ts2);

  // ts: 1970-01-01 05:00:00
  // ts2:1969-12-31 23:30:00
  ts = 18000000;
  ts2 = to_utc_timezone_timestamp(context_ptr, ts, "Asia/Kolkata", len_ist);
  EXPECT_EQ(ts2, -1800000);

  // daylight savings check
  // ts: 2018-03-11 01:00:00
  // ts2:2018-03-11 09:00:00
  ts = 1520730000000;
  ts2 = to_utc_timezone_timestamp(context_ptr, ts, "America/Los_Angeles", len_pst);
  EXPECT_EQ(ts2, 1520758800000);

  // ts: 2018-03-12 01:00:00
  // ts2:2018-03-12 08:00:00
  ts = 1520816400000;
  ts2 = to_utc_timezone_timestamp(context_ptr, ts, "America/Los_Angeles", len_pst);
  EXPECT_EQ(ts2, 1520841600000);

  // Failure case
  ts2 = to_utc_timezone_timestamp(context_ptr, ts, "America/LA", 10);
  EXPECT_THAT(context.get_error(), "America/LA is an invalid time zone name.");
}

TEST(TestGdvFnStubs, TestFromUtcTimezone) {
  ExecutionContext context;
  auto context_ptr = reinterpret_cast<int64_t>(&context);
  auto len_ist = static_cast<gdv_int32>(strlen("Asia/Kolkata"));
  auto len_pst = static_cast<gdv_int32>(strlen("America/Los_Angeles"));

  // ts: 1970-01-01 10:00:00
  // ts2:1970-01-01 15:30:00
  gdv_timestamp ts = 36000000;
  gdv_timestamp ts2 =
      from_utc_timezone_timestamp(context_ptr, ts, "Asia/Kolkata", len_ist);
  EXPECT_EQ(ts2, 55800000);

  // ts: 1969-12-31 23:30:00
  // ts2:1970-01-01 05:00:00
  ts = -1800000;
  ts2 = from_utc_timezone_timestamp(context_ptr, ts, "Asia/Kolkata", len_ist);
  EXPECT_EQ(ts2, 18000000);

  // ts: 2018-03-11 09:00:00
  // ts2:2018-03-11 01:00:00
  ts = 1520758800000;
  ts2 = from_utc_timezone_timestamp(context_ptr, ts, "America/Los_Angeles", len_pst);
  EXPECT_EQ(ts2, 1520730000000);

  // ts: 2018-03-12 08:00:00
  // ts2:2018-03-12 01:00:00
  ts = 1520841600000;
  ts2 = from_utc_timezone_timestamp(context_ptr, ts, "America/Los_Angeles", len_pst);
  EXPECT_EQ(ts2, 1520816400000);

  // Failure case
  ts2 = from_utc_timezone_timestamp(context_ptr, ts, "India", 5);
  EXPECT_THAT(context.get_error(), "India is an invalid time zone name.");
}

TEST(TestGdvFnStubs, TestShowFirstN) {
  gandiva::ExecutionContext ctx;
  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  std::string data = "a???????&";
  int32_t data_len = static_cast<int32_t>(data.length());
  std::string expected = "a???????&";
  const char* result =
      gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 4, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "ab??????";
  data_len = static_cast<int32_t>(data.length());
  expected = "ab????x";
  result = gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 4, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "0123456789";
  data_len = static_cast<int32_t>(data.length());
  expected = "0123456789";
  result =
      gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 10, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  data_len = static_cast<int32_t>(data.length());
  expected = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  result =
      gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 26, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "abcdefghijklmnopqrstuvwxyz";
  expected = "xxxxxxxxxxxxxxxxxxxxxxxxxx";
  result = gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 0, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "aB-6";
  data_len = static_cast<int32_t>(data.length());
  expected = "aB-n";
  result = gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 3, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "aB-6";
  result = gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 5, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "xX-n";
  result =
      gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, -3, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  result = gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 0, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "ABcd-123456";
  data_len = static_cast<int32_t>(data.length());
  expected = "ABcd-123nnn";
  result = gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 8, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "";
  data_len = 0;
  expected = "";
  result = gdv_mask_show_first_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 6, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));
}

TEST(TestGdvFnStubs, TestShowLastN) {
  gandiva::ExecutionContext ctx;
  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  std::string data = "a???????&";
  auto data_len = static_cast<int32_t>(data.length());
  std::string expected = "x???????&";
  const char* result =
      gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 4, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "?????????";
  data_len = static_cast<int32_t>(data.length());
  expected = "?????????";
  result = gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 4, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "a6??????";
  data_len = static_cast<int32_t>(data.length());
  expected = "xnXx??";
  result = gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 1, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "0123456789";
  data_len = static_cast<int32_t>(data.length());
  expected = "nnnnnnnnnn";
  result = gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 0, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  data_len = static_cast<int32_t>(data.length());
  expected = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  result = gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 26, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "abcdefghijklmnopqrstuvwxyz";
  expected = "xxxxxxxxxxxxxxxxxxxxxxxxxx";
  result = gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 0, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "aB-6";
  data_len = static_cast<int32_t>(data.length());
  expected = "xX-6";
  result = gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 2, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "aB-6";
  result = gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 5, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  expected = "xX-n";
  result = gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, -3, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "ABcd-123456";
  data_len = static_cast<int32_t>(data.length());
  expected = "XXxx-n23456";
  result = gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 5, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));

  data = "";
  data_len = 0;
  expected = "";
  result = gdv_mask_show_last_n_utf8_int32(ctx_ptr, data.c_str(), data_len, 6, &out_len);
  EXPECT_EQ(expected, std::string(result, out_len));
}

TEST(TestGdvFnStubs, TestMask) {
  gandiva::ExecutionContext ctx;
  int64_t ctx_ptr = reinterpret_cast<int64_t>(&ctx);
  int32_t out_len = 0;

  std::string data = "Aabbc??dd-9202";
  std::string expected = "XxxxxXxx-nnnn";
  int32_t data_len = static_cast<int32_t>(data.length());
  const char* result = mask_utf8_utf8_utf8_utf8(ctx_ptr, data.c_str(), data_len, "X", 1,
                                                "x", 1, "n", 1, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  result = mask_utf8_utf8_utf8(ctx_ptr, data.c_str(), data_len, "X", 1, "x", 1, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  result = mask_utf8_utf8(ctx_ptr, data.c_str(), data_len, "X", 1, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  result = mask_utf8(ctx_ptr, data.c_str(), data_len, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);

  data = "QwErTy:4)??";
  expected = "U-l-U-l-U-l-:#)l-";
  data_len = static_cast<int32_t>(data.length());
  result = mask_utf8_utf8_utf8_utf8(ctx_ptr, data.c_str(), data_len, "U-", 2, "l-", 2,
                                    "#", 1, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  expected = "U-l-U-l-U-l-:n)l-";
  result =
      mask_utf8_utf8_utf8(ctx_ptr, data.c_str(), data_len, "U-", 2, "l-", 2, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  expected = "U-xU-xU-x:n)x";
  result = mask_utf8_utf8(ctx_ptr, data.c_str(), data_len, "U-", 2, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  expected = "XxXxXx:n)x";
  result = mask_utf8(ctx_ptr, data.c_str(), data_len, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);

  data = "";
  expected = "";
  data_len = static_cast<int32_t>(data.length());
  result = mask_utf8_utf8_utf8_utf8(ctx_ptr, data.c_str(), data_len, "X", 1, "x", 2, "n",
                                    1, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  result = mask_utf8_utf8_utf8(ctx_ptr, data.c_str(), data_len, "X", 1, "x", 1, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  result = mask_utf8_utf8(ctx_ptr, data.c_str(), data_len, "X", 1, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  result = mask_utf8(ctx_ptr, data.c_str(), data_len, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);

  data = "QwErTy:4)??";
  expected = ":)";
  data_len = static_cast<int32_t>(data.length());
  result = mask_utf8_utf8_utf8_utf8(ctx_ptr, data.c_str(), data_len, "", 0, "", 0, "", 0,
                                    &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  expected = ":n)";
  result = mask_utf8_utf8_utf8(ctx_ptr, data.c_str(), data_len, "", 0, "", 0, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
  expected = "xxx:n)x";
  result = mask_utf8_utf8(ctx_ptr, data.c_str(), data_len, "", 0, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);

  data = "hunny-BEE-5121";
  expected = "*****-\?\?\?-####";
  data_len = static_cast<int32_t>(data.length());
  result = mask_utf8_utf8_utf8_utf8(ctx_ptr, data.c_str(), data_len, "\?", 1, "*", 1, "#",
                                    1, &out_len);
  EXPECT_EQ(std::string(result, out_len), expected);
}

}  // namespace gandiva
