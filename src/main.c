#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define CROSS_TIME 1000

SemaphoreHandle_t nsSemaphore[4];
SemaphoreHandle_t ewSemaphore[4];

int ns_car_position[4] = {0};
int ew_car_position[4] = {0};

typedef struct {
    int green_time_ns;
    int yellow_time_ns;
    int red_time_ns;
    int green_time_ew;
    int yellow_time_ew;
    int red_time_ew;
} TrafficTimes;

TrafficTimes crossroadTimes[4];

void initializeCrossroadTimes() {
    crossroadTimes[0] = (TrafficTimes){2000, 1000, 2000, 2000, 1000, 2000};
    crossroadTimes[1] = (TrafficTimes){3000, 1500, 3000, 2500, 1000, 2500};
    crossroadTimes[2] = (TrafficTimes){4000, 1000, 4000, 1500, 1000, 1500};
    crossroadTimes[3] = (TrafficTimes){2500, 1000, 2500, 3000, 1000, 3000};
}

void TrafficLightTask(void *params) {
    int crossroad_id = (int)params;

    while (1) {
        xSemaphoreGive(nsSemaphore[crossroad_id]);
        xSemaphoreTake(ewSemaphore[crossroad_id], 0);
        printf("Crossroad %d: North-South light: Green | East-West light: Red\n", crossroad_id);
        vTaskDelay(pdMS_TO_TICKS(crossroadTimes[crossroad_id].green_time_ns));

        printf("Crossroad %d: North-South light: Yellow | East-West light: Red\n", crossroad_id);
        vTaskDelay(pdMS_TO_TICKS(crossroadTimes[crossroad_id].yellow_time_ns));

        xSemaphoreGive(ewSemaphore[crossroad_id]);
        xSemaphoreTake(nsSemaphore[crossroad_id], 0);
        printf("Crossroad %d: North-South light: Red | East-West light: Green\n", crossroad_id);
        vTaskDelay(pdMS_TO_TICKS(crossroadTimes[crossroad_id].green_time_ew));

        printf("Crossroad %d: North-South light: Red | East-West light: Yellow\n", crossroad_id);
        vTaskDelay(pdMS_TO_TICKS(crossroadTimes[crossroad_id].yellow_time_ew));
    }
}

void NorthSouthVehicleTask(void *params) {
    int crossroad_id = (int)params;

    while (1) {
        printf("Vehicles approaching the North-South traffic light at Crossroad %d\n", crossroad_id);

        if (xSemaphoreTake(nsSemaphore[crossroad_id], portMAX_DELAY)) {
            printf("Vehicles crossing from North to South at Crossroad %d\n", crossroad_id);

            for (int i = 1; i <= 2; i++) {
                ns_car_position[crossroad_id] = i;
                printf("Cars in North-South direction at Crossroad %d: %d\n", crossroad_id, ns_car_position[crossroad_id]);
                vTaskDelay(pdMS_TO_TICKS(CROSS_TIME / 2));
            }

            printf("Vehicles have crossed North-South at Crossroad %d\n", crossroad_id);
            ns_car_position[crossroad_id] = 0;
        }

        int delay_time = (rand() % 8 + 3) * 1000;
        printf("Next vehicles in North-South direction at Crossroad %d will arrive in %d ms\n", crossroad_id, delay_time);
        vTaskDelay(pdMS_TO_TICKS(delay_time));
    }
}

void EastWestVehicleTask(void *params) {
    int crossroad_id = (int)params;

    while (1) {
        printf("Vehicles approaching the East-West traffic light at Crossroad %d\n", crossroad_id);

        if (xSemaphoreTake(ewSemaphore[crossroad_id], portMAX_DELAY)) {
            printf("Vehicles crossing from East to West at Crossroad %d\n", crossroad_id);

            ew_car_position[crossroad_id] = 1;
            printf("Cars in East-West direction at Crossroad %d: %d\n", crossroad_id, ew_car_position[crossroad_id]);
            vTaskDelay(pdMS_TO_TICKS(CROSS_TIME));

            printf("Vehicles have crossed East-West at Crossroad %d\n", crossroad_id);
            ew_car_position[crossroad_id] = 0;
        }

        int delay_time = (rand() % 8 + 3) * 1000;
        printf("Next vehicles in East-West direction at Crossroad %d will arrive in %d ms\n", crossroad_id, delay_time);
        vTaskDelay(pdMS_TO_TICKS(delay_time));
    }
}

int main(void) {
    srand(time(NULL));

    initializeCrossroadTimes();

    for (int i = 0; i < 4; i++) {
        nsSemaphore[i] = xSemaphoreCreateBinary();
        ewSemaphore[i] = xSemaphoreCreateBinary();
        xSemaphoreTake(ewSemaphore[i], 0);
    }

    for (int i = 0; i < 4; i++) {
        xTaskCreate(TrafficLightTask, "Traffic Light Controller", configMINIMAL_STACK_SIZE, (void *)i, 1, NULL);
        xTaskCreate(NorthSouthVehicleTask, "North-South Vehicles", configMINIMAL_STACK_SIZE, (void *)i, 1, NULL);
        xTaskCreate(EastWestVehicleTask, "East-West Vehicles", configMINIMAL_STACK_SIZE, (void *)i, 1, NULL);
    }

    vTaskStartScheduler();

    while (1) {}

    return 0;
}
