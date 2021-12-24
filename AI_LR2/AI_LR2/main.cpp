#include <iostream>

#include "web.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	int command;
	while (true)
	{
		cout << "1 - Обучить\n"
			 << "2 - Распознать все файлы\n"
			 << "3 - Распознать конкретный файл\n"
			 << "0 - Выход\n"
			 << "Выберите действие: ";
		cin >> command;
		switch (command)
		{
		case 1:
		{
			Web web;
			web.Train();
			break;
		}
		case 2:
		{
			Web web;
			web.RecognizeAll();
			break;
		}
		case 3:
		{
			Web web;
			web.RecognizeFile();
			break;
		}
		case 0:
			return 0;

		default:
				cout << "Некорректный номер команды\n";
				cin >> command;
				break;
		}
	}
	return 0;
}