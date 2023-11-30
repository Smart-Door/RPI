# RPI


til linux test compile
g++ -pthread main.cpp AccessControl.cpp SerialCommunication.cpp GPIOControl.cpp SystemState.cpp CommandHandler.cpp -o hello

til rpi compile
arm-rpizw-g++ -pthread main.cpp AccessControl.cpp SerialCommunication.cpp GPIOControl.cpp SystemState.cpp CommandHandler.cpp -o hello
