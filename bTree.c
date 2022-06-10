#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "bTree.h"

// node 생성, 초기화
btNode* create_node(bool isLeaf_) {
	btNode* node;
	node = (struct btNode*)malloc(sizeof(struct btNode));
	node->child_ptr = (struct btNode**)malloc((max_degree) * sizeof(struct btNode*));
	node->isLeaf = isLeaf_;
	node->key_num = 0;
	node->keys = (int*)malloc((max_degree) * sizeof(int));

	for (int i = 0;i < max_degree;i++)
		(node->keys)[i] = -1;

	return node;
}

// tree 생성 및 초기화
bTree* create_btree() {
	bTree* tree;
	tree = (struct bTree*)malloc(sizeof(struct bTree));
	tree->root = create_node(true);
	tree->node_count = 1;

	return tree;
}

// 삽입 연산 각 Case에 따른 처리 함수

// 리프노드에 key노드가 가득 찬 경우에 노드 분할 수행
// 최대 key 개수 초과, 중앙값에서 분할 수행 -> 부모노드로 병합 or 새로 생성
void split(btNode* x, int i) {
	btNode* y;
	btNode* z;
	y = x->child_ptr[i];
	z = create_node(y->isLeaf);
	z->key_num = min_degree - 1;

	for (int j = 0; j < min_degree - 1; j++) {
		z->keys[j] = y->keys[j + min_degree];
	}

	if (!y->isLeaf) {
		for (int j = 0; j < min_degree; j++) {
			z->child_ptr[j] = y->child_ptr[j + min_degree];
		}
	}
	y->key_num = min_degree - 1;

	for (int j = x->key_num; j >= i; j--) {
		x->child_ptr[j + 1] = x->child_ptr[j];
	}
	x->child_ptr[i + 1] = z;

	for (int j = x->key_num - 1; j >= i; j--) {
		x->keys[j + 1] = x->keys[j];
	}
	x->keys[i] = y->keys[min_degree - 1];
	x->key_num = x->key_num + 1;
}

// Max degree 넘지 않는 노드에 삽입되는 경우 
// 삽입하기 적절한 리프 노드 찾아 삽입 
void insertNonFull(btNode* x, int k) {
	int i = x->key_num;
	if (x->isLeaf) {
		while (i >= 0 && k < x->keys[i - 1]) {
			x->keys[i] = x->keys[i - 1];
			i -= 1;
		}
		x->keys[i] = k;
		x->key_num += 1;
	}
	else {
		while (i >= 0 && k < x->keys[i - 1]) {
			i = i - 1;
		}
		if (x->child_ptr[i]->key_num >= 2 * min_degree - 1) {
			split(x, i);
			if (k > x->keys[i]) {
				i += 1;
			}
		}
		insertNonFull(x->child_ptr[i], k);
	}
}

// 값을 tree에 삽입한다. 
void insert(bTree* tree, int k) {
	btNode* r = tree->root;
	printf("starting insert %d to tree\n", k);
	if (r->key_num == max_degree - 1) {	// root가 가득 찬 경우 
		// 새 노드 생성과 분할 수행
		btNode* s;
		s = create_node(false);
		tree->root = s;
		s->key_num = 0;
		s->isLeaf = false;
		(s->child_ptr)[0] = r;
		split(s, 0);
		insertNonFull(s, k);
	}
	else {
		insertNonFull(r, k);
	}
}

// tree에서 key값 탐색하는 함수 
sR* searchNode(btNode* x, int k) {

	int i = 0;
	while (i<x->key_num && k > x->keys[i]) {
		i += 1;
	}
	if (i < x->key_num && k == x->keys[i]) {
		sR* search_result = (sR*)malloc(sizeof(sR));
		search_result->node = x;
		search_result->index = i;
		search_result->result = true;
		return search_result;
	}
	else if (x->isLeaf == true) {
		sR* search_result = (sR*)malloc(sizeof(sR));
		search_result->result = false;
		return search_result;
	}
	else {
		return searchNode(x->child_ptr[i], k);
	}
}

