#ifndef SRC_DATA_MEMORY_HPP_
#define SRC_DATA_MEMORY_HPP_

#include <stddef.h>

namespace spaceBrain
{

class DataMemory
{
public:

	DataMemory();
	~DataMemory();

	void InitData(const size_t size);
	void SetData(void* dataPtr);

	const void* getConstData()
	{
		return (const void*) ptr_;
	}

	void* getMutableData()
	{
		return ptr_;
	}

	size_t size()
	{
		return size_;
	}

private:

	bool isPtrSet_;
	size_t size_;
	void* ptr_;

};

void DataMemoryTest();

}

#endif /* SRC_DATA_MEMORY_HPP_ */
