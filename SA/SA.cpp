#include"SA.h"

float random() {
	return rand()*1.0 / RAND_MAX;
}


float SimAnneal::fx(vector<float> x) {

	double s1 = pow(sin(sqrt(x[0] * x[0] + x[1] * x[1])*sqrt(x[0] * x[0] + x[1] * x[1])), 2) - 0.5;
	double s2 = pow(1 + 0.0001*(x[0] * x[0] + x[1] * x[1]), 2);
	return 4.5 - s1 / s2;
}

SimAnneal::SimAnneal(float temp, float minTemp, float tempRate,
	float step, vector<float> &initX, vector<Range> &xRange, int N)
	:temp(temp), minTemp(minTemp), tempRate(tempRate),
	x(initX), preX(initX), bestX(initX), xRange(xRange), step(step), N(N)
{}

vector<float> SimAnneal::single(vector<float> x) {
	vector<float> tempX = x;
	float p;
	float delta;

	for (int i = 0; i < tempX.size(); i++) {
		do {
			delta = step * random();
			p = random();
			if (p > 0.5) delta *= -1;
			tempX[i] += delta;
		} while (tempX[i] < xRange[i].MIN_X 
			|| tempX[i] > xRange[i].MAX_X);
	}

	return tempX;
}

bool SimAnneal::metropolis(vector<float> nextX, vector<float> x) {
	float p = exp((fx(nextX) - fx(x)) / temp);
	if (random() < p) return 1;
	else return 0;
}

Status SimAnneal::annealStart() {
	vector<float> tempX;
	srand(time(0));
	int count = 0;
	while (temp > minTemp) {
		count++;
		for (int n = 0; n < N; n++) {
			tempX = single(x);
			if (fx(tempX) > fx(bestX)) {
				preX = bestX;
				bestX = tempX;
				cout << "更新:x = " << bestX[0] << " " << bestX[1]
					<< "\ty = " << fx(bestX) << endl;
			}
			if (fx(tempX) > fx(x)) {
				x = tempX;
			}
			else {
				if (metropolis(tempX, x)) {
					x = tempX;
				}
			}
		}
		temp *= tempRate;
	}

	Status status;
	status.x = bestX;
	status.y = fx(bestX);

	cout << "迭代次数："<<count<<endl << "最优解:x = " << 
		bestX[0] << " " << bestX[1] << " " << "y = " << status.y << endl;
	return status;
}
