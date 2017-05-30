#include <iostream>
#include <string>

#include "../data_memory.hpp"
#include "../layers/dropout_layer.hpp"
#include "../layers/pooling_layer.hpp"
#include "../layers/relu_layer.hpp"
#include "../layers/softmax_layer.hpp"
#include "../logger.hpp"
#include "filler.hpp"
#include "maths_functions_test.hpp"

namespace spaceBrain
{

void printHelpMessage(bool commandLineVersion)
{
	if(commandLineVersion)
	{
		std::cout << "Usage: space_brain.exe [OPTIONS...]" << std::endl;
		std::cout << std::endl;
		std::cout << "\t -h \t Display this message" << std::endl;
		std::cout << "\t -t \t Hold testing interface open" << std::endl;
		std::cout << std::endl;
	}
	// testing interface commands
	std::cout << "Test Commands" << std::endl;
	std::cout << "\nIMPLEMENTED:" << std::endl;
	std::cout << "\t'relu' \t - ReLU Layer" << std::endl;
	std::cout << "\t'pool' \t - Pooling Layer" << std::endl;
	std::cout << "\t'drop' \t - Dropout Layer" << std::endl;
	std::cout << "\t'maths'\t - Maths Functions" << std::endl;
	std::cout << "\t'data' \t - Data Memory Class" << std::endl;
	std::cout << "\t'log'  \t - Logging functionality" << std::endl;
	std::cout << "\t'fill' \t - Blob Filling Functions" << std::endl;
	std::cout << "\t'quit' \t - End Program" << std::endl;

	std::cout << "\nTODO:" << std::endl;
	std::cout << "\t'input'\t - Input Layer" << std::endl;
	std::cout << "\t'conv' \t - Convolution Layer" << std::endl;
	std::cout << "\t'norm' \t - LRN Layer" << std::endl;
	std::cout << "\t'fc'   \t - Fully Connected Layer" << std::endl;
	std::cout << "\t'prob' \t - SoftMax Layer" << std::endl;

	std::cout << std::endl;
}

void performCommand(std::string inMessage, bool* keepRunning)
{
	const std::string notYetImplementedString =  "\t-not yet implemented";

	if(inMessage == "help") // HELP message
	{
		printHelpMessage(false);
	}
	else if(inMessage == "input")
	{
		std::cout << "Input Layer" << std::endl;
		std::cout << notYetImplementedString << std::endl;
		std::cout << std::endl;
	}
	else if(inMessage == "conv")
	{
		std::cout << "Convolution Layer" << std::endl;
		std::cout << notYetImplementedString << std::endl;
		std::cout << std::endl;
	}
	else if(inMessage == "relu")
	{
		std::cout << "ReLU (Rectified Linear Unit) Layer" << std::endl;
		spaceBrain::ReluTest();
		std::cout << std::endl;
	}
	else if(inMessage == "norm")
	{
		std::cout << "LRN (Local Response Normalisation) Layer" << std::endl;
		std::cout << notYetImplementedString << std::endl;
		std::cout << std::endl;
	}
	else if(inMessage == "pool")
	{
		std::cout << "MAX Pooling Layer" << std::endl;
		spaceBrain::PoolTest();
		std::cout << std::endl;
	}
	else if(inMessage == "fc")
	{
		std::cout << "Fully Connected/Inner Product Layer" << std::endl;
		std::cout << notYetImplementedString << std::endl;
		std::cout << std::endl;
	}
	else if(inMessage == "drop")
	{
		std::cout << "Dropout Layer" << std::endl;
		spaceBrain::DropoutTest();
		std::cout << std::endl;
	}
	else if(inMessage == "prob")
	{
		std::cout << "SoftMax Layer" << std::endl;
//		std::cout << notYetImplementedString << std::endl;
		spaceBrain::SoftmaxTest();
		std::cout << std::endl;
	}
	else if(inMessage == "maths")
	{
		std::cout << "Maths Func" << std::endl;
		spaceBrain::gemmTest();
		std::cout << std::endl;
	}
	else if(inMessage == "log")
	{
		std::cout << "Logger" << std::endl;
		spaceBrain::LoggerTest();
		std::cout << std::endl;
	}
	else if(inMessage == "data")
	{
		std::cout << "DataMemory" << std::endl;
		spaceBrain::DataMemoryTest();
		std::cout << std::endl;
	}
	else if(inMessage == "fill")
	{
		std::cout << "Filler" << std::endl;
		spaceBrain::FillerTest();
		std::cout << std::endl;
	}
	else if(inMessage == "quit")
	{
		spaceBrain::Logger::CloseLogger();
		*keepRunning = false;
	}
	else
	{
		std::cout << "COMMAND NOT RECOGNISED" << std::endl;
		std::cout << std::endl;
	}
}

}
