#ifndef VECTOR_H
#define VECTOR_H

#include<stdexcept> // for invalid_argument
using namespace std;

template<class T>
class Vector
{
  public:
    // Constructor
    Vector();

    // Copy Constructor
    Vector(const Vector<T>& v);

    // Destructor
    ~Vector();

    // Add elements to the vector
    void push_back(T v);

    // Access elements of the vector
    T& operator[](const unsigned int idx);

    // Number of elements in the vector
    unsigned int length() const;

    // Capacity of the underlying array
    unsigned int capacity() const;

  private:
    unsigned int len;
    unsigned int cap;
    T* array;
};

#include"vector.hpp"
#endif
