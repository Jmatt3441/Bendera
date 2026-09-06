cmake_minimum_required(VERSION 3.24)

project(Bendera LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Build SFML statically so Windows players do not need separate SFML DLLs.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(SFML)

add_executable(Bendera
    "Side Scroller/main.cpp"
    "Side Scroller/Game.cpp"
    "Side Scroller/Player.cpp"
    "Side Scroller/Enemy.cpp"
    "Side Scroller/Platform.cpp"
    "Side Scroller/PowerUp.cpp"
    "Side Scroller/Projectile.cpp"
    "Side Scroller/Collectible.cpp"
    "Side Scroller/Level.cpp"
)

target_include_directories(Bendera PRIVATE "Side Scroller")
target_link_libraries(Bendera PRIVATE SFML::Graphics)

if(WIN32)
    set_target_properties(Bendera PROPERTIES WIN32_EXECUTABLE OFF)
endif()

# Keep runtime data next to the executable because the game loads assets
# with relative paths such as assets/... and arial.ttf.
add_custom_command(TARGET Bendera POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Side Scroller/assets"
            "$<TARGET_FILE_DIR:Bendera>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/Side Scroller/arial.ttf"
            "$<TARGET_FILE_DIR:Bendera>/arial.ttf"
)
