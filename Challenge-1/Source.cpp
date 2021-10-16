﻿#include "Header.h"

SNode* createNode(int value, int currentLevel) {
	SNode* node = new SNode;

	node->value = value;
	node->currentLevel = currentLevel;
	node->next = new SNode * [currentLevel];
	node->width = new int[currentLevel];

	for (int i = 0; i < currentLevel; i++) {
		node->next[i] = NULL;
		node->width[i] = 1;
	}

	return node;
}

void initList(SkipList& list) {
	//Khởi tạo Skiplist với node head mang giá trị -1
	list.head = createNode(-1, list.maxLevel);
}

int randomLevel(SkipList list) {
	int currentLevel = 1;

	//Tăng level dựa trên xác suất 50%
	while (currentLevel < list.maxLevel && (rand() % 2) == 0) currentLevel++;

	return currentLevel;
}
int levelBasedOnVal(int val) {
	int k = 1;

	while (val % 2 == 0) {
		val /= 2;
		k++;
	}

	return k;
}

void increaseMaxLevel(SkipList& list, int val, bool isRandomLevel) {
	int prevMaxLevel = list.maxLevel;
	SNode* prevHead = list.head;

	//Nếu phát sinh level ngẫu nhiên thì tăng max level dựa trên kích thước của Skiplist
	if (isRandomLevel) list.maxLevel = ceil(log2(list.size + 2));
	//Ngược lại tăng max level dựa trên value của node mới thêm
	else list.maxLevel = max(int(ceil(log2(val + 1))), list.maxLevel);

	SNode* newHead = createNode(-1, list.maxLevel);

	for (int i = 0; i < prevMaxLevel; i++) {
		newHead->next[i] = prevHead->next[i];
		newHead->width[i] = prevHead->width[i];
	}
	for (int i = prevMaxLevel; i < list.maxLevel; i++) {
		newHead->width[i] = list.size + 1;
	}

	list.head = newHead;
	delete prevHead;
}

int countDigits(int val) {
	int count = 0;
	while (val != 0) {
		val /= 10;
		count++;
	}
	return count;
}
SNode* getByIndex(int index, SkipList list) {
	SNode* node = list.head;

	index++;
	for (int j = list.currentLevel - 1; j >= 0; j--) {
		while (node->width[j] <= index) {
			index -= node->width[j];
			node = node->next[j];
		}
	}

	return node;
}

void addNode(int value, SkipList& list, bool isRandomLevel) {
	//Tăng max level trước khi thêm node mới
	increaseMaxLevel(list, value, isRandomLevel);

	SNode* current = list.head;
	//Danh sách các node đã đi qua, cần phải thay đổi liến kết để thêm node mới ở các tầng
	SNode** update = new SNode * [list.maxLevel];
	//danh sách khoảng cách các node đã đi qua ở các tầng
	int* stepsAtLevel = new int[list.maxLevel]{ 0 };

	for (int i = list.maxLevel - 1; i >= 0; i--) {
		//Tìm dãy các node lớn nhất nhỏ hơn value ở các tầng
		while (current->next[i] != NULL && current->next[i]->value < value) {
			stepsAtLevel[i] += current->width[i];
			current = current->next[i];
		}
		update[i] = current;
	}

	//Nếu giá trị đã tồn tại kết thúc thuật toán
	if (current->next[0] != NULL && current->next[0]->value == value) {
		cout << value << " already inserted" << endl;
		return;
	}

	//Xác định level của node mới thêm
	int nodeLevel;
	if (isRandomLevel) nodeLevel = randomLevel(list);
	else nodeLevel = levelBasedOnVal(value);

	SNode* node = createNode(value, nodeLevel);

	//Thay đổi level hiện tại của Skiplist nếu level node mới thêm vào lớn hơn
	if (nodeLevel > list.currentLevel) {
		for (int i = list.currentLevel; i < nodeLevel; i++)
			update[i] = list.head;
		list.currentLevel = nodeLevel;
	}

	int steps = 0;
	for (int i = 0; i < nodeLevel; i++) {
		//Nối các liên kết với node mới thêm vào
		node->next[i] = update[i]->next[i];
		update[i]->next[i] = node;
		//Tính độ rộng của liên kết node mới thêm ở các tầng
		//Cập nhật liên kết node trước node mới thêm ở các tầng
		node->width[i] = update[i]->width[i] - steps;
		update[i]->width[i] = steps + 1;
		steps += stepsAtLevel[i];
	}
	
	//Tăng độ rộng liên kết của head cuối Skiplist từ tầng hiện tại đến tầng cao nhất
	for (int i = nodeLevel; i < list.maxLevel; i++) {
		update[i]->width[i] += 1;
	}

	cout << value << " inserted with level " << nodeLevel << endl;

	list.size++;
}

void buildList(int* arr, int n, SkipList& list, bool isRandomLevel) {
	for (int i = 0; i < n; i++) {
		addNode(arr[i], list, isRandomLevel);
	}
	cout << "List built\n";
}

