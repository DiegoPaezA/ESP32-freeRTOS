# Project: Hello World

The "Hello World" project is a simple example that serves as a template for new projects and to check that the configuration of the [ESP-IDF v5.2](https://github.com/espressif/esp-idf/tree/release/v5.2) is working.

The example prints "Hello World!" to the console and then enters a loop that blink the LED on the ESP32 board. The LED is connected to GPIO pin 2.

The board used in this example is the [FireBeetle 2 ESP32-E IoT Microcontroller](https://www.dfrobot.com/product-2195.html).


## Example folder contents

The project **hello-world** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```