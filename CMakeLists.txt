cmake_minimum_required(VERSION 3.16)

project(cmake_sample1 LANGUAGES CXX )

# source codes
set( SRC_WILDCARDS src/*.cpp )
set( TEST_SRC_WILDCARDS src/test/*.cpp )

# macros
set(MACRO "")
set(NDEBUG_MACRO NDEBUG=1)
set(DEBUG_MACRO DEBUG=1;_DEBUG=1)
set(TEST_MACRO TESTBUILD=1)

# compile options
set(COMPLIE_OPT -Wall)
set(NDEBUG_COMPLIE_OPT -O2)
set(DEBUG_COMPLIE_OPT -g -O0)
set(TEST_COMPLIE_OPT -g --coverage -fno-elide-constructors)

# link options
set(LINK_OPT -Wall -lpthread)
set(NDEBUG_LINK_OPT "")
set(DEBUG_LINK_OPT "")
set(TEST_LINK_OPT --coverage -fno-elide-constructors)

set( CMAKE_CXX_FLAGS -std=c++17 ) # ここで c++11 以降を指定しないと gtest のビルドが通らない

file(GLOB SOURCES ${SRC_WILDCARDS})
file(GLOB REL_SOURCES 
  RELATIVE ${CMAKE_CURRENT_BINARY_DIR}
  ${SRC_WILDCARDS})
file(GLOB TEST_SOURCES ${TEST_SRC_WILDCARDS})

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
      ${SOURCES}
      $<$<BOOL:${DEF_TEST}>:${TEST_SOURCES}>
  )
  set_target_properties("${TNAME}" 
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${DEF_COND}"
      RUNTIME_OUTPUT_NAME "${DEF_TARGET}"
  )
  target_compile_definitions("${TNAME}"
    PRIVATE
      ${MACRO}
      $<$<NOT:$<BOOL:${DEF_DEBUG}>>:${NDEBUG_MACRO}>
      $<$<BOOL:${DEF_DEBUG}>:${DEBUG_MACRO}>
      $<$<BOOL:${DEF_TEST}>:${TEST_MACRO}>
  )
  target_compile_options( "${TNAME}"
    PRIVATE
      ${COMPLIE_OPT}
      $<$<NOT:$<BOOL:${DEF_DEBUG}>>:${NDEBUG_COMPLIE_OPT}>
      $<$<BOOL:${DEF_DEBUG}>:${DEBUG_COMPLIE_OPT}>
      $<$<BOOL:${DEF_TEST}>:${TEST_COMPLIE_OPT}>
  )
  target_include_directories("${TNAME}" PRIVATE
    "src"
  )
  target_link_options( "${TNAME}"
    PUBLIC
      ${LINK_OPT}
      $<$<NOT:$<BOOL:${DEF_DEBUG}>>:${NDEBUG_LINK_OPT}>
      $<$<BOOL:${DEF_DEBUG}>:${DEBUG_LINK_OPT}>
      $<$<BOOL:${DEF_TEST}>:${TEST_LINK_OPT}>
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

