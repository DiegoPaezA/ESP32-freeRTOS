# Basic Examples of FreeRTOS for the ESP32 Using ESP-IDF

<p align="justify">ESP32 is a SOC that features a 32-bit Xtensa LX6 dual-core (or single-core, depending on the version) microprocessor and an Ultra Low Power (ULP) Co-processor. Also Bluetooth v4.2 and Wi-Fi 802.11 b/g/n connectivity. As well as a security block that supports all the features of the IEEE 82.11 standard, including secure boot, flash encryption, hardware-accelerated cryptography and others.</p>

<p align="justify">Espressif provides a development environment for the ESP32 called "ESP IoT Development Framework" (ESP-IDF) which has extensive documentation and examples that facilitate the development of applications that implement the different modules of the SOC. One of the advantages of the ESP-IDF is that it is based on the FreeRTOS real-time operating system, which allows to exploit the multitasking features of the ESP32. Additionally, it is possible to program the ESP32 using Arduino or MicroPython, which makes this device one of the most popular tools today for the development of IoT devices and DIY projects.</p>

<p align="justify">This repository presents different examples focused on testing the operation of the different ESP32 functionalities using FreeRTOS. The examples have been validated using an ESP32 DEVKIT DOIT board and the documentation of each of the examples is being done to address the theoretical concepts of each of the topics addressed.</p> 

The documentation will be published on my personal web page https://www.mrdpaez.com.


# Examples

| Firmware       |Descripción			|
|----------------|-------------------------------|
|[Task FreeRTOS](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/Task_FreeRTOS)           |Basic Blink Example from ESP-IDF Using Tasks|
|[Task scheduler](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/tareas_Planificador_1)  |FreeRTOS and ESP32 MultiCore Task Testing|
|[Task notifaction 1](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/TaskNotification_1) |FreeRTOS Task notification Basic Test | 
|[Task notifaction 2](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/Task%20Notification%202)| Send data with a Task Notification|
|[Semaphores Test](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/Semaforo_Test1)      | Basic Binary Semaphore Test   | 
|[Semaphores_Mutex](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/Semaforo_Mutex_1)   | Basic Mutex Semaphore Test   | 
|[Semaphores_Mutex 2](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/mutex_2)   | Basic Mutex Semaphore Test 2  | 
|[Semaphores_Counter](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/Semaforo_Contador1)| Basic Counting Semaphore Test   |
|[cxColas1 - Queue 1](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/cxColas_1)        | Sending Data From Task1 to Task2 Using Queue  | 
|[cxColas2 - Queue 2](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/cxColas_2)        | Sending Data From Task1 and Task2 to Task3 Using Queue  | 
|[timerSoftware](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/timerSoftware_1)       | Basic Software Timer Configuration  | 
|[EventsGroup - 1](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/gruposEventos_1)     | Synchronizing  a Task Using Event Groups  | 
|[EventsGroup - 2](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/gruposEventos_2)     | Synchronizing  One, Two, and Three Tasks at the Same Time Using Event Groups| 
|[uart_DEMO](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/uartdemo)                  |Configuration of ESP32 UART port and sending a "OK" msn checking the signal with a logic analyzer | 
|[uart_ECO](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/uart_eco_vscode)            |UART Eco| 
|[uart_rx_tx_task ](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/uart_rxtx_task)     |Using tasks to TX and RX to validate de cx with the UART port| 
|[DHT Sensor](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/dhtdemo)                  |Example digital temperature and humidity sensor - DHT sensors| 
|[i2c_scanner](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/i2c_scanner)             |Configure i2c port and scan all the devices on it| 
|[i2c_PCA9548a](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/i2cPCA9548a)            |Using i2c multiplexer PCA9548 as example of i2c por configuration| 
|[i2c_PCA_MPU6050](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/i2c_PCA_MPU6050)     |Getting data from the mpu6050 sensor, using i2c multiplexer PCA9548|
|[spidemo_DAC](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/spidemo_dac)             |Configuration of the DAC mcp4822 using the spi port|
|[Timergroup](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/Timergroup)               |Configuration of Timer by Hardware| 
|[WiFi_Basic](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/wifi_basic_1)             |Wi-Fi Basic Connection| 
|[WiFI Scanner](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/wifi_escaner_2)         |Wi-Fi Scanner| 
|[WiFI NTP](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/wifi_ntp_3)                 |Wi-Fi NTP Connection| 
|[WiFI Connect](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/wifi_connect_4)         |Wi-Fi Connect Library| 
|[WiFI GetData](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/wifi_getdata_5)         |Wi-Fi Get Data From Internet| 
|[WiFI MQTT](https://github.com/DiegoPaezA/FreeRTOS_Examples/tree/master/wifi_mqtt_6)               |Wi-Fi MQTT Protocol| 


## References

The following references were used to implement the examples:

[1] https://www.freertos.org/a00106.html

[2] https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/index.html

[3] http://www.iotsharing.com/search/label/FreeRTOS

[3] https://www.embarcados.com.br

[4] https://www.learnesp32.com
