#include<iostream>
#include<time.h>
#include"tabu.h"
using namespace std;


int main() {
	clock_t startTime, endTime;
	startTime = clock();
	vector<float> x = { 1.96, -0.233 };
	Range MAX;
	MAX.MIN_X = -100;
	MAX.MAX_X = 100;
	vector<Range> range = { MAX, MAX };
	Tabu tabu(50 , 10 , 5, x, range, 50000);
	Status status = tabu.Start();
	cout << "\nBest solution is: " << status.x[0] << " "
		<< status.x[1] << "\tmax value is: " << status.y << endl;

	endTime = clock();
	cout << "Totle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;


	system("pause");
	return 0;
}