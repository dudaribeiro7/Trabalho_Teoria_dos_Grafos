#include <iostream>
#include <stdio.h>

using namespace std;

#include "Grafo.h"

#define INPUT_POND_DIR "./input/ponderados/"
#define INPUT_NAO_POND_DIR "./input/nao_ponderados/"

#define HANDOVER_DIR "./input/Handover/"
#define RANREAL240_DIR "./input/RanReal240/"
#define RANREAL480_DIR "./input/RanReal480/"
#define RANREAL960_DIR "./input/RanReal960/"
#define SPARSE82_DIR "./input/Sparse82/"

#define HANDOVER_OUTPUT_DIR "./output/Handover_out/"
#define RANREAL240_OUTPUT_DIR "./output/RanReal240_out/"
#define RANREAL480_OUTPUT_DIR "./output/RanReal480_out/"
#define RANREAL960_OUTPUT_DIR "./output/RanReal960_out/"
#define SPARSE82_OUTPUT_DIR "./output/Sparse82_out/"


void funcionalidades(char opc, Grafo *grafo, string arquivo_saida);
void funcionalidades2(int opc, Grafo *grafo, string arquivo_saida);

void print_help(char *argv)
{
    std::cout << "Uso:" << endl << argv << " <arquivo_entrada> <arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos>" << endl;
    std::cout << "Exemplo: " << argv << " grafo_1000_1.txt grafo_saida.txt 1 0 0" << endl;
    std::cout << endl << "OU" << endl << endl;
    std::cout << "Uso:" << endl << argv << " <arquivo_entrada> <arquivo_saida> <Tipo_Instancia>" << endl;
    std::cout << "Sendo que o <Tipo_Instancia> deve ser: Handover, RanReal240, RanReal480, RanReal960 ou Sparce82" << endl;
    std::cout << "Exemplo: " << argv << " RanReal240_07.txt grafo_saida.txt RanReal240" << endl;
    
    exit(0);
}

void print_menu(Grafo *grafo, string arquivo_saida)
{
    std::cout << "------------------------------ MENU ------------------------------" << endl
         << endl;

    std::cout << "a) Parâmetro: um ID de um vértice de um grafo direcionado " << endl;
    std::cout << "   Saída: o fecho transitivo direto deste vértice." << endl
         << endl;

    std::cout << "b) Parâmetro: um ID de um vértice de um grafo direcionado." << endl;
    std::cout << "   Saída: o fecho transitivo indireto deste vértice." << endl
         << endl;

    std::cout << "c) Parâmetro: o ID de um vértice do grafo" << endl;
    std::cout << "   Saída: o coeficiente de agrupamento local do vértice" << endl
         << endl;

    std::cout << "d) Parâmetro: um grafo." << endl;
    std::cout << "   Saída: o coeficiente de agrupamento médio do grafo." << endl
         << endl;

    std::cout << "e) Parâmetro: dois IDs de vértices do grafo." << endl;
    std::cout << "   Saída: o caminho mínimo entre estes dois vértices usando algoritmo de Djkstra." << endl
         << endl;

    std::cout << "f) Parâmetro: dois IDs de vértices do grafo." << endl;
    std::cout << "   Saída: o caminho mínimo entre estes dois vértices usando algoritmo de Floyd." << endl
         << endl;

    std::cout << "g) Parâmetro: um subconjunto X de vértices de um grafo." << endl;
    std::cout << "   Saída: uma Árvore Geradora Mínima sobre o subgrafo vértice-induzido por X usando o algoritmo de Prim." << endl
         << endl;

    std::cout << "h) Parâmetro: um subconjunto X de vértices do grafo." << endl;
    std::cout << "   Saída: uma Árvore Geradora Mínima sobre o subgrafo vértice-induzido por X usando o algoritmo de Kruskal." << endl
         << endl;

    std::cout << "i) Parâmetro: um ID de vértice." << endl;
    std::cout << "   Saída: a árvore dada pela ordem de caminhamento em profundidade a partir de nó dado parâmetro, destacando as arestas de retorno." << endl
         << endl;

    std::cout << "Digite a opção da funcionalidade que deseja testar: ";
    char opc;
    std::cin >> opc;
    funcionalidades(opc, grafo, arquivo_saida);
}

