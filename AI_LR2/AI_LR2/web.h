#pragma once
#include <fstream>

class Web
{
public:
	void Train();
	void RecognizeAll();
private:
	int mul[5][5];
	int weight[5][5];
	int input[5][5];
	const int limit = 10000;
	int sum = 0;
	void mul_w();
	void Sum();
	bool Result();
	void Load_Weight();
	void Study(bool prov, int& endstudy);
	void Recognize();
	void Inc_Web(int p[5][5]);
	void Dec_Web(int p[5][5]);
	void Get_Input(int** p);
	void Fill_input(std::istream& in);
};
