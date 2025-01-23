#pragma once
#include <vector>
#include <algorithm>
#include "Bombo.hpp"
#include "RandomGen.hpp"
#include "Premio.hpp"

using namespace std;

class Carton {
public:
	static const int Filas = 3;
	static const int Columnas = 5;
	Carton(const vector<int>& numeros);
	Carton() {};
	void marcarBola(int num);
	vector<vector<pair<int, bool>>> getCarton() { return _matrizCarton; }
	bool* getPremiosCarton(const vector<Premio> & premios);
private:
	//De esta forma podemos guardar los cartones con su numero y si se encuentran marcados o no
	vector<vector<pair<int, bool>>> _matrizCarton;
	bool _premios[Premio::TotalPremios] = { 0 };
	int _filasPremiadas[Filas]{ 0 };
	int _nFilasPremiadas = 0;
};