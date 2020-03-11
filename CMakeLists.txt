cmake_minimum_required(VERSION 3.16)

project(cmake_sample1 LANGUAGES CXX )

set( CMAKE_CXX_FLAGS -std=c++17 ) # ここで c++11 移行を指定しないと gtest のビルドが通らない

file(GLOB SOURCES src/*.cpp) # 通常のソースコード
file(GLOB TEST_SOURCES src/test/*.cpp) # テスト用のソースコード

function(define_build)
  cmake_parse_arguments(
    DEF           # 変数名につく prefix
    "DEBUG;TEST"  # bool 型の変数のリスト
    "COND;TARGET" # 値を一つとる変数のリスト
    ""            # 値を複数取る変数のリスト
    ${ARGN})

  set(TNAME "${DEF_COND}" )
  add_executable( "${TNAME}" )
  target_sources( "${TNAME}"
    PRIVATE
      ${SOURCES}                              # 必ず使うソースコード
      $<$<BOOL:${DEF_TEST}>:${TEST_SOURCES}>  # TESTのときだけ使うソースコード
  )
  set_target_properties("${TNAME}" 
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${DEF_COND}"
      RUNTIME_OUTPUT_NAME "${DEF_TARGET}"
  )
  target_compile_definitions("${TNAME}" PRIVATE
    $<$<NOT:$<BOOL:${DEF_DEBUG}>>:
      # 非DEBUG時のマクロ
      NDEBUG=1>
    $<$<BOOL:${DEF_DEBUG}>:
      # DEBUG時のマクロ
      DEBUG=1;_DEBUG=1>
    $<$<BOOL:${DEF_TEST}>:
      # TEST時のマクロ
      TEST=1>
  )
  target_compile_options( "${TNAME}" PUBLIC
    -Wall
    $<$<NOT:$<BOOL:${DEF_DEBUG}>>:
      # 非DEBUG時のオプション
      -O2>
    $<$<BOOL:${DEF_DEBUG}>:
      # DEBUG時のオプション
      -O0 -g>
  )
  target_link_options( "${TNAME}" PUBLIC
    -lpthread
    $<$<BOOL:${DEF_TEST}>:
      # TEST時のオプション
      --coverage>
  )
  target_compile_features("${TNAME}" PUBLIC
    cxx_std_17
  )
  if( ${DEF_TEST} STREQUAL "TRUE" )
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
    configure_file(CMakeLists.txt.in  lib/googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/googletest-download )
    if(result)
      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/googletest-download )
    if(result)
      message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/lib/googletest-src
                    ${CMAKE_CURRENT_BINARY_DIR}/lib/googletest-build
                    EXCLUDE_FROM_ALL)

    # Now simply link against gtest or gtest_main as needed. Eg
    target_link_libraries("${TNAME}" gtest_main)

  endif()

endfunction()

define_build(
  COND "release"
  TARGET "sample.out")

define_build(
  DEBUG
  COND "debug"
  TARGET "sample.out")

define_build(
  TEST
  DEBUG
  COND "test"
  TARGET "sample.out")

