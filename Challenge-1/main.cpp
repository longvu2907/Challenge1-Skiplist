#include "Header.h"

int main() {
	srand(time(NULL));

	SkipList list;
	initList(list);

	string commandLine;

	cout << "Enter Command:\n";
	cout << "* New S x1 x2 ... x3 (Tao Skiplist voi level cac node dua tren value)\n";
	cout << "* New -r S x1 x2 ... x3 (Tao Skiplist voi level cac node ngau nhien)\n";
	cout << "* Print (In Skiplist)\n";
	cout << "* Size (In kich thuoc cua danh sach)\n";
	cout << "* Search val (Tim kiem node co gia tri val)\n";
	cout << "* Insert val (Them Node voi gia tri val voi level dua tren value)\n";
	cout << "* Insert -r val (Them Node voi gia tri val voi level ngau nhien)\n";
	cout << "* Remove val (Xoa node co gia tri val)\n";
	cout << "* Exit (Thoat chuong trinh)\n\n";
	while (true) {
		getline(cin, commandLine);
		handleCommandLine(commandLine, list);
		cout << endl;
	}
	
	return 0;
}