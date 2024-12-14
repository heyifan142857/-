cmake_minimum_required(VERSION 3.27)
project(Leap_And_Land C)

set(CMAKE_C_STANDARD 11)

include_directories("D:/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/include")
link_directories("D:/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/lib")

include_directories(include)
add_subdirectory(src)

target_link_libraries(Leap_And_Land PRIVATE
    mingw32
    SDL2main
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
)