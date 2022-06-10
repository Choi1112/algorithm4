#ifndef _BTREE_

#define _BTREE_
int max_degree, min_degree;

// b-tree node 구조체
typedef struct btNode {
	int key_num;
	int* keys;
	struct btNode** child_ptr;
	bool isLeaf;
}btNode;

// b-tree 구조체
typedef struct bTree {
	struct btNode* root;
	int node_count;
}bTree;

// find_sibilings() 리턴값 구조체
typedef struct find_sib {
	int direction;
	bool result;
}fs;

// searchNode() 함수 리턴값 구조체
typedef struct searchResult {
	btNode* node;
	int index;
	bool result;

}sR;

btNode* create_node(bool isLeaf_);
bTree* create_btree();
void split(btNode* x, int i);
void insertNonFull(btNode* x, int k);
void insert(bTree* tree, int k);
sR* searchNode(btNode* x, int k);
int find_succ(btNode* x);
int find_pred(btNode* x);
fs* find_siblings(btNode* x, int i);
void mergeRightNode(btNode* x, int i);
int deleteNode(btNode* x, int k, bTree* tree);
int deleteCheck(bTree* x, int k);
clock_t checkTime();

#endif