void removeNode(int value, SkipList& list) {
	SNode* current = list.head;
	//Danh sách các node đã đi qua ở các tầng để thay đổi liên kết
	SNode** update = new SNode * [list.maxLevel];

	for (int i = list.maxLevel - 1; i >= 0; i--) {
		while (current->next[i] != NULL && current->next[i]->value < value) {
			current = current->next[i];
		}
		update[i] = current;
	}

	if (current->next[0] == NULL || current->next[0]->value != value) {
		cout << value << " not found" << endl;
		return;
	}

	int removedNodeLevel = current->next[0]->currentLevel;
	for (int i = removedNodeLevel - 1; i >= 0; i--) {
		update[i]->width[i] += update[i]->next[i]->width[i] - 1;
		update[i]->next[i] = update[i]->next[i]->next[i];
	}

	//Giảm độ rộng liên kết của head cuối Skiplist từ tầng hiện tại đến tầng cao nhất
	for (int i = removedNodeLevel; i < list.maxLevel; i++) {
		update[i]->width[i]--;
	}

	list.size--;

	cout << value << " removed" << endl;
}

void printSizeOfList(SkipList list) {
	if (list.head->next[0] == NULL) {
		cout << "List is empty" << endl;
	}
	else {
		cout << "Max Level: " << list.maxLevel << endl;
		for (int i = 0; i < list.currentLevel; i++) {
			cout << "size of Level " << i + 1 << ": ";
			int count = 0;
			SNode* temp = list.head->next[i];
			while (temp != NULL) {
				count++;
				temp = temp->next[i];
			}
			cout << count << endl;
		}
	}
}

void searchValue(int value, SkipList list) {
	SNode* current = list.head;
	//Danh sách các node đã đi qua ở các tầng
	SNode** traversePath = new SNode * [list.maxLevel];
	int traversePathIndex = 0;

	for (int i = list.currentLevel - 1; i >= 0; i--) {
		traversePath[traversePathIndex] = current;
		traversePathIndex++;
		while (current->next[i] != NULL && current->next[i]->value <= value) {
			current = current->next[i];
			traversePath[traversePathIndex] = current;
			traversePathIndex++;
		}
		if (current->value >= value) break;
	}


	if (current == NULL || current->value != value) {
		cout << value << " not found\n";
		return;
	}

	cout << "Level of "<< value << ": " << current->currentLevel << endl;
	cout << "Traverse path: ";
	for (int j = 0; j < traversePathIndex; j++) {
		if (traversePath[j]->value == -1) cout << "HEAD ";
		else cout << traversePath[j]->value << " ";
	}
	cout << endl;
}

void printList(SkipList list) {
	if (list.head->next[0] == NULL) {
		cout << "List is empty\n";
		return;
	}
	for (int i = list.currentLevel - 1; i >= 0; i--) {
		cout << "Level " << i + 1 << ": ";

		int index = 0;
		for (SNode* node = list.head; node != NULL; node = node->next[i]) {
			if (node->value != -1) cout << node->value << " ";

			int currentIndex = node->width[i] + index - 1;

			//Tạo các khoảng cách dựa vào số lượng chữ số các phần tử bỏ qua và độ rộng của liên kết
			for (index; index < currentIndex; index++) {
				for (int j = 0; j < countDigits(getByIndex(index, list)->value); j++) cout << " ";
				cout << " ";
			}
			index++;

		}

		cout << endl;
	}
}

void handleCommandLine(string commandLine, SkipList& list) {
	int argumentCount = 1;
	int pos = 0;
	int index = 0;

	//Xử lý tách chuỗi lệnh
	for (int i = 0; i < commandLine.length(); i++) {
		if (commandLine[i] == ' ') argumentCount++;
	}
	string* argumentList = new string[argumentCount];
	while ((pos = commandLine.find(" ")) != string::npos) {
		argumentList[index] = commandLine.substr(0, pos);
		commandLine.erase(0, pos + 1);
		index++;
	}
	argumentList[index] = commandLine;
	string command = argumentList[0];

	if (command == "New") {
		int n;
		bool isRandomLevel = false;
		int i = 0;
		if (argumentList[1] == "-r") {
			i = 1;
			n = stoi(argumentList[2]) + 1;
			isRandomLevel = true;
		}
		else n = stoi(argumentList[1]);
		int* arr = new int[n];

		for (i = 0; i < n; i++) {
			arr[i] = stoi(argumentList[i + 2]);
		}

		buildList(arr, n, list, isRandomLevel);
	}
	else if (command == "Print") {
		printList(list);
	}
	else if (command == "Size") {
		printSizeOfList(list);
	}
	else if (command == "Search") {
		int val = stoi(argumentList[1]);

		searchValue(val, list);
	}
	else if (command == "Insert") {
		int val;
		bool isRandomLevel = false;
		if (argumentList[1] == "-r") {
			val = stoi(argumentList[2]);
			isRandomLevel = true;
		}
		else val = stoi(argumentList[1]);

		addNode(val, list, isRandomLevel);
	}
	else if (command == "Remove") {
		int val = stoi(argumentList[1]);

		removeNode(val, list);
	}
	else if (command == "Exit") exit(0);
	else cout << "Command not found\n";
}