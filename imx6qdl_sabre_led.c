#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_LEDS 16

typedef struct {
    char name[32];
    int state;
} LED;

LED leds[TOTAL_LEDS];

// Display LED states
void show_leds() {
    printf("\n--- LED STATUS ---\n");
    for (int i = 0; i < TOTAL_LEDS; i++) {
        printf("LED%-2d (%-12s): %s\n", i, leds[i].name, leds[i].state ? "ON" : "OFF");
    }
    printf("------------------\n");
}

// Turn on a range of LEDs
void led_on_range(int start, int end) {
    for (int i = start; i <= end && i < TOTAL_LEDS; i++)
        leds[i].state = 1;
    show_leds();
}

// Turn off all LEDs
void led_off_all() {
    for (int i = 0; i < TOTAL_LEDS; i++)
        leds[i].state = 0;
}

// Simulate boot process
void simulate_boot() {
    printf("=== i.MX6QDL Sabre SD Boot LED Simulation ===\n");

    // Power ON
    printf("\n[POWER] Turning ON Power LED...\n");
    led_on_range(0, 0);
    sleep(1);

    // U-Boot stage - init stage
    printf("\n[U-BOOT] LEDs 1-4 ON\n");
    led_on_range(1, 4);
    sleep(1);

    // Kernel boot stage
    printf("\n[KERNEL] LEDs 5-8 ON\n");
    led_on_range(5, 8);
    sleep(1);

    // RootFS stage - final stage
    printf("\n[ROOTFS] LEDs 9-11 ON\n");
    led_on_range(9, 11);
    sleep(1);

    // Services: Wi-Fi, Bluetooth, Ethernet
    printf("\n[SERVICES] Wi-Fi / BT / Ethernet Initialization\n");
    leds[12].state = 1; strcpy(leds[12].name, "Wi-Fi");
    leds[13].state = 1; strcpy(leds[13].name, "Bluetooth");
    leds[14].state = 1; strcpy(leds[14].name, "Ethernet");
    show_leds();
    sleep(1);

    // System Ready LED
    printf("\n✅ Boot process complete! System Ready LED ON\n");
    leds[15].state = 1; strcpy(leds[15].name, "System Ready");
    show_leds();
}

int main() {
    // Initialize LED names
    strcpy(leds[0].name, "Power");
    strcpy(leds[1].name, "U-Boot1");
    strcpy(leds[2].name, "U-Boot2");
    strcpy(leds[3].name, "U-Boot3");
    strcpy(leds[4].name, "U-Boot4");
    strcpy(leds[5].name, "Kernel1");
    strcpy(leds[6].name, "Kernel2");
    strcpy(leds[7].name, "Kernel3");
    strcpy(leds[8].name, "Kernel4");
    strcpy(leds[9].name, "RootFS1");
    strcpy(leds[10].name, "RootFS2");
    strcpy(leds[11].name, "RootFS3");
    strcpy(leds[12].name, "Wi-Fi");
    strcpy(leds[13].name, "Bluetooth");
    strcpy(leds[14].name, "Ethernet");
    strcpy(leds[15].name, "System Ready");

    led_off_all();
    simulate_boot();

    return 0;
}

