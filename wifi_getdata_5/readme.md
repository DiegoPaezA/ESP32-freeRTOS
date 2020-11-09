
# ESP32 Starter template

on your command line run
```
npx create-esp32-app
```

## Quick overview

This template can be used as is but, its intended as a quick start for the students learning the ESP32-IDF through my course [https://learnesp32.com](https://learnesp32.com)

## prerequisites

1. You will need to have [node](https://nodejs.org) installed.
2. The esp-idf must be set up. you can follow the instructions in my course (free of charge) on the [Setting up Your Environment](https://www.learnesp32.com/2_introduction) module or follow the [official documentation](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/#step-1-set-up-the-toolchain)
3. this template is for [vscode](https://code.visualstudio.com/download) which will need to be installed
4. In VSCODE add the [c++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)

5. ensure tour ESP32 is plugged in and that a COM PORT is established (You may need a driver for your ESP32 dev board)

## Run command

1. in any directory run
```
npx create-esp32-app
```

2. you will be prompted for the name of your project.
3. you will be prompted for the IDF path. select or navigate to the location of the IDF path
3. you will be prompted for the IDF-tools path. select or navigate to the location of the IDF-tools path
4. navigate to the directory of the project name you created
```
cd <project name>
```
4. open the project in vscode ```code .```

## vs code intellisense

intellisense should just work so long as you have set up the paths correctly. If you have trouble double check your idf and tools paths and correct them in the **/.vscode/c_cpp_properties.json** file
## flashing the esp32

1. in vs code, open a new terminal by pressing ctrl + \` (or pressing F1 and typing `open new terminal`)
2. type the following command

```bash
idf.py -p [your com port] flash monitor
```
