cmake_minimum_required(VERSION 3.16)

project(cmake_sample1 LANGUAGES CXX )

set( CMAKE_CXX_FLAGS -std=c++17 ) # ここで c++11 移行を指定しないと gtest のビルドが通らない
set( SRC_WILDCARDS src/*.cpp ) # 通常のソースコード
file(GLOB TEST_SOURCES src/test/*.cpp) # テスト用のソースコード

file(GLOB SOURCES ${SRC_WILDCARDS})
file(GLOB REL_SOURCES 
  RELATIVE ${CMAKE_CURRENT_BINARY_DIR}
  ${SRC_WILDCARDS})

include(CMakeLists.gtest.txt)

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
      TESTBUILD=1>
  )
  target_compile_options( "${TNAME}"
    PRIVATE
      $<$<NOT:$<BOOL:${DEF_DEBUG}>>:
        # 非DEBUG時のオプション
        -O2>
      $<$<BOOL:${DEF_DEBUG}>:
        # DEBUG時のオプション
        -O0 -g>
    PUBLIC
      -Wall
      $<$<BOOL:${DEF_TEST}>:
        # TEST時のオプション
        -g --coverage -fno-elide-constructors  >
  )
  target_include_directories("${TNAME}" PRIVATE
    "src"
  )
  target_link_options( "${TNAME}" PUBLIC
    -Wall -lpthread
    $<$<BOOL:${DEF_TEST}>:
      # TEST時のオプション
       --coverage -fno-elide-constructors >
  )
  target_compile_features("${TNAME}" PUBLIC
    cxx_std_17
  )
  if( ${DEF_TEST} STREQUAL "TRUE" )
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

define_build(
  TEST
  COND "reltest"
  TARGET "sample.out")

add_custom_target(coverage
  DEPENDS test ./coverage.sh
  COMMAND rm -f CMakeFiles/test.dir/**/*.gcda
  COMMAND mkdir -p gcov
  COMMAND rm -f gcov/*.gcov
  COMMAND test/sample.out
  COMMAND ./coverage.sh ${REL_SOURCES}
  COMMAND mv *.gcov gcov)

