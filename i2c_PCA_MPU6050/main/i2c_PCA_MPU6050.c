/*
 * Ejemplo I2C: Se utiliza el multiplexador I2C PCA9548 y el MPU6050 Para
 * mostrar el funcionamiento del protocolo I2C.
 *
 * Se crean funciones para leer aceleración, temperatura y nombre del MPU6050
 * Se crea Función para seleccionar el puerto deseado en el Mux.
 */

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "esp_err.h"

#define PCA9548ADDR   0x70  // W   ADDRESS MUX
// PCA9548 CHANNELS
#define  PCA9548_OFF  0x00  // W   CHANNELS OFF
#define  PCA9548_CH1  0x01  // W   ACTIVATION OF CHANNEL 1
#define  PCA9548_CH2  0x02  // W   ACTIVATION OF CHANNEL 2
#define  PCA9548_CH3  0x04  // W   ACTIVATION OF CHANNEL 3
#define  PCA9548_CH4  0x08  // W   ACTIVATION OF CHANNEL 4
#define  PCA9548_CH5  0x10  // W   ACTIVATION OF CHANNEL 5
#define  PCA9548_CH6  0x20  // W   ACTIVATION OF CHANNEL 6
#define  PCA9548_CH7  0x40  // W   ACTIVATION OF CHANNEL 7
#define  PCA9548_CH8  0x80  // W   ACTIVATION OF CHANNEL 8

#define MPU_ADD 	     0x68
#define MPU6050_PWR      0x6B
#define MPU6050_TEMP     0x41
#define MPU6050_Raw_ACC  0x3B
#define MPU6050_Raw_GYRO 0x43
#define who_Am_I	     0x75

uint8_t mpuName;

#define ACK_VAL    0x0
#define NACK_VAL   0x1
#define I2C_MASTER_FREQ_HZ 100000


void selectI2CChannel(int muxADD, int channelADD) {

	// create and execute the command link
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(muxADD << 1) | I2C_MASTER_WRITE,true);
	i2c_master_write_byte(cmd,channelADD,true);
	i2c_master_stop(cmd);
	if(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS) == ESP_OK) {
		printf("-> Canal 0x%02x Mux I2C Seleccionado \r\n", channelADD);
	}else{
		printf("MUX it´s not connected  \r\n");
	}
	i2c_cmd_link_delete(cmd);

}

void initMPU6050(int mpuAdd,int channelADD){
	// create and execute the command link
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd,(mpuAdd << 1) | I2C_MASTER_WRITE,true);
		i2c_master_write_byte(cmd,MPU6050_PWR,true);
		i2c_master_write_byte(cmd,0x0,true);
		i2c_master_stop(cmd);
		if(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS) == ESP_OK) {
			printf("-> MPU6050 - Canal 0x%02x Inicializado\n", channelADD);
		}else{
		printf("MPU6050 it´s not connected  \r\n");
		}
		i2c_cmd_link_delete(cmd);
}

uint8_t whoAMI (int mpuAdd){
	uint8_t buffer;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(mpuAdd << 1) | I2C_MASTER_WRITE,true);
	i2c_master_write_byte(cmd,who_Am_I,true);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (mpuAdd << 1) | I2C_MASTER_READ, true);
	i2c_master_read_byte(cmd, &buffer, NACK_VAL);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, (1000/ portTICK_RATE_MS));
	i2c_cmd_link_delete(cmd);
	return buffer;
}

int16_t getTemp (int mpuAdd){
	uint8_t TempH, TempL;
	int16_t  TempRaw, TempDeg;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(mpuAdd << 1) | I2C_MASTER_WRITE,true);
	i2c_master_write_byte(cmd,MPU6050_TEMP,true);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (mpuAdd << 1) | I2C_MASTER_READ, true);
	i2c_master_read_byte(cmd, &TempH, ACK_VAL);
	i2c_master_read_byte(cmd, &TempL, NACK_VAL);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, (1000/ portTICK_RATE_MS));
	i2c_cmd_link_delete(cmd);
	TempRaw = (TempH << 8 | TempL);
	TempDeg = TempRaw/340+36.53;
	//printf("\n Temp: %d\n", TempDeg);
	return TempDeg;
}