void print_menu2(Grafo *grafo, string arquivo_saida)
{
    std::cout << "------------------------------ MENU ------------------------------" << endl
         << endl;

    std::cout << "1) Algoritmo Consrutivo Guloso" << endl;
    std::cout << "   Saída: Qualidade, tempo de processamento, semente de randomização e se foi possível encontrar solução viável ou não." << endl
         << endl;

    std::cout << "2) Algoritmo Construtivo Guloso Randomizado e Adaptativo" << endl;
    std::cout << "   Saída: Qualidade, tempo de processamento, semente de randomização e se foi possível encontrar solução viável ou não." << endl
         << endl;

    std::cout << "3) Algoritmo Guloso Randomizado e Reativo" << endl;
    std::cout << "   Saída: Qualidade, tempo de processamento, semente de randomização e se foi possível encontrar solução viável ou não." << endl
         << endl;

    
    std::cout << "Digite a opção da funcionalidade que deseja testar: ";
    int opc;
    std::cin >> opc;
    funcionalidades2(opc, grafo, arquivo_saida);
}

void testarNovamente(char s_n, Grafo *grafo, string arquivo_saida)
{
    switch (s_n)
    {
    case 's':
        print_menu(grafo, arquivo_saida);
        break;

    case 'n':
        exit(0);
        break;

    default:
        std::cout << "Opção inválida. Tente novamente, digitando uma opção válida (s ou n): ";
        std::cin >> s_n;
        testarNovamente(s_n, grafo, arquivo_saida);
        break;
    }
}

void testarNovamente2(char s_n, Grafo *grafo, string arquivo_saida)
{
    switch (s_n)
    {
    case 's':
        print_menu2(grafo, arquivo_saida);
        break;

    case 'n':
        exit(0);
        break;

    default:
        std::cout << "Opção inválida. Tente novamente, digitando uma opção válida (s ou n): ";
        std::cin >> s_n;
        testarNovamente2(s_n, grafo, arquivo_saida);
        break;
    }
}

bool searchInVector(vector<int> vet, int id)
{
    for (int i = 0; i < vet.size(); i++)
        if (vet[i] == id)
            return true;
    return false;
}

