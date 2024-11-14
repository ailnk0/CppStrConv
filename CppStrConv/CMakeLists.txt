cmake_minimum_required(VERSION 3.13)

set(CUR_PROJ CppStrConv)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${CUR_PROJ} INTERFACE)
add_library(bene::${CUR_PROJ} ALIAS ${CUR_PROJ})

target_include_directories(
  ${CUR_PROJ} INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  # -----------------------------------
  # Linux (Clang, GCC)
  # -----------------------------------
  target_compile_options(
    ${CUR_PROJ}
    INTERFACE -Wall # Default warning
              -Wextra # Extra warning
              -Wpedantic # ISO C Standard warning
              -Werror # Treat warning as error
  )

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # -----------------------------------
  # Windows (Visual Studio)
  # -----------------------------------
  target_compile_options(
    ${CUR_PROJ}
    INTERFACE /utf-8 # Treat default encoding
              /W4 # Level 4 warning
              /WX # Treat warning as error
  )

endif()
