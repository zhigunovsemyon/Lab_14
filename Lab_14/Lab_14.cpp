/*
	Вывести индексы всех отрицательных чисел. Умножить положительные элементы массива на 3
	Включить следующие возможности:  считывание массива из файла;
	сохранение массива в другой файл после выполненных с ним
	преобразований (если такой файл есть, его предыдущее содержимое перезаписывается).
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#define MULTIPLIER	3	//Множитель изменения массива
#define ARR_INIT	32	//Исходный размер массива
int GetArrSize(int maxArraySize);
int WayToWork(void);
int FileSolution(int* arr, int maxArraySize, FILE* text);
FILE* OpenUsersFile(const char mode[]);
void ManualSolution(int arrSize, int* nums);
void RandomSolution(int arrSize, int* nums, int a, int b);
int DoWrite(void);
void PrintIndexes(FILE* outstream, int arrSize, int* nums);
void ChangeArr(int arrSize, int* nums);
void PrintArr(FILE* outstream, int arrSize, int* nums, const char* header);

int main(void)
{
	int nums[ARR_INIT];	//Обозначение массива чисел
	int arrSize;

	switch (WayToWork())//Заполнение массива в соответствии с выбранным способом
	{
	case 0:
		arrSize = FileSolution(nums, ARR_INIT, OpenUsersFile("r"));
		//Заполнение массива из файла, возврат в переменную arrSize размера полученного массива.
		//Для этого передали ей массив, максимальный его размер, сам файл, открытый на чтение
		break;
	case 1:
		arrSize = GetArrSize(ARR_INIT); //Ручной ввод размера массива, с которыми будет работа
		ManualSolution(arrSize, nums);
		//Функция ручного заполнения массива. Принимает размер массива и сам массив
		break;
	case 2:
		arrSize = GetArrSize(ARR_INIT); //Ручной ввод размера массива, с которыми мы будем работать

		int a, b;
		printf("Введите минимальное и максимальное значения для массива: \n");
		scanf("%d %d", &a, &b);
		//Перестановка a и b при необходимости для правильной работы программы
		if (a > b)
		{
			int tmp = b;
			b = a;
			a = tmp;
		}
		RandomSolution(arrSize, nums, a, b);
		//Автоматическое заполнение массива случайными числами из переданного диапазона
		break;
	}

	FILE* outfile;
	switch (DoWrite())	//Выбор того, куда будет осуществлена запись
	{
	case 0:	//Только на экран
		PrintArr(stdout, arrSize, nums, "\nТаблица до изменений\n");		//Вывод исходного массива на экран
		ChangeArr(arrSize, nums);											//Модификация массива
		PrintIndexes(stdout, arrSize, nums);								//Вывод индексов отрицательных чисел на экран
		PrintArr(stdout, arrSize, nums, "\nТаблица после изменений\n");	//Вывод изменённого массива на экран
		break;
	case 1:	//Только в файл
		outfile = OpenUsersFile("w");										//Открытие файла на запись
		PrintArr(outfile, arrSize, nums, "\nТаблица до изменений\n");		//Вывод исходного массива в файл
		ChangeArr(arrSize, nums);											//Модификация массива
		PrintIndexes(outfile, arrSize, nums);								//Вывод индексов отрицательных чисел в файл
		PrintArr(outfile, arrSize, nums, "\nТаблица после изменений\n");	//Вывод изменённого массива в файл
		fclose(outfile);													//Закрытие файл
		break;
	case 2:	//И в файл, и на экран
		outfile = OpenUsersFile("w");										//Открытие файла на запись
		PrintArr(outfile, arrSize, nums, "\nТаблица до изменений\n");		//Вывод исходного массива в файл
		PrintArr(stdout, arrSize, nums, "\nТаблица до изменений\n");		//Вывод исходного массива на экран
		ChangeArr(arrSize, nums);											//Модификация массива
		PrintIndexes(stdout, arrSize, nums);								//Вывод индексов отрицательных чисел на экран
		PrintIndexes(outfile, arrSize, nums);								//Вывод индексов отрицательных чисел в файл
		PrintArr(outfile, arrSize, nums, "\nТаблица после изменений\n");	//Вывод модифицированного массива в файл
		PrintArr(stdout, arrSize, nums, "\nТаблица после изменений\n");	//Вывод модифицированного массива на экран
		fclose(outfile);													//Закрытие файла
		break;
	}
	//Завершение работы
	return 0;
}

//Открыть файл по названию от пользователя
FILE* OpenUsersFile(const char mode[])
{
	char filename[256];
	FILE* out;
	do	//Цикл будет выполнятся, пока не будет введёно корректное...
	{	//название файла
		printf("Введите название файла для %s: ",
			(mode[0] == 'r') ? "чтения" : "записи");
		scanf("%s", filename);
		out = fopen(filename, mode);
		if (out)	//Если файл был открыт, то он возвращается
			return out;
		printf("Такого файла нет!\n");
		//В противном случае выводится сообщение об ошибке
	} while (1);
}

/*Функция заполнения массива из файла.*/
int FileSolution(int* arr, int maxArraySize, FILE* text)
{
	int arrSize;
	fscanf(text, "%d", &arrSize);
	//Если полученный из файла размер массива больше максимально возможного размера,...
	//то осуществляется работа с максимальным размером
	if (arrSize > maxArraySize)
		arrSize = maxArraySize;
	for (int i = 0; i < arrSize; ++i)	//Перебор массива и запись в каждую ячейку...
		fscanf(text, "%d", &arr[i]);	//новых значений из файла
	fclose(text);						//Закрытие файла
	return arrSize;						//Возврат размера файла
}

