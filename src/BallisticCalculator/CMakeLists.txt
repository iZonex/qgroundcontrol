find_package(Qt6 REQUIRED COMPONENTS Core Quick Location Positioning)

add_library(BallisticCalculator
    BallisticCalculator.cc
    BallisticCalculator.h
)

target_link_libraries(BallisticCalculator
    PUBLIC
        Qt6::Core
        Qt6::Location
        Qt6::Quick
        Qt6::Positioning
        Settings
        Vehicle
        FactSystem
)

target_include_directories(BallisticCalculator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
) 