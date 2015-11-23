#include "iostream"
#include "Array.h"
using namespace std;

Array::Array(int length)
{
	if (length < 0)
	{
		length = 0;
	}

	mLength = length;
	mSpace = new int[mLength];
}

Array::Array(const Array& obj)
{
	mLength = obj.mLength;

	mSpace = new int[mLength];

	for (int i = 0; i < mLength; i++)
	{
		mSpace[i] = obj.mSpace[i];
	}
}

int Array::length()
{
	return mLength;
}

void Array::setData(int index, int value)
{
	mSpace[index] = value;
}

int Array::getData(int index)
{
	return mSpace[index];
}

Array::~Array()
{
	mLength = -1;
	delete[] mSpace;
}

//²Ù×÷·ûÖØÔØº¯Êý
int& Array::operator[](int i)
{
	if (i > mLength)
	{
		cout << "Error!" << endl;
	}
	else
	{
		return *(mSpace+i);
	}
}

Array& Array::operator=(Array& a)
{
	int i = 0;
	delete[] this->mSpace;
	this->mLength = a.mLength;
	this->mSpace = NULL;
	this->mSpace = new int[this->mLength];
	for (i = 0;i < this->mLength;i++)
	{
		mSpace[i] = a.mSpace[i];
	}
	return *this;
}