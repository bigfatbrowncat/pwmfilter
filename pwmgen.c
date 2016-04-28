#include <stdio.h>
#include <math.h>

double sin_freq = 1e+3 /* 1kHz */ ;
double fu(double t) {
	return sin(2.0 * M_PI * sin_freq * t);
}

int main() {
	
	double t1 = 0, t2 = 3e-3 /* 1ms */ ;
	double pwm_freq = 200 * 1e+3 /* 200kHz */ ;
	double a = 5; /* 5V */
	
	int points = (t2 - t1) / pwm_freq;
	int points_per_value = 256;
	
	/*for (int vi = 0; vi < points / points_per_value; vi++)*/
	for (double t = t1; t < t2; t += 1.0 / pwm_freq) {
		
		int ones = (int)((fu(t) + 1.0) / 2.0 * points_per_value);
		
		for (int p = 0; p < points_per_value; p++) {
			double tx = t + (double)p / pwm_freq / points_per_value;
			printf("%e,%e\n", tx, (p < ones ? a : 0.0));
		}
	}
	
	return 0;

}