find_package(Qt5 REQUIRED COMPONENTS Core Quick Location Positioning)

add_library(BallisticCalculator
    BallisticCalculator.cc
    BallisticCalculator.h
)

target_link_libraries(BallisticCalculator
    PUBLIC
        Qt5::Core
        Qt5::Location
        Qt5::Quick
        Qt5::Positioning
        Settings
        Vehicle
        FactSystem
)

target_include_directories(BallisticCalculator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
) 