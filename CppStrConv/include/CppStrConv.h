/**
 * CppStrConv
 * A header-only C++ library for string conversion on multi-platforms
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2024, Benedict
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BENE_CPPSTRCONV_H_
#define BENE_CPPSTRCONV_H_

#include <array>
#include <codecvt>
#include <locale>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace bene {

static const std::array<char, 3> BOM_UTF8 = {'\xef', '\xbb', '\xbf'};
static const std::array<char, 2> BOM_UTF16BE = {'\xfe', '\xff'};
static const std::array<char, 2> BOM_UTF16LE = {'\xff', '\xfe'};

class CppStrConv {
 public:
  static std::string u16_to_u8(const std::u16string& str) {
    validate_u16(str);
#ifdef _WIN32
    std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
    auto p = reinterpret_cast<const int16_t*>(str.data());
    return convert.to_bytes(p, p + str.size());
#else
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes(str);
#endif
  }

  static std::string u32_to_u8(const std::u32string& str) {
    validate_u32(str);
#ifdef _WIN32
    std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> convert;
    auto p = reinterpret_cast<const int32_t*>(str.data());
    return convert.to_bytes(p, p + str.size());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(str);
#endif
  }

  static std::u16string u8_to_u16(const std::string& str) {
    validiate_u8(str);
#ifdef _WIN32
    std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
    auto asInt = convert.from_bytes(str);
    return std::u16string(reinterpret_cast<char16_t const*>(asInt.data()),
                          asInt.length());
#else
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.from_bytes(str);
#endif
  }

  static std::u32string u8_to_u32(const std::string& str) {
    validiate_u8(str);
#ifdef _WIN32
    std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> convert;
    auto asInt = convert.from_bytes(str);
    return std::u32string(reinterpret_cast<char32_t const*>(asInt.data()),
                          asInt.length());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.from_bytes(str);
#endif
  }

  static std::u16string u32_to_u16(const std::u32string& str) {
    return u8_to_u16(u32_to_u8(str));
  }

  static std::u32string u16_to_u32(const std::u16string& str) {
    return u8_to_u32(u16_to_u8(str));
  }

  static std::string wstring_to_string(const std::wstring& str) {
#ifdef _WIN32
    // Convert UTF-16 (2 char) to MBCS (1 char)
    int length = WideCharToMultiByte(CP_ACP, 0, str.data(), -1, nullptr, 0,
                                     nullptr, nullptr);
    if (length <= 0) {
      return std::string();
    }
    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(length);
    WideCharToMultiByte(CP_ACP, 0, str.data(), -1, buffer.get(), length,
                        nullptr, nullptr);
    return std::string(buffer.get());
#else
    // Convert UCS-4 (4 char) to UTF-8 (1 char)
    std::u32string temp(str.begin(), str.end());
    return u32_to_u8(temp);
#endif
  }

  static std::wstring string_to_wstring(const std::string& str) {
#ifdef _WIN32
    // Convert MBCS (1 char) to UCS-2 (2 char)
    int length = MultiByteToWideChar(CP_ACP, 0, str.data(), -1, nullptr, 0);
    if (length <= 0) {
      return std::wstring();
    }
    std::unique_ptr<wchar_t[]> buffer = std::make_unique<wchar_t[]>(length);
    MultiByteToWideChar(CP_ACP, 0, str.data(), -1, buffer.get(), length);
    return std::wstring(buffer.get());
#else
    // Convert UTF-8 (1 char) to UCS-4 (4 char)
    std::u32string temp = u8_to_u32(str);
    return std::wstring(temp.begin(), temp.end());
#endif
  }

  static std::u16string string_to_u16(const std::string& str) {
#ifdef _WIN32
    // Convert MBCS (1 char) to UTF-16 (2 char)
    std::wstring temp = string_to_wstring(str);
    return std::u16string(temp.begin(), temp.end());
#else
    // Convert UTF-8 (1 char) to UTF-16 (2 char)
    return u8_to_u16(str);
#endif
  }

  static std::u16string wstring_to_u16(const std::wstring& str) {
#ifdef _WIN32
    // Convert UCS-2 (2 char) to UTF-16 (2 char)
    return std::u16string(str.begin(), str.end());
#else
    // Convert UCS-4 (4 char) to UTF-16 (2 char)
    std::u32string temp(str.begin(), str.end());
    return u32_to_u16(temp);
#endif
  }

  static std::string u16_to_string(const std::u16string& str) {
    validate_u16(str);
#ifdef _WIN32
    // Convert UTF-16 (2 char) to MBCS (1 char)
    std::wstring temp(str.begin(), str.end());
    return wstring_to_string(temp);
#else
    // Convert UTF-16 (2 char) to UTF-8 (1 char)
    return u16_to_u8(str);
#endif
  }

  static std::wstring u16_to_wstring(const std::u16string& str) {
    validate_u16(str);
#ifdef _WIN32
    // Convert UTF-16 (2 char) to UCS-2 (2 char)
    return std::wstring(str.begin(), str.end());
#else
    // Convert UTF-16 (2 char) to UCS-4 (4 char)
    std::u32string temp = u16_to_u32(str);
    return std::wstring(temp.begin(), temp.end());
#endif
  }

  static std::vector<char> u16_to_u8_bytes(const std::u16string& str,
                                           bool addBom = false) {
    std::string u8str = u16_to_u8(str);
    size_t offset = addBom ? BOM_UTF8.size() : 0;
    size_t length = u8str.size() + offset;
    std::vector<char> bytes;
    bytes.resize(length);
    if (addBom) {
      std::copy(BOM_UTF8.begin(), BOM_UTF8.end(), bytes.begin());
    }
    std::copy(u8str.begin(), u8str.end(), bytes.begin() + offset);
    return bytes;
  }

  static std::vector<char> u16_to_u16_bytes(const std::u16string& str,
                                            bool addBom = false) {
    return u16_to_u16be_bytes(str, addBom);
  }

  static std::vector<char> u16_to_u16be_bytes(const std::u16string& str,
                                              bool addBom) {
    validate_u16(str);
    size_t offset = addBom ? BOM_UTF16BE.size() : 0;
    size_t length = str.size() * 2 + offset;
    std::vector<char> bytes;
    bytes.resize(length);
    if (addBom) {
      std::copy(BOM_UTF16BE.begin(), BOM_UTF16BE.end(), bytes.begin());
    }
    char* ptr = bytes.data() + offset;
    for (auto& c : str) {
      *ptr++ = (c >> 8) & 0xff;
      *ptr++ = c & 0xff;
    }
    return bytes;
  }

  static std::vector<char> u16_to_u16le_bytes(const std::u16string& str,
                                              bool addBom) {
    validate_u16(str);
    size_t offset = addBom ? BOM_UTF16LE.size() : 0;
    size_t length = str.size() * 2 + offset;
    std::vector<char> bytes;
    bytes.resize(length);
    if (addBom) {
      std::copy(BOM_UTF16BE.begin(), BOM_UTF16BE.end(), bytes.begin());
    }
    char* ptr = bytes.data() + offset;
    for (auto& c : str) {
      *ptr++ = c & 0xff;
      *ptr++ = (c >> 8) & 0xff;
    }
    return bytes;
  }

  static std::vector<char> u32_to_u32_bytes(const std::u32string& str) {
    validate_u32(str);
    std::vector<char> bytes;
    bytes.resize(str.size() * 4);
    size_t index = 0;
    for (char32_t c : str) {
      bytes[index++] = (c >> 24) & 0xFF;
      bytes[index++] = (c >> 16) & 0xFF;
      bytes[index++] = (c >> 8) & 0xFF;
      bytes[index++] = c & 0xFF;
    }
    return bytes;
  }

  static std::vector<char> u16_to_u32_bytes(const std::u16string& str) {
    std::u32string u32str = u16_to_u32(str);
    return u32_to_u32_bytes(u32str);
  }

  static std::vector<char> u16_to_us_ascii_bytes(const std::u16string& str) {
    auto length = str.size();
    std::vector<char> bytes;
    bytes.resize(length);
    for (size_t i = 0; i < length; ++i) {
      bytes[i] = (str[i] <= 0x7F) ? static_cast<char>(str[i]) : '?';
    }
    return bytes;
  }

  static std::vector<char> u16_to_iso_8859_1_bytes(const std::u16string& str) {
    auto length = str.size();
    std::vector<char> bytes;
    bytes.resize(length);
    for (size_t i = 0; i < length; ++i) {
      bytes[i] = (str[i] <= 0xff) ? static_cast<char>(str[i]) : '?';
    }
    return bytes;
  }

  static std::vector<char> u16_to_iso_10646_bytes(const std::u16string& str) {
    return u16_to_u32_bytes(str);
  }

  static std::vector<char> u32_to_iso_10646_bytes(const std::u32string& str) {
    return u32_to_u32_bytes(str);
  }

  static std::u16string u8_bytes_to_u16(const std::vector<char>& bytes) {
    if (is_start_with_bom_u8(bytes)) {
      std::vector<char> bytesExceptBom(bytes.begin() + BOM_UTF8.size(),
                                       bytes.end());
      return u8_bytes_to_u16(bytesExceptBom);
    } else if (is_start_with_bom_u16le(bytes)) {
      throw std::invalid_argument("Invalid BOM for UTF-16LE");
    } else if (is_start_with_bom_u16be(bytes)) {
      throw std::invalid_argument("Invalid BOM for UTF-16BE");
    } else {
      return u8_bytes_to_u16(bytes);
    }
  }

  static std::u16string u16_bytes_to_u16(const std::vector<char>& bytes) {
    if (is_start_with_bom_u8(bytes)) {
      throw std::invalid_argument("Invalid BOM for UTF-8");
    } else if (is_start_with_bom_u16le(bytes)) {
      std::vector<char> bytesExceptBom(bytes.begin() + BOM_UTF16LE.size(),
                                       bytes.end());
      return u16le_bytes_to_u16(bytesExceptBom);
    } else if (is_start_with_bom_u16be(bytes)) {
      std::vector<char> bytesExceptBom(bytes.begin() + BOM_UTF16BE.size(),
                                       bytes.end());
      return u16be_bytes_to_u16(bytesExceptBom);
    } else {
      return u16be_bytes_to_u16(bytes);
    }
  }

  static std::u16string u16be_bytes_to_u16(const std::vector<char>& bytes) {
    std::u16string u16str;
    u16str.resize(bytes.size() / 2);
    auto length = u16str.size();

    for (size_t i = 0; i < length; ++i) {
      u16str[i] = static_cast<char16_t>(
          (static_cast<unsigned char>(bytes[i * 2]) << 8) |
          static_cast<unsigned char>(bytes[i * 2 + 1]));
    }

    return u16str;
  }

  static std::u16string u16le_bytes_to_u16(const std::vector<char>& bytes) {
    return std::u16string(reinterpret_cast<const char16_t*>(bytes.data()));
  }

  static std::u16string us_ascii_bytes_to_u16(const std::vector<char>& bytes) {
    std::u16string u16str;
    u16str.assign(bytes.begin(), bytes.end());
    return u16str;
  }

  static std::u16string iso_8859_1_bytes_to_u16(
      const std::vector<char>& bytes) {
    auto length = bytes.size();
    std::u16string u16str;
    u16str.resize(length);
    for (size_t i = 0; i < length; ++i) {
      u16str[i] = bytes[i] & 0xff;
    }
    return u16str;
  }

  static std::u32string u32_bytes_to_u32(const std::vector<char>& bytes) {
    if (bytes.size() % 4 != 0) {
      throw std::invalid_argument(
          "Invalid byte vector length for UTF-32 conversion");
    }

    std::u32string result;
    result.resize(bytes.size() / 4);

    for (size_t i = 0; i < bytes.size(); i += 4) {
      char32_t code_point = (static_cast<unsigned char>(bytes[i]) << 24) |
                            (static_cast<unsigned char>(bytes[i + 1]) << 16) |
                            (static_cast<unsigned char>(bytes[i + 2]) << 8) |
                            static_cast<unsigned char>(bytes[i + 3]);
      result[i / 4] = code_point;
    }

    return result;
  }

  static std::u32string iso_10646_bytes_to_u32(const std::vector<char>& bytes) {
    return u32_bytes_to_u32(bytes);
  }

  static std::u16string iso_10646_bytes_to_u16(const std::vector<char>& bytes) {
    std::u32string u32str = u32_bytes_to_u32(bytes);
    return u32_to_u16(u32str);
  }

  static bool is_start_with_bom_u16le(const std::vector<char>& bytes) {
    return bytes.size() >= BOM_UTF16LE.size() &&
           std::equal(BOM_UTF16LE.begin(), BOM_UTF16LE.end(), bytes.begin());
  }

  static bool is_start_with_bom_u16be(const std::vector<char>& bytes) {
    return bytes.size() >= BOM_UTF16BE.size() &&
           std::equal(BOM_UTF16BE.begin(), BOM_UTF16BE.end(), bytes.begin());
  }

  static bool is_start_with_bom_u8(const std::vector<char>& bytes) {
    return bytes.size() >= BOM_UTF8.size() &&
           std::equal(BOM_UTF8.begin(), BOM_UTF8.end(), bytes.begin());
  }

  static void validiate_u8(const std::string& str) {
    // Validate UTF-8 sequence for overlong encodings
    for (size_t i = 0; i < str.size();) {
      unsigned char byte = str[i];
      size_t remainingBytes;

      // Check the number of bytes in the UTF-8 character based on the first
      // byte
      if ((byte & 0x80) == 0) {
        remainingBytes = 0;
      } else if ((byte & 0xE0) == 0xC0) {
        // 2-byte character, should be in the range U+0080 to U+07FF
        if (byte == 0xC0 || byte == 0xC1) {  // Overlong encoding for ASCII
          throw std::invalid_argument(
              "Invalid UTF-8 sequence: overlong encoding");
        }
        remainingBytes = 1;
      } else if ((byte & 0xF0) == 0xE0) {
        // 3-byte character, should be in the range U+0800 to U+FFFF
        if (byte == 0xE0 &&
            (str[i + 1] & 0xE0) ==
                0x80) {  // Overlong encoding for U+0080 to U+07FF
          throw std::invalid_argument(
              "Invalid UTF-8 sequence: overlong encoding");
        }
        remainingBytes = 2;
      } else if ((byte & 0xF8) == 0xF0) {
        // 4-byte character, should be in the range U+10000 to U+10FFFF
        if (byte == 0xF0 &&
            (str[i + 1] & 0xF0) ==
                0x80) {  // Overlong encoding for U+0800 to U+FFFF
          throw std::invalid_argument(
              "Invalid UTF-8 sequence: overlong encoding");
        }
        remainingBytes = 3;
      } else {
        throw std::invalid_argument(
            "Invalid UTF-8 sequence: invalid start byte");
      }

      // Check for correct continuation bytes
      for (size_t j = 1; j <= remainingBytes; ++j) {
        if (i + j >= str.size() || (str[i + j] & 0xC0) != 0x80) {
          throw std::invalid_argument(
              "Invalid UTF-8 sequence: missing continuation byte");
        }
      }
      i += remainingBytes + 1;
    }
  }

  static void validate_u16(const std::u16string& str) {
    size_t i = 0;
    while (i < str.size()) {
      char16_t c = str[i];
      if (c >= 0xD800 && c <= 0xDBFF) {
        if (i + 1 >= str.size() || str[i + 1] < 0xDC00 || str[i + 1] > 0xDFFF) {
          throw std::invalid_argument(
              "Invalid UTF-16 sequence: lone high surrogate");
        }
        i += 2;
      } else if (c >= 0xDC00 && c <= 0xDFFF) {
        throw std::invalid_argument(
            "Invalid UTF-16 sequence: lone low surrogate");
      } else {
        ++i;
      }
    }
  }

  static void validate_u32(const std::u32string& str) {
    for (char32_t ch : str) {
      if (ch > 0x10FFFF) {
        throw std::invalid_argument(
            "The string contains invalid UTF-32 code points.");
      }
    }
  }
};

}  // namespace bene

#endif  // include guard
