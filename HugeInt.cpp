#include "HugeInt.h"
#include <iostream>
#include <exception>
#include <cmath>

std::ostream& operator<<(std::ostream& out, const HugeInt& object) {
	for (int i = object.m_max_length - object.m_length; i < object.m_max_length; ++i) {
		out << object.m_data[i];
	}
	out << std::endl;
	return out;
}


const int MAX_LENGTH = (std::numeric_limits<long int>::digits10 + 1) * 2;

bool is_number(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

namespace {
	int get_int(char c) {
		return c - '0';
	}

	char get_char(int i) {
		return i + '0';
	}
}

HugeInt::HugeInt(string value)
	:m_max_length(MAX_LENGTH)
	, m_length(0)
	, m_data(nullptr)
{
	if (!is_number(value)) {
		throw std::exception("Expresion must be number");
	}

	if (value.length() > MAX_LENGTH) {
		m_max_length = value.length();
	}

	m_length = value.length();
	m_data = new char[m_max_length] {};

	for (int i = m_max_length - 1, j = value.length() - 1; i >= m_max_length - m_length; --i,--j) {
		m_data[i] = value[j];
	}
}

HugeInt::HugeInt(unsigned int value) 
	:m_max_length(MAX_LENGTH)
	,m_length(0)
	,m_data(nullptr)
{
	m_data = new char[m_max_length] {};
	if (value == 0) {
		++m_length;
		m_data[m_max_length - 1] = get_char(0);
	}
	for (int i = m_max_length - 1; value != 0; --i, value /= 10) {
		m_data[i] = get_char(value % 10);
		++m_length;
	}
}

HugeInt::HugeInt(const HugeInt& arg)
	:m_max_length(arg.m_max_length)
	,m_length(arg.m_length)
	,m_data(nullptr)
{
	allocate_and_initialize(arg.m_data);
}

HugeInt& HugeInt::operator=(const HugeInt& arg) {
	if (this != &arg) {
		deallocate();
		m_max_length = arg.m_max_length;
		m_length = arg.m_length;
		allocate_and_initialize(arg.m_data);
	}
	return *this;
}

HugeInt HugeInt::operator+(const HugeInt& arg) const {
	HugeInt tmp(*this);
	
	if (m_length == 1 && m_data[m_max_length - 1] == '0') {
		return arg;
	}
	if (arg.m_length == 1 && arg.m_data[arg.m_max_length - 1] == '0') {
		return tmp;
	}
	if (tmp.m_length == tmp.m_max_length || arg.m_length > tmp.m_length) {
		tmp.resize(tmp, (std::max(tmp.m_max_length, arg.m_max_length) * 2));
	}
	int min_index = tmp.m_max_length - std::max(tmp.m_length, arg.m_length);
	int in_mind{}, sum{}, tmp_digit{}, arg_digit{};
	int i = tmp.m_max_length - 1, j = arg.m_max_length - 1;
	for (; i >= min_index; --i, --j) {
		tmp_digit = (i >= tmp.m_max_length - tmp.m_length) ? get_int(tmp.m_data[i]) : 0;
		arg_digit = (j >= arg.m_max_length - arg.m_length) ? get_int(arg.m_data[j]) : 0;
		sum = tmp_digit + arg_digit + in_mind;
		tmp.m_data[i] = get_char(sum % 10);
		in_mind = sum / 10;
	}
	if (in_mind != 0) {
		tmp.m_data[i] = '1';
	}
	
	tmp.m_length = tmp.m_max_length - i;
	return tmp;
}

HugeInt& HugeInt::operator+=(const HugeInt& arg) {
	return *this = *this + arg;
}

HugeInt HugeInt::operator*(const HugeInt& arg) const {
	HugeInt tmp(*this);
	tmp.resize(tmp, (std::max(tmp.m_max_length, arg.m_max_length) * 2));
	int sum1{ 0 }, sum2{ 0 };
	int in_mind{}, tmp_digit{}, arg_digit{}, counter1{}, counter2{};
	int i = tmp.m_max_length - 1, j = arg.m_max_length - 1;
	for (; i >= tmp.m_max_length - tmp.m_length; --i) {
		tmp_digit = (i >= tmp.m_max_length - tmp.m_length) ? get_int(tmp.m_data[i]) : 1;
		for (; j >= arg.m_max_length - arg.m_length; --j) {
			arg_digit = (j >= arg.m_max_length - arg.m_length) ? get_int(arg.m_data[j]) : 1;
			sum1 += tmp_digit * arg_digit * pow(10, counter1);
			++counter1;
		}
		sum2 += sum1 * pow(10, counter2);
		j = arg.m_max_length - 1;
		counter1 = 0;
		sum1 = 0;
		++counter2;
	}
	HugeInt h(sum2);
	return h;
}

HugeInt::~HugeInt() {
	deallocate();
}

void HugeInt::resize(HugeInt& arg,int new_length) {
	int old_max_length = arg.m_max_length;
	arg.m_max_length = new_length;
	char* tmp = new char[arg.m_max_length] {};
	for (int i = arg.m_max_length - arg.m_length, j = old_max_length - arg.m_length; i < arg.m_max_length; ++i, ++j) {
		tmp[i] = arg.m_data[j];
	}
	delete[] arg.m_data;
	arg.m_data = tmp;
}

void HugeInt::allocate_and_initialize(const char* data) {
	m_data = new char[m_max_length] {};
	for (int i = m_max_length - m_length; i < m_max_length; ++i) {
		m_data[i] = data[i];
	}
}

void HugeInt::deallocate() {
	delete[]m_data;
}
