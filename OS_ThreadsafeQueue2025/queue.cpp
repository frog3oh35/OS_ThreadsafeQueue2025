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
	
	//���ڷ� ���� ť�� NULL�̸� �� �� ����
	if (queue == NULL) return;
	
	//���� ��ġ�� head���� �����ؼ�, ���������� (NULL ����) �ݺ�
	Node* cur = queue->head;
	while (cur != NULL) {
		// ����� ��带 �̸� �����ϱ� (���� ��� ������ next �� ��)
		Node* next = cur->next;
		//���� ��� �޸𸮿��� ������! ���� �ݺ�
		nfree(cur);
		cur = next;
	}
	//ť ��ü�� delete�� ������!
	delete queue;
}

Node* nalloc(Item item) {
	// �� Node�� ���� �޸� ���� �Ҵ�
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) return NULL;

	// Item ���� value(������ ��ü)�� ������ �޸� ���� �Ҵ�
	void* copied_value = malloc(item.value_size);
	if (copied_value == NULL) {
		free(node);
		return NULL;
	}
	// ���� ������ item.value ������ �����ؼ� ���� (���� ����)
	memcpy(copied_value, item.value, item.value_size);

	// ��� ���� item ä���
	node->item.key = item.key;		// key ����
	node->item.value = copied_value;	// ���� ����� value ����
	node->item.value_size = item.value_size;	 // value�� ũ�⵵ ���� ����
	node->next = NULL;	// �ʱ� ���¿����� ���� ��� ����

	return node;	// ������� �� ��� ��ȯ
}

void nfree(Node* node) {
	// NULL�̸� �ƹ��͵� �� ��
	if (node == NULL) return;
	
	// value(���� ������ ������) ���� ����
	free(node->item.value);
	// ���������� Node ��ü �޸� ����
	free(node);
}

Node* nclone(Node* node) {
	if (node == NULL) return NULL;

	// �� ��� �޸� �Ҵ�
	Node* new_node = (Node*)malloc(sizeof(Node));
	// �޸� �Ҵ� ���� �� ����
	if (new_node == NULL) return NULL;

	// item.value ���� ���縦 ���� �޸� �Ҵ�
	void* copied_value = malloc(node->item.value_size);
	// value ���� ���� �� �����ϰ� return
	if (copied_value == NULL) {
		free(new_node);
		return NULL;
	}

	// �޸� ����
	memcpy(copied_value, node->item.value, node->item.value_size);
	
	// �� ����� item ����
	new_node->item.key = node->item.key;
	new_node->item.value = copied_value;
	new_node->item.value_size = node->item.value_size;

	new_node->next = NULL;
	//Ŭ�� ���� ���� X

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