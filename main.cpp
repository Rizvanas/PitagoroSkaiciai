#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

int main(int argc, char *argv[]) {

	float fmas1 [1000];
	float fmas2 [1000];
	float tempResult [4];
	int fmas1Size = sizeof(fmas1) / sizeof(float);
	std::string results;

	for (int i = 0; i < fmas1Size; ++i) {
		fmas2[i] = fmas1[i] = i + 1;
	}

	float *fPtr1{ nullptr };
	float *fPtr2{ nullptr };
	float *tempResultPtr{ tempResult };

	for (int i = 0; i < fmas1Size; i+=4) {
		fPtr1 = fmas1 + i;
		for (int j = i + 1; j < fmas1Size - 4; ++j) {
			fPtr2 = fmas2 + j;
			_asm {
				mov eax, fPtr1
				mov ebx, fPtr2

				movups xmm0, [eax]
				movups xmm1, [ebx]

				movups xmm2, xmm0
				movups xmm3, xmm1

				mulps xmm2, xmm2
				mulps xmm3, xmm3

				movups xmm4, xmm2
				addps  xmm4, xmm3

				movups xmm5, xmm4
				sqrtps xmm5, xmm5

				//----------Fast floor() function--------
				cvttps2dq xmm6, xmm5
				cvtdq2ps  xmm5, xmm6
				//---------------------------------------
				mulps xmm5, xmm5
				subps xmm5, xmm4

				mov	    eax,  tempResultPtr
				movaps [eax], xmm5
			}

			for (int k = 0; k < 4; ++k) {
				if (0 == tempResult[k]) {
					std::stringstream ss;
					ss << std::left << std::setw(4)
						<< fmas1[i + k] << fmas2[j + k] << std::right << std::setw(5) 
						<< std::sqrt(fmas1[i + k] * fmas1[i + k] + fmas2[j + k] * fmas2[j + k]) << std::endl;
					results += ss.str();
				}
			}
		}
 	}

	std::cout << results << std::endl;
	return 0;
}