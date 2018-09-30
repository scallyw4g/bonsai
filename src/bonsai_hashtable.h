template <typename T>
struct hashtable
{
  umm Size;
  T* Table;
};

template <typename T> hashtable<T>
AllocateHashtable(umm Count, memory_arena* Memory)
{
  hashtable<T> Result = {};
  Result.Table = Allocate(T, Memory, Count);
  Result.Size = Count;
  return Result;
}
