cmake_minimum_required(VERSION 3.14)
project(ORB_SLAM3)

IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF ()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall   -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall   -O3")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -march=native")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
endif ()

if (MSVC)
    add_definitions(-DCOMPILEDWITHC11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj") #                 Increases the number of addressable sections in an .obj file.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c11") #                Use c++11
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm2000") #                 Specifies the precompiled header memory allocation limit
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W1") #                     Enables level 1 (severe) warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") #                     Builds multiple source files concurrently
    #SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /verbose:lib")# Prints out which libraries are being searched in which order
    add_compile_definitions(_ENFORCE_MATCHING_ALLOCATORS=0)

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)#                          Has to be turned off because it causes "LINK : fatal error LNK1189: library limit of 65535 objects exceeded" => need to declare relevant symbols as __declspec(dllexport) in the header files
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS TRUE)
endif ()

find_package(OpenCV 3.1)
#find_package(OpenCV 4.4)
if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV > 4.4 not found.")
endif ()

MESSAGE("OPENCV VERSION:")
MESSAGE(${OpenCV_VERSION})

find_package(Eigen3 3.1.0 REQUIRED)
find_package(Pangolin REQUIRED)
#(Boost_INCLUDE_DIR C:/lib/boost_1_85_0)
set(Boost_ROOT "C:/lib/boost_1_85_0")
set(BOOST_LIBRARYDIR "C:/lib/boost_1_85_0/stage/x64/lib")
find_package(Boost REQUIRED)
SET(OPENSSL_ROOT_DIR "C:/lib/vcpkg/installed/x64-windows")
find_package(OpenSSL REQUIRED)
#find_package(realsense2)

message("================================= Flags ======================================================")
message(STATUS "Cmake flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Cmake debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "Cmake release flags: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "Linker flags: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "Boost include: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost libs: ${BOOST_LIBRARYDIR}")
message(STATUS "Boost libs: ${Boost_LIBRARIES}")

include_directories(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/CameraModels
        ${PROJECT_SOURCE_DIR}/Thirdparty/Sophus
        ${EIGEN3_INCLUDE_DIR}
        ${Pangolin_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${OPENSSL_INCLUDE_DIR}
)

add_library(${PROJECT_NAME} SHARED
        src/System.cc
        src/Tracking.cc
        src/LocalMapping.cc
        src/LoopClosing.cc
        src/ORBextractor.cc
        src/ORBmatcher.cc
        src/FrameDrawer.cc
        src/Converter.cc
        src/MapPoint.cc
        src/KeyFrame.cc
        src/Atlas.cc
        src/Map.cc
        src/MapDrawer.cc
        src/Optimizer.cc
        src/Frame.cc
        src/KeyFrameDatabase.cc
        src/Sim3Solver.cc
        src/Viewer.cc
        src/ImuTypes.cc
        src/G2oTypes.cc
        src/CameraModels/Pinhole.cpp
        src/CameraModels/KannalaBrandt8.cpp
        src/OptimizableTypes.cpp
        src/MLPnPsolver.cpp
        src/GeometricTools.cc
        src/TwoViewReconstruction.cc
        src/Config.cc
        src/Settings.cc
        src/Global.cpp
        include/System.h
        include/Tracking.h
        include/LocalMapping.h
        include/LoopClosing.h
        include/ORBextractor.h
        include/ORBmatcher.h
        include/FrameDrawer.h
        include/Converter.h
        include/MapPoint.h
        include/KeyFrame.h
        include/Atlas.h
        include/Map.h
        include/MapDrawer.h
        include/Optimizer.h
        include/Frame.h
        include/KeyFrameDatabase.h
        include/Sim3Solver.h
        include/Viewer.h
        include/ImuTypes.h
        include/G2oTypes.h
        include/CameraModels/GeometricCamera.h
        include/CameraModels/Pinhole.h
        include/CameraModels/KannalaBrandt8.h
        include/OptimizableTypes.h
        include/MLPnPsolver.h
        include/GeometricTools.h
        include/TwoViewReconstruction.h
        include/SerializationUtils.h
        include/Config.h
        include/Settings.h
        include/Global.h)

LINK_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/Thirdparty/DBoW2/lib
        ${PROJECT_SOURCE_DIR}/Thirdparty/g2o/lib
        ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
        ${EIGEN3_LIBS}
        ${Pangolin_LIBRARIES}
        debug ${PROJECT_SOURCE_DIR}/Thirdparty/DBoW2/lib/Debug/DBoW2.lib optimized ${PROJECT_SOURCE_DIR}/Thirdparty/DBoW2/lib/Release/DBoW2.lib
        debug ${PROJECT_SOURCE_DIR}/Thirdparty/g2o/lib/Debug/g2o.lib optimized ${PROJECT_SOURCE_DIR}/Thirdparty/g2o/lib/Release/g2o.lib
        debug C:/lib/boost_1_85_0/stage/x64/lib/libboost_serialization-vc143-mt-gd-x64-1_85.lib optimized C:/lib/boost_1_85_0/stage/x64/lib/libboost_serialization-vc143-mt-x64-1_85.lib
        OpenSSL::SSL
)

# If RealSense SDK is found the library is added and its examples compiled
if (realsense2_FOUND)
    include_directories(${PROJECT_NAME}
            ${realsense_INCLUDE_DIR}
    )
    target_link_libraries(${PROJECT_NAME}
            ${realsense2_LIBRARY}
    )
endif ()

if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN_EXPORT)
endif ()