/*Ввод размера массива пользователем*/
int
GetArrSize(int maxArraySize)
{
	int arrSize;
	do	//Цикл ввода корректного значения
	{
		printf("Введите размер массива чисел (не более %d): ", maxArraySize);
		scanf("%d", &arrSize);
		if (arrSize > 0 && arrSize <= maxArraySize)
			return arrSize;
		printf("Размер массива должен быть хотя бы 1, но не более %d\n", maxArraySize);
	} while (1);
}

//Функция выбора способа заполнения массива
int	WayToWork(void)
{
	int way;
	do	//Цикл будет продолжаться, пока не будет введено правильное значение
	{
		printf("Выберите способ заполнения массива:\n%s%s",
			"0 - для заполнения из файла, 1 для ручного,\n",
			"2 - для автозаполнения из диапазона: ");
		scanf("%d", &way);
		if (way >= 0 && way <= 2)
			return way;
		printf("Введите правильное значение\n");
	} while (1);	//Вывод сообщения об ошибке и повторный ввод
}

/*Функция поячеечного ввода значений в массив*/
void ManualSolution(int arrSize, int* nums)
{
	//Ввод значения в каждую ячейку массива
	for (int i = 0; i < arrSize; ++i)
	{
		printf("%d-я ячейка: ", i + 1);
		scanf("%d", &nums[i]);
	}
}

/*Функция заполнения массива случайными числами из переданного диапазона*/
void RandomSolution(int arrSize, int* nums, int a, int b)
{
	srand(time(NULL));	//Создаёт псевдослучайный сид на основе времени
	//Заполнение каждой ячейки случайным значением
	for (int i = 0; i < arrSize; i++)
		nums[i] = rand() % (b - a + 1) + a;
}

//Функция спрашивает у пользователя, будет ли он выводить массив в файл
int DoWrite(void)
{
	int out;
	do
	{
		printf("Введите 0 чтобы выводить массив только на экран,\n%s%s",
			"1 - чтобы записать в файл, не выводя на экран\n",
			"2 - чтобы вывести массив на экран и в файл: ");
		scanf("%d", &out);
		if (out >= 0 && out <= 2)
			return out;
		printf("Введите корректное значение\n");
	} while (1);
};

/*Функция умножения положительных чисел массива на 3*/
void ChangeArr(int arrSize, int* nums)
{
	for (int i = 0; i < arrSize; ++i)
		if (nums[i] > 0)
			nums[i] *= MULTIPLIER;
}

/*Вывод массива в виде таблицы */
void PrintArr(FILE* outstream, int arrSize, int* nums, const char* header)
{
	fprintf(outstream, header);	//Печать заголовка

	fprintf(outstream, "%s%s%s%s",	//Шапка таблицы
		"╔════════════════════════════╗\n",
		"║           Вывод            ║\n",
		"╟──────────────╥─────────────╢\n",
		"║ Номер ячейки ║ Её значение ║\n");
	for (int i = 0; i < arrSize; ++i)	//Вывод заполненного массива
	{
		fprintf(outstream, "╟──────────────╫─────────────╢\n");
		fprintf(outstream, "║      %02d      ║ %-11d ║\n", i + 1, nums[i]);
	}
	fprintf(outstream, "╚══════════════╩═════════════╝\n");
}

/*Вывод индексов отрицательных чисел*/
void PrintIndexes(FILE* outstream, int arrSize, int* nums)
{	//Цикл перебирает элементы массива и, если значение в них отрицательное...
	//в переданный поток выводится индекс этого элемента 
	fprintf(outstream, "\nИндексы отрицательных чисел:\n");
	for (int i = 0; i < arrSize; ++i)
		if (nums[i] < 0)
			fprintf(outstream, "%d ", i + 1);
}
