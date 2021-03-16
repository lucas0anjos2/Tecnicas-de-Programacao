#include "../includes/intserial.h"

intSerial::intSerial() : serializable() {
	value = 0;
}

intSerial::intSerial(int i) : serializable() {
	value = i;
}

intSerial::intSerial(const intSerial& other) {
	value = other.getValue();
}

intSerial::~intSerial() {

}

intSerial intSerial::operator=(const intSerial& other) {
	intSerial aux(other);
	if(this == &other)
		return *this;

	value = other.getValue();
	return aux;
}

bool intSerial::operator==(const intSerial &other) const {
	return value == other.getValue();
}

bool intSerial::operator<(const intSerial &other) const {
	return value < other.getValue();
}

bool intSerial::operator<=(const intSerial &other) const {
	return value <= other.getValue();
}

bool intSerial::operator>(const intSerial &other) const {
	return value > other.getValue();
}

bool intSerial::operator>=(const intSerial &other) const {
	return value >= other.getValue();
}

bool intSerial::operator!=(const intSerial &other) const {
	return value != other.getValue();
}

intSerial intSerial::operator+(const intSerial& other) const {
	intSerial aux;
	aux.setValue(this->value + other.getValue());
	return aux;
}

intSerial intSerial::operator-(const intSerial& other) const {
	intSerial aux;
	aux.setValue(this->value - other.getValue());
	return aux;
}

intSerial intSerial::operator*(const intSerial& other) const {
	intSerial aux;
	aux.setValue(this->value * other.getValue());
	return aux;
}

intSerial intSerial::operator/(const intSerial& other) const {
	intSerial aux;
	aux.setValue(this->value / other.getValue());
	return aux;
}

intSerial intSerial::operator++(int) {
	this->value+=1;
	return *this;
}

intSerial intSerial::operator--(int) {
	this->value-=1;
    return *this;
}

void intSerial::setValue(int v) {
	value = v;
}

int intSerial::getValue() const {
	return value;
}

string intSerial::toString() {
    string aux = "";
    aux += string(reinterpret_cast<char*>(&value), sizeof(value));
    return aux;
}

void intSerial::fromString(string repr) {
    repr.copy(reinterpret_cast<char*>(&value), sizeof(value));
}


unsigned long long int intSerial::size() const {
	return sizeof(value);
}
