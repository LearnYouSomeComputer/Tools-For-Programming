#include "catch.hpp"
#include "vector.h"

template class Vector<int>;

TEST_CASE("Vector Sizing", "[vector]")
{
  Vector<int> v;

  SECTION("An empty vector has zero length and defined size")
  {
    CHECK(v.length() == 0);
    CHECK(v.capacity() == 4);
  }

  SECTION("A copied vector has the same size")
  {
    Vector<int> c(v);

    CHECK(v.length() == c.length());
    CHECK(v.capacity() == c.capacity());
  }

  for(int i = 0; i < 5; i++)
  {
    v.push_back(i);
  }

  SECTION("Adding elements causes the vector to recapacity")
  {
    CHECK(v.length() == 5);
    CHECK(v.capacity() >= 5);
  }

  SECTION("A copied vector has the same capacity")
  {
    Vector<int> c(v);

    CHECK(v.length() == c.length());
    CHECK(v.capacity() == c.capacity());
  }
}

TEST_CASE("Vector Elements", "[vector]")
{
  Vector<int> v;

  for(int i = 0; i < 5; i++)
  {
    v.push_back(i);
  }

  SECTION("Elements added with push_back are accessible")
  {
    for(int i = 0; i < 5; i++)
    {
      CHECK(v[i] == i);
    }
  }

  SECTION("A copied vector is identical")
  {
    Vector<int> c(v);
    for(int i = 0; i < v.length(); i++)
    {
      CHECK(v[i] == c[i]);
    }
  }

  SECTION("Vector copies are deep copies")
  {
    Vector<int> c(v);
    for(int i = 0; i < v.length(); i++)
    {
      c[i] = -1;
      CHECK(v[i] != c[i]);
    }
  }

  SECTION("Out-of-bounds accesses are detected")
  {
    CHECK_THROWS_WITH(v[-1], "Index out of range");
    CHECK_THROWS_WITH(v[5], "Index out of range");
  }
}
