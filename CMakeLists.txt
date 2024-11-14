# CMakeList.txt : Top-level CMake project file, do global configuration and
# include sub-projects here.
#
cmake_minimum_required(VERSION 3.13)

# -----------------------------------
# Check if the project is top-level
# -----------------------------------
if(NOT DEFINED PROJECT_NAME)
  set(BENE_CPPSTRCONV_IS_TOP_LEVEL ON)
else()
  set(BENE_CPPSTRCONV_IS_TOP_LEVEL OFF)
endif()

# -----------------------------------
# Project Definition
# -----------------------------------
project(
  CppStrConv
  VERSION 0.1.0
  DESCRIPTION
    "A header-only C++ library for string conversion on multi-platforms"
  HOMEPAGE_URL "https://github.com/ailnk0/CppStrConv"
  LANGUAGES CXX)

# -----------------------------------
# Options
# -----------------------------------
option(BENE_CPPSTRCONV_INSTALL "Include an install target"
       ${BENE_CPPSTRCONV_IS_TOP_LEVEL})
option(BENE_CPPSTRCONV_BUILD_TESTS "Build tests"
       ${BENE_CPPSTRCONV_IS_TOP_LEVEL})

# -----------------------------------
# Global Configuration
# -----------------------------------
# enable testing
enable_testing()

# export compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
  )
endif()

# -----------------------------------
# Sub-projects
# -----------------------------------
add_subdirectory("third_party" "lib/third_party")
add_subdirectory("CppStrConv" "lib/CppStrConv")
if(BENE_CPPSTRCONV_BUILD_TESTS)
  add_subdirectory("CppStrConvTest" "lib/CppStrConvTest")
endif()

# -----------------------------------
# Install & Packaging
# -----------------------------------
if(BENE_CPPSTRCONV_INSTALL)
  include(GNUInstallDirs)
  install(TARGETS CppStrConv EXPORT CppStrConvConfig)
  install(
    EXPORT CppStrConvConfig
    NAMESPACE bene::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
  install(DIRECTORY "CppStrConv/include/"
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  set(CMAKE_CONFIG_VERSION_FILE_NAME
      "${CMAKE_CURRENT_BINARY_DIR}/CppStrConvConfig-version.cmake")
  set(CMAKE_CONFIG_FILE_NAME
      "${CMAKE_CURRENT_BINARY_DIR}/CppStrConvConfig.cmake")

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "${CMAKE_CONFIG_VERSION_FILE_NAME}"
    COMPATIBILITY SameMajorVersion ${OPTIONAL_ARCH_INDEPENDENT})

  export(EXPORT CppStrConvConfig NAMESPACE bene::)

  install(FILES "${CMAKE_CONFIG_VERSION_FILE_NAME}"
          DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

  include(InstallRequiredSystemLibraries)
  set(CPACK_PACKAGE_VENDOR "CppStrConv")
  set(CPACK_GENERATOR "STGZ;ZIP")
  set(CPACK_ARCHIVE_TGZ_OPTIONS "--format=gnu")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
endif()

include(CPack)
