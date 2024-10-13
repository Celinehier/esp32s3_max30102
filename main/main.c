#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "main.h"
#include "max30102_api.h"
#include "algorithm.h"
#include "i2c_api.h"
//#include "mqtt_wifi.h"
#include <stdio.h>
#include <stdint.h>


TaskHandle_t processor_handle = NULL;
TaskHandle_t sensor_reader_handle = NULL;

int32_t red_data = 0;
int32_t ir_data = 0;
int32_t red_data_buffer[BUFFER_SIZE];
int32_t ir_data_buffer[BUFFER_SIZE];
double auto_correlationated_data[BUFFER_SIZE];

char *data = NULL;

#define DELAY_AMOSTRAGEM 40

#define DEBUG true

void app_main(void)
{
	//init_wifi();
	//mqtt_app_start();
	xTaskCreatePinnedToCore(sensor_data_reader, "Data", 10240, NULL, 2, &sensor_reader_handle, 1);
}


void sensor_data_reader(void *pvParameters)
{
    uint8_t sta_mac[6] = {0};
    esp_wifi_get_mac(ESP_IF_WIFI_STA, sta_mac);
	i2c_init();
	vTaskDelay(pdMS_TO_TICKS(100));
	max30102_init(&max30102_configuration);
	init_time_array();
	uint64_t ir_mean;
	uint64_t red_mean;
	float temperature;
	double r0_autocorrelation;
	//size_t size;

	for(;;){
		//vTaskDelay(pdMS_TO_TICKS(100));
		fill_buffers_data();
		temperature = get_max30102_temp();
		remove_dc_part(ir_data_buffer, red_data_buffer, &ir_mean, &red_mean);
		remove_trend_line(ir_data_buffer);
		remove_trend_line(red_data_buffer);
		double person_correlation = correlation_datay_datax(red_data_buffer, ir_data_buffer);
		int heart_rate = calculate_heart_rate(ir_data_buffer, &r0_autocorrelation, auto_correlationated_data);

		printf("\n");
		printf("HEART_RATE %d\n", heart_rate);
		printf("correlation %f\n", person_correlation);
		printf("Temperature %f\n", temperature);

		if(person_correlation >= 0.7){ //Os dois sinais IR e RED devem ser fortemente relacionados.
			double spo2 = spo2_measurement(ir_data_buffer, red_data_buffer, ir_mean, red_mean);
			printf("SPO2 %f\n", spo2);

	        //size = asprintf(&data, "{\"mac\": \"%02x%02x%02x%02x%02x%02x\", \"spo2\":%f, \"heart_rate\":%d}",MAC2STR(sta_mac), spo2, heart_rate);
			// mqtt_publish(data, size);
		}
		printf("\n");

#if DEBUG
		for(int i = 0; i < BUFFER_SIZE; i++){
			printf("%ld", ir_data_buffer[i]);
			printf(" ");
			printf("%ld", red_data_buffer[i]);
			printf(" ");
			printf("%f\n", auto_correlationated_data[i]);
		}
#endif
	}
}


void fill_buffers_data()
{
	for(int i = 0; i < BUFFER_SIZE; i++){
		read_max30102_fifo(&red_data, &ir_data);
		ir_data_buffer[i] = ir_data;
		red_data_buffer[i] = red_data;
		//printf("%d\n", red_data);
		ir_data = 0;
		red_data = 0;
		vTaskDelay(pdMS_TO_TICKS(DELAY_AMOSTRAGEM));
	}
}
