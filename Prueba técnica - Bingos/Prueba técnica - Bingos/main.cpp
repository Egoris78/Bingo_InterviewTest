#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <windows.h>
#include <random>
#include <fstream>
#include <conio.h>
#include <sstream>
#include "Carton.hpp"
#include "pugixml.hpp"
#include "Premio.hpp"
using namespace pugi;
using namespace std;
static const int Separacion = 20;
//Los cartones tienen que ser pares
static const int NumeroCartones = 4;
static const int BolasASacar = 30;
static const int keyCrypt = 78;
int _totalPremios[Premio::TotalPremios]{0};
Bombo _bombo;
Carton _cartones[NumeroCartones];
vector<Premio> _premios;
enum _IdPremios {
    _Linea = 0,
    _DobleLinea = 1,
    _Bandera = 2,
    _Bingo = 3,
};

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return width;
}

void imprimirTitulo(string titulo, int width) {
    int textLength = titulo.length();
    if (textLength < width) {
        cout << string((width - textLength) / 2, ' ');
    }
    cout << titulo << endl;
}

int printTop(int width,bool first) {
    string top;
    top.append(string(1, 201));
    for (int i = 0; i < Carton::Columnas; i++) {
        top.append(string(1, 205));
        top.append(string(1, 205));
        top.append(string(1, 203));
    }
    //he optado por esto en vez de poner un condicionador en cada bucle
    top.pop_back();
    top.append(string(1, 187));
    int separationCarton = (width - top.length() * 2 - Separacion) / 2;
    if(first)
        cout << string(separationCarton, ' ');
    cout << top;
    return separationCarton;
}

void imprimirNumeros(const vector<pair<int, bool>>& row) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 15);
    for (auto col : row) {
        cout << (string(1, 186));
        if (col.second)
            SetConsoleTextAttribute(h, 11);
        if (col.first < 10)
            cout << (to_string(0));
        cout << (to_string(col.first));
        SetConsoleTextAttribute(h, 15);
    }
    cout << (string(1, 186));
}

void imprimirMid() {
    string mid;
    mid.append(string(1, 204));
    for (int i = 0; i < Carton::Columnas; i++) {
        mid.append(string(1, 205));
        mid.append(string(1, 205));
        mid.append(string(1, 206));
    }
    mid.pop_back();
    mid.append(string(1, 185));
    cout << mid;
}

void imprimirBot() {
    string mid;
    mid.append(string(1, 200));
    for (int i = 0; i < Carton::Columnas; i++) {
        mid.append(string(1, 205));
        mid.append(string(1, 205));
        mid.append(string(1, 202));
    }
    mid.pop_back();
    mid.append(string(1, 188));
    cout << mid;
}

void imprimirCartones() {
    int width = getConsoleWidth();
    string tituloCartones = "----- Cartones -----";
    imprimirTitulo(tituloCartones, width);
    //Empezamos a printear los cartones
    for (int i = 0; i <= NumeroCartones / 2; i+=2) {
        //Top
        vector<vector<pair<int, bool>>> carL = _cartones[i].getCarton(), carR = _cartones[i+1].getCarton();
        int separationCarton = printTop(width, true);
        cout << string(20, ' ');
        printTop(width, false);
        cout << endl;
        int count = 0;
        for (auto row : carL) {
            //Number
            string mid;
            cout << string(separationCarton, ' ');
            imprimirNumeros(row);
            cout << string(20, ' ');
            imprimirNumeros(carR[count]);
            cout << endl;
            count++;
            //mid
            cout << string(separationCarton, ' ');
            if (count < Carton::Filas) {
                imprimirMid();
                cout << string(20, ' ');
                imprimirMid();
                cout << endl;
            }
            else {
                imprimirBot();
                cout << string(20, ' ');
                imprimirBot();
                cout << endl;
            }
        }
    }

}

