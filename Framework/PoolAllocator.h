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
	//objectSized
	T* memoryBlock; 
	std::vector<T*> unusedBlocks; 
    size_t MAX_POOL_SIZE = 100; 

	size_t allocationsCount = 0;      // Number of allocations made
	size_t deallocationsCount = 0;    // Number of deallocations made
	size_t totalAllocatedMemory = 0;  // Total bytes allocated
	size_t totalFreedMemory = 0;      // Total bytes freed

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
		cout << "[PoolAllocator] Destructor called.\n"
			<< "Total allocations: " << allocationsCount << "\n"
			<< "Total deallocations: " << deallocationsCount << "\n"
			<< "Total allocated memory: " << totalAllocatedMemory << " bytes\n"
			<< "Total freed memory: " << totalFreedMemory << " bytes\n";
		free(memoryBlock); 
	}

	void* Allocate()
	{
		T* object = nullptr;
		if (unusedBlocks.empty())
		{
			cout << "Memory pool is exhausted, allocate from the heap from now!" << endl;
			object = static_cast<T*>(malloc(sizeof(T)));
			//return malloc(sizeof(T)); 
		}
		else {
			object = unusedBlocks.back();
			unusedBlocks.pop_back();
			cout << "[PoolAllocator] Allocating from pool. Remaining slots: " << unusedBlocks.size() << "\n";
		}

		allocationsCount++;
		totalAllocatedMemory += sizeof(T);
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

		deallocationsCount++;
		totalFreedMemory += sizeof(T);

	}
};



#endif
