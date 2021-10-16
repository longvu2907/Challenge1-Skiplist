#pragma once

#include <iostream>
#include <ctime>
#include <math.h>
#include <string>

using namespace std;

//Cấu trúc Node của Skiplist
struct SNode {
	int value;
	int level;
	int* width;
	SNode** next;
};

//Cấu trúc của Skiplist
struct SkipList {
	SNode* head;
	int maxLevel = 1;
	int currentLevel = 1;
	int size = 0;

};

//Tạo node mới
SNode* createNode(int value, int level);

//Khởi tạo Skiplist
void initList(SkipList& list);

//Phát sinh level ngẫu nhiên
int randomLevel(SkipList list);
//Level dựa trên value
int levelBasedOnVal(int val);

//Tăng max level của Skiplist khi thêm phần tử mới
void increaseMaxLevel(SkipList& list, int val, bool isRandomLevel);

//Đếm số lượng chữ số (để in danh sách Skiplist)
int countDigits(int val);
//Lấy ra Node bởi index của nó
SNode* getByIndex(int index, SkipList list);

//Thêm node mới
void addNode(int value, SkipList& list, bool isRandomLevel);

//Tạo Skiplist từ array
void buildList(int* arr, int n, SkipList& list, bool isRandomLevel);

//Xóa node trong Skiplist
void removeNode(int value, SkipList& list);

//In max level của Skiplist, số lượng phần tử ở các tầng
void printSizeOfList(SkipList list);

//Tìm kiếm ,in ra level của value và các node mà nó đã đi qua
void searchValue(int val, SkipList list);

//In Skiplist
void printList(SkipList list);

//Xử lý lệnh
void handleCommandLine(string commandLine, SkipList& list);