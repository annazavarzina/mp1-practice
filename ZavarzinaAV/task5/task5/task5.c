#include <stdio.h>
#include <stdlib.h>
//#include"Console.h"
#include <io.h>
#include <memory.h>
#include<time.h>
#include<omp.h>
#include<locale.h>

void BubbleSort1(struct _finddata_t* a, size_t size) { //макс элемент погружается
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

//void BubbleSort2(struct _finddata_t* a, size_t size) {  
//	long i, j;
//	struct _finddata_t temp;
//	for (i = 0; i < size; i++) {
//		for (j = size-1; j < size; j--) {
//			if (a[j-1].size > a[j].size) {
//				temp = a[j-1];
//				a[j-1] = a[j];
//				a[j] = temp;
//			}
//		}
//	}
//
//}

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

void print_file(struct _finddata_t c_file) {
	char buffer[32];
	ctime_s(buffer, _countof(buffer), &c_file.time_write);
	printf("%-30.30s %.24s %10lu\n", c_file.name, buffer, c_file.size);
}

void list_dir(int sort_method, int sort_direction, char* path) {
	char search[_MAX_PATH + 4 + 1];
	strcpy_s(search, sizeof(search), path);
	strcat_s(search, sizeof(search), "\\*.*");
	search[_MAX_PATH + 4] = '\0' ;

}