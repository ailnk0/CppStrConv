#include "CppStrConv.h"

#include <gtest/gtest.h>

namespace bene {

TEST(CppStrConvTest, u16_bytes_to_u16_Success) {
  // Given
  std::vector<char> input = {'\x00', '\x41', '\x30', '\x42', '\xD8',
                             '\x3D', '\xDE', '\x00'};  // 'A', 'あ', '😀'
  std::u16string expected = u"Aあ😀";

  // When
  std::u16string result = CppStrConv::u16_bytes_to_u16(input);

  // Then
  EXPECT_EQ(result, expected);
}

TEST(CppStrConvTest, u16_bytes_to_u16_InvalidBOM) {
  // Given
  std::vector<char> invalid_bom = {'\xef', '\xbb', '\xbf'};

  // When/Then
  EXPECT_THROW(CppStrConv::u16_bytes_to_u16(invalid_bom),
               std::invalid_argument);
}

TEST(CppStrConvTest, u16_to_u8_EmptyString) {
  // Given
  std::u16string utf16_str = u"";

  // When
  std::string utf8_str = CppStrConv::u16_to_u8(utf16_str);

  // Then
  EXPECT_EQ(utf8_str, "");
}

TEST(CppStrConvTest, u16_to_u8_InvalidU16) {
  // Given
  std::u16string invalid_utf16_high_surrogate = {
      0xD800};                                            // Lone high surrogate
  std::u16string invalid_utf16_low_surrogate = {0xDC00};  // Lone low surrogate

  // When/Then
  EXPECT_THROW(CppStrConv::u16_to_u8(invalid_utf16_high_surrogate),
               std::invalid_argument);
  EXPECT_THROW(CppStrConv::u16_to_u8(invalid_utf16_low_surrogate),
               std::invalid_argument);
}

TEST(CppStrConvTest, u16_to_u8_bytes_WithBOM) {
  // Given
  std::u16string utf16_str = u"Test BOM";

  // When
  std::vector<char> utf8_bytes = CppStrConv::u16_to_u8_bytes(utf16_str, true);

  // Then
  EXPECT_TRUE(CppStrConv::is_start_with_bom_u8(utf8_bytes));
}

TEST(CppStrConvTest, u16_to_us_ascii_bytes_Success) {
  // Given
  std::u16string ascii_str = u"ASCII Text";
  std::vector<char> ascii_bytes = CppStrConv::u16_to_us_ascii_bytes(ascii_str);

  // When
  std::string result(ascii_bytes.begin(), ascii_bytes.end());

  // Then
  EXPECT_EQ(result, "ASCII Text");
}

TEST(CppStrConvTest, u16_to_us_ascii_bytes_Invalid) {
  // Given
  std::u16string non_ascii = u"Hello, ñ";  // ñ is not ASCII

  // When
  std::vector<char> ascii_bytes = CppStrConv::u16_to_us_ascii_bytes(non_ascii);
  std::string result(ascii_bytes.begin(), ascii_bytes.end());

  // Then
  EXPECT_EQ(result, "Hello, ?");
}

TEST(CppStrConvTest, u32_bytes_to_u32_InvalidLength) {
  // Given
  std::vector<char> invalid_input = {0x00, 0x00, 0x00};

  // When/Then
  EXPECT_THROW(CppStrConv::u32_bytes_to_u32(invalid_input),
               std::invalid_argument);
}

TEST(CppStrConvTest, u32_bytes_to_u32_Success) {
  // Given
  std::vector<char> input = {'\x00', '\x00', '\x00', '\x41', '\x00',
                             '\x00', '\x30', '\x42', '\x00', '\x01',
                             '\xF6', '\x00'};  // 'A', 'あ', '😀'
  std::u32string expected = U"Aあ😀";

  // When
  std::u32string result = CppStrConv::u32_bytes_to_u32(input);

  // Then
  EXPECT_EQ(result, expected);
}

TEST(CppStrConvTest, u32_to_u32_bytes_Success) {
  // Given
  std::u32string input = U"Aあ😀";
  std::vector<char> expected = {'\x00', '\x00', '\x00', '\x41', '\x00', '\x00',
                                '\x30', '\x42', '\x00', '\x01', '\xF6', '\x00'};

  // When
  std::vector<char> result = CppStrConv::u32_to_u32_bytes(input);

  // Then
  EXPECT_EQ(result, expected);
}

TEST(CppStrConvTest, u32_to_u8_InvalidU32) {
  // Given
  std::u32string invalid_utf32 = {0x110000};  // Above U+10FFFF

  // When/Then
  EXPECT_THROW(CppStrConv::u32_to_u8(invalid_utf32), std::invalid_argument);
}

TEST(CppStrConvTest, u32_to_u8_Success) {
  // Given
  std::u32string utf32_str = U"Hello, 안녕하세요, こんにちは, 你好, 😘";
  std::string expected = u8"Hello, 안녕하세요, こんにちは, 你好, 😘";

  // When
  std::string utf8_str = CppStrConv::u32_to_u8(utf32_str);

  // Then
  EXPECT_EQ(utf8_str, expected);
}

TEST(CppStrConvTest, u8_to_u16_EmptyString) {
  // Given
  std::string empty_str;

  // When
  std::u16string utf16_str = CppStrConv::u8_to_u16(empty_str);

  // Then
  EXPECT_EQ(utf16_str, u"");
}

TEST(CppStrConvTest, u8_to_u16_InvalidUTF8) {
  // Given
  std::string invalid_utf8 = "\x80";               // Invalid byte
  std::string invalid_utf8_3bytes = "\xE2\x82";    // Incomplete 3-byte sequence
  std::string overlong_utf8 = "\xF0\x80\x80\x80";  // Overlong encoding

  // When/Then
  EXPECT_THROW(CppStrConv::u8_to_u16(invalid_utf8), std::invalid_argument);
  EXPECT_THROW(CppStrConv::u8_to_u16(invalid_utf8_3bytes),
               std::invalid_argument);
  EXPECT_THROW(CppStrConv::u8_to_u16(overlong_utf8), std::invalid_argument);
}

TEST(CppStrConvTest, u8_to_u16_Success) {
  // Given
  std::string utf8_str = u8"Hello, 안녕하세요, こんにちは, 你好, 😘";
  std::u16string expected = u"Hello, 안녕하세요, こんにちは, 你好, 😘";

  // When
  std::u16string utf16_str = CppStrConv::u8_to_u16(utf8_str);

  // Then
  EXPECT_EQ(utf16_str, expected);
}

TEST(CppStrConvTest, u8_bytes_to_u16_InvalidBOM) {
  // Given
  std::vector<char> invalid_utf16_bom = {'\xfe', '\xff'};

  // When/Then
  EXPECT_THROW(CppStrConv::u8_bytes_to_u16(invalid_utf16_bom),
               std::invalid_argument);
}

}  // namespace bene
