#ifndef SRC_LAYERS_POOLING_LAYER_HPP_
#define SRC_LAYERS_POOLING_LAYER_HPP_

#include <string>

#include "../blob.hpp"
#include "../layer.hpp"

namespace spaceBrain
{

class PoolingLayer : public Layer
{
public:

	PoolingLayer(std::string name, std::string bottom, std::string top, int pad, int kernelSize, int stride);
	virtual ~PoolingLayer(){}

	virtual void LayerSetUp(const Blob<int>* bottom, const Blob<int>* top);

	virtual void Reshape(const Blob<int>* bottom, Blob<int>* top);

	virtual void Forward(const Blob<int>* bottom, Blob<int>* top);

	virtual inline const int type() const
	{
		return POOL;
	}

protected:
	 // pad_, kernel_size_, and pooled_size_ are both height and width as only square blobs are supported
	int pad_;
	int kernel_size_;
	int stride_;
	int output_size_;
	int input_size_;

	int num_;
	int depth_;
};

bool PoolTest();

}

#endif /* SRC_LAYERS_POOLING_LAYER_HPP_ */
