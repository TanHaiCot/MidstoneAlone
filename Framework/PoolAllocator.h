#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <vector>
#include <iostream>

using namespace std; 

template <typename T>
class PoolAllocator
{
private:
	size_t poolSize; 
	T* memoryBlock; 
	std::vector<T*> unusedBlocks; 
    size_t MAX_POOL_SIZE = 100; 

public:
	PoolAllocator(size_t poolsize_) : poolSize(poolsize_)
	{
		if (poolSize > MAX_POOL_SIZE)
		{
			//cout << "Requested pool size(" << poolSize << ") exceeds max pool size (" << MAX_POOL_SIZE << ")" << endl;
			poolSize = MAX_POOL_SIZE;
		}

		this->poolSize = poolSize;

		memoryBlock = static_cast<T*>(malloc(sizeof(T) * poolSize));
		if (!memoryBlock)
		{
			throw bad_alloc();
		}

		for (size_t i = 0; i < poolSize; ++i)
		{
			unusedBlocks.push_back(&memoryBlock[i]);
		}
	}
	

	~PoolAllocator() {
		free(memoryBlock); 
	}

	void* Allocate()
	{
		if (unusedBlocks.empty())
		{
			cout << "Memory pool is exhausted, allocate from the heap from now!" << endl;
			return malloc(sizeof(T)); 
		}
		T* object = unusedBlocks.back();
		unusedBlocks.pop_back();
		cout << "[PoolAllocator] Allocating from pool. Remaining slots: " << unusedBlocks.size() << "\n";
		return object; 
	}

	void Deallocate(void* ptr)
	{
		if (ptr >= memoryBlock && ptr < memoryBlock + poolSize)
		{
			unusedBlocks.push_back(static_cast<T*>(ptr));
		}
		else
		{
			free(ptr); 
		}
	}
};



#endif
