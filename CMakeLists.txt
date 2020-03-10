cmake_minimum_required(VERSION 3.16)

project( cmake_sample1 CXX)

function(define_build cond target)
  message( "${cond} -- ${target}")
  set(TNAME "${cond}.${target}" )
  add_executable( "${TNAME}" src/main.cpp )
  set_target_properties("${TNAME}" 
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${cond}"
      RUNTIME_OUTPUT_NAME "${target}"
  )
  target_compile_definitions("${TNAME}" PUBLIC
    $<$<STREQUAL:release,${cond}>:NDEBUG=1>
    $<$<STREQUAL:debug,${cond}>:DEBUG=1>
    $<$<STREQUAL:debug,${cond}>:_DEBUG=1>
  )
  target_compile_options( "${TNAME}" PUBLIC
    -Wall
    $<$<STREQUAL:release,${cond}>:-O2>
    $<$<STREQUAL:debug,${cond}>:-O0 -g>
  )
endfunction()

define_build( "release" "sample.out")
define_build( "debug" "sample.out")

