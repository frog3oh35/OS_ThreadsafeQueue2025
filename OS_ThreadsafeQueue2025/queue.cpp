#include <iostream>
#include "qtype.h"
#include "queue.h"

Queue* init(void) {
	//1. �� Queue ����ü ���� �Ҵ�
	Queue* q = new Queue;

	//2. ���� ���� null �ʱ�ȭ
	q->head = nullptr;
	q->tail = nullptr;

	//3. ������� ť ������ ����
	return q; 
}

void release(Queue* queue) {
	return;
}

Node* nalloc(Item item) {
	// Node ����, item���� �ʱ�ȭ
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