void funcionalidades(char opc, Grafo *grafo, string arquivo_saida)
{
    int id, id1, id2, n;
    float coefA_local, coefA_medio;
    vector<int> X;

    ofstream arq_out;
    arq_out.open(arquivo_saida, ios::app);
    if(!arq_out.is_open())
    {
        std::cout << "FALHA AO ABRIR ARQUIVO DE SAIDA" << endl;
        exit(0);
    }

    switch (opc)
    {
    case 'a':
        if (grafo->isDirecionado())
        {
            std::cout << "Digite o ID de um vértice do grafo, entre 0 e " << grafo->getNumVertices() - 1 << ": ";
            std::cin >> id;
            while(id >= grafo->getNumVertices() || id < 0)
            {
                std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                std::cin >> id;
            }
            vector<No *> fechoT_direto = grafo->fechoTransDir(id);

            std::cout << "f+(" << id << ") = {";
            for(int i = 0; i < fechoT_direto.size(); i++)
            {
                std::cout << fechoT_direto[i]->getId();
                if(i+1 < fechoT_direto.size())
                    std::cout << ", ";
            }
            std::cout << "}" << endl;

            arq_out << "Funcionalidade a) Fecho Transitivo Direto:" << endl;
            arq_out << "f+(" << id << ") = {";
            for(int i = 0; i < fechoT_direto.size(); i++)
            {
                arq_out << fechoT_direto[i]->getId();
                if(i+1 < fechoT_direto.size())
                    arq_out << ", ";
            }
            arq_out << "}" << endl << endl;

        }
        else
        {
            std::cout << "Esse grafo não é direcionado. Logo, não é possível realizar esta funcionalidade." << endl;
            arq_out << "Funcionalidade a) Fecho Transitivo Direto:" << endl;
            arq_out << "Esse grafo não é direcionado. Logo, não é possível realizar esta funcionalidade." << endl << endl;
        }

        break;

    case 'b':
        if (grafo->isDirecionado())
        {
            std::cout << "Digite o ID de um vértice do grafo, entre 0 e " << grafo->getNumVertices() - 1 << ": ";
            std::cin >> id;
            while(id >= grafo->getNumVertices() || id < 0)
            {
                std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                std::cin >> id;
            }
            vector<No *> fechoT_indireto = grafo->fechoTransInd(id);
            std::cout << "f-(" << id << ") = {";
            for(int i = 0; i < fechoT_indireto.size(); i++)
            {
                std::cout << fechoT_indireto[i]->getId();
                if(i+1 < fechoT_indireto.size())
                    std::cout << ", ";
            }
            std::cout << "}" << endl;

            arq_out << "Funcionalidade b) Fecho Transitivo Indireto:" << endl;
            arq_out << "f+(" << id << ") = {";
            for(int i = 0; i < fechoT_indireto.size(); i++)
            {
                arq_out << fechoT_indireto[i]->getId();
                if(i+1 < fechoT_indireto.size())
                    arq_out << ", ";
            }
            arq_out << "}" << endl << endl;
        }
        else
        {
            std::cout << "Esse grafo não é direcionado. Logo, não é possível realizar esta funcionalidade." << endl;
            arq_out << "Funcionalidade b) Fecho Transitivo Indireto:" << endl;
            arq_out << "Esse grafo não é direcionado. Logo, não é possível realizar esta funcionalidade." << endl << endl;
        }

        break;

    case 'c':
        std::cout << "Digite o ID de um vértice do grafo, entre 0 e " << grafo->getNumVertices() - 1 << ": ";
        std::cin >> id;
        while(id >= grafo->getNumVertices() || id < 0)
        {
            std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
            std::cin >> id;
        }
        coefA_local = grafo->coeficienteAgrupamentoLocal(id);
        std::cout << "O coeficiente de agrupamento local do nó " << id << " é igual a " << coefA_local << endl;

        arq_out << "Funcionalidade c) Coeficiente de Agrupamento Local:" << endl;
        arq_out << "O coeficiente de agrupamento local do nó " << id << " é igual a " << coefA_local << endl << endl;

        break;

    case 'd':
        coefA_medio = grafo->coeficienteAgrupamentoMedio();
        std::cout << "O coeficiente de agrupamento médio do grafo é igual a " << coefA_medio << endl;

        arq_out << "Funcionalidade d) Coeficiente de Agrupamento Médio:" << endl;
        arq_out << "O coeficiente de agrupamento médio do grafo é igual a " << coefA_local << endl << endl;

        break;

    case 'e':
        std::cout << "Digite o ID de um vértice do grafo, entre 0 e " << grafo->getNumVertices() - 1 << ": ";
        std::cin >> id1;
        while(id1 >= grafo->getNumVertices() || id1 < 0)
        {
            std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
            std::cin >> id1;
        }
        std::cout << "Digite o ID de outro vértice do grafo, entre 0 e " << grafo->getNumVertices() - 1 << ", diferente de " << id1 << ": ";
        std::cin >> id2;
        while(id2 >= grafo->getNumVertices() || id2 < 0)
        {
            std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
            std::cin >> id2;
        }
        while(id2 == id1)
        {
            std::cout << "Esse vértice já foi passado. Por favor, tente novamente com um valor diferente: ";
            std::cin >> id2;
            while(id2 >= grafo->getNumVertices() || id2 < 0)
            {
                std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                std::cin >> id2;
            }
        }
        arq_out << "Funcionalidade e) Caminho Mínimo com algoritmo de Dijkstra:" << endl;
        arq_out << "Caminho entre os vértices " << id1 << " e " << id2 << ":" << endl;
        grafo->dijkstra(id1, id2, arquivo_saida);
        arq_out << endl << endl;;

        break;

    case 'f':
        std::cout << "Digite o ID de um vértice do grafo, entre 0 e " << grafo->getNumVertices() - 1 << ": ";
        std::cin >> id1;
        while(id1 >= grafo->getNumVertices() || id1 < 0)
        {
            std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
            std::cin >> id1;
        }
        std::cout << "Digite o ID de outro vértice do grafo, entre 0 e " << grafo->getNumVertices() - 1 << ", diferente de " << id1 << ": ";
        std::cin >> id2;
        while(id2 >= grafo->getNumVertices() || id2 < 0)
        {
            std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
            std::cin >> id2;
        }
        while(id2 == id1)
        {
            std::cout << "Esse vértice já foi passado. Por favor, tente novamente com um valor diferente: ";
            std::cin >> id2;
            while(id2 >= grafo->getNumVertices() || id2 < 0)
            {
                std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                std::cin >> id2;
            }
        }
        arq_out << "Funcionalidade f) Caminho Mínimo com algoritmo de Floyd:" << endl;
        arq_out << "Caminho entre os vértices " << id1 << " e " << id2 << ":" << endl;
        grafo->floyd(id1, id2, arquivo_saida);
        arq_out << endl << endl;

        break;

    case 'g':
        if (grafo->isDirecionado())
            std::cout << "Esse grafo é direcionado. Logo, não é possível realizar esta funcionalidade." << endl;
        else
        {
            std::cout << "Digite o número de vértices que você deseja que o subconjunto X de vértices tenha: ";
            std::cin >> n;
            while(n > grafo->getNumVertices() || n < 1)
            {
                std::cout << "Valor inválido. Por favor, tente novamente, digitando um valor entre 1 e " << grafo->getNumVertices() << ": ";
                std::cin >> n;
            }
            for (int i = 0; i < n; i++)
            {
                std::cout << "Digite o ID do " << i+1 << "º vértice a ser adicionado no subconjunto X de vértices, entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                std::cin >> id;
                while(id >= grafo->getNumVertices() || id < 0)
                {
                    std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                    std::cin >> id;
                }
                while(searchInVector(X, id))
                {
                    std::cout << "Esse vértice já foi adicionado ao subconjunto X. Por favor, tente novamente: ";
                    std::cin >> id;
                    while(id >= grafo->getNumVertices() || id < 0)
                    {
                        std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                        std::cin >> id;
                    }
                }
                X.push_back(id);
            }
            arq_out << "Funcionalidade g) Árvore Geradora Mínima com algoritmo de Prim:" << endl;
            arq_out << "Subconjunto de vértices: X = {";
            for(int i = 0; i < X.size(); i++)
            {
                arq_out << X[i];
                if(i+1 < X.size())
                    arq_out << ", ";
            }
            arq_out << "}" << endl;
            grafo->prim(X, arquivo_saida);
            arq_out << endl << endl;
        }

        break;

    case 'h':
        if (grafo->isDirecionado())
            std::cout << "Esse grafo é direcionado. Logo, não é possível realizar esta funcionalidade." << endl;
        else
        {
            std::cout << "Digite o número de vértices que você deseja que o subconjunto X de vértices tenha: ";
            std::cin >> n;
            while(n > grafo->getNumVertices() || n < 1)
            {
                std::cout << "Valor inválido. Por favor, tente novamente, digitando um valor entre 1 e " << grafo->getNumVertices() << ": ";
                std::cin >> n;
            }
            for (int i = 0; i < n; i++)
            {
                std::cout << "Digite o ID do " << i+1 << "º vértice a ser adicionado no subconjunto X de vértices, entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                std::cin >> id;
                while(id >= grafo->getNumVertices() || id < 0)
                {
                    std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                    std::cin >> id;
                }
                while(searchInVector(X, id))
                {
                    std::cout << "Esse vértice já foi adicionado ao subconjunto X. Por favor, tente novamente: ";
                    std::cin >> id;
                    while(id >= grafo->getNumVertices() || id < 0)
                    {
                        std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                        std::cin >> id;
                    }
                }
                X.push_back(id);
            }
            arq_out << "Funcionalidade h) Árvore Geradora Mínima com algoritmo de Kruskal:" << endl;
            arq_out << "Subconjunto de vértices: X = {";
            for(int i = 0; i < X.size(); i++)
            {
                arq_out << X[i];
                if(i+1 < X.size())
                    arq_out << ", ";
            }
            arq_out << "}" << endl;
            grafo->kruskal(X, arquivo_saida);
            arq_out << endl << endl;
        }

        break;

    case 'i':
        if (grafo->isDirecionado())
            std::cout << "Esse grafo é direcionado. Logo, não é possível realizar esta funcionalidade." << endl;
        else
        {
            std::cout << "Digite o ID de um vértice do grafo, entre 0 e " << grafo->getNumVertices() - 1 << ": ";
            std::cin >> id;
            while(id >= grafo->getNumVertices() || id < 0)
            {
                std::cout << "Valor de ID inválido. Por favor, tente novamente, digitando um valor entre 0 e " << grafo->getNumVertices() - 1 << ": ";
                std::cin >> id;
            }
            arq_out << "Funcionalidade i) Árvore dada pela ordem de caminhamento em profundidade:" << endl;
            arq_out << "Caminhamento a partir do vértice " << id << endl;
            grafo->caminhamentoProfundidade(id, arquivo_saida);
            arq_out << endl << endl;
        }

        break;

    default:
        std::cout << "Opção inválida. Tente novamente, digitando uma opção válida (a, b, c, d, e, f, g, h ou i): ";
        std::cin >> opc;
        funcionalidades(opc, grafo, arquivo_saida);
        break;
    }

    std::cout << endl
         << "Deseja testar outra funcionalidade? (s/n)" << endl;
    char s_n;
    std::cin >> s_n;
    testarNovamente(s_n, grafo, arquivo_saida);

    arq_out.close();
}

