message(STATUS "downloading...
     src='http://www.libsdl.org/tmp/SDL-2.0.zip'
     dst='/home/greg/workspace/cpp-invaders/build/sdl/src/SDL-2.0.zip'
     timeout='none'")




file(DOWNLOAD
  "http://www.libsdl.org/tmp/SDL-2.0.zip"
  "/home/greg/workspace/cpp-invaders/build/sdl/src/SDL-2.0.zip"
  SHOW_PROGRESS
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'http://www.libsdl.org/tmp/SDL-2.0.zip' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
