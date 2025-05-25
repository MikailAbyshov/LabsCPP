set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.26)

project(Arkanoid)

#Попытался сделать так, чтобы не приходилось устанавливать ручками
include(FetchContent)
set(BUILD_SHARED_LIBS OFF)

# Явно указываем, какие компоненты SFML нам нужны
set(SFML_BUILD_WINDOW TRUE)
set(SFML_BUILD_GRAPHICS TRUE)
set(SFML_BUILD_AUDIO FALSE)  # Отключаем аудио, иначе оно само пытается подтянкться и вызывает ошибку
set(SFML_BUILD_NETWORK FALSE)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SFML)

# Настройка проекта
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(Arkanoid src/main.cpp
        includes/Game.h
        src/Game.cpp
        includes/Paddle.h
        src/Paddle.cpp
        src/Ball.cpp
        src/Block.cpp
        src/Bonus.cpp)

# Линкуем
target_link_libraries(Arkanoid
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

if(WIN32)
    target_compile_definitions(Arkanoid PRIVATE SFML_STATIC)
endif()