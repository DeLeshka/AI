#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "web.h"

using namespace std;

typedef vector<vector<int>> IntMatrix;

void Web::mul_w()
{
	for (size_t i = 0; i < 5; i++)
		for (size_t j = 0; j < 5; j++)
			this->mul[i][j] = this->input[i][j] * this->weight[i][j];
}

void Web::Sum()
{
	this->sum = 0;
	for (const auto& row : this->mul)
		for (int signal : row)
			this->sum += signal;
}

bool Web::Result()
{
	return sum >= limit;
}

void Web::Get_Input(int** p)
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			input[i][j] = p[i][j];
}

void Web::Load_Weight()
{
	cout << "Введите название файла с весами: ";
	string filename_load;
	cin >> filename_load;
	ifstream load_weight(filename_load);


	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			load_weight >> weight[i][j];

	load_weight.close();
}

void Web::Recognize()
{
	mul_w();
	Sum();
	if (Result())
		cout << " - True, Sum = " << sum << endl;
	else
		cout << " - False, Sum = " << sum << endl;
}

void Web::Study(bool prov, int& endstudy)
{
	mul_w();
	Sum();
	if (prov > Result())// неправильное за правильное
	{
		endstudy += 1;
		Inc_Web(input);
	}
	else if (prov < Result())
	{
		endstudy += 1;
		Dec_Web(input);
	}
}

void Web::Inc_Web(int p[5][5])
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			weight[i][j] += p[i][j];
}

void Web::Dec_Web(int p[5][5])
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			weight[i][j] -= p[i][j];
}

void Web::Train()
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			this->weight[i][j] = 0;

	string filenames_God[7];
	for (int i = 0; i < 7; i++)
	{
		filenames_God[i] = "Good/" + to_string(i + 1) + ".txt";
	}

	string filenames_Bad[41];
	for (int i = 0; i < 41; i++)
	{
		filenames_Bad[i] = "Bad/" + to_string(i) + ".txt";
	}

	vector<int**> Inputs;
	for (int k = 0; k < 48; k++)
	{
		int** p_mem_input = new int* [5];
		for (int i = 0; i < 5; ++i)
			p_mem_input[i] = new int[5];

		if (k < 41)
		{
			ifstream load_input(filenames_Bad[k]);
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
				{
					int ch;
					load_input >> ch;
					p_mem_input[i][j] = ch;
				}
			load_input.close();
			Inputs.push_back(p_mem_input);
		}
		else
		{
			ifstream load_input(filenames_God[k - 41]);
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					load_input >> p_mem_input[i][j];
			load_input.close();
			Inputs.push_back(p_mem_input);
		}
	}
	int endstudy;
	int round = 0;
	do
	{
		endstudy = 0;
		int k = 0;
		for (auto it : Inputs)
		{
			this->Get_Input(it);
			if (k < 41)
			{
				this->Study(0, endstudy);
			}
			else
			{
				this->Study(1, endstudy);
			}
			k += 1;
		}
		round += 1;
		cout << round << " раунд завершен" << endl;
	} while (endstudy != 0);
	cout << "Обучение окончено!" << endl;

	cout << "Введите файл для сохранения веса: ";
	string filename_save;
	cin >> filename_save;
	ofstream save_weight(filename_save);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
			save_weight << this->weight[i][j] << ' ';
		save_weight << endl;
	}
	save_weight.close();
}

void Web::Fill_input(istream& in)
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			in >> this->input[i][j];
}

void Web::RecognizeAll()
{
	this->Load_Weight();
	for (int i = 1; i < 8; i++)
	{
		ifstream input_file("Good\\" + to_string(i) + ".txt");
		this->Fill_input(input_file);
		cout << "Файл Good\\" << i << ".txt";
		this->Recognize();
	}
	for (int i = 1; i < 41; i++)
	{
		ifstream input_file("Bad\\" + to_string(i) + ".txt");
		this->Fill_input(input_file);
		cout << "Файл Bad\\" << i << ".txt";
		this->Recognize();
	}
}