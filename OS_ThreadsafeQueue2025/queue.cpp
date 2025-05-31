#include <iostream>
#include "qtype.h"
#include "queue.h"

Queue* init(void) {
	//1. 새 Queue 구조체 동적 할당
	Queue* q = new Queue;

	//2. 헤드랑 테일 null 초기화
	q->head = nullptr;
	q->tail = nullptr;

	//3. 만들어진 큐 포인터 리턴
	return q; 
}

void release(Queue* queue) {
	return;
}

Node* nalloc(Item item) {
	// Node 생성, item으로 초기화
	return NULL;
}

void nfree(Node* node) {
	return;
}

Node* nclone(Node* node) {
	return NULL;
}

Reply enqueue(Queue* queue, Item item) {
	Reply reply = { false, NULL };
	return reply;
}

Reply dequeue(Queue* queue) {
	Reply reply = { false, NULL };
	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	return NULL;
}