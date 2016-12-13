# Install script for directory: /Users/PATANY/Desktop/CS184/cs184/test/OpenCV/modules/java

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/src")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "java")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/OpenCV/java" TYPE FILE OPTIONAL FILES "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/bin/opencv-300.jar")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "java")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/OpenCV/java" TYPE SHARED_LIBRARY OPTIONAL FILES "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_java300.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/OpenCV/java/libopencv_java300.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/OpenCV/java/libopencv_java300.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "lib/libopencv_java300.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_calib3d.3.0.dylib" "lib/libopencv_calib3d.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_core.3.0.dylib" "lib/libopencv_core.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_features2d.3.0.dylib" "lib/libopencv_features2d.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_flann.3.0.dylib" "lib/libopencv_flann.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_highgui.3.0.dylib" "lib/libopencv_highgui.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_imgcodecs.3.0.dylib" "lib/libopencv_imgcodecs.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_imgproc.3.0.dylib" "lib/libopencv_imgproc.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_ml.3.0.dylib" "lib/libopencv_ml.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_objdetect.3.0.dylib" "lib/libopencv_objdetect.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_photo.3.0.dylib" "lib/libopencv_photo.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_video.3.0.dylib" "lib/libopencv_video.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_videoio.3.0.dylib" "lib/libopencv_videoio.3.0.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/OpenCV/java/libopencv_java300.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/src/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/OpenCV/java/libopencv_java300.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/OpenCV/java/libopencv_java300.dylib")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/modules/java/pure_test/cmake_install.cmake")

endif()

