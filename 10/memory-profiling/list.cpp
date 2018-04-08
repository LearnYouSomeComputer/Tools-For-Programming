#include<iostream>
using namespace std;

class  List
{
  public:
    List(unsigned long num)
    {
      n = num;
      nxt = NULL;
    }

    ~List()
    {
      if(nxt)
        delete nxt;
    }

    unsigned long number()
    {
      return n;
    }

    List* next()
    {
      return nxt;
    }

    List* prepend(unsigned long num)
    {
      List* ret = new List(num);
      ret->nxt = this;
      return ret;
    }

  private:
    unsigned long n;
    List* nxt;
};

List* fibonnacci(unsigned long n)
{
  List* fib = new List(1);

  fib = fib->prepend(1);

  for(unsigned long i = 2; i <= n; i++)
  {
    fib = fib->prepend(fib->number() + fib->next()->number());
  }

  return fib;
}

int main()
{
  List* fib = fibonnacci(2000);
  List* iterator = fib;

  while(iterator != NULL)
  {
    cout << iterator->number() << endl;
    iterator = iterator->next();
  }

  delete fib;
  return 0;
}