void getRawAcc (int mpuAdd){
	uint8_t AccXH, AccXL, AccYH, AccYL,AccZH, AccZL ;
	int16_t AccX, AccY, AccZ;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd,(mpuAdd << 1) | I2C_MASTER_WRITE,true);
	i2c_master_write_byte(cmd,MPU6050_Raw_ACC,true);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (mpuAdd << 1) | I2C_MASTER_READ, true);
	i2c_master_read_byte(cmd, &AccXH, ACK_VAL);
	i2c_master_read_byte(cmd, &AccXL, ACK_VAL);
	i2c_master_read_byte(cmd, &AccYH, ACK_VAL);
	i2c_master_read_byte(cmd, &AccYL, ACK_VAL);
	i2c_master_read_byte(cmd, &AccZH, ACK_VAL);
	i2c_master_read_byte(cmd, &AccZL, NACK_VAL);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, (1000/ portTICK_RATE_MS));
	i2c_cmd_link_delete(cmd);

	AccX = (AccXH << 8 | AccXL);
	AccY = (AccYH << 8 | AccYL);
	AccZ = (AccZH << 8 | AccZL);

	esp_log_level_set("R_ACC", ESP_LOG_INFO);
	ESP_LOGI("R_ACC", "AccX : %d", AccX);
	ESP_LOGI("R_ACC", "AccY : %d", AccY);
	ESP_LOGI("R_ACC", "AccZ : %d", AccZ);
}

void getRawGyro(int mpuAdd){
    uint8_t GyroXH, GyroXL, GyroYH, GyroYL,GyroZH, GyroZL;
    int16_t GyroX, GyroY, GyroZ;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,(mpuAdd << 1) | I2C_MASTER_WRITE,true);
    i2c_master_write_byte(cmd,MPU6050_Raw_GYRO,true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (mpuAdd << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &GyroXH, ACK_VAL);
    i2c_master_read_byte(cmd, &GyroXL, ACK_VAL);
    i2c_master_read_byte(cmd, &GyroYH, ACK_VAL);
    i2c_master_read_byte(cmd, &GyroYL, ACK_VAL);
    i2c_master_read_byte(cmd, &GyroZH, ACK_VAL);
    i2c_master_read_byte(cmd, &GyroZL, NACK_VAL);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, (1000/ portTICK_RATE_MS));
    i2c_cmd_link_delete(cmd);
    GyroX = (GyroXH << 8 | GyroXL);
    GyroY = (GyroYH << 8 | GyroYL);
    GyroZ = (GyroZH << 8 | GyroZL);
    esp_log_level_set("G_ACC", ESP_LOG_INFO);
    ESP_LOGI("G_ACC", "GyroX : %d", GyroX);
    ESP_LOGI("G_ACC", "GyroY : %d", GyroY);
    ESP_LOGI("G_ACC", "GyroZ : %d", GyroZ);
}

void i2c_scan(void){
	int devices_found = 0;
	printf("Iniciando el Escaneo \r\n\r\n");
		for(int address = 1; address < 127; address++) {

			// create and execute the command link
			i2c_cmd_handle_t cmd = i2c_cmd_link_create();
			i2c_master_start(cmd);
			i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
			i2c_master_stop(cmd);
			if(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS) == ESP_OK) {
				printf("-> found device with address 0x%02x\r\n", address);
				devices_found++;
			}
			i2c_cmd_link_delete(cmd);
		}
		if(devices_found == 0) printf("\r\n-> no devices found\r\n");
		printf("\r\n...scan completed!\r\n");
}

// loop task
void loop_task(void *pvParameter)
{
	esp_log_level_set("I2C", ESP_LOG_INFO);

 	selectI2CChannel(PCA9548ADDR,PCA9548_CH1);
 	initMPU6050(MPU_ADD,PCA9548_CH1);
// 	i2c_scan();
// 	selectI2CChannel(PCA9548ADDR,PCA9548_CH2);
// 	initMPU6050(MPU_ADD,PCA9548_CH2);
// 	i2c_scan();
//
    while(1) {

    	mpuName = whoAMI(MPU_ADD);
    	ESP_LOGI("I2C", "MPU Name : 0x%02x", mpuName);
    	float tempD = getTemp(MPU_ADD);
    	ESP_LOGI("I2C", "MPU Temp: %0.2f", tempD);
    	printf("**************** \n");
    	getRawAcc(MPU_ADD);
    	printf("****************\n");
    	getRawGyro(MPU_ADD);
    	printf("****************\n");
		vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


// Main application
void app_main() {

	printf("i2c scanner\r\n\r\n");
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html
	// configure the i2c controller 0 in master mode, normal speed
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = 21;
	conf.scl_io_num = 22;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = I2C_MASTER_FREQ_HZ; //100000
	conf.clk_flags = 0; //(V4.4)  is 0, the clock allocator will select only according to the desired frequency.
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
	printf("- i2c controller configured\r\n");

	// install the driver
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
	printf("- i2c driver installed\r\n\r\n");


	//int devices_found = 0;

	// start the loop task
	xTaskCreate(&loop_task, "loop_task", 2048, NULL, 5, NULL);
}
