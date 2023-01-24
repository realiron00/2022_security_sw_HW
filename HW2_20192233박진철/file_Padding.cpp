
#include <iostream>
#include <fstream>

using namespace std;

typedef unsigned char byte;

int FileSize(const char *file_name) {
	ifstream fin;
	//fin.open("file-1.bin", ios::binary);
	fin.open(file_name, ios::binary);
	if (fin.fail()) {
		cout << "Input File Open Error" << endl;
		return -1; //������ ����, -1�� ��ȯ
	}
	int file_length;

	fin.seekg(0, fin.end); // get-pointer ��ġ��Ű��
	file_length = fin.tellg(); // tell me "get-pointer"
	//cout << " File size (bytes) = " << file_length << endl;
	fin.close();

	return file_length;
}

//�е��Լ�: �Է� (0-15����Ʈ ������), ������ ����, ���: 16����Ʈ(�Ѻ��)
// ��: 01020304, 4 ---> 01020304 80000000 00000000 00000000 
// ����: �Է� 0����Ʈ�� �����ϵ��� (�е��� 16����Ʈ ���)

//void Padding(byte in[], int in_length, byte out[]) {
void Padding(byte in[], int in_length, byte out[16]) {
	byte padding_start = 0x80;
	for (int i = 0; i < in_length; i++) {
		out[i] = in[i];
	}
	out[in_length] = padding_start;
	for (int i = in_length + 1; i < 16; i++) {
		out[i] = 0x00;
	}
}

//�Է�����(source) --> [Padding] --> �������(dest)
//�Է�����ũ�� (N ����Ʈ): 
//   16�� ��� --> ���ũ�� N+16 (�е��� �Ѻ��)
//   16�� ����� �ƴϸ� --> N���� ū ���� 16�� ���
void FilePadding(const char* source, const char* dest) {
	ifstream fin;
	ofstream fout;
	fin.open(source, ios::binary);
	if (fin.fail()) {
		cout << "Input File Open Error" << endl;
		return;
	}
	fout.open(dest, ios::binary);
	if (fout.fail()) {
		cout << "Output File Open Error" << endl;
		return;
	}
	int file_len;
	file_len = FileSize(source); //�Է����� ����(����Ʈ)
	// file_len = 15 -> num_blocks = 1  (remainder = 15)
	// file_len = 16 -> num_blocks = 2  (remainder = 0)
	// file_len = 17 -> num_blocks = 2  (remainder = 1)
	// remainder     = 0, 1, 2, ... , 15
	// padding bytes =16,15,14, ... ,  1 
	// remainder + (padding bytes) = 16
	int num_blocks = file_len / 16 + 1;
	int remainder = file_len - (num_blocks - 1) * 16;

	byte buffer[16];
	// �е��� ������� �ʴ� ���
	for (int i = 0; i < num_blocks - 1; i++) {
		fin.read((char*)buffer, 16);
		// �Ѻ�� ��ȣȭ
		fout.write((char*)buffer, 16);
	}
	byte last_in_block[16];
	byte last_out_block[16];
	byte ch;
	// ���� ������ �б� (0, 1, ... , 15)
	for (int i = 0; i < remainder; i++) {
		fin.read((char*)&ch, 1);
		last_in_block[i] = ch;
	}

	Padding(last_in_block, remainder, last_out_block);
	// �Ѻ�� ��ȣȭ
	fout.write((char*)last_out_block, 16);

	fin.close();
	fout.close();
}