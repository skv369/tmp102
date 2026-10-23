/* Used when we work in kernel space */
#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/byteorder/generic.h>
#else
/* Used when we work in user space */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#endif

/*----------------------*/
/* Macros to define the register addresses */
/*----------------------*/

/* I2C Address Pins and Target Addresses (Table 6-4.) */

#define TMP102_ADDR_GND         0x48    /* GND (0b1001000) */
#define TMP102_ADDR_VCC         0x49    /* V+  (0b1001001) */
#define TMP102_ADDR_SDA         0x4A    /* SDA (0b1001010) */
#define TMP102_ADDR_SCL         0x4B    /* SCL (0b1001011) */

/* Pointer Addresses (Table 6-7.) */

#define TMP102_REG_TEMP         0x00    /* Temperature, read only   */
#define TMP102_REG_CONFIG       0x01    /* Configuration, R/W       */
#define TMP102_REG_TLOW         0x02    /* Low limit, R/W           */
#define TMP102_REG_THIGH        0x03    /* High limit, R/W          */

/* Electrical CHaracteristics (Table 5.5) */

#define TMP102_VCC_MIN_MV       1400	/*MV stands for Millivolts*/
#define TMP102_VCC_MAX_MV       3600
#define TMP102_TEMP_MIN_C       (-40)
#define TMP102_TEMP_MAX_C       125

/* Macros required for temperature conversion */

#define VAL_MASK_12_BIT 	0x0FFF
#define SIGN_MASK_12_BIT 	0x0800
#define SHIFT_12_BIT 		4
#define VAL_MASK_13_BIT 	0x1FFF
#define SIGN_MASK_13_BIT 	0x1000
#define SHIFT_13_BIT 		3



