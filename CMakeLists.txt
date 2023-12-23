cmake_minimum_required(VERSION 3.16)
project(MyQtProject LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

# Set minimum required version of Qt
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Find OpenCV
find_package(OpenCV REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

if (Qt6_FOUND)
    message(STATUS "Found Qt ${Qt6_VERSION}")
else()
    message(FATAL_ERROR "Qt not found!")
endif()

# Set your source files
file(GLOB SOURCES "*.cpp" "source_files/*.cpp")

# Set your header files
file(GLOB HEADERS "*.h" "header_files/*.h")

# Set your UI files (if any)
set(UIS mainwindow.ui)

# Set your resource files (if any)
set(RESOURCES images.qrc)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UIS} ${RESOURCES})

# Specify include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/source_files
)

# Link against Qt and OpenCV libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Gui ${OpenCV_LIBS})

# Set C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
