#pragma once
#include<iostream>
#include<vector> 
#include<cmath> 
#include<cstdlib>   
#include<ctime>  
#include<deque>

using namespace std;


struct Status {
	vector<float> x;
	float y;
};

struct Range {
	float MIN_X;
	float MAX_X;
};

class Tabu {
public:
	Tabu(int size,int length,
		float step, vector<float> &initX, vector<Range> &xRange, int N);
	float fx(vector<float> x);
	vector<float> single(vector<float> x);
	bool istaboo(vector<float> x0);
	Status Start();
private:
	vector<float> preX;
	vector<float> bestX;
	float best;
	vector<float> x;
	vector<Range> xRange;
	float step;
	int size;
	int length;
	deque<vector<float>> tabulist;
	int N;
};