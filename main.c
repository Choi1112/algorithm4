#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "bTree.h"

void main() {
	while (1) {
		printf("Input Max Degree : ");	// Maximum degree 값 입력
		scanf("%d", &max_degree);
		if (max_degree >= 4 && max_degree % 2 == 0) {
			break;
		}
		else {
			printf("wrong max_degree");
		}
	}
	int arr[1000];
	clock_t start1 = 0, end1 = 0, start2 = 0, end2 = 0; // 수행 시간 할당받을 변수
	min_degree = max_degree / 2;	// Minimum degree 값 설정
	bTree* tr = create_btree();

	srand((unsigned)time(NULL));

	start1 = checkTime();
	for (int i = 0;i < 1000;i++) {
		int random = rand() % 10000;	//0 ~ 9999 사이 1000개의 랜덤값 삽입
		insert(tr, random);
		arr[i] = random;
	}
	end1 = checkTime();

	start2 = checkTime();
	for (int i = 0; i < 500;i++) {	// 500개의 값 삭제 
		deleteCheck(tr, arr[i]);
	}
	end2 = checkTime();

	printf("b-tree insertion execution time : %.3f\n", (float)(end1 - start1) / CLOCKS_PER_SEC);	// 알고리즘 수행시간 출력
	printf("b-tree deletion execution time : %.3f\n", (float)(end2 - start2) / CLOCKS_PER_SEC);
}
