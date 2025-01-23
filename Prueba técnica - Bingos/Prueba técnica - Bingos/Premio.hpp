#pragma once
#include <iostream>
#include <vector>
using namespace std;
class Premio {
private:
	
public:
	static const int TotalPremios = 4;
	int _id;
	string _name;
	int _value;
	string _exlusiones;
	int _condicionesFila;
	vector<vector<bool>> _patron{};
	Premio(const int id,const string& name,const int value,const string& exlusiones,const int condicionesFila, const vector<vector<bool>>& patron) : _id(id), _name(name), _value(value), _exlusiones(exlusiones), _condicionesFila(condicionesFila), _patron(patron){}
};