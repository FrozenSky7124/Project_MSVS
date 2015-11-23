#ifndef _ARRAY_H_
#define _ARRAY_H_

class Array
{
private:
	int mLength;
	int* mSpace;

public:
	Array(int length);
	Array(const Array& obj);
	int length();
	void setData(int index, int value);
	int getData(int index);
	~Array();
	//²Ù×÷·ûÖØÔØº¯Êı
	int& operator[](int i);
	Array& operator=(Array& a);
};

#endif
