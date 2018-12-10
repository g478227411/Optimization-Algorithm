#include"tabu.h"


float random() {
	return rand()*1.0 / RAND_MAX;
}


float Tabu::fx(vector<float> x) {

	double s1 = pow(sin(sqrt(x[0] * x[0] + x[1] * x[1])*sqrt(x[0] * x[0] + x[1] * x[1])), 2) - 0.5;
	double s2 = pow(1 + 0.0001*(x[0] * x[0] + x[1] * x[1]), 2);
	return 4.5 - s1 / s2;
}

Tabu::Tabu(int size,int length,
	float step, vector<float> &initX, vector<Range> &xRange, int N)
	:size(size),length(length),
	x(initX), preX(initX), bestX(initX), xRange(xRange), step(step), N(N)
{
	best = fx(bestX);
}

vector<float> Tabu::single(vector<float> x) {
	vector<float> tempX = x;
	float p;
	float delta;
	float dp;
	
	for (int i = 0; i < tempX.size(); i++) {
		do {
			delta = step * random();
			p = random();
			dp = (tempX[i] - xRange[i].MIN_X) / (xRange[i].MAX_X - xRange[i].MIN_X);
			//dp = (dp - 0.5) * 10 + 0.5;
			
			//cout <<x[i]<< "i, dp: "<<dp<<endl;
			if (p > dp) delta *= -1;
			tempX[i] += delta* exp(abs(dp - 0.5));
		} while (tempX[i] < xRange[i].MIN_X
			|| tempX[i] > xRange[i].MAX_X);
	}

	return tempX;
}
bool Tabu::istaboo(vector<float> x0) {
	int num = x0.size();
	float tabu_range = step * step/16 ;
	for (auto x : this->tabulist) {
		double diff = 0;
		for (int i = 0; i < num; i++) {
			diff += pow(x[i] - x0[i],2);
		}
		if (diff < tabu_range)return true;
	}
	return false;
}

Status Tabu::Start() {

	for (int n = 0; n < N; n++) {
		step *= 0.99;
		vector<vector<float>> nearby;
		for (int i = 0; i < size; i++) {
			vector<float>temp = single(preX);
			if (!istaboo(temp)) {
				nearby.push_back(temp);
			}
			else {
				if (fx(temp) > best) {
					nearby.push_back(temp);
				}
			}
		}
		
		if (nearby.size() == 0)break;

		//cout << nearby.size() << endl;

		vector<float> temp_bestX=nearby[0];
		float temp_best=fx(temp_bestX);
		for (auto node : nearby) {
			if (fx(node) > temp_best) {
				temp_bestX = node;
				temp_best = fx(node);
			}
		}


		tabulist.push_back(temp_bestX);
		if (tabulist.size() > length) {
			tabulist.pop_front();
		}
		if (temp_best > best) {
			bestX = temp_bestX;
			best = temp_best;
			cout << "¸üÐÂ£ºx = ";
			for (auto x : temp_bestX) {
				cout << " " << x;
			}
			cout << "\ty = " << temp_best << endl;
		}
	}
	
	Status status;
	status.x = bestX;
	status.y = best;
	return status;
}