// inorder successor 찾음
int find_succ(btNode* x) {
	int succ;
	if (x->isLeaf == true) {
		succ = x->keys[0];
		return succ;
	}
	else {
		find_succ(x->child_ptr[0]);
	}
}

// inorder predecessor 찾음
int find_pred(btNode* x) {
	int pred;
	if (x->isLeaf == true) {
		pred = x->keys[x->key_num - 1];
		return pred;
	}
	else {
		find_pred(x->child_ptr[x->key_num]);
	}
}

// 병합, 대체 될 수 있는 형제 노드 찾는 함수
fs* find_siblings(btNode* x, int i) {
	fs* find_result = (fs*)malloc(sizeof(fs));
	if (i == 0) { // 왼쪽 끝에 위치
		find_result->direction = 1;
		if (x->child_ptr[i + 1]->key_num > min_degree - 1) {
			find_result->result = true;
			return find_result;
		}
		else {
			find_result->result = false;
			return find_result;
		}
	}
	else if (i < x->key_num) { // 중간에 위치
		if (x->child_ptr[i + 1]->key_num > min_degree - 1) {
			find_result->direction = 1;
			find_result->result = true;
			return find_result;
		}
		else if (x->child_ptr[i - 1]->key_num > min_degree - 1) {
			find_result->direction = -1;
			find_result->result = true;
			return find_result;
		}
		else {
			find_result->direction = 1;
			find_result->result = false;
			return find_result;
		}
	}
	else if (i == x->key_num) { // 오른쪽 끝에 위치
		find_result->direction = -1;
		if (x->child_ptr[i - 1]->key_num > min_degree - 1) {
			find_result->result = true;
			return find_result;
		}
		else {
			find_result->result = false;
			return find_result;
		}
	}
	else {
		printf("finding siblings failed");
		find_result->direction = 0;
		find_result->result = false;
		return find_result;
	}
}

// 삭제 연산을 위한 함수들 
// 노드 삭제시, 수행되는 병합 
void mergeRightNode(btNode* x, int i) {
	btNode* left_node = x->child_ptr[i];
	btNode* right_node = x->child_ptr[i + 1];
	int left_keynum = left_node->key_num;
	int right_keynum = right_node->key_num;

	left_node->keys[left_keynum] = x->keys[i];

	for (int j = 0; j < right_keynum; j++) {
		left_node->keys[left_keynum + j + 1] = right_node->keys[j];
	}

	if (x->child_ptr[i]->isLeaf == false) {
		for (int j = 0; j <= right_keynum; j++) {
			left_node->child_ptr[left_keynum + j + 1] = right_node->child_ptr[j];
		}
	}
	left_node->key_num = left_keynum + right_keynum + 1;


	for (int j = i + 1; j < x->key_num; j++) {
		x->child_ptr[j] = x->child_ptr[j + 1];
		x->keys[j - 1] = x->keys[j];
	}
	x->key_num -= 1;
	free(right_node->child_ptr);
	free(right_node->keys);
	free(right_node);
}


