#ifndef TENSAOTENSAO
#define TENSAOTENSAO
/**
 * Modelo basico componentes de fontes controladas
 */
#include "fontescontroladas.cpp"

/* Necessario para nao precisar escrever std:: */
using namespace std;

class TensaoTensao : public FontesControladas
{
    public:
        /**
         * Construtor
         */
        TensaoTensao(string n, int a, int b, int c, int d, double g) : FontesControladas(n, a, b, c, d, g)
        {
        }
};

#endif