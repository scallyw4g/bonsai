template <typename T>
struct hashtable
{
  umm ElementCount;
  T* Elements;

  T* operator[](umm HashIndex)
  {
    Assert(HashIndex < this->ElementCount);

    T* Result = this->Elements + HashIndex;
    return Result;
  }

};

template <typename T> hashtable<T>
AllocateHashtable(umm ElementCount, memory_arena* Memory)
{
  hashtable<T> Result = {};
  Result.Elements = Allocate(T, Memory, ElementCount);
  Result.ElementCount = ElementCount;
  return Result;
}