// 루트 노드가 x인 subtree에서 k value 삭제하는 함수
int deleteNode(btNode* x, int k, bTree* tree) {
	int i;

	if (x->isLeaf == true) {	// x가 리프 노드인 경우
		for (i = 0; i < x->key_num; i++) {
			if (x->keys[i] == k) {
				break;
			}
		}
		printf("%dth key was %d. Delete Success\n", i, k);
		for (; i < x->key_num - 1; i++) {
			x->keys[i] = x->keys[i + 1];
		}
		x->key_num -= 1;
		return 1;
	}
	i = 0;

	while (x->keys[i] < k && i < x->key_num - 1) {
		i += 1;
	}

	// k값이 internal node x에 위치
	if (x->keys[i] == k) {

		if (x->child_ptr[i]->key_num > min_degree - 1) {
			int replace_num = find_pred(x->child_ptr[i]);
			x->keys[i] = replace_num;
			deleteNode(x->child_ptr[i], replace_num, tree);
		}

		else if (x->child_ptr[i + 1]->key_num > min_degree - 1) {
			int replace_num = find_succ(x->child_ptr[i + 1]);
			x->keys[i] = replace_num;
			deleteNode(x->child_ptr[i + 1], replace_num, tree);
		}
 
		else {
			mergeRightNode(x, i);
			if (tree->root->key_num == 0) {
				tree->root = x->child_ptr[i];
				deleteNode(x->child_ptr[i], k, tree);
				free(x->child_ptr);
				free(x->keys);
				free(x);
			}
			else {
				deleteNode(x->child_ptr[i], k, tree);
			}
		}
	}

	// k값이 노드 x에 존재하지 않는다.
	else {

		if (k > x->keys[i]) {
			i += 1;
		}

		if (x->child_ptr[i]->key_num > min_degree - 1) {
			deleteNode(x->child_ptr[i], k, tree);
		}
		else {

			fs* find_result = find_siblings(x, i);
			if (find_result->direction == 1 && find_result->result == true) {

				x->child_ptr[i]->keys[min_degree - 1] = x->keys[i];
				x->child_ptr[i]->key_num += 1;
				x->keys[i] = x->child_ptr[i + 1]->keys[0];
				x->child_ptr[i]->child_ptr[min_degree] = x->child_ptr[i + 1]->child_ptr[0];


				for (int j = 0; j < x->child_ptr[i + 1]->key_num - 1; j++) {
					x->child_ptr[i + 1]->keys[j] = x->child_ptr[i + 1]->keys[j + 1];
				}

				if (x->child_ptr[i + 1]->isLeaf == false) {
					for (int j = 0; j < x->child_ptr[i + 1]->key_num; j++) {
						x->child_ptr[i + 1]->child_ptr[j] = x->child_ptr[i + 1]->child_ptr[j + 1];
					}
				}
				x->child_ptr[i + 1]->key_num -= 1;
				deleteNode(x->child_ptr[i], k, tree);
			}
			else if (find_result->direction == 1 && find_result->result == false) {

				mergeRightNode(x, i);
				if (tree->root->key_num == 0) {
					tree->root = x->child_ptr[i];
					deleteNode(x->child_ptr[i], k, tree);
					free(x->child_ptr);
					free(x->keys);
					free(x);
				}
				else {
					deleteNode(x->child_ptr[i], k, tree);
				}
			}
			else if (find_result->direction == -1 && find_result->result == false) {

				mergeRightNode(x, i - 1);
				if (tree->root->key_num == 0) {
					tree->root = x->child_ptr[i - 1];
					deleteNode(x->child_ptr[i - 1], k, tree);
					free(x->child_ptr);
					free(x->keys);
					free(x);
				}
				else {
					deleteNode(x->child_ptr[i - 1], k, tree);
				}
			}
			else if (find_result->direction == -1 && find_result->result == true) {

				for (int j = x->child_ptr[i]->key_num - 1; j >= 0; j--) {
					x->child_ptr[i]->keys[j + 1] = x->child_ptr[i]->keys[j];
				}
				x->child_ptr[i]->keys[0] = x->keys[i - 1];
				x->keys[i - 1] = x->child_ptr[i - 1]->keys[x->child_ptr[i - 1]->key_num - 1];

				for (int j = x->child_ptr[i]->key_num; j >= 0;j--) {
					x->child_ptr[i]->child_ptr[j + 1] = x->child_ptr[i]->child_ptr[j];
				}
				x->child_ptr[i]->child_ptr[0] = x->child_ptr[i - 1]->child_ptr[x->child_ptr[i - 1]->key_num];
				x->child_ptr[i]->key_num += 1;
				x->child_ptr[i - 1]->key_num -= 1;
				deleteNode(x->child_ptr[i], k, tree);
			}
			else {
				printf("Error\n");
			}
		}
	}
}

// tree내의 값이 있는지 check, 없으면 0 return, 있으면 deleteNode 함수 수행 
int deleteCheck(bTree* x, int k) {
	sR* search_result;
	btNode* r = x->root;
	search_result = searchNode(r, k);
	if (search_result->result == false) {
		return 0;
	}
	else {
		deleteNode(r, k, x);
	}
}

// 프로세스, CPU 실행시간 측정 코드
clock_t checkTime() {
	clock_t time = clock();
	return time;
}
