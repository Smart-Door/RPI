cmake_minimum_required(VERSION 3.2)
project(RPI_koden_final)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(RPI_koden_final
        CommandHandler.cpp
        CommandHandler.h
        main.cpp
        PasswordManager.cpp
        PasswordManager.h
        SerialCommunication.cpp
        SerialCommunication.h
        SystemState.cpp
        SystemState.h
        TimeUtility.cpp
        TimeUtility.h
)
