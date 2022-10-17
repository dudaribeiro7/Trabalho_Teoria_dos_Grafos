#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stack>
#include <vector>
#include <algorithm>
#include <math.h>

#include "No.h"

using namespace std;

class Grafo
{
    private:    
        No **nos_grafo;         // Vetor de ponteiros do tipo No (lista de adjacência)
        int n_vertices;         // Número de vertices do grafo
        bool direcionado;       // Variável que determina se o grafo é direcionado ou não
        bool ponderadoArestas;  // Variável que determina se o grafo é ponderado nas arestas ou não
        bool ponderadoNos;      // Variável que determina se o grafo é ponderado nos vértices ou não
        vector<int> L;          // Limite inferior para o Problema de Clusterização Capacitada
        vector<int> U;          // Limite superior para o Problema de Clusterização Capacitada
        int n_clusters;         // Número de clusters para o Problema de Clusterização Capacitada
        float ccapacity;        // Capacidade dos Clusters para o Problema de Clusterização Capacitada
        float seed;             // Semente de Randomização
        
        void leArquivo(string nomeArquivo); 
        void leArquivo2(string nomeArquivo);
        void leArquivo3(string nomeArquivo);
        Grafo* subgrafoVerticeInduzido(vector<int> X);
        Grafo* subgrafoArestaInduzido(vector<Aresta*> Y);
        No* getNoInVector(No** _nos_grafo, int id);
        void cP(int id, bool v[], vector<No*> *vetor, int aux);
        void fechoaux(int id, bool v[], vector<No*> *vetor);
        bool verificaSolucao(vector<Aresta*> *solucao);
        int randomRange(int min, int max);
        float escolheAlfa(float probabilidade[], int tamanho);
        void AtualizaProbabilidade(float probabilidade[], float coeficientes[], int tamanho);
        void atualizaCoeficientes(float coeficientes[], float melhorSolucao[], float mediaSolucoes[], int tamanho);


    public:
        Grafo(string nomeArquivo, int direc, int peso_aresta, int peso_nos);
        Grafo(No **_nos_grafo, int _n_vertices, bool _direc, bool _pesoAresta, bool _pesoNos);
        ~Grafo();
        void printGrafo();
        int getNumVertices();
        No** getNosGrafo();
        bool isDirecionado();
        bool isPonderadoArestas();
        bool isPonderadoNos();
        Aresta* arestaMenorPeso();
        vector<No*> fechoTransDir(int id);
        vector<No*> fechoTransInd (int id);
        float coeficienteAgrupamentoLocal(int id);
        float coeficienteAgrupamentoMedio();
        void dijkstra(int inicio, int destino, string arquivo_saida);
        void floyd(int id1, int id2, string arquivo_saida);
        void prim(vector<int> X, string arquivo_saida);
        void kruskal(vector<int> X, string arquivo_saida);
        vector<No*> caminhamentoProfundidade(int id, string arquivo_saida);
        float algoritmoGuloso();
        float algoritmoGulosoRandomizadoAdaptativo(float alfa, int n_iter);
        float algoritmoGulosoRandomizadoReativo(vector<float> alfaVec, int n_iter, int bloco, string arquivo_saida);
        float getSeed();
};

#endif