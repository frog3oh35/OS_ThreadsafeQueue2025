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
	// �⺻ ���� ���·� �ʱ�ȭ, ���� �ʵ� ���� 0���� �ʱ�ȭ
	Reply reply = { false, {0} };

	Node* current = queue->head;
	while (current != NULL) {
		if (current->item.key == item.key) {
			// ���� key �����ϸ� -> value�� ���� ����� ��ü
			if (current->item.value != NULL) {
				// ���� �޸� ����
				free(current->item.value);
			}

			// �� �޸� Ȯ��
			current->item.value = malloc(item.value_size);
			// malloc ���� �� �״�� false ����
			if (current->item.value == NULL) return reply;

			// �� ����
			memcpy(current->item.value, item.value, item.value_size);
			// size�� ����
			current->item.value_size = item.value_size;

			reply.success = true;
			// ���� �� �������� ���Ͽ� reply�� ����
			reply.item = current->item;
			return reply;
		}
		current = current->next;
	}

	// ť �ȿ� �ش� key�� ������ -> �� ��� ����
	Node* new_node = (Node*) malloc(sizeof(Node));
	if (new_node == NULL) return reply;

	new_node->item.key = item.key;
	
	// ���� ���縦 �ϱ� ���� �޸� Ȯ��
	new_node->item.value = malloc(item.value_size);
	if (new_node->item.value == NULL) {
		free(new_node); // ���� �� ��� �޸� ����
		return reply;
	}

	// �ǵ����� ����
	memcpy(new_node->item.value, item.value, item.value_size);
	new_node->item.value_size = item.value_size;

	// ���� ����Ʈ�� next ������ �ʱ�ȭ
	new_node->next = NULL;

	// �� ť�� ��� ó�� ��� ����
	if (queue->tail == NULL) {
		queue->head = queue->tail = new_node;
	}
	else {
		// ���� tail �ڿ� ����
		queue->tail->next = new_node;
		queue->tail = new_node;
	}

	// ���ϰ� set
	reply.success = true;
	reply.item = new_node->item;
	return reply;
}

Reply dequeue(Queue* queue) {
	Reply reply = { false, {0} };

	if (queue->head == NULL) {
		// �� ť�� �״�� ���� ��ȯ
		return reply;
	}

	// current ����Ʈ ��ȸ�� / prev ���� ��� ������
	Node* prev = NULL;
	Node* current = queue->head;
	// max_node key�� ���� ū ��� / max_prev max_node �ٷ� �� ���
	Node* max_prev = NULL;
	Node* max_node = queue->head;

	//�ִ� key ã��, ���� ����Ʈ ��ü ��ȸ.
	//���� ū�� max_node�� ����
	//�����ؾ� �ϴϱ� max_prev�� ���� ��� ����
	while (current != NULL) {
		if (current->item.key > max_node->item.key) {
			max_node = current;
			max_prev = prev;
		}
		prev = current;
		current = current->next;
	}

	// ���� ����Ʈ���� max_node ����
	if (max_prev == NULL) {
		// max_node�� head�� ���
		// head�� ���� ���� �ٲ۴�. ����Ʈ�� �� ���� => tail�� ���
		queue->head = max_node->next;
		if (queue->tail == max_node) {
			queue->tail == NULL;
		}
	}
	else { //���� ����� �߰� or ���̸� -> ���� ���� �� ���� �ձ�
		//tai�� ����
		max_prev->next = max_node->next;
		if (queue->tail == max_node) {
			queue->tail = max_prev;
		}
	}

	// ���ϰ� (deep copy ����)
	// reply�� success ǥ��, key�� size ����
	reply.success = true;
	reply.item.key = max_node->item.key;
	reply.item.value_size = max_node->item.value_size;
	
	// ���� ����� value ����
	// reply.item.value�� �ܺο��� �����ϰ� ��� �����ϵ��� ����
	reply.item.value = malloc(max_node->item.value_size);
	if (reply.item.value != NULL) {
		memcpy(reply.item.value, max_node->item.value, max_node->item.value_size);
	}

	// ���ŵ� ��� �޸� ����
	free(max_node->item.value);
	free(max_node);

	return reply;
}

// Ư�� ������ key�� �����ؼ� ���ο� ť�� �����ϴ� �Լ�
Queue* range(Queue* queue, Key start, Key end) {
	// �Ű������� ���� ť�� �ƿ� NULL�̸� ����x
	if (queue == NULL) return NULL;

	// ������ ����� ���� �� ť ����
	// �޸� ���� ������ ������ �� ������ check
	Queue* new_queue = init();
	if (new_queue == NULL) return NULL;

	// ���� ť�� ó������ ������ ��ȸ�ϱ� ���� ������
	Node* current = queue->head;

	// ���� ����Ʈ ��ȸ ����
	while (current != NULL) {
		// ���� ����� key�� ���ϴ� ������ �ִ��� üũ
		Key k = current->item.key;
		if (k >= start && k <= end) {
			// ������ �� ������ ����ü ����
			// ���� �ȿ� �ִ� item -> deep copy
			Item new_item;
			new_item.key = current->item.key;
			new_item.value_size = current->item.value_size;
			
			// ���� ���縦 ���� value�� �޸� ���� �Ҵ�
			new_item.value = malloc(new_item.value_size);

			if (new_item.value == NULL) {
				// ���� �� -> ���� �� ť �޸� ���� �� NULL ����
				release(new_queue);
				return NULL;
			}

			// value�� ���� �� ���� (deep copy)
			memcpy(new_item.value, current->item.value, new_item.value_size);

			// �� ��� ����
			Node* new_node = (Node*) malloc(sizeof(Node));
			// ��� ���� ���� �� -> item�� value ����, ť ����, NULL ����
			if (new_node == NULL) {
				free(new_item.value);
				release(new_queue);
				return NULL;
			}

			// ��忡 item ����, next�� �� ť�� ���̶� NULL �ʱ�ȭ
			new_node->item = new_item;
			new_node->next = NULL;

			// �� ť�� ù ��° ��� �ִ� �Ÿ� head/tail set
			if (new_queue->tail == NULL) {
				new_queue->head = new_queue->tail = new_node;
			} // ���� ���� tail �ڿ� ���̰� tail ����
			else {
				new_queue->tail->next = new_node;
				new_queue->tail = new_node;
			}
		}
		// ���� ť�� ���� ���� �̵�
		current = current->next;
	}
	return new_queue;
}