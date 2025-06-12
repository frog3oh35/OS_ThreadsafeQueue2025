#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "queue.h"
#include "qtype.h"

using namespace std;

int main() {
    Queue* queue = init();

    // ====== enqueue 3개 ======
    const char* msg1 = "apple";
    const char* msg2 = "banana";
    const char* msg3 = "carrot";

    Item item1;
    item1.key = 3;
    item1.value_size = strlen(msg1) + 1;
    item1.value = malloc(item1.value_size);
    if (item1.value == NULL) {
        cout << "malloc 실패 (item1)" << endl;
        return 1;
    }
    strcpy((char*)item1.value, msg1);
    enqueue(queue, item1);

    Item item2;
    item2.key = 10;
    item2.value_size = strlen(msg2) + 1;
    item2.value = malloc(item2.value_size);
    if (item2.value == NULL) {
        cout << "malloc 실패 (item2)" << endl;
        return 1;
    }
    strcpy((char*)item2.value, msg2);
    enqueue(queue, item2);

    Item item3;
    item3.key = 7;
    item3.value_size = strlen(msg3) + 1;
    item3.value = malloc(item3.value_size);
    if (item3.value == NULL) {
        cout << "malloc 실패 (item3)" << endl;
        return 1;
    }
    strcpy((char*)item3.value, msg3);
    enqueue(queue, item3);

    // ====== dequeue 1개 ======
    Reply reply = dequeue(queue);

    if (reply.success) {
        cout << "Dequeued key: " << reply.item.key << endl;
        cout << "Dequeued value: " << (char*)reply.item.value << endl;

        free(reply.item.value);  // 깊은 복사된 값 해제
    }
    else {
        cout << "Dequeue failed. Queue was empty." << endl;
    }

    // ====== range() 테스트 추가 ======
    cout << "\n--- Range Test : key 5 ~ 10 ---" << endl;

    Queue* sub = range(queue, 5, 10);
    if (sub == NULL) {
        cout << "range() 실패" << endl;
        release(queue); //기존 큐 해제
        return 1;
    }

    while (true) {
        Reply r = dequeue(sub);
        if (!r.success) break;

        cout << "Range result key: " << r.item.key << ", value: " << (char*)r.item.value << endl;
        free(r.item.value); //복사된 값 해제
    }

    release(sub); //range로 맏느 새 큐 해제

    // ====== 마무리 ======
    release(queue);
    return 0;
}