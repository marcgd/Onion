#ifndef ADC_H__
#define ADC_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <pthread.h>
#include <signal.h>

#ifdef PLATFORM_MIYOOMINI
#include "shmvar/shmvar.h"
#endif

#include "system/system.h"
#include "system/display.h"
#include "utils/flags.h"
#include "utils/config.h"
#include "utils/file.h"
#include "utils/log.h"

#define CHECK_BATTERY_TIMEOUT 10000 //ms - check battery percentage every 10s

// for reading battery
#define SARADC_IOC_MAGIC 'a'
#define IOCTL_SAR_INIT _IO(SARADC_IOC_MAGIC, 0)
#define IOCTL_SAR_SET_CHANNEL_READ_VALUE _IO(SARADC_IOC_MAGIC, 1)
typedef struct {
    int channel_value;
    int adc_value;
} SAR_ADC_CONFIG_READ;

static SAR_ADC_CONFIG_READ adcConfig = {0,0};
static bool adcthread_active;
static pthread_t adc_pt;
static bool quit = false;
static int sar_fd, adc_value;
static bool is_suspended = false;

static void sigHandler(int sig);
void cleanup(void);
bool isCharging(void);
int updateADCValue(int);
int batteryPercentage(int);
static void* batteryWarning_thread(void* param);
void batteryWarning_show(void);
void batteryWarning_hide(void);

#endif // ADC_H__
