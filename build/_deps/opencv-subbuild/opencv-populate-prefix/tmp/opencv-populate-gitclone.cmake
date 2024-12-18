# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

if(EXISTS "/home/dexer/Repos/cpp/bic/build/_deps/opencv-subbuild/opencv-populate-prefix/src/opencv-populate-stamp/opencv-populate-gitclone-lastrun.txt" AND EXISTS "/home/dexer/Repos/cpp/bic/build/_deps/opencv-subbuild/opencv-populate-prefix/src/opencv-populate-stamp/opencv-populate-gitinfo.txt" AND
  "/home/dexer/Repos/cpp/bic/build/_deps/opencv-subbuild/opencv-populate-prefix/src/opencv-populate-stamp/opencv-populate-gitclone-lastrun.txt" IS_NEWER_THAN "/home/dexer/Repos/cpp/bic/build/_deps/opencv-subbuild/opencv-populate-prefix/src/opencv-populate-stamp/opencv-populate-gitinfo.txt")
  message(VERBOSE
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/home/dexer/Repos/cpp/bic/build/_deps/opencv-subbuild/opencv-populate-prefix/src/opencv-populate-stamp/opencv-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

# Even at VERBOSE level, we don't want to see the commands executed, but
# enabling them to be shown for DEBUG may be useful to help diagnose problems.
cmake_language(GET_MESSAGE_LOG_LEVEL active_log_level)
if(active_log_level MATCHES "DEBUG|TRACE")
  set(maybe_show_command COMMAND_ECHO STDOUT)
else()
  set(maybe_show_command "")
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/dexer/Repos/cpp/bic/build/_deps/opencv-src"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/dexer/Repos/cpp/bic/build/_deps/opencv-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"
            clone --no-checkout --depth 1 --no-single-branch --progress --config "advice.detachedHead=false" "https://github.com/opencv/opencv.git" "opencv-src"
    WORKING_DIRECTORY "/home/dexer/Repos/cpp/bic/build/_deps"
    RESULT_VARIABLE error_code
    ${maybe_show_command}
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(NOTICE "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/opencv/opencv.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"
          checkout "4.6.0" --
  WORKING_DIRECTORY "/home/dexer/Repos/cpp/bic/build/_deps/opencv-src"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '4.6.0'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/home/dexer/Repos/cpp/bic/build/_deps/opencv-src"
    RESULT_VARIABLE error_code
    ${maybe_show_command}
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/dexer/Repos/cpp/bic/build/_deps/opencv-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/home/dexer/Repos/cpp/bic/build/_deps/opencv-subbuild/opencv-populate-prefix/src/opencv-populate-stamp/opencv-populate-gitinfo.txt" "/home/dexer/Repos/cpp/bic/build/_deps/opencv-subbuild/opencv-populate-prefix/src/opencv-populate-stamp/opencv-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/dexer/Repos/cpp/bic/build/_deps/opencv-subbuild/opencv-populate-prefix/src/opencv-populate-stamp/opencv-populate-gitclone-lastrun.txt'")
endif()
