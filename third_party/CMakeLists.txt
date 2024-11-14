cmake_minimum_required(VERSION 3.14)

include(FetchContent)

if(BENE_CPPSTRCONV_BUILD_TESTS)
  if(NOT TARGET GTest::gtest_main)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://gitlab.hancom.io/pdf-service/googletest
      GIT_TAG v1.15.2)
    set(gtest_force_shared_crt
        ON
        CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
  endif()
endif()
