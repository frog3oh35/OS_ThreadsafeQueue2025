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
	
	//인자로 받은 큐가 NULL이면 할 거 없음
	if (queue == NULL) return;
	
	//현재 위치를 head부터 시작해서, 끝날때까지 (NULL 까지) 반복
	Node* cur = queue->head;
	while (cur != NULL) {
		// ↓다음 노드를 미리 저장하기 (현재 노드 날리면 next 못 감)
		Node* next = cur->next;
		//현재 노드 메모리에서 날리기! 이하 반복
		nfree(cur);
		cur = next;
	}
	//큐 자체도 delete로 날리기!
	delete queue;
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