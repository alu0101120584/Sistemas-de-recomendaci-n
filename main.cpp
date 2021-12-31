#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <stdio.h>


using namespace std;

vector<string> dividirEspacios(string text);
bool IsParenthesesOrDash(char c);
int cuenta(vector<string> vec, string str);
double idf(int n, int df);
vector<pair<string, int>> cuentaTotal(vector<vector<string>> solucion, vector<string> palabras);

int main(int argc, char *argv[]) {
  string filename(argv[1]);
  string filename2(argv[2]);
  vector<string> lines;
  vector<vector<string>> solucion;
  string line;
  vector<vector<vector<string>>> ocurrenciaPalabras;
  ifstream input_file(filename);
  if (!input_file.is_open()) {
      cerr << "Could not open the file - '"
              << filename << "'" << endl;
      return EXIT_FAILURE;
  }

  // Se lee el documento de entrada y se divide cada línea por palabras
  while (getline(input_file, line)){
        lines.push_back(line);
    }
  for (size_t i = 0; i < lines.size(); i++) {
      solucion.push_back(dividirEspacios(lines[i]));
  }

  // Mediante este buclse se eliminan los puntos y las comas de los valores almacenados
  for (int i = 0; i < solucion.size(); i++) {
    for (int j = 0; j < solucion[i].size(); j++) {
      solucion[i][j].erase(remove_if(solucion[i][j].begin(), solucion[i][j].end(), &IsParenthesesOrDash), solucion[i][j].end());
    }
  }

  for (int i = 0; i < solucion.size(); i++) {
    int aux;
    vector<vector<string>> aux2;
    for (int j = 0; j < solucion[i].size(); j++) {
      if (solucion[i][j] !=  " ") {
        aux = cuenta(solucion[i], solucion[i][j]);
        vector<string> aux3;
        aux3.push_back(solucion[i][j]);
        aux3.push_back(to_string(aux));
        aux2.push_back(aux3);
      }
    }
    ocurrenciaPalabras.push_back(aux2);
  }

  // Vector que almacena todo el texto de todos los documentos juntos
  vector<string> texto;
  for (int i = 0; i < solucion.size()-1; i++) {
    texto.reserve( solucion[i].size() + solucion[i+1].size() );       
    texto.insert( texto.end(), solucion[i].begin(), solucion[i].end() );
    texto.insert( texto.end(), solucion[i+1].begin(), solucion[i+1].end() );
  }
  
  auto last = std::unique(texto.begin(), texto.end());
  texto.resize(std::distance(texto.begin(), last));

  // Par que almacena el IDF de cada palabra de toda la colección de documentos
  vector<pair<string, string>> resultado;
  for (int i = 0; i < cuentaTotal(solucion, texto).size(); i++ ) {
    pair<string, string> aux;
    aux.first = texto[i];
    aux.second = to_string(idf(ocurrenciaPalabras.size(), cuentaTotal(solucion, texto)[i].second));
    resultado.push_back(aux);
  }
  
  for (int k = 0; k < resultado.size(); k++) {
    for (int i = 0; i < ocurrenciaPalabras.size(); i++) {
      for (int j = 0; j < ocurrenciaPalabras[i].size(); j++) {
        if (ocurrenciaPalabras[i][j][0] == resultado[k].first) {
          ocurrenciaPalabras[i][j].push_back(resultado[k].second);
        }
      }
    }
  }

  for (int i = 0; i < ocurrenciaPalabras.size(); i++) {
    for (int j = 0; j < ocurrenciaPalabras[i].size(); j++) {
      double aux7;
      double oper1 = stod(ocurrenciaPalabras[i][j][1]);
      double oper2 = stod(ocurrenciaPalabras[i][j][2]);

      aux7 = oper1 * oper2;
      string str1 = to_string(aux7);
      ocurrenciaPalabras[i][j].push_back(str1);
    }
  }

  fstream file_out;

  file_out.open(filename2, std::ios_base::out);
  if (!file_out.is_open()) {
      cout << "failed to open " << filename2 << '\n';
  } else {
      for (size_t i = 0; i < ocurrenciaPalabras.size(); i++) {
        file_out << "DOCUMENTO " << i+1 << endl;
        file_out << "   Término   TF  IDF        TF-IDF" << endl;
        file_out << "----------------------------------" << endl;
          for (size_t j = 0; j < ocurrenciaPalabras[i].size(); j++) {
              file_out << setw(10) << setfill(' ') << ocurrenciaPalabras[i][j][0] << "   " << ocurrenciaPalabras[i][j][1] << "   " << ocurrenciaPalabras[i][j][2] << "   " << ocurrenciaPalabras[i][j][3] <<endl;
          }
          file_out << endl;
      }   
      cout << "Done Writing!" << endl;
  }

  input_file.close();
  return EXIT_SUCCESS;
}

vector<pair<string, int>> cuentaTotal(vector<vector<string>> solucion, vector<string> texto) {
  vector<int> aux2;
  int contador2 = 0;
  bool flag = false;
  for (int k = 0; k < texto.size(); k++) {
    contador2 = 0;
    for (int i = 0; i < solucion.size(); i++) {
      flag = false;
      for (int j = 0; j < solucion[i].size(); j++) {
        if (texto[k] == solucion[i][j] && flag == false) {
          flag = true;
          contador2++;
        }
      }
    }
    aux2.push_back(contador2);
  }

  vector<pair<string, int>> unir;
  for (int i = 0; i<aux2.size(); i++) {
    pair<string,int> auxi;
    auxi.first = texto[i];
    auxi.second = aux2[i];
    unir.push_back(auxi);
  }
  return unir;
}

double idf(int n, int df) {
  double param = n / df;
  return log10(param);
}

int cuenta(vector<string> vec, string str) {
  int x = 0;
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == str) {
      x++;
    }
  }
  return x;
}

bool IsParenthesesOrDash(char c) {
    switch(c)
    {
    case '(':
    case ')':
    case ',':
    case '.':
    case '-':
        return true;
    default:
        return false;
    }
}

vector<string> dividirEspacios(string text) {
    string delimiter = " ";
    vector<string> words;

    int start = 0;
    int end = text.find(delimiter);
    while (end != -1) {
        words.push_back(text.substr(start, end - start));
        start = end + delimiter.size();
        end = text.find(delimiter, start);
    }
    words.push_back(text.substr(start, end - start));
    
    return words;
}
