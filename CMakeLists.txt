cmake_minimum_required(VERSION 3.16)
project(ParkingSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

set(SOURCES
    src/main.cpp
    src/App.cpp
    src/MainFrame.cpp
    src/SpotWidget.cpp
    src/ParkingSystem.cpp
)

set(HEADERS
    includes/Models.h
    includes/ParkingSystem.h
    includes/SpotWidget.h
    includes/MainFrame.h
    includes/App.h
)

if(WIN32)
    add_executable(ParkingSystem WIN32 ${SOURCES} ${HEADERS})
else()
    add_executable(ParkingSystem ${SOURCES} ${HEADERS})
endif()

target_include_directories(ParkingSystem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_link_libraries(ParkingSystem PRIVATE ${wxWidgets_LIBRARIES})
