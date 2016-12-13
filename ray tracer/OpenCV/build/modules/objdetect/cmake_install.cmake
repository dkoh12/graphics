# Install script for directory: /Users/PATANY/Desktop/CS184/cs184/test/OpenCV/modules/objdetect

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libs")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY OPTIONAL FILES
    "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_objdetect.3.0.0.dylib"
    "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_objdetect.3.0.dylib"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_objdetect.3.0.0.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_objdetect.3.0.dylib"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      execute_process(COMMAND "/usr/bin/install_name_tool"
        -id "lib/libopencv_objdetect.3.0.dylib"
        -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_core.3.0.dylib" "lib/libopencv_core.3.0.dylib"
        -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_highgui.3.0.dylib" "lib/libopencv_highgui.3.0.dylib"
        -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_imgcodecs.3.0.dylib" "lib/libopencv_imgcodecs.3.0.dylib"
        -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_imgproc.3.0.dylib" "lib/libopencv_imgproc.3.0.dylib"
        -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_ml.3.0.dylib" "lib/libopencv_ml.3.0.dylib"
        -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_videoio.3.0.dylib" "lib/libopencv_videoio.3.0.dylib"
        "${file}")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "/src/lib"
        "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_objdetect.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_objdetect.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_objdetect.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "lib/libopencv_objdetect.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_core.3.0.dylib" "lib/libopencv_core.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_highgui.3.0.dylib" "lib/libopencv_highgui.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_imgcodecs.3.0.dylib" "lib/libopencv_imgcodecs.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_imgproc.3.0.dylib" "lib/libopencv_imgproc.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_ml.3.0.dylib" "lib/libopencv_ml.3.0.dylib"
      -change "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/build/lib/libopencv_videoio.3.0.dylib" "lib/libopencv_videoio.3.0.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_objdetect.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/src/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_objdetect.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_objdetect.dylib")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2" TYPE FILE OPTIONAL FILES "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/modules/objdetect/include/opencv2/objdetect.hpp")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/objdetect" TYPE FILE OPTIONAL FILES "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/modules/objdetect/include/opencv2/objdetect/detection_based_tracker.hpp")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/objdetect" TYPE FILE OPTIONAL FILES "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/modules/objdetect/include/opencv2/objdetect/objdetect.hpp")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/objdetect" TYPE FILE OPTIONAL FILES "/Users/PATANY/Desktop/CS184/cs184/test/OpenCV/modules/objdetect/include/opencv2/objdetect/objdetect_c.h")
endif()
