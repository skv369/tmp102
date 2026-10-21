/*
 * test_tmp102.c - checks tmp102_raw_to_celsius() against Table 6-2
 */
#include <stdint.h>
#include <stdio.h>

float tmp102_raw_to_celsius(uint16_t reg)
{
	int32_t raw = (reg >> 4) & 0x0FFF;	/* keep the 12 temperature bits */

	if (raw & 0x0800) {			/* sign bit (T11) set -> negative */
		raw -= 0x1000;			/* sign-extend 12-bit two's complement */
	}
	return raw * 0.0625f;
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
