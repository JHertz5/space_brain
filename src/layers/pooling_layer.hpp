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

	virtual void LayerSetUp(const Blob<float>* bottom, const Blob<float>* top);

	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top);

	virtual void Forward(const Blob<float>* bottom, Blob<float>* top);

	virtual inline const char* type() const
	{
		return "Pooling";
	}

protected:
	 // pad_, kernel_size_, and pooled_size_ are both height and width as only square blobs are supported
	int pad_;
	int kernel_size_;
	int stride_;
	int pooled_size_;
	int channels_, height_, width_;

};

bool PoolTest();

}

#endif /* SRC_LAYERS_POOLING_LAYER_HPP_ */
