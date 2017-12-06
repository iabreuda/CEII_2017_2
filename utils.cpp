#ifndef UTILS
#define UTILS
/**
 * Inclui:
 *  - istringstream
 */
#include <sstream>
 /**
 * Inclui:
 *  - vector
 */
#include <vector>
/**
 * Tratamento de excecoes
 */
#include <stdexcept>
#include <math.h>
#include <algorithm>

/* Necessario para nao precisar escrever std:: */
using namespace std;

/**
 * Transformar uma string em elementos de um array utilizando um caracter delimitador
 * Ex. explode("Hello World", ' ')
 * Retorna:
 *     vector[0] = "Hello"
 *     vector[1] = "World"
 */
const vector<string> explode(const string& phrase, char delim)
{
    vector<string> result;
    istringstream iss(phrase);

    for (string token; getline(iss, token, delim); )
    {
        result.push_back(move(token));
    }

    return result;
}

/**
 * Retorna as tensoes nodais quando voce passa a matriz de condutancia
 * e a matriz de corrente, aplica a eliminacao de gauss para transformar a matriz
 * de condutividade em uma matriz identidade.
 */
vector<double> gauss(vector<vector<double> > condutancia, vector<double> correntes, int nos)
{
    int condutanciaRows = condutancia.size();
    int correnteRows = correntes.size();

    /**
     * Matrizes de condutancia e conrrente devem ter os
     * mesmos numeros de linhas
     */
    if (condutanciaRows != correnteRows) {
        throw invalid_argument("Matrizes de condutancia e corrente nao tem o msm tamanho");
    }
    /*
    cout << "Antes" << endl;
    cout << "Condutancia" << endl;
    for (int i = 1; i < condutanciaRows; i++) {
        for (int j = 1; j < condutanciaRows; j++)
        {
            cout << condutancia[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Correntes" << endl;
    for (int i = 1; i < condutanciaRows; i++) {
        cout << correntes[i] << endl;
    }*/
    /**
     * Analisa cada linha da matriz de conduntancias
     */
    for(int row = 1; row < condutanciaRows; row++) {
        double maxValue = 0;
        int maxIndex = 0;
        /**
         * Evita divisao por 0
         */
         /*
        cout << "Condutancia Antes" << endl;
        for (int i = 1; i < condutanciaRows; i++) {
            for (int j = 1; j < condutanciaRows; j++)
            {
                cout << condutancia[i][j] << " ";
            }
            cout << endl;
        }*/

        for (int linha = 1; linha < condutanciaRows; linha++) {
            if (fabs(condutancia[linha][row]) > maxValue && linha >= row) {
                maxValue = fabs(condutancia[linha][row]);
                maxIndex = linha;
            }
        }

        swap(condutancia[row], condutancia[maxIndex]);
        swap(correntes[row], correntes[maxIndex]);

        /*
        cout << "Condutancia Depois" << endl;
        for (int i = 1; i < condutanciaRows; i++) {
            for (int j = 1; j < condutanciaRows; j++)
            {
                cout << condutancia[i][j] << " ";
            }
            cout << endl;
        }*/


        double pivot = condutancia[row][row];
        if (pivot == 0) {
            throw invalid_argument("Matrizes Singular");
        }

        int column = condutancia[row].size();
        if (condutanciaRows != column) {
            throw invalid_argument("Matrizes de condutancia deve ser quadrada");
        }

        correntes[row] /= pivot;
        for(int col = 1; col < column; col++) {
            condutancia[row][col] /= pivot;
        }

        for(int r = 1; r < condutanciaRows; r++) {
            if (r != row) {
                double fator = condutancia[r][row];
                correntes[r] -= correntes[row] * fator;
                for(int c = 1; c < column; c++) {
                    condutancia[r][c] -= condutancia[row][c] * fator;
                }
            }
        }
    }
    /*
    cout << "tensoes nodais" << endl;
    for (int k = 1; k < 3; k++)
    {
        cout << correntes[k] << endl;
    }*/
    return correntes;
}

bool comparar(vector<double> vetor1, vector<double> vetor2)
{
    int nosIguais = 0;
    double limite = 10e-5;
    int sizeOne = vetor1.size();
    int sizeTwo = vetor2.size();

    if (sizeOne != sizeTwo) {
        throw invalid_argument("Matrizes de condutancia e corrente nao tem o msm tamanho");
    }

    for(int i = 0; i < sizeOne; i++) {
        double lLimit = fabs(vetor1[i]) - (fabs(vetor1[i]) * limite);
        double uLimit = fabs(vetor1[i]) + (fabs(vetor1[i]) * limite);
        if (fabs(vetor2[i]) <= uLimit && fabs(vetor2[i]) >= lLimit) {
            nosIguais++;
        } else {
            /*cout << "no diferente: " << vetor2[i] << " : vetor 1 " << vetor1[i] << endl;*/
        }
    }
    /*
    cout << "Nos iguaos" << endl;
    cout << nosIguais << endl;
    cout << sizeOne << endl;
    cout << sizeTwo << endl;
    cout << "-------------" << endl;*/
    if (nosIguais == sizeOne) {
        return true;
    }
    return false;
}


bool is_not_digit(char c)
{
    return ! isdigit(c);
}

bool numeric_string_compare(const string& s1, const string& s2)
{
    string::const_iterator it1 = s1.begin(), it2 = s2.begin();

    if (isdigit(s1[0]) && isdigit(s2[0])) {
        int n1, n2;
        stringstream ss(s1);
        ss >> n1;
        ss.clear();
        ss.str(s2);
        ss >> n2;

        if (n1 != n2) return n1 < n2;

        it1 = find_if(s1.begin(), s1.end(), is_not_digit);
        it2 = find_if(s2.begin(), s2.end(), is_not_digit);
    }

    return lexicographical_compare(it1, s1.end(), it2, s2.end());
}
#endif