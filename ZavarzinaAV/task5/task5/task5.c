#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <memory.h>
#include<time.h>
#include<omp.h>
#include<locale.h>

void BubbleSort(struct _finddata_t* a, size_t size) { 
	long i, j;
	struct _finddata_t temp;
	for (i = 0; i < size-1; i++) {
		for (j = 0; j < size - i - 1; j++) {
			if (a[j].size > a[j + 1].size) {
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
	}

}

void SelectSort(struct _finddata_t* a, size_t size) {
	long i, j, idx;
	struct _finddata_t temp;
	for (i = 0; i < size; i++) {
		idx = i;
		temp = a[i];
		for (j = i + 1; j < size; j++) {
			if (a[j].size < temp.size) {
				temp = a[j];
				idx = j;
			}
		}
		a[idx] = a[i];
		a[i] = temp;
	}
 }

void InsertSort(struct _finddata_t* a, size_t size) {
	long i, j;
	struct _finddata_t temp;
	for (i = 1; i < size; i++) {
		temp = a[i];
		j = i - 1;
		while (j >= 0 && a[j].size > temp.size) {
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = temp;
	}
}

void Merge(struct _finddata_t* a, long l, long split, long r) {
	long pos1 = l;
	long pos2 = split + 1;
	long pos3 = 0;
	struct _finddata_t* temp = malloc(sizeof(struct _finddata_t) * (r - l + 1));
	while (pos1 <= split && pos2 <= r) {
		if (a[pos1].size < a[pos2].size)
			temp[pos3++] = a[pos1++];
		else
			temp[pos3++] = a[pos2++];
	}
	while (pos2 <= r)
		temp[pos3++] = a[pos2++];
	while (pos1 <= split)
		temp[pos3++] = a[pos1++];
	for (pos3 = 0; pos3 < r - l + 1; pos3++) 
		a[l + pos3] = temp[pos3];
	free(temp);
}

void MergeSort(struct _finddata_t* a, long l, long r) {
	long split;
	if (l < r) {
		split = (l + r) / 2;

		MergeSort(a, l, split);
		MergeSort(a, split + 1, r);
		Merge(a, l, split, r);
	}
}

void SimpleSort(struct _finddata_t* a, size_t size) {
	int i, j;
	struct _finddata_t temp;
	for (i = 0; i < size; i++) {
		for (j = i + 1; j < size; j++) {
			if (a[i].size > a[j].size) {
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}
	}
}

void print_file(struct _finddata_t c_file) {
	char buffer[32];
	ctime_s(buffer, _countof(buffer), &c_file.time_write);
	printf("%-30.30s %.24s %10lu\n", c_file.name, buffer, c_file.size);
}

void list_dir(int sort_method, int sort_direction, char* path) {
	char search[_MAX_PATH + 4 + 1];
	strcpy_s(search, sizeof(search), path);
	strcat_s(search, sizeof(search), "\\*.*");

	intptr_t hFile;
	struct _finddata_t c_file;
	if ((hFile = _findfirst(search, &c_file)) == -1L) {

		printf("нет файлов в каталоге\n");
		return;
	}

	size_t count = 0;
	do {
		count++;
	} while (_findnext(hFile, &c_file) == 0);
	_findclose(hFile);

	struct _finddata_t* arr = malloc(count * sizeof(struct _finddata_t));

	hFile = _findfirst(search, &c_file);
	arr[0] = c_file;
	for (size_t i = 1; i < count; i++) {
		_findnext(hFile, &c_file);
		arr[i] = c_file;
	}
	_findclose(hFile);

	double start_time = omp_get_wtime();
	
	switch (sort_method) {
	case 1:
		BubbleSort(arr, count);
		break;
	case 2:
		SelectSort(arr, count);
		break;
	case 3:
		InsertSort(arr, count);
		break;
	case 4:
		MergeSort(arr, 0, count-1);
		break;
	case 5:
		SimpleSort(arr, count);
		break;
	default:
		printf("Неизвестная сортировка\n");
		free(arr);
		return;
	}

	double end_time = omp_get_wtime();

	if (sort_direction != 1 && sort_direction != 2) {
		printf("неизвестное направление сортировки\n");
		return;
	}

	printf("Текущая директория: %s\n", path);
	printf("%-30.30s %-25s %10s\n", "file", "data", "size");

	if (sort_direction == 1) {
		for (size_t i = 0; i < count; i++) {
			print_file(arr[i]);
		}
	}
	else {
		for (size_t i = count; i-- > 0; ) {
			print_file(arr[i]);
		}
	}
	printf("Время сортировки: %lf сек\n", end_time - start_time);
	free(arr);
}


int main(){

	setlocale(LC_ALL, "Russian");

	int last_sort_id = -1;
	int last_sort_order = -1;
	char last_path[_MAX_PATH + 1] = "#";

	int sort_id;
	int sort_order;
	char path[_MAX_PATH + 1];


	system("cls");
	while (1)
	{
		printf("Введите путь (если вы уже выбирали путь и желаете остаться в этом каталоге, нажмите Enter): ");
		gets_s(path, sizeof(path));

		printf("Список алгоритмов сортировки:\n");
		if (last_sort_id != -1)
		{
			printf(" 0 - Последний выбранный алгоритм\n");
		}
		printf(" 1 - Пузырьком\n");
		printf(" 2 - Выбором\n");
		printf(" 3 - Вставками\n");
		printf(" 4 - Слиянием\n");
		printf(" 5 - Простой\n");
	
		printf("Выберите алгоритм сортировки: \n");
		scanf_s("%d", &sort_id);

		printf("Список методов сортировки: \n");
		if (last_sort_order != -1)
		{
			printf(" 0 - Последний выбранный метод\n");
		}
		printf(" 1 - По возрастанию\n");
		printf(" 2 - По убыванию\n");
		printf("Выберите метод сортировки: ");
		scanf_s("%d", &sort_order);

		if (sort_id == 0)
		{
			if (last_sort_id == -1)
			{
				printf("Пожалуйста, выберите начальный алгоритм сортировки\n");
				continue;
			}
			sort_id = last_sort_id;
		}
		if (sort_order == 0)
		{
			if (last_sort_order == -1)
			{
				printf("Пожалуйста, выберите начальный метод сортировки\n");
				continue;
			}
			sort_order = last_sort_order;
		}
		if (strlen(path) == 0)
		{
			if (last_path[0] == '#')
			{
				printf("Пожалуйста, выберите начальный каталог для сортировки\n");
				continue;
			}
		}
		else
		{
			strcpy_s(last_path, sizeof(last_path), path);
		}

		last_sort_id = sort_id;
		last_sort_order = sort_order;

		system("cls");
		list_dir(last_sort_id, last_sort_order, last_path);
		getchar();
	}
	return 0;
}
