# FreeRTOS Basic Task Example

<p align="justify">The study of RTOS consists of planning and scheduling the workload on a processor so that the timeline guarantees for the workload are never violated. The workload is quantified in discrete pieces that we refer to as a task. A task is, then, an independent program that performs specific operations using one or more resources (memory, RAM, CPU, UART, GPIO, etc) of the microcontroller or SOC in the case of the ESP32.</p>

<p align="justify">The program has two tasks, the first one displays the message "Hello World" on the serial port and the second one blinks a LED. As both tasks run on the same ESP32 core, the scheduler assigns the same execution time to each task, as shown in the diagram.</p>

![Diagram](https://github.com/DiegoPaezA/ESP32-freeRTOS/blob/master/Task_FreeRTOS/diagram.png)

[Resumen Teórico](https://www.mrdpaez.com/2022/04/sistema-operativo-de-tiempo-real.html)
[Theoretical Summary](https://www.mrdpaez.com/2022/04/real-time-operating-system-freertos.html)