void funcionalidades2(int opc, Grafo *grafo, string arquivo_saida)
{
    float qualidade;
    float qualidadeMedia = 0;
    float tempo;
    float tempoMedio = 0;
    vector<float> alfas = {0.05, 0.1, 0.15, 0.3, 0.5};
    int n_iter;

    ofstream arq_out;
    arq_out.open(arquivo_saida, ios::app);
    if(!arq_out.is_open())
    {
        std::cout << "FALHA AO ABRIR ARQUIVO DE SAIDA" << endl;
        exit(0);
    }

    switch (opc)
    {
    case 1:
    {
        std::cout << "1) Algoritmo Construtivo Guloso:" << endl << endl;
        arq_out << "1) Algoritmo Construtivo Guloso:" << endl<< endl;

        clock_t tempoInicial = clock();
        
        qualidade = grafo->algoritmoGuloso();
        
        tempo = 1000 * (clock() - tempoInicial) / CLOCKS_PER_SEC * 0.001; // Tempo em segundos 

        std::cout << "Qualidade: " << qualidade << endl;
        std::cout << "Tempo de Processamento: " << tempo << " segundos" << endl;
        std::cout << "Semente de Randomização:" <<grafo->getSeed()<< endl << endl; 
        std::cout << "*****************************************************************************************************************************************************" << endl << endl;

        arq_out << "Qualidade: " << qualidade << endl;
        arq_out << "Tempo de Processamento: " << tempo << " segundos" << endl;
        arq_out << "Semente de Randomização:" <<grafo->getSeed()<< endl << endl; 
        arq_out << "*****************************************************************************************************************************************************" << endl << endl;

        break;
    }

    case 2:
    {
        std::cout << "2) Algoritmo Construtivo Guloso Randomizado e Adaptativo:" << endl << endl;
        arq_out << "2) Algoritmo Construtivo Guloso Randomizado e Adaptativo:" << endl << endl;

        n_iter = 500;
        qualidadeMedia = 0;
        tempoMedio = 0;

        for(int i=0; i<alfas.size(); i++)
        {
            float tempoMedioAlfa = 0;
            float qualidadeMediaAlfa = 0;

            std::cout << "***Para alfa = " << alfas[i] << " e " << n_iter << " iterações:***" << endl << endl;
            arq_out << "***Para alfa = " << alfas[i] << " e " << n_iter << " iterações:***" << endl << endl;

            for(int j=0; j<10; j++)
            {
                clock_t tempoInicial = clock();
            
                qualidade = grafo->algoritmoGulosoRandomizadoAdaptativo(alfas[i],n_iter);
            
                tempo = 1000 * (clock() - tempoInicial) / CLOCKS_PER_SEC * 0.001; // Tempo em segundos

                std::cout << j+1 << "ª execução:" << endl;
                arq_out << j+1 << "ª execução:" << endl;

                std::cout << "Qualidade: " << qualidade << endl;
                std::cout << "Tempo de Processamento: " << tempo << " segundos." << endl;
                std::cout << "Semente de Randomização: " <<grafo->getSeed()<< endl << endl; 

                arq_out << "Qualidade: " << qualidade << endl;
                arq_out << "Tempo de Processamento: " << tempo << " segundos." << endl;
                arq_out << "Semente de Randomização: " <<grafo->getSeed()<< endl << endl; //???????????

                qualidadeMediaAlfa += qualidade;
                tempoMedioAlfa += tempo;
            }

            tempoMedioAlfa = tempoMedioAlfa/(float)10;
            qualidadeMediaAlfa = qualidadeMediaAlfa/(float)10;

            tempoMedio += tempoMedioAlfa;
            qualidadeMedia += qualidadeMediaAlfa;

            std::cout << "O tempo médio de execução para alfa=" << alfas[i] << " foi de " << tempoMedioAlfa << " segundos e a sua qualidade média foi " << qualidadeMediaAlfa << endl << endl;
            std::cout << "========================================================================================================" << endl << endl;
            arq_out << "O tempo médio de execução para alfa=" << alfas[i] << " foi de " << tempoMedioAlfa << " segundos e a sua qualidade média foi " << qualidadeMediaAlfa << endl << endl;
            arq_out << "========================================================================================================" << endl << endl;
            
        }

        qualidadeMedia = qualidadeMedia/(float)alfas.size();
        tempoMedio = tempoMedio/(float)alfas.size();

        std::cout << "O tempo de execução médio final foi de " << tempoMedio << " segundos e a qualidade média final foi " << qualidadeMedia << endl << endl;
        std::cout << "*****************************************************************************************************************************************************" << endl << endl;
        arq_out << "O tempo de execução médio final foi de " << tempoMedio << " segundos e a qualidade média final foi " << qualidadeMedia << endl << endl;
        arq_out << "*****************************************************************************************************************************************************" << endl << endl;

        break;
    }

    case 3:
    {
        std::cout << "3) Algoritmo Construtivo Guloso Randomizado Reativo:" << endl << endl;
        arq_out << "3) Algoritmo Construtivo Guloso Randomizado Reativo:" << endl << endl;

        n_iter = 2500;
        qualidadeMedia = 0;
        tempoMedio = 0;

        for(int i = 0; i < 10; i++)
        {
            std::cout << i+1 << "ª execução:" << endl;
            arq_out << i+1 << "ª execução:" << endl;
            
            clock_t tempoInicial = clock();
            
            qualidade=grafo->algoritmoGulosoRandomizadoReativo(alfas, n_iter, 100, arquivo_saida); 
            //alfa e solução impressas e arquivadas na função

            tempo = 1000 * (clock() - tempoInicial) / CLOCKS_PER_SEC * 0.001; // Tempo em segundos

            std::cout << "Qualidade: " << qualidade << endl;
            std::cout << "Tempo de Processamento: " << tempo << " segundos." << endl;
            std::cout << "Semente de Randomização: " <<grafo->getSeed()<< endl << endl; 
            std::cout << "========================================================================================================" << endl << endl;
            
            arq_out << "Qualidade: " << qualidade << endl;
            arq_out << "Tempo de Processamento: " << tempo << " segundos." << endl;
            arq_out << "Semente de Randomização: " <<grafo->getSeed()<< endl << endl; //???????????
            arq_out << "========================================================================================================" << endl << endl;

            tempoMedio += tempo;
            qualidadeMedia += qualidade;
        }

        qualidadeMedia = qualidadeMedia/(float)10;
        tempoMedio = tempoMedio/(float)10;

        std::cout << "O tempo de execução médio foi de " << tempoMedio << " segundos e a qualidade média foi " << qualidadeMedia << endl << endl;
        std::cout << "*****************************************************************************************************************************************************" << endl << endl;
        arq_out << "O tempo de execução médio foi de " << tempoMedio << " segundos e a qualidade média foi " << qualidadeMedia << endl << endl;
        arq_out << "*****************************************************************************************************************************************************" << endl << endl;

        break;
    }
    default:
    {
        std::cout << "Opção inválida. Tente novamente, digitando uma opção válida (1,2,3): ";
        std::cin >> opc;
        funcionalidades2(opc, grafo, arquivo_saida);
        break;
    }
    }

    std::cout << endl
         << "Deseja testar outra funcionalidade? (s/n)" << endl;
    char s_n;
    std::cin >> s_n;
    testarNovamente2(s_n, grafo, arquivo_saida);

    arq_out.close();
}

