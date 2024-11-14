cmake_minimum_required(VERSION 3.13)

set(CUR_PROJ CppStrConvTest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${CUR_PROJ} "src/CppStrConvTest.cpp")

target_link_libraries(${CUR_PROJ} PRIVATE GTest::gtest_main bene::CppStrConv)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  # -----------------------------------
  # Linux (Clang, GCC)
  # -----------------------------------
  target_compile_options(
    ${CUR_PROJ}
    INTERFACE -Wall # Default warning
              -Wextra # Extra warning
              -Wpedantic # ISO C Standard warning
  )

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # -----------------------------------
  # Windows (Visual Studio)
  # -----------------------------------
  target_compile_options(
    ${CUR_PROJ} INTERFACE /utf-8 # Treat default encoding
                          /W4 # Level 4 warning
  )

endif()

include(GoogleTest)
gtest_discover_tests(${CUR_PROJ})
