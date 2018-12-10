#pragma once
#include<iostream>
#include<vector> 
#include<cmath> 
#include<cstdlib>   
#include<ctime>  

using namespace std;


struct Status {
	vector<float> x;
	float y;
};

struct Range {
	float MIN_X;
	float MAX_X;
};

class SimAnneal {
public:
	SimAnneal(float temp, float minTemp, float tempRate, 
		float step, vector<float> &initX, vector<Range> &xRange,int N);
	float fx(vector<float> x);
	vector<float> single(vector<float> x);
	Status annealStart();
	bool metropolis(vector<float> nextX, vector<float> x);
private:
	vector<float> preX;
	vector<float> bestX;
	vector<float> x;
	vector<Range> xRange;
	float minTemp;
	float temp;
	float tempRate;
	float step;
	int N;
};