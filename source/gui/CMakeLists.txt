# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Sources
target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/InteractiveDisplay.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PXGUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RotationDisplay.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/TranslationDisplay.cpp
)
