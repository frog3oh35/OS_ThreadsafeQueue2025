#ifndef _QTYPE_H // header guard
#define _QTYPE_H


// ==========이 파일은 수정 가능==========

typedef unsigned int Key; //값이 클수록 높은 우선순위
typedef void* Value;

typedef struct {
	Key key;
	Value value;
	int value_size; //byte, 0607 공지확인 후 수정사항..
} Item;

typedef struct {
	bool success; // true : 성공, false : 실패
	Item item;
	//필드 추가 가능
} Reply;

typedef struct node_t {
	Item item;
	struct node_t* next;
	//필드 추가 가능
} Node;

typedef struct {
	Node* head, * tail;
	//필드 추가 가능
} Queue;

//자료형 정의, 추가/수정 가능


#endif