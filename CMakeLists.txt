cmake_minimum_required(VERSION 3.5)

project(Airport_inspector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Concurrent)
find_package(Qt6 COMPONENTS Widgets Charts REQUIRED)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets )

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        congestion_airport.cpp
        congestion_airport.h
        congestion_airport.ui
        Struct.h
        graphic.cpp
        graphic.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Airport_inspector
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()
    if(ANDROID)
        add_library(Airport_inspector SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Airport_inspector
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs)

target_link_libraries(Airport_inspector PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(Airport_inspector PRIVATE Qt6::Sql
                                                Qt6::Concurrent
                                                Qt6::Charts
                                                libcrypto-3-x64
                                                libiconv-2
                                                libintl-9
                                                libpq
                                                libssl-3-x64)


set_target_properties(Airport_inspector PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Airport_inspector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Airport_inspector)
endif()
