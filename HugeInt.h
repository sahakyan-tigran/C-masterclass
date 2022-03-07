#pragma once

#include <string>
using namespace std;

class HugeInt {
	friend std::ostream& operator<<(std::ostream&, const HugeInt&);
	friend std::istream& operator>>(std::istream&, HugeInt&);

public:
	HugeInt(string);
	explicit HugeInt(unsigned int = 0);
	HugeInt(const HugeInt&);
	HugeInt& operator=(const HugeInt&);
	~HugeInt();

	HugeInt& operator+=(const HugeInt&);
	HugeInt operator * (const HugeInt&) const;
	HugeInt operator + (const HugeInt&) const;
private:
	void resize(HugeInt&,int);
	void deallocate();
	void allocate_and_initialize(const char*);
private:
	char* m_data;
	int m_length;
	int m_max_length;
};
