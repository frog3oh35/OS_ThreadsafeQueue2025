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
	// 새 Node를 위한 메모리 공간 할당
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) return NULL;

	// Item 안의 value(데이터 본체)를 복사할 메모리 공간 할당
	void* copied_value = malloc(item.value_size);
	if (copied_value == NULL) {
		free(node);
		return NULL;
	}
	// 원래 데이터 item.value 내용을 복사해서 넣음 (깊은 복사)
	memcpy(copied_value, item.value, item.value_size);

	// 노드 안의 item 채우기
	node->item.key = item.key;		// key 복사
	node->item.value = copied_value;	// 깊은 복사된 value 저장
	node->item.value_size = item.value_size;	 // value의 크기도 같이 저장
	node->next = NULL;	// 초기 상태에서는 다음 노드 없음

	return node;	// 만들어진 새 노드 반환
}

void nfree(Node* node) {
	// NULL이면 아무것도 안 함
	if (node == NULL) return;
	
	// value(깊은 복사한 데이터) 먼저 해제
	free(node->item.value);
	// 마지막으로 Node 자체 메모리 해제
	free(node);
}

Node* nclone(Node* node) {
	if (node == NULL) return NULL;

	// 새 노드 메모리 할당
	Node* new_node = (Node*)malloc(sizeof(Node));
	// 메모리 할당 실패 시 리턴
	if (new_node == NULL) return NULL;

	// item.value 깊은 복사를 위한 메모리 할당
	void* copied_value = malloc(node->item.value_size);
	// value 복사 실패 시 정리하고 return
	if (copied_value == NULL) {
		free(new_node);
		return NULL;
	}

	// 메모리 복사
	memcpy(copied_value, node->item.value, node->item.value_size);
	
	// 새 노드의 item 구성
	new_node->item.key = node->item.key;
	new_node->item.value = copied_value;
	new_node->item.value_size = node->item.value_size;

	new_node->next = NULL;
	//클론 노드는 연결 X

	return new_node;
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