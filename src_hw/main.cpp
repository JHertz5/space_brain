#include <util/io.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	bool keepRunning = false;

	for (int argIndex = 1; argIndex < argc; argIndex++)
	{
		if(argv[argIndex][0] == '-' && (argv[argIndex][2] == '\0' || argv[argIndex][2] == ' '))
		{
			switch(argv[argIndex][1])
			{
			case 'h':
				spaceBrain::printHelpMessage(true);
				return 0;

			case 't':
				keepRunning = true;
				std::cout << "Testing Interface Open" << std::endl;
				break;

			default:
				std::cout << "COMMAND NOT RECOGNISED" << std::endl;
			}
		}
		else
		{
			spaceBrain::performCommand(std::string(argv[argIndex]), &keepRunning);
		}
	}

	std::string inMessage;

	while(keepRunning)
	{
		std::cout << "Ready for input: (input 'help' for test commands)" << std::endl;
		std::cin >> inMessage;

		// respond to command
		spaceBrain::performCommand(inMessage, &keepRunning);
	} // end of while

	//spaceBrain::Logger::CloseLogger();

}


/*
	int test_passed = 0;
float *A, *B, *C, *D, *D_sw;

A = (float *)sds_alloc(N * N * sizeof(float));
B = (float *)sds_alloc(N * N * sizeof(float));
C = (float *)sds_alloc(N * N * sizeof(float));
D = (float *)sds_alloc(N * N * sizeof(float));
D_sw = (float *)malloc(N * N * sizeof(float));

if (!A || !B || !C || !D || !D_sw) {
	if (A) sds_free(A);
	if (B) sds_free(B);
	if (C) sds_free(C);
	if (D) sds_free(D);
	if (D_sw) free(D_sw);
	return 2;
}

test_passed = mmult_test(A, B, C, D, D_sw);

std::cout << "TEST " << (test_passed ? "FAILED" : "PASSED") << std::endl;

sds_free(A);
sds_free(B);
sds_free(C);
sds_free(D);
free(D_sw);
 */
//return (test_passed ? -1 : 0);