void imprimirBolasExtraidas() {
    int width = getConsoleWidth();
    string tituloBolasExtraidas = "----- Bolas Extraidas -----";
    imprimirTitulo(tituloBolasExtraidas, width);
    string bolasExtraidas;
    for (int bolas : _bombo.getBolasExtraidas()) {
        int textLength = bolasExtraidas.length();
        if (textLength > (width-Separacion)) {
            bolasExtraidas.erase(textLength - 3, 3);
            cout << string((width - textLength) / 2, ' ');
            cout << bolasExtraidas << endl;
            bolasExtraidas = "";
        }
        if (bolas < 10)
            bolasExtraidas.append(to_string(0));
        bolasExtraidas.append(to_string(bolas));
        bolasExtraidas.append(" - ");
    }
    int textLength = bolasExtraidas.length();
    bolasExtraidas.erase(textLength - 3, 3);
    cout << string((width - textLength) / 2, ' ');
    cout << bolasExtraidas << endl;
    //Esta separacion es simplemente estetica
    cout << endl;
}
/// <summary>
/// Revisa los premios de los cartones y si estos necesitan excluir alguno
/// </summary>
void verificarPremios(int idCarton) {
    bool* premiosGanados;
    premiosGanados = _cartones[idCarton].getPremiosCarton(_premios);
    int premiosCarton[Premio::TotalPremios]{ 0 };
    for (int i = 0; i < Premio::TotalPremios; i++)
    {
        if (premiosGanados[i]) {
            premiosCarton[i] = true;
            string  exs(_premios[i]._exlusiones);
            if (exs != "") {
                stringstream ex(exs);
                string idEx;
                while (getline(ex, idEx, ',')) {
                    premiosCarton[stoi(idEx)] = false;
                }
            }
        }
    }
    for (int i = 0; i < Premio::TotalPremios; i++)
        _totalPremios[i] += premiosCarton[i];
}

