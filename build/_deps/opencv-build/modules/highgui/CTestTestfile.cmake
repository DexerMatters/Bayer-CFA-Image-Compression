# CMake generated Testfile for 
# Source directory: /home/dexer/Repos/cpp/bic/build/_deps/opencv-src/modules/highgui
# Build directory: /home/dexer/Repos/cpp/bic/build/_deps/opencv-build/modules/highgui
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_highgui "/home/dexer/Repos/cpp/bic/build/bin/opencv_test_highgui" "--gtest_output=xml:opencv_test_highgui.xml")
set_tests_properties(opencv_test_highgui PROPERTIES  LABELS "Main;opencv_highgui;Accuracy" WORKING_DIRECTORY "/home/dexer/Repos/cpp/bic/build/test-reports/accuracy" _BACKTRACE_TRIPLES "/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVUtils.cmake;1739;add_test;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/cmake/OpenCVModule.cmake;1352;ocv_add_test_from_target;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/modules/highgui/CMakeLists.txt;294;ocv_add_accuracy_tests;/home/dexer/Repos/cpp/bic/build/_deps/opencv-src/modules/highgui/CMakeLists.txt;0;")
