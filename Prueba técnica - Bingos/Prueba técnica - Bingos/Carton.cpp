#include "Carton.hpp"


/// <summary>
/// Construye el carton a partir de un vector de numeros
/// </summary>
Carton::Carton(const vector<int>& numeros)
{
	for (int i = 0; i < Filas; i++) {
		vector<pair<int, bool>> col;
		for (int y = 0; y < Columnas; y++) {
			int pos = y * Filas + i;
			col.push_back(pair<int, bool>(numeros[y* Filas +i], false));
		}
		_matrizCarton.push_back(col);
	}
}

/// <summary>
/// Busca si existe el numero dentro del carton, en caso de estarlo lo marca
/// </summary>
void Carton::marcarBola(int num)
{
	for (int i = 0; i < Filas; i++) {
		auto it = find_if(_matrizCarton[i].begin(), _matrizCarton[i].end(),
			[&num](const pair<int, bool>& element) { return element.first == num; });
		if (it != _matrizCarton[i].end()) {
			int pos = distance(_matrizCarton[i].begin(), it);
			_matrizCarton[i][pos].second = true;
			_filasPremiadas[i]++;
			if (_filasPremiadas[i] >= 5)
				_nFilasPremiadas++;
			return;
		}
	}
}

/// <summary>
/// Comprueba todos los premios posibles del carton
/// </summary>
bool* Carton::getPremiosCarton(const vector<Premio>& premios)
{
	for (int i = 0; i < Premio::TotalPremios; i++)
	{
		if (!_premios[i]) {
			bool cumple = true;
			if (premios[i]._condicionesFila == -1) {
				const vector<vector<bool>>& patron = premios[i]._patron;
				int fila = 0, columna = 0;
				while (fila < Filas && columna < Columnas) {
					if (patron[fila][columna]) {
						if (patron[fila][columna] != _matrizCarton[fila][columna].second) {
							cumple = false;
							break;
						}
					}
					columna++;
					if (columna >= Columnas) {
						columna = 0;
						fila++;
					}
				}
			}
			else
			{
				if (_nFilasPremiadas < premios[i]._condicionesFila)
					cumple = false;
			}
			if (cumple)
				_premios[premios[i]._id] = true;
		}
	}
	return _premios;
}