# Build examples

#Monocular examples

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples/Monocular/$<CONFIG>)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples/Monocular/$<CONFIG>)
#set_target_properties(${PROJECT_NAME}
#        PROPERTIES
#        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples/Monocular/$<CONFIG>
#        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples/Monocular/$<CONFIG>
#)
#add_executable(mono_tum
#        Examples/Monocular/mono_tum.cc)
#target_link_libraries(mono_tum ${PROJECT_NAME})
#
#add_executable(mono_kitti
#        Examples/Monocular/mono_kitti.cc)
#target_link_libraries(mono_kitti ${PROJECT_NAME})
#
#add_executable(mono_euroc
#        Examples/Monocular/mono_euroc.cc)
#target_link_libraries(mono_euroc ${PROJECT_NAME})
#
#add_executable(mono_tum_vi
#        Examples/Monocular/mono_tum_vi.cc)
#target_link_libraries(mono_tum_vi ${PROJECT_NAME})


#Monocular inertial examples
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples/Monocular-Inertial/$<CONFIG>)
#set_target_properties(${PROJECT_NAME}
#        PROPERTIES
#        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples/Monocular-Inertial/$<CONFIG>
#        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples/Monocular-Inertial/$<CONFIG>
#)

add_executable(mono_inertial_euroc
        Examples/Monocular-Inertial/mono_inertial_euroc.cc)
target_link_libraries(mono_inertial_euroc ${PROJECT_NAME})

add_executable(mono_inertial_tum_vi
        Examples/Monocular-Inertial/mono_inertial_tum_vi.cc)
target_link_libraries(mono_inertial_tum_vi ${PROJECT_NAME})

if (realsense2_FOUND)
    add_executable(mono_inertial_realsense_t265
            Examples/Monocular-Inertial/mono_inertial_realsense_t265.cc)
    target_link_libraries(mono_inertial_realsense_t265 ${PROJECT_NAME})

    add_executable(mono_inertial_realsense_D435i
            Examples/Monocular-Inertial/mono_inertial_realsense_D435i.cc)
    target_link_libraries(mono_inertial_realsense_D435i ${PROJECT_NAME})
endif ()

#Monocular examples
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples_old/Monocular/$<CONFIG>)
#set_target_properties(${PROJECT_NAME}
#        PROPERTIES
#        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples_old/Monocular/$<CONFIG>
#        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples_old/Monocular/$<CONFIG>
#)
#
#add_executable(mono_tum_old
#        Examples_old/Monocular/mono_tum.cc)
#target_link_libraries(mono_tum_old ${PROJECT_NAME})
#
#add_executable(mono_kitti_old
#        Examples_old/Monocular/mono_kitti.cc)
#target_link_libraries(mono_kitti_old ${PROJECT_NAME})
#
#add_executable(mono_euroc_old
#        Examples_old/Monocular/mono_euroc.cc)
#target_link_libraries(mono_euroc_old ${PROJECT_NAME})
#
#add_executable(mono_tum_vi_old
#        Examples_old/Monocular/mono_tum_vi.cc)
#target_link_libraries(mono_tum_vi_old ${PROJECT_NAME})
#
#if (realsense2_FOUND)
#    add_executable(mono_realsense_t265_old
#            Examples_old/Monocular/mono_realsense_t265.cc)
#    target_link_libraries(mono_realsense_t265_old ${PROJECT_NAME})
#
#    add_executable(mono_realsense_D435i_old
#            Examples_old/Monocular/mono_realsense_D435i.cc)
#    target_link_libraries(mono_realsense_D435i_old ${PROJECT_NAME})
#endif ()
#
##Monocular inertial examples
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples_old/Monocular-Inertial/$<CONFIG>)
#set_target_properties(${PROJECT_NAME}
#        PROPERTIES
#        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples_old/Monocular-Inertial/$<CONFIG>
#        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples_old/Monocular-Inertial/$<CONFIG>
#)
#
#add_executable(mono_inertial_euroc_old
#        Examples_old/Monocular-Inertial/mono_inertial_euroc.cc)
#target_link_libraries(mono_inertial_euroc_old ${PROJECT_NAME})
#
#add_executable(mono_inertial_tum_vi_old
#        Examples_old/Monocular-Inertial/mono_inertial_tum_vi.cc)
#target_link_libraries(mono_inertial_tum_vi_old ${PROJECT_NAME})
#
#if (realsense2_FOUND)
#    add_executable(mono_inertial_realsense_t265_old
#            Examples_old/Monocular-Inertial/mono_inertial_realsense_t265.cc)
#    target_link_libraries(mono_inertial_realsense_t265_old ${PROJECT_NAME})
#
#    add_executable(mono_inertial_realsense_D435i_old
#            Examples_old/Monocular-Inertial/mono_inertial_realsense_D435i.cc)
#    target_link_libraries(mono_inertial_realsense_D435i_old ${PROJECT_NAME})
#endif ()