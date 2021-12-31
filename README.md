# Sistemas de recomendación. Modelos Basados en el Contenido
 Autor: Juan Eduardo Reyes Pérez
 Fecha: 30-12-2021

 ## Objetivos

 El objetivo de esta práctica es implementar un sistema de recomendación siguiendo el modelo basados en el contenido.

 ## Requisitos

- Estudiar los modelos basados en el contenido. En las diapositivas de clase hay ejemplos sobre este campo.
- Crear un software que reciba por línea de comandos un arrchivo de texto plano con extensión .txt. Cada documento viene representado en una línea del archivo. 
- El software debe proporcionar como salida lo siguiente:
  Para cada documento, tabla con las siguientes columnas:
  - Índice del término.
  - Término.
  - TF.
  - IDF.
  - TF-IDF.
  - Similaridad coseno entre cada par de documentos.

## Ejecución del programa

El programa se compilará utilizando: `g++ main.cpp -o main`
El programa se ejecutará utilizando: `./main {nombre fichero de entrada} {nombre fichero de salida}`

## Explicación del código fuente

En primer lugar el programa se encarga de leer el fichero de texto que contiene la información y separa cada línea de este archivo, posteriormente se pasa a eliminar los espacios de cada línea y se divide cada línea en un vector de strings.

```C++
  while (getline(input_file, line)){
        lines.push_back(line);
    }
  for (size_t i = 0; i < lines.size(); i++) {
      solucion.push_back(dividirEspacios(lines[i]));
  }
```
Función que se encarga de eliminar los espacios:
```C++
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
```

El siguiente paso que se realiza es el cálculo de TF de cada palabra de los documentos. Para ello se utilizan unos bucles for para recorrer los vectores donde están almacenadas las palabras y una función que cuenta el valor de TF.
```C++
  for (int i = 0; i < solucion.size(); i++) {
    double aux;
    vector<vector<string>> aux2;
    for (int j = 0; j < solucion[i].size(); j++) {
      if (solucion[i][j] !=  " ") {
        aux = tf(solucion[i], solucion[i][j]);
        vector<string> aux3;
        aux3.push_back(solucion[i][j]);
        aux3.push_back(to_string(aux));
        aux2.push_back(aux3);
      }
    }
    ocurrenciaPalabras.push_back(aux2);
  }
}
```
Función que calcula el TF
```C++
double tf(vector<string> vec, string str) {
  int x = 0;
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == str) {
      x++;
    }
  }
  return x;
}
```

El siguiente paso es calcular el IDF de cada palabra, para ello se utiliza un bucle for para recorrer el vector y dos funciones, una que se encarga de calcular en cuantos documentos aparece la palabra indicada y otra que calcula el IDF.

```C++
vector<pair<string, string>> resultado;
  for (int i = 0; i < cuentaTotal(solucion, texto).size(); i++ ) {
    pair<string, string> aux;
    aux.first = texto[i];
    aux.second = to_string(idf(ocurrenciaPalabras.size(), cuentaTotal(solucion, texto)[i].second));
    resultado.push_back(aux);
  }
```
Función que calcula el número de veces que aparece cada palabra
```C++
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
```
Función que calcula el IDF
```C++
double idf(int n, int df) {
  double param = n / df;
  return log10(param);
}
```

En el siguiente paso se almacenan en el mismo vector el término, IDF y TF

```C++
for (int k = 0; k < resultado.size(); k++) {
    for (int i = 0; i < ocurrenciaPalabras.size(); i++) {
      for (int j = 0; j < ocurrenciaPalabras[i].size(); j++) {
        if (ocurrenciaPalabras[i][j][0] == resultado[k].first) {
          ocurrenciaPalabras[i][j].push_back(resultado[k].second);
        }
      }
    }
  }
```

Y por último se calcula el TF-IDF

```C++
for (int i = 0; i < ocurrenciaPalabras.size(); i++) {
    for (int j = 0; j < ocurrenciaPalabras[i].size(); j++) {
      ocurrenciaPalabras[i][j].push_back(to_string(tfidf(stod(ocurrenciaPalabras[i][j][1]), stod(ocurrenciaPalabras[i][j][2]))));
    }
  }
```
Función que calcula TF-IDF
```C++
double tfidf(double tf, double idf) {
  return tf*idf;
}
```