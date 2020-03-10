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
  if(cond STREQUAL "release")
    message( "release" )
    target_compile_definitions("${TNAME}"
      PUBLIC -DNDEBUG=1)
    target_compile_options( "${TNAME}"
      PUBLIC -O2 )
  elseif(cond STREQUAL "debug")
    message( "debug" )
    target_compile_definitions("${TNAME}"
      PUBLIC -DDEBUG=1
      PUBLIC -D_DEBUG=1
    )
    target_compile_options( "${TNAME}"
      PUBLIC -O0 )
  else()
    message( "unknown condition: ${cond}" )
  endif()
  target_compile_options( "${TNAME}"
    PUBLIC -Wall )
endfunction()

define_build( "release" "sample.out")
define_build( "debug" "sample.out")

