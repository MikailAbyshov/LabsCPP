cmake_minimum_required(VERSION 3.15)

set(VCPKG_MANIFEST_MODE      ON  CACHE BOOL    "" FORCE)
set(VCPKG_TARGET_TRIPLET     "x64-windows" CACHE STRING "" FORCE)
set(CMAKE_TOOLCHAIN_FILE     "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")

project(Match3Game LANGUAGES CXX)
set(CMAKE_CXX_STANDARD       20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/$<CONFIG>)

find_package(SFML COMPONENTS graphics window system CONFIG REQUIRED)
find_package(TGUI CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    Match3Game.cpp
    Match3Game.h
    src/Game.cpp        src/Game.h
    src/Board.cpp       src/Board.h
    src/Tile.cpp        src/Tile.h
        src/Pos.h
    src/Bonuses/BonusGenerator.cpp  src/Bonuses/BonusGenerator.h
    src/Bonuses/IBonusStrategy.h
    src/Bonuses/BombBonusStrategy.cpp   src/Bonuses/BombBonusStrategy.h
    src/Bonuses/PainterBonusStrategy.cpp src/Bonuses/PainterBonusStrategy.h
    src/Animations/Animation.cpp         src/Animations/Animation.h
    src/Animations/MoveAnimation.cpp     src/Animations/MoveAnimation.h
    src/Animations/ScaleAnimation.cpp    src/Animations/ScaleAnimation.h
    src/Animations/AnimationManager.cpp  src/Animations/AnimationManager.h
    src/Animations/BonusAnimation.cpp    src/Animations/BonusAnimation.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Includes)

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    TGUI::TGUI
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd26495)
endif()

if(WIN32)
  file(GLOB VCPKG_DLLS_DEBUG   "${VCPKG_INSTALLED_DIR}/x64-windows/debug/bin/*.dll")
  file(GLOB VCPKG_DLLS_RELEASE "${VCPKG_INSTALLED_DIR}/x64-windows/bin/*.dll")

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${VCPKG_DLLS_DEBUG}
      $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${VCPKG_DLLS_RELEASE}
      $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Includes"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Includes"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)
