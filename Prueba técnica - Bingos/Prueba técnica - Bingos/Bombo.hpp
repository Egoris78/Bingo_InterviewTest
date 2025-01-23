#pragma once
#include <algorithm>
#include <vector>
#include "RandomGen.hpp"
using namespace std;

static const int NumeroTotalDeBolas = 60;

class Bombo {
private:
	//He decidido hacerlo un bool array para de esta forma comprobar si ha salido la bola o no.
	bool _numeros[NumeroTotalDeBolas] = { 0 };
	vector<int> _bolasExtraidas;
public:
	int extraerBola();
	vector<int> getBolasExtraidas() { return _bolasExtraidas; }
};