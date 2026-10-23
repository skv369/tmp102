/*
 * test_tmp102.c - checks tmp102_raw_to_celsius() against Table 6-2
 */
#include <stdint.h>
#include <stdio.h>
#include "myheader.h"

float tmp102_raw_to_celsius(uint16_t reg)
{
	uint16_t val_mask, sign_mask, raw;
	float celsius;
	if (reg & 0x1000) {
		shift = SHIFT_13_BIT;
		val_mask = VAL_MASK_13_BIT;
		sign_mask = SIGN_MASK_13_BIT;
	} else {
		shift = SHIFT_12_BIT;
		val_mask = VAL_MASK_12_BIT;
		sign_mask = SIGN_MASK_12_BIT;
	}
	
	raw = (reg >> shift) & val_mask;
	negative = (raw & sign_mask) != 0;
	
	if (negative) {
		raw = (~raw + 1) & val_mask;
	}
	celsius = raw * RESOLUTION_VALUE;
	return negative ? -celsius : celsius;
}

struct tmp102_case {
	uint16_t reg;	/* register value = 12-bit value << 4 */
	float expect;
};

static const struct tmp102_case cases[] = {
	{ 0x7FF0u, 127.9375f },
	{ 0x1900u, 25.0f },
	{ 0x0000u, 0.0f },
	{ 0xFFC0u, -0.25f },
	{ 0xE700u, -25.0f },
	{ 0xC900u, -55.0f },
};

int main(void)
{
	unsigned int i;
	int fails = 0;

	for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
		float got = tmp102_raw_to_celsius(cases[i].reg);

		if (got != cases[i].expect) {
			printf("FAIL 0x%04X: got %f, expected %f\n",
			       cases[i].reg, got, cases[i].expect);
			fails++;
		} else {
			printf("ok   0x%04X -> %8.4f C\n", cases[i].reg, got);
		}
	}
	return (fails == 0) ? 0 : 1;
}
