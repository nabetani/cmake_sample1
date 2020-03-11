cmake_minimum_required(VERSION 3.16)

project( cmake_sample1 CXX)

file(GLOB SOURCES src/*.cpp)

function(define_build)
  cmake_parse_arguments(DEF "DEBUG" "COND" "TARGET" ${ARGN})

  set(TNAME "${DEF_COND}.${DEF_TARGET}" )
  add_executable( "${TNAME}" )
  target_sources( "${TNAME}"
    PRIVATE ${SOURCES}
  )
  set_target_properties("${TNAME}" 
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${DEF_COND}"
      RUNTIME_OUTPUT_NAME "${DEF_TARGET}"
  )
  target_compile_definitions("${TNAME}" PRIVATE
    $<$<NOT:$<BOOL:${DEF_DEBUG}>>:NDEBUG=1>
    $<$<BOOL:${DEF_DEBUG}>:DEBUG=1>
    $<$<BOOL:${DEF_DEBUG}>:_DEBUG=1>
  )
  target_compile_options( "${TNAME}" PUBLIC
    -Wall
    $<$<NOT:$<BOOL:${DEF_DEBUG}>>:-O2>
    $<$<BOOL:${DEF_DEBUG}>:-O0 -g>
  )
  target_compile_features("${TNAME}" PUBLIC
    cxx_std_17
  )

endfunction()

define_build(
  COND "release"
  TARGET "sample.out")

define_build(
  DEBUG
  COND "debug"
  TARGET "sample.out")

