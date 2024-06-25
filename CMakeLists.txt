cmake_minimum_required(VERSION 3.5)

project(spectro10 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        spectropage.h spectropage.cpp
        settingspage.h settingspage.cpp
        spectrogram.h spectrogram.cpp
        oscilloscope.h oscilloscope.cpp
        controlsmenu.h controlsmenu.cpp
        audioprocessor.h audioprocessor.cpp
        utils.h utils.cpp
        fftmanager.h fftmanager.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()


# ======== link & include libraries ========
# Configure pkg-config
find_package(PkgConfig REQUIRED)

# rtaudio config
pkg_search_module(LIBRTAUDIO REQUIRED IMPORTED_TARGET rtaudio)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::LIBRTAUDIO)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRTAUDIO_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${LIBRTAUDIO_CFLAGS})

# fftw3 config
pkg_search_module(LIBFFTW REQUIRED IMPORTED_TARGET fftw3)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::LIBFFTW)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBFFTW_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${LIBFFTW_CFLAGS})

# speexdsp config
pkg_search_module(LIBSPEEXDSP REQUIRED IMPORTED_TARGET speexdsp)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::LIBSPEEXDSP)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBSPEEXDSP_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${LIBSPEEXDSP_CFLAGS})

# q config
pkg_search_module(LIBQ REQUIRED IMPORTED_TARGET q)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::LIBQ)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBQ_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${LIBQ_CFLAGS})

# mongocxx config
pkg_search_module(LIBMONGOCXX REQUIRED IMPORTED_TARGET libmongocxx)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::LIBMONGOCXX)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBMONGOCXX_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${LIBMONGOCXX_CFLAGS})
