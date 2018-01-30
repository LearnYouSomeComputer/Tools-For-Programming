#include<iostream>
using namespace std;

struct my_array {
  int* elements;
  int size;
};

void pretty_print(const my_array& array) {
  int i = 0;
  cout << "[";

  for(; i < array.size - 1; i++) {
    cout << array.elements[i] << ", ";
  }
  cout << array.elements[array.size] << "]";
}

int main() {
  my_array stuff;
  stuff.size = 5;
  stuff.elements = 0;

  pretty_print(stuff);

  return 0;
}