# CppStrConv

A header-only C++ library for string conversion across multiple platforms (Windows, Linux, UWP, Android NDK).

The goal of this project is to simplify string encoding handling in straightforward C++ code.

If you are looking for a reliable string handling library for commercial projects, I recommend icu. However, if you want a simple, platform-independent string encoding handling in C++ with a small binary size, this library might be for you.

## 1. Quick Start: Building with CMake

The tutorial aims to quickly create and run a sample for string encoding conversion using CMake.

### 1.1. Requirements

- OS: Windows, Linux, UWP, Android NDK
- Compiler supporting C++11 or higher
- Compiler supporting C++14 or higher (for test code)
- Below C++20
- CMake-compatible build tools (MSVC, Ninja, Make, etc.)

### 1.2. Creating a Custom Project

First, create your project.

```sh
$ mkdir my_project && cd my_project
```

Create a CMakeLists.txt file and add the following:

```cmake
cmake_minimum_required(VERSION 3.13)

project(my_project)

# Fetch CppStrConv
include(FetchContent)
FetchContent_Declare(
  CppStrConv
  GIT_REPOSITORY https://github.com/ailnk0/CppStrConv
  GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(CppStrConv)

# Define executable and sources
add_executable(
  my_app
  main.cpp
)

# Set up linkage
target_link_libraries(
  my_app
  bene::CppStrConv
)
```

### 1.3. Writing Code

Create a main.cpp file and add the following code:

```cpp
#include <iostream>
#include <string>

#include "CppStrConv.h"
using conv = bene::CppStrConv;

int main()
{
  std::u16string u16str = u"Hello World!, 안녕하세요 세상! 你好世界！";

  try {
    std::string str = conv::u16_to_string(u16str);
    std::cout << str << std::endl;
  } catch (const std::invalid_argument& e) {
    std::cout << e.what() << std::endl;
  }
}
```

### 1.4. Build and Run

Build and run the executable.

```sh
$ cmake -S . -B build
$ cmake --build build
$ cd build
$ ./my_app
```

You should see the following output in the console:

```
hello world!, 안녕하세요 세상! 你好世界！
```

## 2. Testing

If you configure CppStrConv as the root project or set the BENE_CPPSTRCONV_BUILD_TESTS option to ON, the test code is available.

Below I will show you how to configure CppStrConv as the root project and then run the tests.

First, clone the project.

```sh
$ git clone https://github.com/ailnk0/CppStrConv
$ cd CppStrConv
```

Then, build the project and run the test code.

```sh
$ cmake -S . -B build
$ cmake --build build
$ cd build/${your_build_dir}
$ ./CppStrConvTest
```

If a test fails, please report the issue along with details about your test environment for review and correction.

## 3. String Encoding Guide

| String Type      | Char Type  | Char Size (Win) | Char Size (Other) | Encoding       | Req   |
| ---------------- | ---------- | --------------- | ----------------- | -------------- | ----- |
| `std::string`    | `char`     | 1 byte          | 1 byte            | MBCS or UTF-8  |       |
| `std::wstring`   | `wchar_t`  | 2 bytes         | 2 or 4 bytes      | UCS-2 or UCS-4 |       |
| `std::u16string` | `char16_t` | 2 bytes         | 2 bytes           | UTF-16         | C++11 |
| `std::u32string` | `char32_t` | 4 bytes         | 4 bytes           | UTF-32         | C++11 |
| `std::u8string`  | `char8_t`  | 1 byte          | 1 byte            | UTF-8          | C++20 |

## 4. API List

If a problem occurs during encoding conversion, `std::invalid_argument` or `std::runtime_error` is thrown.

```cpp
std::string u16_to_u8(const std::u16string& str);
std::string u32_to_u8(const std::u32string& str);
std::u16string u8_to_u16(const std::string& str);
std::u16string u32_to_u16(const std::u32string& str);
std::u32string u8_to_u32(const std::string& str);
std::u32string u16_to_u32(const std::u16string& str);
std::string wstring_to_string(const std::wstring& str);
std::wstring string_to_wstring(const std::string& str);
std::u16string string_to_u16(const std::string& str);
std::u16string wstring_to_u16(const std::wstring& str);
std::string u16_to_string(const std::u16string& str);
std::wstring u16_to_wstring(const std::u16string& str);
std::vector<char> u16_to_u8_bytes(const std::u16string& str, bool addBom = false);
std::vector<char> u16_to_u16_bytes(const std::u16string& str, bool addBom = false);
std::vector<char> u16_to_u16be_bytes(const std::u16string& str, bool addBom);
std::vector<char> u16_to_u16le_bytes(const std::u16string& str, bool addBom);
std::vector<char> u32_to_u32_bytes(const std::u32string& str);
std::vector<char> u16_to_u32_bytes(const std::u16string& str);
std::vector<char> u16_to_us_ascii_bytes(const std::u16string& str);
std::vector<char> u16_to_iso_8859_1_bytes(const std::u16string& str);
std::vector<char> u16_to_iso_10646_bytes(const std::u16string& str);
std::vector<char> u32_to_iso_10646_bytes(const std::u32string& str);
std::u16string u8_bytes_to_u16(const std::vector<char>& bytes);
std::u16string u16_bytes_to_u16(const std::vector<char>& bytes);
std::u16string u16be_bytes_to_u16(const std::vector<char>& bytes);
std::u16string u16le_bytes_to_u16(const std::vector<char>& bytes);
std::u16string us_ascii_bytes_to_u16(const std::vector<char>& bytes);
std::u16string iso_8859_1_bytes_to_u16(const st std::vector<char>& bytes);
std::u32string u32_bytes_to_u32(const std::vector<char>& bytes) ;
std::u32string iso_10646_bytes_to_u32(const std::vector<char>& bytes);
std::u16string iso_10646_bytes_to_u16(const std::vector<char>& bytes);
bool is_start_with_bom_u16le(const std::vector<char>& bytes);
bool is_start_with_bom_u16be(const std::vector<char>& bytes);
bool is_start_with_bom_u8(const std::vector<char>& bytes);
void validiate_u8(const std::string& str);
void validate_u16(const std::u16string& str);
void validate_u32(const std::u32string& str);
```