int main(int argc, char *argv[])
{
    string arquivo_entrada;
    string arquivo_saida;
    string tipo_instancia;
    int opc_direc;
    int opc_peso_aresta;
    int opc_peso_nos;

    if (argc == 6)
    {
        arquivo_entrada = argv[1];
        arquivo_saida = argv[2];
        opc_direc = stoi(argv[3]);
        opc_peso_aresta = stoi(argv[4]);
        opc_peso_nos = stoi(argv[5]);

        string input;
        if(opc_peso_aresta == 1)
            input = INPUT_POND_DIR + arquivo_entrada;
        else
            input = INPUT_NAO_POND_DIR + arquivo_entrada;

        Grafo *grafo = new Grafo(input, opc_direc, opc_peso_aresta, opc_peso_nos);

        ofstream arq_out;
        arq_out.open(arquivo_saida, ios::out);
        if(!arq_out.is_open())
        {
            std::cout << "FALHA AO ABRIR ARQUIVO DE SAIDA" << endl;
            exit(0);
        }

        print_menu(grafo, arquivo_saida);

        arq_out.close();
    }
    if(argc == 4)
    {
        arquivo_entrada = argv[1];
        arquivo_saida = argv[2];
        tipo_instancia = argv[3];

        string input;
        if(tipo_instancia == "Handover")
        {
            input = HANDOVER_DIR + arquivo_entrada;
            arquivo_saida = HANDOVER_OUTPUT_DIR + arquivo_saida;
        }
        else if(tipo_instancia == "RanReal240")
        {
            input = RANREAL240_DIR + arquivo_entrada;
            arquivo_saida = RANREAL240_OUTPUT_DIR + arquivo_saida;
        }
        else if(tipo_instancia == "RanReal480")
        {
            input = RANREAL480_DIR + arquivo_entrada;
            arquivo_saida = RANREAL480_OUTPUT_DIR + arquivo_saida;
        }
        else if(tipo_instancia == "RanReal960")
        {
            input = RANREAL960_DIR + arquivo_entrada;
            arquivo_saida = RANREAL960_OUTPUT_DIR + arquivo_saida;
        }
        else if(tipo_instancia == "Sparse82")
        {
            input = SPARSE82_DIR + arquivo_entrada;
            arquivo_saida = SPARSE82_OUTPUT_DIR + arquivo_saida;
        }
        else
            print_help(argv[0]);

        Grafo *grafo = new Grafo(input, 0, 1, 1);

        ofstream arq_out;
        arq_out.open(arquivo_saida, ios::out);
        if(!arq_out.is_open())
        {
            std::cout << "FALHA AO ABRIR ARQUIVO DE SAIDA" << endl;
            exit(0);
        }

        print_menu2(grafo, arquivo_saida);

        arq_out.close();
    }
    else
        print_help(argv[0]);


    return 0;
}