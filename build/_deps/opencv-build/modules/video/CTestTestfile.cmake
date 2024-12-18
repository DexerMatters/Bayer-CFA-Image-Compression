# CMake generated Testfile for 
# Source directory: /home/dexer/Repos/cpp/bic/build/_deps/opencv-src/modules/video
# Build directory: /home/dexer/Repos/cpp/bic/build/_deps/opencv-build/modules/video
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_video "/home/dexer/Repos/cpp/bic/build/bin/opencv_test_video" "--gtest_output=xml:opencv_test_video.xml")
set_tests_properties(opencv_test_video PROPERTIES  LABELS "Main;opencv_video;Accuracy" WORKING_DIRECTORY "/home/dexer/Repos/cpp/bic/build/test-reports/accuracy" _BACKTRACE_TRIPLES "/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVUtils.cmake;1739;add_test;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVModule.cmake;1352;ocv_add_test_from_target;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVModule.cmake;1110;ocv_add_accuracy_tests;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/modules/video/CMakeLists.txt;2;ocv_define_module;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/modules/video/CMakeLists.txt;0;")
add_test(opencv_perf_video "/home/dexer/Repos/cpp/bic/build/bin/opencv_perf_video" "--gtest_output=xml:opencv_perf_video.xml")
set_tests_properties(opencv_perf_video PROPERTIES  LABELS "Main;opencv_video;Performance" WORKING_DIRECTORY "/home/dexer/Repos/cpp/bic/build/test-reports/performance" _BACKTRACE_TRIPLES "/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVUtils.cmake;1739;add_test;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVModule.cmake;1251;ocv_add_test_from_target;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVModule.cmake;1111;ocv_add_perf_tests;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/modules/video/CMakeLists.txt;2;ocv_define_module;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/modules/video/CMakeLists.txt;0;")
add_test(opencv_sanity_video "/home/dexer/Repos/cpp/bic/build/bin/opencv_perf_video" "--gtest_output=xml:opencv_perf_video.xml" "--perf_min_samples=1" "--perf_force_samples=1" "--perf_verify_sanity")
set_tests_properties(opencv_sanity_video PROPERTIES  LABELS "Main;opencv_video;Sanity" WORKING_DIRECTORY "/home/dexer/Repos/cpp/bic/build/test-reports/sanity" _BACKTRACE_TRIPLES "/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVUtils.cmake;1739;add_test;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVModule.cmake;1252;ocv_add_test_from_target;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVModule.cmake;1111;ocv_add_perf_tests;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/modules/video/CMakeLists.txt;2;ocv_define_module;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/modules/video/CMakeLists.txt;0;")
