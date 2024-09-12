cmake_minimum_required(VERSION 3.15)

project(IndividualMiniproject)

set(INCLUDE_PATHS external_libraries/boost_1_85_0 external_libraries/Crow-1.2.0-Darwin/include)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

# Main project executable
add_executable(IndividualMiniproject 
    src/main.cpp 
    src/Globals.cpp
    src/Course.cpp
    src/Department.cpp
    src/MyApp.cpp
    src/MyFileDatabase.cpp
    src/RouteController.cpp
)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/ff233bdd4cac0a0bf6e5cd45bda3406814cb2796.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

# Include directories for main executable
target_include_directories(IndividualMiniproject PUBLIC 
    ${INCLUDE_PATHS} 
    include
    #/opt/homebrew/Cellar/asio/1.30.211
    /opt/homebrew/opt/asio/include
)

target_link_libraries(IndividualMiniproject PRIVATE 
    gtest 
    gtest_main
)

enable_testing()

# Test executable
add_executable(IndividualMiniprojectTests
    #test/sample.cpp
    test/CourseUnitTests.cpp
    test/DepartmentUnitTests.cpp
    #test/MyAppUnitTests.cpp
    #test/MyFileDatabaseUnitTests.cpp
    #test/RouteControllerUnitTests.cpp

    src/Course.cpp
    src/Department.cpp
    #src/MyApp.cpp
    #src/MyFileDatabase.cpp
    #src/RouteController.cpp
    
)

target_include_directories(IndividualMiniprojectTests PRIVATE 
    ${INCLUDE_PATHS} 
    include
    #/opt/homebrew/opt/asio/1.30.2/include
    /opt/homebrew/opt/asio/include
)

target_link_libraries(IndividualMiniprojectTests PRIVATE 
    gtest 
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(IndividualMiniprojectTests)

add_custom_target(
    run_tests
    COMMAND $<TARGET_FILE:IndividualMiniprojectTests>
    DEPENDS IndividualMiniprojectTests
    COMMENT "Running tests to generate coverage data"
)

# Add custom target for coverage analysis
add_custom_target(
    coverage
    DEPENDS run_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/CMakeFiles/IndividualMiniprojectTests.dir/src
    COMMAND gcov *.cpp.gcno
    COMMAND lcov --capture --directory . --output-file Coverage.info --ignore-errors inconsistent --filter range
    COMMAND lcov --remove Coverage.info "/usr*" "include/" -o FilteredCoverage.info
    COMMAND genhtml FilteredCoverage.info --output-directory UnitTestCoverageOutput --ignore-errors inconsistent --filter range 
    COMMAND open UnitTestCoverageOutput/index.html
    COMMENT "Generating code coverage report"
)

# Find the cpplint program
find_program(CPPLINT cpplint)

if (CPPLINT)
    # List of source files to be checked by cpplint
    set(SOURCE_FILES
        src/main.cpp 
        src/Course.cpp 
        src/Department.cpp 
        src/MyFileDatabase.cpp 
        src/RouteController.cpp
        src/MyApp.cpp
        src/Globals.cpp

        test/sample.cpp
        test/CourseUnitTests.cpp
        test/CourseUnitTests.cpp
        test/DepartmentUnitTests.cpp
        test/MyAppUnitTests.cpp
        test/MyFileDatabaseUnitTests.cpp
        test/RouteControllerUnitTests.cpp
    )

    # Custom target to run cpplint
    add_custom_target(
        cpplint
        COMMAND ${CPPLINT} --filter=-whitespace,-build/include_what_you_use ${SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cpplint for Google C++ Style Guide compliance"
    )
else()
    message(WARNING "cpplint not found! Skipping style checks.")
endif()
