#include <math.h>
#include <stdio.h>

int sinewave_length_div_16 = 32;

int main() {

	printf("const int sinewave_length = %d;\n", sinewave_length_div_16 * 16);
	printf("const unsigned char sinewave_data[] PROGMEM = {\n");
	for (int i = 0; i < sinewave_length_div_16; i++) {
		printf("\t");
		for (int j = 0; j < 16; j++) {
			int k = i * 16 + j;
			int v = 128 * sin(2 * 3.14159 * k / (sinewave_length_div_16 * 16)) + 128;
			v = v > 255 ? 255 : v;
			
			printf("0x%02x", v);
			if (j < 15 || i < sinewave_length_div_16 - 1) printf(", ");
		}
		printf("\n");
	}
	printf("};\n");
}