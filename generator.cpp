#include <iostream>
#include <cmath>
#include <time.h>

using namespace std;

int rand_exp(int from, int to) {
  while (true) {
		srand(time(NULL));
		float r = 1 / (float)rand();
		float a = - 1 / (float)rand();
		if ( r < exp (a)) {
			return from + (int) a * to;
		}
	}
}
