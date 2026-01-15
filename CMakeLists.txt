cmake_minimum_required(VERSION 3.15)
project(HotelManagementSystem VERSION 1.0.0 LANGUAGES CXX)

# ==========================================
# C++ Standard
# ==========================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==========================================
# Build Type
# ==========================================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# ==========================================
# Compiler Flags
# ==========================================
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wpedantic -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# ==========================================
# Output Directories
# ==========================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ==========================================
# Find Required Packages
# ==========================================

# OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    message(STATUS "Found OpenGL: ${OPENGL_LIBRARIES}")
endif()

# GLFW3
find_package(glfw3 REQUIRED)
if(glfw3_FOUND)
    message(STATUS "Found GLFW3")
endif()

# PostgreSQL
find_package(PostgreSQL REQUIRED)
if(PostgreSQL_FOUND)
    message(STATUS "Found PostgreSQL: ${PostgreSQL_INCLUDE_DIRS}")
endif()

# libpqxx (C++ wrapper for PostgreSQL)
# Try find_package first, fallback to pkg-config if not found
find_package(libpqxx QUIET)
if(NOT libpqxx_FOUND)
    message(STATUS "libpqxx not found via find_package, trying pkg-config...")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PQXX REQUIRED libpqxx)
    if(PQXX_FOUND)
        message(STATUS "Found libpqxx via pkg-config: ${PQXX_LIBRARIES}")
    endif()
else()
    message(STATUS "Found libpqxx via find_package")
endif()

# ==========================================
# ImGui Setup
# ==========================================
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

if(NOT EXISTS ${IMGUI_DIR})
    message(WARNING "ImGui not found at ${IMGUI_DIR}. Please download it from https://github.com/ocornut/imgui")
    message(WARNING "Run: git clone https://github.com/ocornut/imgui.git external/imgui")
endif()

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# ==========================================
# ImPlot Setup
# ==========================================
set(IMPLOT_DIR ${CMAKE_SOURCE_DIR}/external/implot)

if(NOT EXISTS ${IMPLOT_DIR})
    message(WARNING "ImPlot not found at ${IMPLOT_DIR}. Please download it from https://github.com/epezent/implot")
    message(WARNING "Run: git clone https://github.com/epezent/implot.git external/implot")
endif()

set(IMPLOT_SOURCES
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

# ==========================================
# Include Directories
# ==========================================
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${IMPLOT_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${PostgreSQL_INCLUDE_DIRS}
)

# Add libpqxx include directories
if(libpqxx_FOUND)
    # Using find_package result
    # Includes are automatically handled by target_link_libraries
else()
    # Using pkg-config result
    include_directories(${PQXX_INCLUDE_DIRS})
endif()

# ==========================================
# Source Files Collection
# ==========================================

# Core sources
file(GLOB CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/core/*.cpp
)

# Database sources
file(GLOB DATABASE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/database/*.cpp
    ${CMAKE_SOURCE_DIR}/src/database/models/*.cpp
    ${CMAKE_SOURCE_DIR}/src/database/repositories/*.cpp
)

# UI sources
file(GLOB UI_SOURCES
    ${CMAKE_SOURCE_DIR}/src/ui/*.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/components/*.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/views/*.cpp
)

# Utils sources
file(GLOB UTILS_SOURCES
    ${CMAKE_SOURCE_DIR}/src/utils/*.cpp
)

# All application sources
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CORE_SOURCES}
    ${DATABASE_SOURCES}
    ${UI_SOURCES}
    ${UTILS_SOURCES}
    ${IMGUI_SOURCES}
    ${IMPLOT_SOURCES}
)

# ==========================================
# Executable Target
# ==========================================
add_executable(${PROJECT_NAME} ${SOURCES})

# ==========================================
# Link Libraries
# ==========================================
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    ${PostgreSQL_LIBRARIES}
)

# Link libpqxx
if(libpqxx_FOUND)
    target_link_libraries(${PROJECT_NAME} libpqxx::pqxx)
else()
    target_link_libraries(${PROJECT_NAME} ${PQXX_LIBRARIES})
    target_link_directories(${PROJECT_NAME} PRIVATE ${PQXX_LIBRARY_DIRS})
endif()

# ==========================================
# Platform-Specific Settings
# ==========================================
if(APPLE)
    message(STATUS "Configuring for macOS")
    target_link_libraries(${PROJECT_NAME}
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
    # Set GLSL version for macOS
    target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GL3W)
elseif(UNIX)
    message(STATUS "Configuring for Linux")
    target_link_libraries(${PROJECT_NAME} dl pthread)
elseif(WIN32)
    message(STATUS "Configuring for Windows")
    # Windows-specific libraries can be added here
endif()

# ==========================================
# Compile Definitions
# ==========================================
target_compile_definitions(${PROJECT_NAME} PRIVATE
    # ImGui backend configuration
    IMGUI_IMPL_OPENGL_LOADER_GL3W
    # Project version
    PROJECT_VERSION="${PROJECT_VERSION}"
)

# ==========================================
# Post-Build Commands
# ==========================================

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Creating assets directory in build folder"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying assets to build directory"
)

# Copy config example
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/config/database.ini.example
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/database.ini.example
    COMMENT "Copying database config example"
)

# ==========================================
# Installation Rules
# ==========================================
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(DIRECTORY assets/
    DESTINATION share/${PROJECT_NAME}/assets
)

install(FILES config/database.ini.example
    DESTINATION share/${PROJECT_NAME}/
)

# ==========================================
# Print Configuration Summary
# ==========================================
message(STATUS "")
message(STATUS "=== Hotel Management System Configuration ===")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Dependencies:")
message(STATUS "  OpenGL: ${OPENGL_LIBRARIES}")
message(STATUS "  GLFW3: Found")
message(STATUS "  PostgreSQL: ${PostgreSQL_LIBRARIES}")
if(libpqxx_FOUND)
    message(STATUS "  libpqxx: Found via find_package")
else()
    message(STATUS "  libpqxx: Found via pkg-config")
endif()
message(STATUS "  ImGui: ${IMGUI_DIR}")
message(STATUS "  ImPlot: ${IMPLOT_DIR}")
message(STATUS "")
message(STATUS "Build directories:")
message(STATUS "  Executables: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  Libraries: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "=============================================")
message(STATUS "")

# ==========================================
# Optional: Enable Testing
# ==========================================
# enable_testing()
# add_subdirectory(tests)
