// Constructor
template<class T>
Vector<T>::Vector()
{
  cap = 4;
  len = 0;
  array = new T[cap];
}

// Copy Constructor
template<class T>
Vector<T>::Vector(const Vector<T>& v)
{
  cap = v.cap;
  len = v.len;
  array = new T[cap];
  for(unsigned int i = 0; i < len; i++)
  {
    array[i] = v.array[i];
  }
}

// Destructor
template<class T>
Vector<T>::~Vector()
{
  delete [] array;
}

// Add elements to the vector
template<class T>
void Vector<T>::push_back(T v)
{
  // Expand vector if needed
  if(len == cap)
  {
    cap *= 2;
    T* new_array = new T[cap];
    for(unsigned int i = 0; i < len; i++)
    {
      new_array[i] = array[i];
    }

    delete[] array;
    array = new_array;
  }

  array[len] = v;
  len++;
}

// Read elements of the vector
template<class T>
T& Vector<T>::operator[](const unsigned int idx)
{
  if(idx < 0 || idx >= len)
  {
    throw invalid_argument("Index out of range");
  }

  return array[idx];
}

// length getter
template<class T>
unsigned int Vector<T>::length() const
{
  return len;
}

// size getter
template<class T>
unsigned int Vector<T>::capacity() const
{
  return cap;
}

