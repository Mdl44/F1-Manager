cmake_minimum_required(VERSION 3.26)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
# for now, the project name is used as the executable name
project(oop)
# set(CMAKE_PROJECT_VERSION_MAJOR 0)
# set(CMAKE_PROJECT_VERSION_MINOR 0)
# set(CMAKE_PROJECT_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)

###############################################################################

# external dependencies with FetchContent

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# required because of SFML
set(WARNINGS_AS_ERRORS OFF)

# NOTE: Also update SFML_VERSION env var in .github/workflows/cmake.yml:84
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.1.0
    # GIT_TAG        origin/master
    GIT_SHALLOW    1  # works only with branches or tags, not with arbitrary commit hashes
)

FetchContent_MakeAvailable(SFML)

###############################################################################

# external dependencies with find_package

find_package(Threads REQUIRED)
if(APPLE)
elseif(UNIX)
    find_package(X11)
endif()

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${PROJECT_NAME}
        main.cpp
        generated/src/Helper.cpp
        env_fixes.h
        sources/Driver.cpp
        headers/Driver.h
        sources/Car.cpp
        headers/Car.h
        sources/Team.cpp
        headers/Team.h
        sources/Player.cpp
        headers/Player.h
        sources/RaceWeekend.cpp
        headers/RaceWeekend.h
        headers/Season.h
        sources/Season.cpp
        headers/GameManager.h
        sources/GameManager.cpp
        headers/DryCondition.h
        sources/DryCondition.cpp
        headers/IntermediateCondition.h
        sources/IntermediateCondition.cpp
        headers/WeatherCondition.h
        sources/WeatherCondition.cpp
        headers/WetCondition.h
        sources/WetCondition.cpp
        headers/NightCondition.h
        sources/NightCondition.cpp
        headers/TopTeam.h
        sources/TopTeam.cpp
        headers/Menu.h
        sources/Menu.cpp
        headers/Exceptions.h
        sources/Exceptions.cpp
        headers/WeatherConditionFactory.h
        sources/WeatherConditionFactory.cpp
        headers/RegulationChanges.h
        sources/RegulationChanges.cpp
        headers/MixedCondition.h
        sources/MixedCondition.cpp
        headers/WindyCondition.h
        sources/WindyCondition.cpp
        headers/HotCondition.h
        sources/HotCondition.cpp
        headers/Stats.h
        sources/Stats.cpp
        headers/PerformanceTracker.h
        headers/GameRules.h
        headers/GameView.h
        headers/ConsoleView.h
        sources/ConsoleView.cpp
        headers/TrackMap.h
        sources/TrackMap.cpp

)

include(cmake/CompilerFlags.cmake)

###############################################################################

# use SYSTEM so cppcheck and clang-tidy do not report warnings from these directories
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE generated/include)
target_include_directories(${PROJECT_NAME}  PRIVATE headers)
# target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ext/<SomeHppLib>/include)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system Threads::Threads)

if(APPLE)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE X11)
endif()

###############################################################################

# copy binaries to "bin" folder; these are uploaded as artifacts on each release
# DESTINATION_DIR is set as "bin" in cmake/Options.cmake:6
install(TARGETS ${PROJECT_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()


include(cmake/CopyHelper.cmake)
copy_files(FILES keybindings.txt)
copy_files(FILES car_data.txt)
copy_files(FILES teams.txt)
copy_files(FILES circuits.txt)
copy_files(FILES legacy_circuits.txt)
copy_files(FILES driver_pool.txt)
copy_files(DIRECTORY tracks)
# copy_files(FILES keybindings.txt config.json DIRECTORY images sounds)
# copy_files(DIRECTORY images sounds)
