#include<iostream>
using namespace std;

long fib_iterative(const long n)
{
  long fib = 1;
  long prev_fib = 0;

  for(long i = 0; i < n; i++)
  {
    long tmp = fib;
    fib += prev_fib;
    prev_fib = tmp;
  }

  return fib;
}

void do_iterative(const int count)
{
  volatile long fib;
  cout << "Iterative: ";
  for(int i = 0; i < count; i++)
    fib = fib_iterative(20);
  cout << fib << endl;
}

long fib_recursive(const long n)
{
  if(n <= 1) return 1;
  return fib_recursive(n-1) + fib_recursive(n-2);
}

void do_recursive(const int count)
{
  volatile long fib;
  cout << "Recursive: ";
  for(int i = 0; i < count; i++)
    fib = fib_recursive(20);
  cout << fib << endl;
}

int main()
{
  const int count = 200;

  do_iterative(count * 1000);

  do_recursive(count);
  return 0;
}