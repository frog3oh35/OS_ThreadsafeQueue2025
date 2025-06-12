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
	// 기본 실패 상태로 초기화, 내부 필드 전부 0으로 초기화
	Reply reply = { false, {0} };

	Node* current = queue->head;
	while (current != NULL) {
		if (current->item.key == item.key) {
			// 같은 key 존재하면 -> value만 깊은 복사로 교체
			if (current->item.value != NULL) {
				// 기존 메모리 해제
				free(current->item.value);
			}

			// 새 메모리 확보
			current->item.value = malloc(item.value_size);
			// malloc 실패 시 그대로 false 리턴
			if (current->item.value == NULL) return reply;

			// 값 복사
			memcpy(current->item.value, item.value, item.value_size);
			// size도 저장
			current->item.value_size = item.value_size;

			reply.success = true;
			// 수정 된 아이템을 리턴용 reply에 담음
			reply.item = current->item;
			return reply;
		}
		current = current->next;
	}

	// 큐 안에 해당 key가 없으면 -> 새 노드 생성
	Node* new_node = (Node*) malloc(sizeof(Node));
	if (new_node == NULL) return reply;

	new_node->item.key = item.key;
	
	// 깊은 복사를 하기 위한 메모리 확보
	new_node->item.value = malloc(item.value_size);
	if (new_node->item.value == NULL) {
		free(new_node); // 실패 시 노드 메모리 해제
		return reply;
	}

	// 실데이터 복사
	memcpy(new_node->item.value, item.value, item.value_size);
	new_node->item.value_size = item.value_size;

	// 연결 리스트용 next 포인터 초기화
	new_node->next = NULL;

	// 빈 큐인 경우 처음 노드 삽입
	if (queue->tail == NULL) {
		queue->head = queue->tail = new_node;
	}
	else {
		// 기존 tail 뒤에 연결
		queue->tail->next = new_node;
		queue->tail = new_node;
	}

	// 리턴값 set
	reply.success = true;
	reply.item = new_node->item;
	return reply;
}

Reply dequeue(Queue* queue) {
	Reply reply = { false, {0} };

	if (queue->head == NULL) {
		// 빈 큐면 그대로 실패 반환
		return reply;
	}

	// current 리스트 순회요 / prev 직전 노드 추적용
	Node* prev = NULL;
	Node* current = queue->head;
	// max_node key가 가장 큰 노드 / max_prev max_node 바로 전 노드
	Node* max_prev = NULL;
	Node* max_node = queue->head;

	//최대 key 찾기, 연결 리스트 전체 순회.
	//가장 큰거 max_node에 저장
	//삭제해야 하니까 max_prev에 직전 노드 저장
	while (current != NULL) {
		if (current->item.key > max_node->item.key) {
			max_node = current;
			max_prev = prev;
		}
		prev = current;
		current = current->next;
	}

	// 연결 리스트에서 max_node 제거
	if (max_prev == NULL) {
		// max_node가 head일 경우
		// head를 다음 노드로 바꾼다. 리스트에 한 개뿐 => tail도 비움
		queue->head = max_node->next;
		if (queue->tail == max_node) {
			queue->tail == NULL;
		}
	}
	else { //삭제 대상이 중간 or 끝이면 -> 연결 끊고 뒷 노드로 잇기
		//tai도 갱신
		max_prev->next = max_node->next;
		if (queue->tail == max_node) {
			queue->tail = max_prev;
		}
	}

	// 리턴값 (deep copy 유지)
	// reply에 success 표시, key랑 size 복사
	reply.success = true;
	reply.item.key = max_node->item.key;
	reply.item.value_size = max_node->item.value_size;
	
	// 깊은 복사로 value 복사
	// reply.item.value가 외부에서 안전하게 사용 가능하도록 복사
	reply.item.value = malloc(max_node->item.value_size);
	if (reply.item.value != NULL) {
		memcpy(reply.item.value, max_node->item.value, max_node->item.value_size);
	}

	// 제거된 노드 메모리 해제
	free(max_node->item.value);
	free(max_node);

	return reply;
}

// 특정 범위의 key만 복사해서 새로운 큐를 리턴하는 함수
Queue* range(Queue* queue, Key start, Key end) {
	// 매개변수로 받은 큐가 아예 NULL이면 진행x
	if (queue == NULL) return NULL;

	// 복사한 결과를 담을 새 큐 생성
	// 메모리 부족 등으로 실패할 수 있으니 check
	Queue* new_queue = init();
	if (new_queue == NULL) return NULL;

	// 기존 큐를 처음부터 끝까지 순회하기 위한 포인터
	Node* current = queue->head;

	// 연결 리스트 순회 루프
	while (current != NULL) {
		// 현재 노드의 key가 원하는 범위에 있는지 체크
		Key k = current->item.key;
		if (k >= start && k <= end) {
			// 복사할 새 아이템 구조체 생성
			// 범위 안에 있는 item -> deep copy
			Item new_item;
			new_item.key = current->item.key;
			new_item.value_size = current->item.value_size;
			
			// 깊은 복사를 위해 value용 메모리 새로 할당
			new_item.value = malloc(new_item.value_size);

			if (new_item.value == NULL) {
				// 실패 시 -> 만든 새 큐 메모리 해제 후 NULL 리턴
				release(new_queue);
				return NULL;
			}

			// value도 실제 값 복사 (deep copy)
			memcpy(new_item.value, current->item.value, new_item.value_size);

			// 새 노드 생성
			Node* new_node = (Node*) malloc(sizeof(Node));
			// 노드 생성 실패 시 -> item의 value 해제, 큐 정리, NULL 리턴
			if (new_node == NULL) {
				free(new_item.value);
				release(new_queue);
				return NULL;
			}

			// 노드에 item 연결, next는 새 큐의 끝이라 NULL 초기화
			new_node->item = new_item;
			new_node->next = NULL;

			// 새 큐에 첫 번째 노드 넣는 거면 head/tail set
			if (new_queue->tail == NULL) {
				new_queue->head = new_queue->tail = new_node;
			} // 이후 노드는 tail 뒤에 붙이고 tail 갱신
			else {
				new_queue->tail->next = new_node;
				new_queue->tail = new_node;
			}
		}
		// 기존 큐의 다음 노드로 이동
		current = current->next;
	}
	return new_queue;
}