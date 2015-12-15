#pragma once
class String
{
private:
	char *s;
public:
	String(char *sin);
	~String();
	friend bool operator==(String &a, String &b);
	friend bool operator>(String &a, String &b);
	friend bool operator<(String &a, String &b);
	void show();
};

