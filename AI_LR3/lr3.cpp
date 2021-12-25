#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>


using namespace std;

float fderiv(float z)//функция, эквивалентная взятию производной
{
	return (1 - z) * z;
}

float deltaO(int x, float y)
{
	return (x - y) * fderiv(y);
}

float deltaH(float k, float l, float h)//функция верна только в том случае, если у скрытых (H) нейронов на выходе лишь один
{									   //синапс. По-настоящему формула выглядит так:
	return fderiv(h) * (k * l);		   // f'(in)*algebSum(wi*di)
}

float sigmoid(float x)
{
	return 1 / (1 + exp(-x));
}

void menu()
{
	cout << "(1)Проверка \n(2)Обучение \n(3)Сброс \n(4)Выход \n\n(1 or 2 or 3 or 4)?" << endl;
}

int main()
{
	setlocale(LC_ALL, "");
	const float E = 0.4;//эпсилон - это скорость обучения
	const float A = 0.3;//альфа - это момент
	/*const*/ int maxEpoch = 15000;
	const int trainSet = 4;
	string dec;
	int I1, I2, O1ideal;
	float prevW;
	//float w1 = 0.45, w2 = 0.78, w3 = -0.12, w4 = 0.13, w5 = 1.5, w6 = -2.3;
	float w1, w2, w3, w4, w5, w6;
	float H1input, H1output, H2input, H2output, O1input, O1output, Error, dO1, dH1, dH2;
	float** changeW = new float* [6];//6 строк = количеству синапсов
	for (int count = 0; count < 6; count++)
		changeW[count] = new float[trainSet];// 4 столбца = количеству тренировочных сетов (trainSet'ов) или же, по-другому, итераций
	vector<float> GRADw(6);
	vector<float> O1outError(4);
	////////////////////////////////////////////
	ifstream Fop("wValues.txt");
	Fop >> w1;
	Fop >> w2;
	Fop >> w3;
	Fop >> w4;
	Fop >> w5;
	Fop >> w6;
	Fop.close();
	///////////////////////////////////////////
	cout << "Нейронная сеть, решающая 'Исключающее ИЛИ' (XOR)"<<endl<<endl<<endl;
	menu();
	while (1)
	{
		cout << ">";
		cin >> dec;
		if (dec == "4")
			break;
		if (dec == "3")
		{
			string drop;
			cout << endl <<"Вы уверены, что хотите сбросить нейронную сеть? Это действие необратимо! (Yes or No)" << endl;
			while (1)
			{
				cout << ">";
				cin >> drop;
				if (drop == "Yes")
				{
					w1 = 0.45; w2 = 0.78; w3 = -0.12; w4 = 0.13; w5 = 1.5; w6 = -2.3;
					ofstream Fcl("wValues.txt");
					Fcl << w1 << endl << w2 << endl << w3 << endl << w4 << endl << w5 << endl << w6;
					Fcl.close();
					menu();
					break;
				}
				if (drop == "No")
				{
					menu();
					break;
				}
			}
		}
		if (dec == "1")
		{
			cout << endl << "Введите по очереди две переменные со значениями, равными 0 или 1\n";
			cout << "I1 = ";
			cin >> I1;
			cout << "I2 = ";
			cin >> I2;
			//////
			if (((I1 != 1) && (I1 != 0)) || ((I2 != 1) && (I2 != 0)))
			{
				cout << endl << "Некорректные значения!" << endl << endl;
				menu();
				continue;
			}
			if (((I1 == 1) && (I2 == 1)) || ((I1 == 0) && (I2 == 0)))
				O1ideal = 0;
			else O1ideal = 1;
			H1input = I1 * w1 + I2 * w3;
			H1output = sigmoid(H1input);
			H2input = I1 * w2 + I2 * w4;
			H2output = sigmoid(H2input);
			O1input = H1output * w5 + H2output * w6;
			O1output = sigmoid(O1input);
			float OutError = abs(O1ideal - O1output);
			//////
			cout << endl << "Ответ равен " << O1output << endl << endl << "Ошибка " << OutError*100 << "%" << endl << endl;
			menu();
		}
		if (dec == "2")
		{
			cout << endl << "Введите количество эпох\n";
			cout << "maxEpoch = ";
			cin >> maxEpoch;
			cout << endl;
			for (int i = 0; i < maxEpoch; i++)
			{
				for (int j = 0; j < trainSet; j++)
				{
					switch (j)
					{
					case 0:
						I1 = 0;
						I2 = 0;
						O1ideal = 0;
						break;
					case 1:
						I1 = 0;
						I2 = 1;
						O1ideal = 1;
						break;
					case 2:
						I1 = 1;
						I2 = 0;
						O1ideal = 1;
						break;
					case 3:
						I1 = 1;
						I2 = 1;
						O1ideal = 0;
						break;
					}
					///////////////////////////////////
					H1input = I1 * w1 + I2 * w3;
					H1output = sigmoid(H1input);
					H2input = I1 * w2 + I2 * w4;
					H2output = sigmoid(H2input);
					O1input = H1output * w5 + H2output * w6;
					O1output = sigmoid(O1input);
					O1outError[j] = pow((O1ideal - O1output), 2);
					cout << "Эпоха " << i << ", Сет " << j << ":" << endl << "I1=" << I1 << "; I2=" << I2 << "; w1=" << w1 << "; w2=" << w2 << "; w3=" << w3 << ";w4=" << w4 << "; w5=" << w5 << "; w6=" << w6 << endl << " Результат равен " << O1output << ";" << endl << endl;
					////////////////////////////////для O1
					dO1 = deltaO(O1ideal, O1output);
					////////////////////////////////для w5
					dH1 = deltaH(w5, dO1, H1output);
					GRADw[4] = H1output * dO1;
					if (j == 0)
						prevW = 0;
					else
						prevW = changeW[4][j - 1];
					changeW[4][j] = E * GRADw[4] + A * prevW;
					w5 = w5 + changeW[4][j];
					//////////////////////////////////для w6
					dH2 = deltaH(w6, dO1, H2output);
					GRADw[5] = H2output * dO1;
					if (j == 0)
						prevW = 0;
					else
						prevW = changeW[5][j - 1];
					changeW[5][j] = E * GRADw[5] + A * prevW;
					w6 = w6 + changeW[5][j];
					///////////////////////////////для w1, w2, w3, w4
					GRADw[0] = I1 * dH1;
					GRADw[1] = I1 * dH2;
					GRADw[2] = I2 * dH1;
					GRADw[3] = I2 * dH2;
					/////////////////////////////////////////
					if (j == 0)
						prevW = 0;
					else
						prevW = changeW[0][j - 1];
					changeW[0][j] = E * GRADw[0] + A * prevW;
					w1 = w1 + changeW[0][j];
					//////////////////////////////////////////////
					if (j == 0)
						prevW = 0;
					else
						prevW = changeW[1][j - 1];
					changeW[1][j] = E * GRADw[1] + A * prevW;
					w2 = w2 + changeW[1][j];
					//////////////////////////////////////////
					if (j == 0)
						prevW = 0;
					else
						prevW = changeW[2][j - 1];
					changeW[2][j] = E * GRADw[2] + A * prevW;
					w3 = w3 + changeW[2][j];
					////////////////////////////////////////////
					if (j == 0)
						prevW = 0;
					else
						prevW = changeW[3][j - 1];
					changeW[3][j] = E * GRADw[3] + A * prevW;
					w4 = w4 + changeW[3][j];
				}
				Error = (O1outError[0] + O1outError[1] + O1outError[2] + O1outError[3]) / trainSet;
				cout << "Ошибка равна " << Error * 100 << "%." << endl << endl;
			}
			/////////////////////////////////////////
			for (int count = 0; count < 6; count++)
				delete[]changeW[count];
			/////////////////////////////////////////
			ofstream Fcl("wValues.txt");
			Fcl << w1 << endl << w2 << endl << w3 << endl << w4 << endl << w5 << endl << w6;
			Fcl.close();
			menu();
		}
		/////////////////////////////////////////
	}
	system("pause");
	return 0;
}
