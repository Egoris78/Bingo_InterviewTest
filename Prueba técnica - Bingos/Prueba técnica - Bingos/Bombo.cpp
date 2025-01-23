#include "Bombo.hpp"

int Bombo::extraerBola()
{
	RandomGenerator rng(1, NumeroTotalDeBolas);
	int num = rng.generate();
	while (_numeros[num-1]) {
		num = rng.generate();
	}
	_numeros[num-1] = true;
	_bolasExtraidas.push_back(num);
	return num;
}