void imprimirPremios() {
    int width = getConsoleWidth();
    string tituloBolasExtraidas = "----- Premios -----";
    imprimirTitulo(tituloBolasExtraidas, width);
    memset(_totalPremios,0, sizeof(_totalPremios));
    for (int i = 0; i < NumeroCartones; i++)
    {
        verificarPremios(i);
    }
    int count = 0,totalPrize=0, lastLenght=0;
    string prize;

    for (int i = 0; i < Premio::TotalPremios; i++)
    {
        prize = "";
        if (_totalPremios[i] > 0) {
            cout << endl;
            prize.append(to_string(_totalPremios[i]));
            prize.append("x ");
            prize.append(_premios[i]._name);
            prize.append(" - ");
            int prizeValue = _premios[i]._value * _totalPremios[i];
            totalPrize += prizeValue;
            prize.append(to_string(prizeValue));
            //El simbolo de € no se encuentra dentro de los caracteres ASCII asi que no lo podre poner por consola
            prize.append("EUR");
            cout << string(Separacion, ' ');
            cout << prize;
            if (count == 0) {
                cout << string((width/2 - prize.length() - Separacion), ' ');
                cout << string(width / 4, ' ');
                cout << "Total";
            }
            lastLenght = prize.length();
            count++;
        }
    }
    if (count == 0) {
        cout << string((width*3 / 4), ' ');
        cout << "Total"<<endl;
    }
    if (count == 1) {
        cout << endl;
    }
    int value = (lastLenght != 0 && count!= 1) ? lastLenght + Separacion : 0;
    cout << string((width / 2 - value), ' ');
    cout << string(width / 4 , ' ');

    cout << totalPrize <<"EUR" << endl;


}
/// <summary>
/// Comprueba que si el numero se encuentra en cada uno de los cartones
/// </summary>
void comprobarNumeros(int num) {
    for (int i = 0; i < NumeroCartones;i++) {
        _cartones[i].marcarBola(num);
    }
}
/// <summary>
/// Llama a todas las funciones para hacer todo el print
/// </summary>
void pantalla() {
    imprimirCartones();
    imprimirBolasExtraidas();
    imprimirPremios();
}
/// <summary>
/// Usado para testear y formzar premios
/// </summary>
Carton forzar(int id) {
    vector<int> numeros1 = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
    Carton Car(numeros1);
    switch (id)
    {
    case _Linea:
        Car.marcarBola(1);
        Car.marcarBola(4);
        Car.marcarBola(7);
        Car.marcarBola(10);
        Car.marcarBola(13);
        break;
    case _DobleLinea:
        Car.marcarBola(1);
        Car.marcarBola(4);
        Car.marcarBola(7);
        Car.marcarBola(10);
        Car.marcarBola(13);
        Car.marcarBola(2);
        Car.marcarBola(5);
        Car.marcarBola(8);
        Car.marcarBola(11);
        Car.marcarBola(14);
        break;
    case _Bandera:
        Car.marcarBola(1);
        Car.marcarBola(3);
        Car.marcarBola(7);
        Car.marcarBola(5);
        Car.marcarBola(9);
        Car.marcarBola(11);
        Car.marcarBola(13);
        Car.marcarBola(15);
        break;
    case _Bingo:
        Car.marcarBola(1);
        Car.marcarBola(4);
        Car.marcarBola(7);
        Car.marcarBola(10);
        Car.marcarBola(13);
        Car.marcarBola(2);
        Car.marcarBola(5);
        Car.marcarBola(8);
        Car.marcarBola(11);
        Car.marcarBola(14);
        Car.marcarBola(3);
        Car.marcarBola(6);
        Car.marcarBola(9);
        Car.marcarBola(12);
        Car.marcarBola(15);
        break;
    default:
        break;
    }
    return Car;
}
/// <summary>
/// Lee el XML y se lo pasa a la clase premios para poder comprobarlos
/// </summary>
void leerPremiosXml() {
    xml_document doc;
    xml_parse_result result = doc.load_file("premios.xml");

    if (!result) {
        cerr << "Error al cargar el archivo" << result.description() << std::endl;
        return;
    }
    xml_node root = doc.child("Premios");
    for (xml_node premio_node : root.children("Premio")) {
        vector<vector<bool>> patron = {};
        premio_node.attribute("patron").value();
        const int cond = stoi(premio_node.attribute("condicionesFila").value());
        string exs = premio_node.attribute("patron").value();
        if (cond == -1) {
            stringstream ex(exs);
            string fs,pos;
            while (getline(ex, fs, '/')) {
                vector<bool> fila;
                stringstream f(fs);
                while (getline(f, pos, ','))
                    fila.push_back(pos == "0"?0:1);
                patron.push_back(fila);
            }
        }
        Premio premio(
        stoi(premio_node.attribute("id").value()),
        premio_node.attribute("nombre").value(),
        stoi(premio_node.attribute("valor").value()),
        premio_node.attribute("exclusiones").value(),
        stoi(premio_node.attribute("condicionesFila").value()),
        patron
        );
        _premios.push_back(premio);
    }
}
/// <summary>
/// Inizializa los valores para que se pueda jugar una partida desde 0
/// </summary>
void inicializarValores() {
    _bombo = Bombo();
    for (int carton = 0; carton < NumeroCartones; carton++) {
        vector<int> numeros;
        for (int numCar = 0; numCar < (Carton::Filas * Carton::Columnas); numCar++) {
            int bolaExtraida = _bombo.extraerBola();
            numeros.push_back(bolaExtraida);
        }
        sort(numeros.begin(), numeros.end());
        _cartones[carton] = Carton(numeros);
    }
    _bombo = Bombo();
}

void pedirTecla() {
    string peticion = "Presiona cualquier tecla para jugar";
    imprimirTitulo(peticion, getConsoleWidth());
    while (!_kbhit()) {
    }
    //Limpio la entrada de teclas para que no se quede permanente mente en bucle
    while (_kbhit()) {
        _getch();
    }
}

int main()
{
    ifstream archivo("premios.xml");
    if (!archivo.good()) {
        cout << "premios.xml no encontrado,copialo en la misma carpeta del ejecutable" << endl;
        system("pause");
    }
    leerPremiosXml();
    pedirTecla();
    bool restart = false;
    do {
        restart = false;
        inicializarValores();
        //Testing/////////////////////////
        //_cartones[3] = forzar(_Linea);
        for (int i = 0; i < BolasASacar; i++) {
            system("CLS");
            comprobarNumeros(_bombo.extraerBola());
            pantalla();
            //Esperamos 150ms para sacar la siguiente bola
            this_thread::sleep_for(chrono::milliseconds(150));
        }
        pedirTecla();
        restart = true;
    } while (restart);
    
}