#include"SA.h"
#include<time.h>


int main() {
	clock_t startTime, endTime;
	startTime = clock();
	vector<float> x = { 1.96, -0.33 };
	Range MAX;
	MAX.MIN_X = -100;
	MAX.MAX_X = 100;
	vector<Range> range = { MAX, MAX };
	SimAnneal sa(1000, 0.001, 0.99, 0.15, x, range ,20);
	Status status = sa.annealStart();
	endTime = clock();
	cout << "Totle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

	system("pause");
	return 0;
}