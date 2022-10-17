#include "Grafo.h"

#define infinito 999999999.0

// @brief Retorna a posição de um vértice a partir do ID
// @param _nos_grafo São os nós do grafo que se desfeja verificar o vértice específico
// @param id É a identificação do nó a ser verificado
// @param tam É o numero de vertices no vetor
// @return int - A posição do nó de ID id
int getPosId(No **_nos_grafo, int id, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        if (_nos_grafo[i]->getId() == id)
            return i;
    }

    return -1;
}

// @brief Verifica se é possível existir uma aresta entre os nós.
// @brief Elimina a possibilidade de existência de self-loops e multiarestas.
// @param _nos_grafo São os nós do grafo que se desfeja verificar a existência da aresta
// @param id1 É a identificação do no 1 a ser verificado
// @param id2 É a identificação do no 2 a ser verificado
// @return TRUE (se for possível) ou FALSE (se não for possível, pois é multiaresta ou self-loop)
bool verificaAresta(No **_nos_grafo, int id1, int id2, int tam)
{
    if (id1 == id2)
        return false;

    if (!_nos_grafo[getPosId(_nos_grafo, id1, tam)]->getArestas().empty())
    {
        Aresta *aux = _nos_grafo[getPosId(_nos_grafo, id1, tam)]->getArestas()[0];
        for (int j = 0; j < _nos_grafo[getPosId(_nos_grafo, id1, tam)]->getGrau(); j++)
        {
            if (aux->getNo2()->getId() == id2)
                return false;

            if ((j + 1) < _nos_grafo[getPosId(_nos_grafo, id1, tam)]->getGrau())
                aux = _nos_grafo[getPosId(_nos_grafo, id1, tam)]->getArestas()[j + 1];
        }
        return true;
    }
    else
        return true;
}

// @brief Verifica se é possível existir um arco entre os nós.
// @brief Elimina a possibilidade de existência de self-loops e multiarcos.
// @param _nos_grafo São os nós do grafo que se desfeja verificar a existência do arco
// @param id1 É a identificação do no 1 a ser verificado
// @param id2 É a identificação do no 2 a ser verificado
// @return TRUE (se for possível) ou FALSE (se não for possível, pois é multiarco ou self-loop)
bool verificaArco(No **_nos_grafo, int id1, int id2, int tam)
{
    if (id1 == id2)
        return false;

    if (!_nos_grafo[getPosId(_nos_grafo, id1, tam)]->getArcos().empty())
    {
        Arco *aux = _nos_grafo[getPosId(_nos_grafo, id1, tam)]->getArcos()[0];
        for (int j = 0; j < _nos_grafo[getPosId(_nos_grafo, id1, tam)]->getGrauSaida(); j++)
        {
            if (aux->getNoDestino()->getId() == id2)
                return false;

            if (j + 1 < _nos_grafo[getPosId(_nos_grafo, id1, tam)]->getGrauSaida())
                aux = _nos_grafo[getPosId(_nos_grafo, id1, tam)]->getArcos()[j + 1];
        }
        return true;
    }
    else
        return true;
}

// @brief Função para ler o arquivo contendo o grafo
// @param nomeArquivo É o nome do arquivo com as instâncias do grafo a ser lido
void Grafo::leArquivo(string nomeArquivo)
{
    // abertura do arquivo:
    fstream arq;
    arq.open(nomeArquivo, ios::in);
    if (!arq.is_open())
    {
        std::cout << "FALHA AO ABRIR O ARQUIVO (1)" << endl;
        exit(0);
    }

    string line;
    getline(arq, line);
    // a primeira linha do arquivo é o numero de vertices do grafo:
    n_vertices = stoi(line);

    // instanciando o vetor de vetores com o numero de vertices:
    nos_grafo = new No *[n_vertices];
    // preenche os vetores de nós:
    // int pesoNo;
    for (int i = 0; i < n_vertices; i++)
    {
        // if (ponderadoNos)
        //     pesoNo = i;
        // else
        //     pesoNo = 1;
        nos_grafo[i] = new No(i, 1);
    }

    // enquanto ainda houverem linhas a serem lidas:
    while (getline(arq, line))
    {
        // separa a linha, obtendo os nós e os pesos das arestas:
        stringstream ss(line);
        string aux;
        vector<int> vet_;
        while (getline(ss, aux, ' ')) // espaços separam os nós e peso da aresta no arquivo
            vet_.push_back(stoi(aux));

        int no1 = vet_[0];
        int no2 = vet_[1];
        float pesoAresta;
        if (ponderadoArestas)
            pesoAresta = vet_[2];
        else
            pesoAresta = 1;

        if (direcionado)
        {
            if (verificaArco(this->nos_grafo, no1, no2, this->n_vertices))
            {
                // conecta o nó ao arco que o liga ao seu nó adjacente SUCESSOR:
                nos_grafo[no1]->adcArco(nos_grafo[no2], pesoAresta);
            }
        }
        else
        {
            if (verificaAresta(this->nos_grafo, no1, no2, this->n_vertices))
            {
                // conecta aos nós as arestas que os ligam aos seus adjacentes:
                nos_grafo[no1]->adcAresta(nos_grafo[no2], pesoAresta);
                nos_grafo[no2]->adcAresta(nos_grafo[no1], pesoAresta);
            }
        }
    }

    arq.close();
}

// @brief Função para ler o arquivo contendo o grafo no caso de ser do tipo Handover
// @param nomeArquivo É o nome do arquivo com as instâncias do grafo a ser lido
// @author: @marianaricha
void Grafo::leArquivo2(string nomeArquivo)
{
    // abertura do arquivo:
    fstream arq;
    arq.open(nomeArquivo, ios::in);
    if (!arq.is_open())
    {
        std::cout << "FALHA AO ABRIR O ARQUIVO (2)" << endl;
        exit(0);
    }

    stringstream ss(nomeArquivo);
    string aux;
    vector<string> vet;
    while (getline(ss, aux, '/'))
        vet.push_back(aux);

    stringstream ss2(vet[3]);
    string aux2;
    vector<string> vet2;
    while (getline(ss2, aux2, '_'))
        vet2.push_back(aux2);
    
    //vet2[0] = número de nós
    //vet2[1] = número de clusters
    //vet2[2] = semente

    seed=stof(vet2[2]); //semente de randomização

    // Leitura da primeira linha do arquivo:
    string line;
    getline(arq, line);

    // a primeira linha do arquivo é o numero de vertices do grafo:
    n_vertices = stoi(line);

    // pega a próxima linha
    getline(arq, line);

    // a segunda linha do arquivo é o numero de clusters do grafo:
    n_clusters = stoi(line);

    // pega a próxima linha
    getline(arq, line);

    // a terceira linha do arquivo é a capacidade dos clusters do grafo (todos tem a mesma capacidade)
    ccapacity = stof(line);


    // instanciando o vetor de vetores com o numero de vertices:
    nos_grafo = new No *[n_vertices];

    float pesoNo;

    for (int i = 0; i < n_vertices; i++)
    { // pega os valores do peso dos nós

        // pega a próxima linha
        getline(arq, line);

        // pega o peso
        pesoNo = stof(line);

        // preenchendo o vetor com os nós e seus respectivos pesos
        nos_grafo[i] = new No(i, pesoNo);
    }

    // pega a próxima linha que contém as arestas
    getline(arq, line);

    // separa a linha, obtendo os pesos das arestas:
    stringstream ss3(line);
    string aux3;
    float pesoAresta;

    for (int i = 0; i < n_vertices; i++)
    {
        // faz o processo para cada nó

        for (int j = 0; j < n_vertices; j++)
        {
            getline(ss3, aux3, ' ');  // espaços separam os pesos das arestas
            pesoAresta = stof(aux3); // pega o peso da aresta

            if (pesoAresta != 0 && verificaAresta(this->nos_grafo, nos_grafo[i]->getId(), nos_grafo[j]->getId(), this->n_vertices)){
                nos_grafo[i]->adcAresta(nos_grafo[j], pesoAresta);     // adiciona aresta no nó da linha
                nos_grafo[j]->adcAresta(nos_grafo[i], pesoAresta);     // adiciona aresta no nó da coluna
            }
        }
    }

    arq.close();
}

// @brief Função para ler o arquivo contendo o grafo no caso de ser RanReal240/480/960 ou Sparce82
// @param nomeArquivo É o nome do arquivo com as instâncias do grafo a ser lido
// @author: @dudaribeiro7
void Grafo::leArquivo3(string nomeArquivo)
{
    // abertura do arquivo:
    fstream arq;
    arq.open(nomeArquivo, ios::in);
    if (!arq.is_open())
    {
        std::cout << "FALHA AO ABRIR O ARQUIVO (3)" << endl;
        exit(0);
    }

    // Leitura da primeira linha do arquivo:
    string line;
    getline(arq, line);

    // A primeira linha do arquivo tem os 6 elementos a seguir, separados por espaços:
    stringstream sst(line);
    string aux1;
    vector<string> vet_;
    while (getline(sst, aux1, ' '))
        vet_.push_back(aux1);

    // 1. Inteiro indicando o número de nós:
    n_vertices = stoi(vet_[0]);

    // instanciando o vetor de vetores com o numero de vertices:
    nos_grafo = new No *[n_vertices];

    // 2. Inteiro indincando o número de clusters:
    n_clusters = stoi(vet_[1]);

    // 3. Tipo da cluster - "ss" (same size) ou "ds" (different size):
    string type = vet_[2];

    // 4. Limites da cluster - L e U para cada cluster:
    for (int i = 3; i < (2 * n_clusters + 3); i += 2)
    {
        L.push_back(stoi(vet_[i]));
        U.push_back(stoi(vet_[i + 1]));
    }
    this->ccapacity = -1;

    // 5. Centinela - letra W - é apenas um separador
    int i = 2 * n_clusters + 4;

    // 6. Peso de cada nó:
    for (int j = 0; j < n_vertices; j++)
    {
        float pesoNo = stof(vet_[i]);
        nos_grafo[j] = new No(j, pesoNo);
        i++;
    }

    // As próximas linhas são as arestas e seus pesos:
    // enquanto ainda houverem linhas a serem lidas:
    while (getline(arq, line))
    {
        // separa a linha, obtendo os nós e os pesos das arestas:
        stringstream ss(line);
        string aux;
        vector<int> vet_;
        while (getline(ss, aux, ' ')) // espaços separam os nós e peso da aresta no arquivo
            vet_.push_back(stoi(aux));

        int no1 = vet_[0];
        int no2 = vet_[1];
        float pesoAresta = vet_[2];

        if (pesoAresta != 0)
        {
            if (verificaAresta(this->nos_grafo, no1, no2, this->n_vertices))
            {
                // conecta aos nós as arestas que os ligam aos seus adjacentes:
                nos_grafo[no1]->adcAresta(nos_grafo[no2], pesoAresta);
                nos_grafo[no2]->adcAresta(nos_grafo[no1], pesoAresta);
            }
        }
    }

    arq.close();
}

// @brief Verifica se um determinado ID está presente no vetor
// @param vet É o vetor no qual se deseja buscar o ID
// @param id É o ID que se deseja buscar
// @return TRUE se encontrar e FALSE caso contrário
template <typename T>
bool searchInVector(vector<T> vet, T id)
{
    for (int i = 0; i < vet.size(); i++)
        if (vet[i] == id)
            return true;
    return false;
}

// @brief Retorna um subgrafo vértice induzido pelo conjunto de vértices passados por parâmetro.
// @param X subconjunto de vértices do grafo para achar o subgrafo vértice induzido
// @return Grafo* - o subgrafo vértice induzido
Grafo *Grafo::subgrafoVerticeInduzido(vector<int> X)
{
    // Aloca um novo vetor de ponteiros para os nós do subgrafo vertice induzido que será retornado:
    No **nos_subgrafo = new No *[X.size()];
    // Percorre o vetor que contém os IDs dos nós do subgrafo:
    for (int i = 0; i < X.size(); i++)
    {
        // Resgata o peso do nó a ser adicionado no subgrafo:
        float pesoNo = this->nos_grafo[X[i]]->getPesoNo();
        // Adiciona o nó no subgrafo:
        nos_subgrafo[i] = new No(X[i], pesoNo);
    }

    // Percorre o vetor que contém os IDs dos nós do subgrafo:
    for (int i = 0; i < X.size(); i++)
    {
        // Verifica se é direcionado ou não:
        if (this->direcionado)
        {
            // Percorre os nós sucessores aos nós contidos no vetor X:
            for (int j = 0; j < this->nos_grafo[X[i]]->getGrauSaida(); j++)
            {
                // Resgata o ID do no sucessor:
                int id_sucessor = this->nos_grafo[X[i]]->getNosSuc()[j]->getId();
                // Verifica se o nó sucessor também está contido no vetor X:
                if (searchInVector<int>(X, id_sucessor))
                {
                    // Se estiver, resgata o peso do arco que será adicionado e verifica se é possível, no subgrafo, existir esse arco:
                    float pesoArco = this->nos_grafo[X[i]]->getArcos()[j]->getPeso();
                    if (verificaArco(nos_subgrafo, X[i], id_sucessor, X.size()))
                    {
                        nos_subgrafo[getPosId(nos_subgrafo, X[i], X.size())]->adcArco(nos_subgrafo[getPosId(nos_subgrafo, id_sucessor, X.size())], pesoArco);
                    }
                }
            }
        }
        else
        {
            // Percorre os nós adjacentes dos nós contidos no vetor X:
            for (int j = 0; j < this->nos_grafo[X[i]]->getGrau(); j++)
            {
                // Resgata o ID do nó adjacente:
                int id_adjacente = this->nos_grafo[X[i]]->getArestas()[j]->getNo2()->getId();
                // Verifica se o nó adjacente também está contido no vetor X:
                if (searchInVector(X, id_adjacente))
                {
                    // Se estiver, resgata o peso da aresta que será adicionada e verifica se é possível, no subgrafo, existir essa aresta:
                    float pesoAresta = this->nos_grafo[X[i]]->getArestas()[j]->getPeso();
                    if (verificaAresta(nos_subgrafo, X[i], id_adjacente, X.size()))
                    {
                        nos_subgrafo[getPosId(nos_subgrafo, X[i], X.size())]->adcAresta(nos_subgrafo[getPosId(nos_subgrafo, id_adjacente, X.size())], pesoAresta);
                        nos_subgrafo[getPosId(nos_subgrafo, id_adjacente, X.size())]->adcAresta(nos_subgrafo[getPosId(nos_subgrafo, X[i], X.size())], pesoAresta);
                    }
                }
            }
        }
    }

    Grafo *subgrafo = new Grafo(nos_subgrafo, X.size(), this->direcionado, this->ponderadoArestas, this->ponderadoNos);
    // subgrafo->printGrafo();
    return subgrafo;
}

// @brief Retorna um subgrafo aresta induzido pelo conjunto de arestas passados por parâmetro.
// @param X subconjunto de arestas do grafo para achar o subgrafo aresta induzido
// @return Grafo* - o subgrafo aresta induzido
// @author: @dudaribeiro7
Grafo *Grafo::subgrafoArestaInduzido(vector<Aresta *> Y)
{
    // Vetor com todos os vertices presentes nas arestas de Y:
    vector<No *> X;
    for (int i = 0; i < Y.size(); i++)
    {
        No *no1 = Y[i]->getNo1();
        No *no2 = Y[i]->getNo2();

        // Se o vertice ja nao estiver no vetor, adiciona ele:
        if (!searchInVector<No *>(X, no1))
            X.push_back(no1);
        if (!searchInVector<No *>(X, no2))
            X.push_back(no2);
    }

    // Aloca um novo vetor de ponteiros para os nós do subgrafo vertice induzido que será retornado:
    No **nos_subgrafo = new No *[X.size()];
    // Percorre o vetor que contém os IDs dos nós do subgrafo:
    for (int i = 0; i < X.size(); i++)
    {
        // Resgata o peso do nó a ser adicionado no subgrafo:
        float pesoNo = this->nos_grafo[X[i]->getId()]->getPesoNo();
        // Adiciona o nó no subgrafo:
        nos_subgrafo[i] = new No(X[i]->getId(), pesoNo);
    }

    // Adiciona as arestas aos nós:
    for (int i = 0; i < Y.size(); i++)
    {
        int id1 = Y[i]->getNo1()->getId();
        int id2 = Y[i]->getNo2()->getId();
        int pos1 = getPosId(nos_subgrafo, id1, X.size());
        int pos2 = getPosId(nos_subgrafo, id2, X.size());
        float pesoAresta = Y[i]->getPeso();
        nos_subgrafo[pos1]->adcAresta(nos_subgrafo[pos2], pesoAresta);
        nos_subgrafo[pos2]->adcAresta(nos_subgrafo[pos1], pesoAresta);
    }

    Grafo *subgrafo = new Grafo(nos_subgrafo, X.size(), this->direcionado, this->ponderadoArestas, this->ponderadoNos);
    return subgrafo;
}

// @brief Construtor
// @param nomeArquivo É o nome do arquivo com as instâncias do grafo a ser lido
// @param direc Contém a opção do grafo ser direcionado (1) ou não (0)
// @param peso_aresta Contém a opção do grafo ser ponderado nas arestas (1) ou não (0)
// @param peso_nos Contém a opção do grafo ser ponderado nos vértices (1) ou não (0)
Grafo::Grafo(string nomeArquivo, int direc, int peso_aresta, int peso_nos)
{
    if (direc == 0)
        direcionado = false;
    else
        direcionado = true;

    if (peso_aresta == 0)
        ponderadoArestas = false;
    else
        ponderadoArestas = true;

    if (peso_nos == 0)
        ponderadoNos = false;
    else
        ponderadoNos = true;

    stringstream ss(nomeArquivo);
    string aux;
    vector<string> vet;
    while (getline(ss, aux, '/'))
        vet.push_back(aux);

    // vet[0] = .
    // vet[1] = input

    if (vet[2] == "ponderados" || vet[2] == "nao_ponderados")
        leArquivo(nomeArquivo);

    else if (vet[2] == "Handover")
        leArquivo2(nomeArquivo);

    else if (vet[2] == "RanReal240" || vet[2] == "RanReal480" || vet[2] == "RanReal960" || vet[2] == "Sparce82")
        leArquivo3(nomeArquivo);
}

// @brief Construtor
// @param _nos_grafo É o vetor de ponteiros dos nós do grafo
// @param _n_vertices É o número de vértices do grafo
// @param _direc Informação sobre se o grafo é direcionado ou não
// @param _pesoAresta Informação sobre se o grafo é ponderado nas arestas ou não
// @param _pesoNos Informação sobre se o grafo é ponderado nos vértices ou não
Grafo::Grafo(No **_nos_grafo, int _n_vertices, bool _direc, bool _pesoAresta, bool _pesoNos)
{
    this->nos_grafo = _nos_grafo;
    this->n_vertices = _n_vertices;
    this->direcionado = _direc;
    this->ponderadoArestas = _pesoAresta;
    this->ponderadoNos = _pesoNos;
}

// @brief Destrutor
Grafo::~Grafo()
{
    for (int i = 0; i < n_vertices; i++)
    {
        delete nos_grafo[i];
    }

    delete[] nos_grafo;
}

// @brief Imprime o grafo na tela
void Grafo::printGrafo()
{
    if (direcionado)
    {
        for (int i = 0; i < n_vertices; i++)
        {
            std::cout << nos_grafo[i]->getId() << " -> ";
            Arco *aux = nos_grafo[i]->getArcos()[0];
            for (int j = 0; j < nos_grafo[i]->getGrauSaida(); j++)
            {
                std::cout << aux->getNoDestino()->getId() << " -> ";
                if (j + 1 < nos_grafo[i]->getGrauSaida())
                    aux = nos_grafo[i]->getArcos()[j + 1];
            }
            std::cout << endl;
        }
    }
    else
    {
        for (int i = 0; i < n_vertices; i++)
        {
            std::cout << nos_grafo[i]->getId() << " - ";
            Aresta *aux = nos_grafo[i]->getArestas()[0];
            for (int j = 0; j < nos_grafo[i]->getGrau(); j++)
            {
                std::cout << aux->getNo2()->getId() << " - ";
                if (j + 1 < nos_grafo[i]->getGrau())
                    aux = nos_grafo[i]->getArestas()[j + 1];
            }

            std::cout << endl;
        }
    }
}

// @brief Retorna o número de vértices do grafo
// @return int
int Grafo::getNumVertices()
{
    return this->n_vertices;
}

// @brief Retorna a semente de randomização
// @return float
float Grafo::getSeed()
{
    return this->seed;
}

// @brief Retorna o vetor de ponteiros para os nós do grafo
// @return No**
No **Grafo::getNosGrafo()
{
    return this->nos_grafo;
}

// @brief Retorna se o grafo é direcionado ou não
// @return bool
bool Grafo::isDirecionado()
{
    return this->direcionado;
}

// @brief Retorna se o grafo é ponderado nas arestas ou não
// @return bool
bool Grafo::isPonderadoArestas()
{
    return this->ponderadoArestas;
}

// @brief Retorna se o grafo é ponderado nos nós ou não
// @return bool
bool Grafo::isPonderadoNos()
{
    return this->ponderadoNos;
}

// @author @marianaricha
// @brief Realiza o caminhamento em profundidade a partir de um determinado vértice
// @param id um ID de vértice
// @return vector<No*> - Vetor de nós em que o nó deste id chega
vector<No *> Grafo::caminhamentoProfundidade(int id, string arquivo_saida)
{
    // abertura do arquivo de escrita para o dot
    fstream arq;
    arq.open("arqDotCaminhamento", ios::app);
    if (!arq.is_open())
    {
        std::cout << "FALHA AO ABRIR O ARQUIVO (caminhamento)" << endl;
        exit(0);
    }

    arq << "graph G {" << endl;

    bool visitados[n_vertices];
    for (int i = 0; i < n_vertices; i++)
    {
        visitados[i] = false; // a posição no vetor de visitados será igual ao id do vértice
    }

    vector<No *> vetor;
    cP(id, visitados, &vetor, -1); // adiciona cada nó em que o vetor chega em um vetor de nós
    arq << "}";
    return vetor;
}

// @author @marianaricha
// @brief Adiciona o nó alcançado ao vetor.
// @brief Imprime a árvore dada pela ordem de caminhamento em profundidade a partir de nó dado parâmetro, destacando as arestas de retorno
// @param id um ID de vértice
void Grafo::cP(int id, bool v[], vector<No *> *vetor, int aux)
{
    // abrindo arquivo de novo
    fstream arq;
    arq.open("arqDotCaminhamento", ios::app);
    if (!arq.is_open())
    {
        std::cout << "FALHA AO ABRIR O ARQUIVO" << endl;
        exit(0);
    }

    v[id] = true;
    std::cout << "Visitando o vértice " << id << endl;

    for (int j = 0; j < nos_grafo[id]->getNosAdj().size(); j++)
    { // verifica se é folha
        int w = nos_grafo[id]->getNosAdj()[j]->getId();
        if (!v[w])
        {
            arq << id << "--" << w << endl;
            cP(w, v, vetor, id); // recurssividade
        }
    }
    if (aux != -1)
        std::cout << "Volta para o vértice " << aux << " pela aresta (" << id << "," << aux << ")" << endl;
    vetor->push_back(nos_grafo[id]); // retorna o nó que ele chegou
}

// @author @mariana_richa
// @brief Encontra o fecho transitivo direto de um nó
// @param id ID de um vértice do grafo
// @return vector<No*> - Fecho Transitivo Direto (vetor de vértices)
vector<No *> Grafo::fechoTransDir(int id)
{
    bool visitados[n_vertices];
    for (int i = 0; i < n_vertices; i++)
    {
        visitados[i] = false; // a posição no vetor de visitados será igual ao id do vértice
    }

    vector<No *> vetor;
    fechoaux(id, visitados, &vetor); // adiciona cada nó em que o vetor chega em um vetor de nós
    vetor.pop_back();
    return vetor; // vetor de nós em que o id chega
}

void Grafo::fechoaux(int id, bool v[], vector<No *> *vetor)
{
    v[id] = true;

    for (int j = 0; j < nos_grafo[id]->getNosSuc().size(); j++)
    { // verifica se é folha
        int w = nos_grafo[id]->getNosSuc()[j]->getId();
        if (!v[w])
        {
            fechoaux(w, v, vetor); // recursividade
        }
    }

    vetor->push_back(nos_grafo[id]); // retorna o nó que ele chegou
}

// @author @mariana_richa
// @brief Encontra o fecho transitivo indireto de um nó
// @param id ID de um vértice do grafo
// @return vector<No*> - Fecho Transitivo Indireto (vetor de vértices)
vector<No *> Grafo::fechoTransInd(int id)
{
    vector<No *> vetor;
    for (int i = 0; i < n_vertices; i++)
    {
        vector<No *> aux = fechoTransDir(i);
        if (searchInVector<No *>(aux, nos_grafo[id]) && i != id)
            vetor.push_back(nos_grafo[i]); // vetor de nós que contém o nó procurado em seu fecho transitivo direto
    }

    return vetor;
}

// @author @vitor-frnds
// @brief Encontra o coeficiente de agrupamento local de um vertice
// @param id ID de um vértice do grafo
// @return float - Coeficiente de agrupamento local do vértice
float Grafo::coeficienteAgrupamentoLocal(int id)
{
    if (direcionado)
    {
        No *NoAux;
        No *no = nos_grafo[id];

        Arco *arco1;

        float coef;
        float grau = no->getGrauSaida();
        float pv = 0;

        int a1 = 0; // indice para o arco 1
        arco1 = no->getArcos()[a1];

        for (int k = 0; k < no->getGrauSaida(); k++)
        {
            NoAux = arco1->getNoDestino();

            for (int l = 0; l < NoAux->getGrauSaida(); l++)
            {
                vector<No *> suc = no->getNosSuc();

                for (int i = 0; i < suc.size(); i++)
                {
                    if (suc[i]->getId() == NoAux->getId())
                        pv++;
                }
            }

            a1++;
            arco1 = NoAux->getArcos()[a1];
        }

        grau = grau / 2;
        coef = float(pv / ((pow(grau, 2) - 1) / 2));

        return coef;
    }
    else
    {
        No *NoAux;
        No *no = nos_grafo[id];

        Aresta *aresta1;

        float coef;
        float grau = no->getGrau();
        float pv = 0;

        int a1 = 0; // indice para a aresta 1

        aresta1 = no->getArestas()[a1];

        for (int k = 0; k < no->getGrau(); k++)
        {
            NoAux = aresta1->getNo2();

            for (int l = 0; l < NoAux->getGrau(); l++)
            {
                vector<No *> adj = no->getNosAdj();

                for (int i = 0; i < adj.size(); i++)
                {
                    if (adj[i]->getId() == NoAux->getId())
                        pv++;
                }
            }

            a1++;
            aresta1 = no->getArestas()[a1];
        }

        grau = grau / 2;
        coef = float(pv / ((pow(grau, 2) - 1) / 2));

        return coef;
    }
}

// @author @vitor-frnds
// @brief Encontra o coeficiente de agrupamento médio do grafo
// @return Coeficiente de agrupamento médio do grafo
float Grafo::coeficienteAgrupamentoMedio()
{
    int som = 0; // somatorio dos coeficientes de agrupamentos locais

    for (int i = 0; i < n_vertices; i++)
        som = som + coeficienteAgrupamentoLocal(nos_grafo[i]->getId());

    return (som / n_vertices);
}

// @author @RiUza02
// @brief Encontra o caminho mínimo entre esses dois vértices usando o algoritmo de Dijkstra
// @param inicio/destino dois IDs de vértices do grafo
void Grafo::dijkstra(int inicio, int destino, string arquivo_saida)
{
    ofstream arq_out;
    arq_out.open(arquivo_saida, ios::app);
    if (!arq_out.is_open())
    {
        std::cout << "FALHA AO ABRIR ARQUIVO DE SAIDA" << endl;
        exit(0);
    }

    vector<int> beta;           // vetor de custos
    vector<int> fi;             // vetor de marcação
    vector<int> pi;             // vetor de antecessores (para achar a sequencia de vertices do caminho minimo)
    int w, r, flag, ehPossivel; // auxiliares

    // inicialização
    ehPossivel = 0;
    r = inicio;
    flag = 0;
    for (int i = 0; i < n_vertices; i++)
    {
        beta.push_back(infinito);
        fi.push_back(0);
        pi.push_back(0);
    }
    beta[inicio] = 0;
    fi[inicio] = 1;

    if (direcionado)
    {
        // verifica se o Nó inicial consegue chegar no Nó final
        vector<No *> nos = fechoTransDir(inicio);
        for (int i = 0; i < nos.size(); i++)
        {
            if (nos[i]->getId() == destino)
            {
                ehPossivel = 1;
            }
        }
        if (ehPossivel)
        {
            while (flag == 0)
            {
                // varre todos os adjacentes do nó da interação atual e atualiza seus custos
                w = r;
                vector<Arco *> auxAresta = nos_grafo[w]->getArcos();
                for (int i = 0; i < auxAresta.size(); i++)
                {
                    if (beta[auxAresta[i]->getNoDestino()->getId()] > beta[w] + auxAresta[i]->getPeso())
                    {
                        beta[auxAresta[i]->getNoDestino()->getId()] = beta[w] + auxAresta[i]->getPeso();
                        pi[auxAresta[i]->getNoDestino()->getId()] = w;
                    }
                }

                // pega o primeiro nó que não esta no conjunto solução
                int flagMenor = 0;
                int contador = 0;
                while ((flagMenor == 0) && (contador < n_vertices))
                {
                    if (fi[contador] != 1)
                    {
                        r = contador;
                        flagMenor = 1;
                    }
                    contador++;
                }

                // escolhe entre todos os nós do conjunto solução o menor deles
                for (int i = 0; i < n_vertices; i++)
                {
                    if (fi[i] != 1)
                    {
                        if ((beta[i] < beta[r]))
                        {
                            r = i;
                        }
                    }
                }

                // encerra a execução se possivel
                fi[r] = 1;
                if (w == destino)
                {
                    flag = 1;
                }
            }
        }
        else
        {
            std::cout << "nao existe caminho entre esses vertices";
            arq_out << "nao existe caminho entre esses vertices";
            return;
        }
    }
    else
    {
        while (flag == 0)
        {
            // varre todos os adjacentes do nó da interação atual e atualiza seus custos
            w = r;
            vector<Aresta *> auxAresta = nos_grafo[w]->getArestas();
            for (int i = 0; i < auxAresta.size(); i++)
            {
                if (beta[auxAresta[i]->getNo2()->getId()] > beta[w] + auxAresta[i]->getPeso())
                {
                    beta[auxAresta[i]->getNo2()->getId()] = beta[w] + auxAresta[i]->getPeso();
                    pi[auxAresta[i]->getNo2()->getId()] = w;
                }
            }

            // pega o primeiro nó que não esta no conjunto solução
            int flagMenor = 0;
            int contador = 0;
            while ((flagMenor == 0) && (contador < n_vertices))
            {
                if (fi[contador] != 1)
                {
                    r = contador;
                    flagMenor = contador;
                }
                contador++;
            }

            // escolhe entre todos os nós do conjunto solução o menor deles
            for (int i = 0; i < n_vertices; i++)
            {
                if (fi[i] != 1)
                {
                    if ((beta[i] < beta[r]))
                    {
                        r = i;
                    }
                }
            }

            // encerra a execução se possivel
            fi[r] = 1;
            if (w == destino)
            {
                flag = 1;
            }
        }
    }

    if (direcionado)
    {
        if (ehPossivel)
        {
            // organiza o menor caminho
            vector<int> solucao;
            flag = 0;
            int aux1 = destino;
            int aux2 = pi[destino];
            solucao.push_back(aux1);
            while (flag == 0)
            {
                aux1 = aux2;
                aux2 = pi[aux1];
                solucao.push_back(aux1);
                if (aux1 == inicio)
                {
                    flag = 1;
                }
            }

            // imprime a resposta
            std::cout << endl;
            for (int i = 0; i < solucao.size(); i++)
            {
                std::cout << " - " << pi[i] << " - ";
                arq_out << " - " << pi[i] << " - ";
            }
            std::cout << endl
                      << "custo: " << beta[destino];
            arq_out << endl
                    << "custo: " << beta[destino];
        }
    }
    else
    {
        // organiza o menor caminho
        vector<int> solucao;
        flag = 0;
        int aux1 = destino;
        int aux2 = pi[destino];
        solucao.push_back(aux1);
        while (flag == 0)
        {
            aux1 = aux2;
            aux2 = pi[aux1];
            solucao.push_back(aux1);
            if (aux1 == inicio)
            {
                flag = 1;
            }
        }

        // imprime a resposta
        std::cout << endl;
        for (int i = 0; i < solucao.size(); i++)
        {
            std::cout << " - " << pi[i] << " - ";
            arq_out << " - " << pi[i] << " - ";
        }
        std::cout << endl
                  << "custo: " << beta[destino];
        arq_out << endl
                << "custo: " << beta[destino];
    }
    arq_out.close();
}

// @author @RiUza02
// @brief Encontra o caminho mínimo entre dois vértices usando o algoritmo de Floyd
// @param id1/id2 dois IDs de vértices do grafo
void Grafo::floyd(int inicio, int destino, string arquivo_saida)
{
    ofstream arq_out;
    arq_out.open(arquivo_saida, ios::app);
    if (!arq_out.is_open())
    {
        std::cout << "FALHA AO ABRIR ARQUIVO DE SAIDA" << endl;
        exit(0);
    }

    float matrizAdj[n_vertices][n_vertices]; // matriz de custos
    int pi[n_vertices][n_vertices];          // matriz de antecessores (para achar a sequencia de vertices do caminho minimo)
    int ehPossivel;                          // auxiliar

    // Inicialização
    ehPossivel = 0;
    for (int i = 0; i < n_vertices; i++)
    {
        for (int j = 0; j < n_vertices; j++)
        {
            if (i == j)
            {
                matrizAdj[i][j] = 0;
            }
            else
            {
                matrizAdj[i][j] = infinito;
            }
            pi[i][j] = 0;
        }
    }

    if (direcionado)
    {
        // verifica se o Nó inicial consegue chegar no Nó final
        vector<No *> nos = fechoTransDir(inicio);
        for (int i = 0; i < nos.size(); i++)
        {
            if (nos[i]->getId() == destino)
            {
                ehPossivel = 1;
            }
        }

        if (ehPossivel)
        {
            // Inserindos os valores dos Nós imediatos
            vector<Arco *> auxArcos;
            for (int i = 0; i < n_vertices; i++)
            {
                auxArcos = nos_grafo[i]->getArcos();
                for (int j = 0; j < n_vertices; j++)
                {
                    matrizAdj[i][auxArcos[j]->getNoDestino()->getId()] = auxArcos[j]->getPeso();
                }
            }

            // preenchimento dos valores de distancias
            for (int i = 0; i < n_vertices; i++)
            {
                for (int j = 0; j < n_vertices; j++)
                {
                    for (int k = 0; k < n_vertices; k++)
                    {
                        if ((matrizAdj[j][i] != infinito) && (matrizAdj[i][k] != infinito))
                        {
                            if (matrizAdj[j][k] > (matrizAdj[j][i] + matrizAdj[i][k]))
                            {
                                matrizAdj[j][k] = matrizAdj[j][i] + matrizAdj[i][k];
                                pi[j][k] = i;
                            }
                        }
                    }
                }
            }

            // imprime a resposta
            std::cout << endl;
            int a = inicio;
            int b = destino;
            while (pi[a][b] != b)
            {
                std::cout << "-" << b << "-";
                arq_out << "-" << b << "-";
                b = pi[a][b];
            }
        }
        else
        {
            std::cout << "nao existe caminho entre esses vertices";
            arq_out << "nao existe caminho entre esses vertices";
            return;
        }
    }
    else
    {
        // Inserindos os valores dos Nós imediatos
        vector<Aresta *> auxAresta;
        for (int i = 0; i < n_vertices; i++)
        {
            auxAresta = nos_grafo[i]->getArestas();
            for (int j = 0; j < n_vertices; j++)
            {
                matrizAdj[i][auxAresta[j]->getNo2()->getId()] = auxAresta[j]->getPeso();
            }
        }

        // preenchimento dos valores de distancias
        for (int i = 0; i < n_vertices; i++)
        {
            for (int j = 0; j < n_vertices; j++)
            {
                for (int k = 0; k < n_vertices; k++)
                {
                    if ((matrizAdj[j][i] != infinito) && (matrizAdj[i][k] != infinito))
                    {
                        if (matrizAdj[j][k] > (matrizAdj[j][i] + matrizAdj[i][k]))
                        {
                            matrizAdj[j][k] = matrizAdj[j][i] + matrizAdj[i][k];
                            pi[j][k] = i;
                        }
                    }
                }
            }
        }

        // imprime a resposta
        std::cout << endl;
        int a = inicio;
        int b = destino;
        while (pi[a][b] != b)
        {
            std::cout << "-" << b << "-";
            arq_out << "-" << b << "-";
            b = pi[a][b];
        }
    }
    arq_out.close();
}

// @author @dudaribeiro7
// @brief Encontra a aresta de menor peso em todo o grafo
// @return Aresta* - A aresta de menor peso
Aresta *Grafo::arestaMenorPeso()
{
    float menorPeso = nos_grafo[0]->getArestas()[0]->getPeso();
    int idx_i = 0;
    int idx_j = 0;
    for (int i = 0; i < n_vertices; i++)
    {
        for (int j = 0; j < nos_grafo[i]->getGrau(); j++)
        {
            float peso = nos_grafo[i]->getArestas()[j]->getPeso();
            if (peso < menorPeso)
            {
                menorPeso = peso;
                idx_i = i;
                idx_j = j;
            }
        }
    }
    return nos_grafo[idx_i]->getArestas()[idx_j];
}

// @author @dudaribeiro7
// @brief Procura um item dentro de um vetor e retorna a sua posição nele.
// @param arr É o vetor no qual se deseja buscar a posição do item
// @param item É o item que se deseja saber a posição
// @return auto - A posição do item no vetor
auto findIndex(const vector<No *> arr, No *item)
{
    for (auto i = 0; i < arr.size(); ++i)
    {
        if (arr[i] == item)
            return i;
    }
    return -1;
}

bool ehConexo(Grafo *grafo)
{
    vector<No *> fechoT_dir;
    for (int i = 0; i < grafo->getNumVertices(); i++)
    {
        fechoT_dir = grafo->fechoTransDir(grafo->getNosGrafo()[i]->getId());
        if (fechoT_dir.size() != grafo->getNumVertices() - 1)
            return false;
    }
    return true;
}

// @author @dudaribeiro7
// @brief Encontra uma Árvore Geradora Mínima sobre o subgrafo vértice-induzido por X usando o algoritmo de Prim
// @param X um subconjunto de vértices de um grafo
void Grafo::prim(vector<int> X, string arquivo_saida)
{
    ofstream arq_out;
    arq_out.open(arquivo_saida, ios::app);
    if (!arq_out.is_open())
    {
        std::cout << "FALHA AO ABRIR ARQUIVO DE SAIDA" << endl;
        exit(0);
    }

    Grafo *subgrafo = subgrafoVerticeInduzido(X);
    if (!ehConexo(subgrafo))
    {
        std::cout << "Não é possível gerar uma Árvore Geradora Mínima com o algoritmo de Prim para esse subgrafo, pois ele não é conexo." << endl;
        arq_out << "Não é possível gerar uma Árvore Geradora Mínima com o algoritmo de Prim para esse subgrafo, pois ele não é conexo." << endl;
        return;
    }
    vector<Aresta *> S;
    vector<No *> nos_conectados;
    vector<No *> nos_nao_conectados;
    vector<Aresta *> aux_arestas_menorPeso;
    float menorPeso;
    float peso;
    int idx = 0;

    for (int i = 0; i < subgrafo->getNumVertices(); i++)
        nos_nao_conectados.push_back(subgrafo->nos_grafo[i]);

    // Adiciona a aresta de menor peso do subgrafo no vetor de solução:
    S.push_back(subgrafo->arestaMenorPeso());

    // Adiciona os nós da aresta de menor peso do subgrafo no vetor de nós ja conectados:
    nos_conectados.push_back(subgrafo->arestaMenorPeso()->getNo1());
    nos_conectados.push_back(subgrafo->arestaMenorPeso()->getNo2());

    // Remove esses nós do vetor de nós não conectados:
    nos_nao_conectados.erase(nos_nao_conectados.begin() + findIndex(nos_nao_conectados, subgrafo->arestaMenorPeso()->getNo1()));
    nos_nao_conectados.erase(nos_nao_conectados.begin() + findIndex(nos_nao_conectados, subgrafo->arestaMenorPeso()->getNo2()));

    while (!nos_nao_conectados.empty())
    {
        // Encontra, para cada nó já conectado, a aresta de menor peso que o liga a um nó ainda não conectado, se esta aresta existir:
        for (int i = 0; i < nos_conectados.size(); i++)
        {
            for (int j = 0; j < nos_conectados[i]->getGrau(); j++)
            {
                if (searchInVector<No *>(nos_nao_conectados, nos_conectados[i]->getNosAdj()[j]))
                {
                    menorPeso = nos_conectados[i]->getArestas()[j]->getPeso();
                    for (int k = 0; k < nos_conectados[i]->getGrau(); k++)
                    {
                        if (searchInVector<No *>(nos_nao_conectados, nos_conectados[i]->getNosAdj()[k]))
                        {
                            peso = nos_conectados[i]->getArestas()[k]->getPeso();
                            idx = k;
                            if (peso < menorPeso)
                            {
                                menorPeso = peso;
                                idx = k;
                            }
                        }
                    }
                    aux_arestas_menorPeso.push_back(nos_conectados[i]->getArestas()[idx]);
                    break;
                }
            }
        }
        // Encontra, dentre as arestas de menor peso dos nós já conectados, aquela que tem o menor peso de todos:
        if (!aux_arestas_menorPeso.empty())
        {
            menorPeso = aux_arestas_menorPeso[0]->getPeso();
            idx = 0;
            for (int i = 0; i < aux_arestas_menorPeso.size(); i++)
            {
                peso = aux_arestas_menorPeso[i]->getPeso();
                if (peso < menorPeso)
                {
                    menorPeso = peso;
                    idx = i;
                }
            }

            // Adiciona a aresta de menor peso encontrada no vetor de solução:
            S.push_back(aux_arestas_menorPeso[idx]);

            // Adiciona os nós da aresta de menor peso encontrada no vetor de nós ja conectados (se eles já não estiverem no vetor):
            if (!searchInVector<No *>(nos_conectados, aux_arestas_menorPeso[idx]->getNo1()))
                nos_conectados.push_back(aux_arestas_menorPeso[idx]->getNo1());
            if (!searchInVector<No *>(nos_conectados, aux_arestas_menorPeso[idx]->getNo2()))
                nos_conectados.push_back(aux_arestas_menorPeso[idx]->getNo2());

            // Remove esses nós do vetor de nós não conectados (se eles estiverem no vetor):
            if (searchInVector<No *>(nos_nao_conectados, aux_arestas_menorPeso[idx]->getNo1()))
                nos_nao_conectados.erase(nos_nao_conectados.begin() + findIndex(nos_nao_conectados, aux_arestas_menorPeso[idx]->getNo1()));
            if (searchInVector<No *>(nos_nao_conectados, aux_arestas_menorPeso[idx]->getNo2()))
                nos_nao_conectados.erase(nos_nao_conectados.begin() + findIndex(nos_nao_conectados, aux_arestas_menorPeso[idx]->getNo2()));

            // Esvazia o vetor auxiliar de arestas de menor peso:
            aux_arestas_menorPeso.clear();
        }
    }

    // arquivo para dot
    fstream arq;
    arq.open("arqDotPrim", ios::app);
    if (!arq.is_open())
    {
        std::cout << "FALHA AO ABRIR O ARQUIVO" << endl;
        exit(0);
    }

    if (!isPonderadoArestas())
    {
        arq << "graph G {" << endl;
        for (int i = 0; i < S.size(); i++)
        {
            arq << S[i]->getNo1()->getId() << "--" << S[i]->getNo2()->getId() << endl;
        }
        arq << "}";
    }

    if (isPonderadoArestas())
    {
        arq << "graph G {" << endl;
        for (int i = 0; i < S.size(); i++)
        {
            arq << S[i]->getNo1()->getId() << "--" << S[i]->getNo2()->getId() << "[label=\"" << S[i]->getPeso() << "\"]" << endl;
        }
        arq << "}";
    }

    arq.close();

    std::cout << "O conjunto solução das arestas da Árvore Geradora Mínima é:" << endl;
    std::cout << "S = {";
    for (int i = 0; i < S.size(); i++)
    {
        std::cout << "(" << S[i]->getNo1()->getId() << ", " << S[i]->getNo2()->getId() << ")";
        if (i + 1 < S.size())
            std::cout << " , ";
    }
    std::cout << "}";

    arq_out << "O conjunto solução das arestas da Árvore Geradora Mínima é:" << endl;
    arq_out << "S = {";
    for (int i = 0; i < S.size(); i++)
    {
        arq_out << "(" << S[i]->getNo1()->getId() << ", " << S[i]->getNo2()->getId() << ")";
        if (i + 1 < S.size())
            arq_out << " , ";
    }
    arq_out << "}";

    arq_out.close();
}

// @author @dudaribeiro7
// @brief Função de comparação cujo critério é o peso da aresta
// @param a/b Duas arestas a serem comparadas
// @return TRUE se o peso da aresta "b" for menor do que o peso da aresta "a" ; FALSE caso contrário
bool compara(Aresta *&a, Aresta *&b)
{
    // Vai haver troca se o peso do segundo for menor que o do primeiro
    return b->getPeso() < a->getPeso();
}

bool arestasIguais(Aresta *aresta, int id_a, int id_b)
{
    int id1 = aresta->getNo1()->getId();
    int id2 = aresta->getNo2()->getId();
    if (((id1 == id_a) && (id2 == id_b)) || ((id1 == id_b) && (id2 == id_a)))
        return true;
    return false;
}

bool verificaArestasIguais(vector<Aresta *> vet, int id_a, int id_b)
{
    for (int i = 0; i < vet.size(); i++)
        if (arestasIguais(vet[i], id_a, id_b))
            return true;
    return false;
}

// @author @dudaribeiro7
// @brief Encontra uma Árvore Geradora Mínima sobre o subgrafo vértice-induzido por X usando o algoritmo de Kruskal
// @param X um subconjunto de vértices de um grafo
void Grafo::kruskal(vector<int> X, string arquivo_saida)
{
    ofstream arq_out;
    arq_out.open(arquivo_saida, ios::app);
    if (!arq_out.is_open())
    {
        std::cout << "FALHA AO ABRIR ARQUIVO DE SAIDA" << endl;
        exit(0);
    }

    Grafo *subgrafo = subgrafoVerticeInduzido(X); // subgrafo vertice induzido por X
    vector<Aresta *> S;                           // vetor de arestas solução
    vector<Aresta *> arestas;                     // vetor com todas as arestas do subgrafo, ordenadas em ordem crescente de pesos
    int c = 0;                                    // número de arestas inseridas na AGM
    int c_max = subgrafo->getNumVertices() - 1;   // número máximo de arestas que podem ser inseridas na AGM

    // Adiciona todas as arestas do grafo no vetor arestas:
    for (int i = 0; i < subgrafo->getNumVertices(); i++)
    {
        for (int j = 0; j < subgrafo->nos_grafo[i]->getGrau(); j++)
        {
            if (!searchInVector<Aresta *>(arestas, subgrafo->nos_grafo[i]->getArestas()[j]))
            {
                int no1_aux = subgrafo->nos_grafo[i]->getArestas()[j]->getNo1()->getId();
                int no2_aux = subgrafo->nos_grafo[i]->getArestas()[j]->getNo2()->getId();
                if (!verificaArestasIguais(arestas, no1_aux, no2_aux))
                    arestas.push_back(subgrafo->nos_grafo[i]->getArestas()[j]);
            }
        }
    }

    // Ordena o vetor arestas:
    std::sort(arestas.begin(), arestas.end(), compara);

    int tam = arestas.size();

    // Subarvores contendo cada uma um nó isolado:
    int n_subarvores = subgrafo->getNumVertices();
    vector<No *> *subarvores = new vector<No *>[n_subarvores];
    for (int i = 0; i < n_subarvores; i++)
        subarvores[i].push_back(subgrafo->nos_grafo[i]);

    // Adiciona a aresta no vetor de soluções se ela não formar ciclos com as arestas que já estão na solução:
    int i = 0;
    while ((c < c_max) && (i < tam))
    {
        No *u = arestas[i]->getNo1();
        No *v = arestas[i]->getNo2();
        bool ciclo = false;

        // Verifica se u e v estão na mesma subárvore:
        for (int j = 0; j < n_subarvores; j++)
        {
            // Procura os nós em todas as subarvores
            if (searchInVector<No *>(subarvores[j], u) && searchInVector<No *>(subarvores[j], v))
            {
                // Se u e v estiverem na mesma subárvore:
                ciclo = true;
                break;
            }
        }

        // Se u e v não estão na mesma subárvore:
        if (!ciclo)
        {
            // Adiciona a aresta na solução:
            S.push_back(arestas[i]);
            c++;

            // Une as subárvores que contém u e v:
            int idx_u, idx_v;
            for (int j = 0; j < n_subarvores; j++)
                if (searchInVector<No *>(subarvores[j], u))
                {
                    idx_u = j;
                    break;
                }
            for (int j = 0; j < n_subarvores; j++)
                if (searchInVector<No *>(subarvores[j], v))
                {
                    idx_v = j;
                    break;
                }
            for (int t = 0; t < subarvores[idx_v].size(); t++)
                subarvores[idx_u].push_back(subarvores[idx_v][t]);
            subarvores[idx_v].clear();
        }

        i++;
    }

    // arquivo para dot
    fstream arq;
    arq.open("arqDotKruskal", ios::app);
    if (!arq.is_open())
    {
        std::cout << "FALHA AO ABRIR O ARQUIVO" << endl;
        exit(0);
    }

    if (!isPonderadoArestas())
    {
        arq << "graph G {" << endl;
        for (int i = 0; i < S.size(); i++)
        {
            arq << S[i]->getNo1()->getId() << "--" << S[i]->getNo2()->getId() << endl;
        }
        arq << "}";
    }

    if (isPonderadoArestas())
    {
        arq << "graph G {" << endl;
        for (int i = 0; i < S.size(); i++)
        {
            arq << S[i]->getNo1()->getId() << "--" << S[i]->getNo2()->getId() << "[label=\"" << S[i]->getPeso() << "\"]" << endl;
        }
        arq << "}";
    }

    arq.close();

    std::cout << "O conjunto solução das arestas da Árvore Geradora Mínima é:" << endl;
    std::cout << "S = {";
    for (int i = 0; i < S.size(); i++)
    {
        std::cout << "(" << S[i]->getNo1()->getId() << ", " << S[i]->getNo2()->getId() << ")";
        if (i + 1 < S.size())
            std::cout << " , ";
    }
    std::cout << "}";

    arq_out << "O conjunto solução das arestas da Árvore Geradora Mínima é:" << endl;
    arq_out << "S = {";
    for (int i = 0; i < S.size(); i++)
    {
        arq_out << "(" << S[i]->getNo1()->getId() << ", " << S[i]->getNo2()->getId() << ")";
        if (i + 1 < S.size())
            arq_out << " , ";
    }
    arq_out << "}";

    arq_out.close();

    delete subgrafo;
    delete[] subarvores;
}

bool Grafo::verificaSolucao(vector<Aresta *> *solucao)
{
    vector<No *> nos_solucao;
    for (int i = 0; i < n_clusters; i++)
        for (int j = 0; j < solucao[i].size(); j++)
        {
            if (!searchInVector<No *>(nos_solucao, solucao[i][j]->getNo1()))
                nos_solucao.push_back(solucao[i][j]->getNo1());

            if (!searchInVector<No *>(nos_solucao, solucao[i][j]->getNo2()))
                nos_solucao.push_back(solucao[i][j]->getNo2());
        }

    for (int i = 0; i < n_vertices; i++)
        if (!searchInVector<No *>(nos_solucao, nos_grafo[i]))
            return false; // se qualquer nó não estiver na solução, ela não é válida.

    return true;
}

//@return qualidade da solução obtida (soma dos pesos das arestas da solução)
float Grafo::algoritmoGuloso()
{
    vector<Aresta *> arestas;                                              // vetor com todas as arestas do subgrafo, ordenadas em ordem crescente de pesos (candidatos)
    vector<Aresta *> *solucao_clusters = new vector<Aresta *>[n_clusters]; // vetor com os grupos de arestas para o algoritmo (solução)
    vector<Grafo *> clusters;                                              // vetor com todas as clusters
    vector<float> soma_pesos_arestas_clusters;                             // vetor com as somas dos pesos das arestas em cada cluster
    vector<float> soma_pesos_nos_clusters;                                 // vetor com as somas dos pesos dos vertices em cada cluster
    vector<int> qual_cluster;                                              // vetor de marcação dos vertices, registrando em qual partição ele se encontra
    vector<int> cont;                                                      // auxiliar

    // Adiciona todas as arestas do grafo no vetor arestas:
    for (int i = 0; i < this->getNumVertices(); i++)
    {
        for (int j = 0; j < this->nos_grafo[i]->getGrau(); j++)
        {
            if (!searchInVector<Aresta *>(arestas, this->nos_grafo[i]->getArestas()[j]))
            {
                int no1_aux = this->nos_grafo[i]->getArestas()[j]->getNo1()->getId();
                int no2_aux = this->nos_grafo[i]->getArestas()[j]->getNo2()->getId();
                if (!verificaArestasIguais(arestas, no1_aux, no2_aux))
                    arestas.push_back(this->nos_grafo[i]->getArestas()[j]);
            }
        }
    }

    // Ordena o vetor arestas:
    std::sort(arestas.begin(), arestas.end(), compara);

    // std::cout << "vetor de arestas candidatas: ";
    // for(int i = 0; i < arestas.size(); i++)
    //     std::cout << "(" << arestas[i]->getNo1()->getId() << ", " << arestas[i]->getNo2()->getId() << "); ";
    // std::cout << endl << endl;

    // inicializa o vetor de marcação
    for (int i = 0; i < this->getNumVertices(); i++)
        qual_cluster.push_back(-1);

    for (int i = 0; i < this->getNumVertices(); i++)
        cont.push_back(0);

    // inicializa o vetor de limites
    for (int i = 0; i < n_clusters; i++)
        soma_pesos_arestas_clusters.push_back(0);

    // inicializa o vetor de limites
    for (int i = 0; i < n_clusters; i++)
        soma_pesos_nos_clusters.push_back(0);

    // preenchimento do vetor de vetores solução
    while (!(arestas.empty()))
    {
        for (int i = 0; i < n_clusters; i++)
        {
            if (arestas.empty())
                break;

            float peso1 = arestas.front()->getNo1()->getPesoNo();
            float peso2 = arestas.front()->getNo2()->getPesoNo();
            int id1 = arestas.front()->getNo1()->getId();
            int id2 = arestas.front()->getNo2()->getId();

            // se nenhum dos dois vertices está em nenhuma partição:
            if ((qual_cluster[id1] == qual_cluster[id2]) && (qual_cluster[id1] == -1))
            {
                if (ccapacity == -1)
                {
                    // se a soma dos pesos dos vertices nao exceder o limite superior:
                    if ((soma_pesos_nos_clusters[i] + peso1 + peso2) <= this->U[i])
                    {
                        // adiciona a aresta na partição i:
                        solucao_clusters[i].push_back(arestas.front());
                        // atualiza a soma dos pesos das arestas da partição i:
                        soma_pesos_arestas_clusters[i] += arestas.front()->getPeso();
                        // atualiza a soma dos pesos dos vertices da partição i:
                        soma_pesos_nos_clusters[i] += peso1 + peso2;
                        // atualiza os vetores com as partições de cada vertice adicionado:
                        qual_cluster[id1] = i;
                        qual_cluster[id2] = i;
                        // remove a aresta do vetor de arestas:
                        arestas.erase(arestas.begin());
                    }
                    else
                    {
                        cont[id1]++;
                        cont[id2]++;
                        if (cont[id1] >= n_clusters || cont[id2] >= n_clusters)
                        {
                            cont[id1] = 0;
                            cont[id2] = 0;
                            arestas.erase(arestas.begin());
                        }
                    }
                }
                else
                {
                    // se a soma dos pesos dos vertices nao exceder o limite superior:
                    if ((soma_pesos_nos_clusters[i] + peso1 + peso2) <= ccapacity)
                    {
                        // adiciona a aresta na partição i:
                        solucao_clusters[i].push_back(arestas.front());
                        // atualiza a soma dos pesos das arestas da partição i:
                        soma_pesos_arestas_clusters[i] += arestas.front()->getPeso();
                        // atualiza a soma dos pesos dos vertices da partição i:
                        soma_pesos_nos_clusters[i] += peso1 + peso2;
                        // atualiza os vetores com as partições de cada vertice adicionado:
                        qual_cluster[id1] = i;
                        qual_cluster[id2] = i;
                        // remove a aresta do vetor de arestas:
                        arestas.erase(arestas.begin());
                    }
                    else
                    {
                        cont[id1]++;
                        cont[id2]++;
                        if (cont[id1] >= n_clusters || cont[id2] >= n_clusters)
                        {
                            cont[id1] = 0;
                            cont[id2] = 0;
                            arestas.erase(arestas.begin());
                        }
                    }
                }
            }

            // se os dois nós estão na partição atual:
            else if ((qual_cluster[id1] == qual_cluster[id2]) && (qual_cluster[id1] == i))
            {
                // adiciona a aresta na partição i:
                solucao_clusters[i].push_back(arestas.front());
                // atualiza a soma dos pesos das arestas da partição i:
                soma_pesos_arestas_clusters[i] += arestas.front()->getPeso();
                // remove a aresta do vetor de arestas:
                arestas.erase(arestas.begin());
            }

            // se o vertice 1 não está em nenhuma partição e o vertice 2 está na partição atual:
            else if ((qual_cluster[id1] == -1) && (qual_cluster[id2] == i))
            {
                if (ccapacity == -1)
                {
                    // se a soma dos pesos dos vertices nao exceder o limite superior:
                    if ((soma_pesos_nos_clusters[i] + peso1) <= this->U[i])
                    {
                        // adiciona a aresta na partição i:
                        solucao_clusters[i].push_back(arestas.front());
                        // atualiza a soma dos pesos das arestas da partição i:
                        soma_pesos_arestas_clusters[i] += arestas.front()->getPeso();
                        // atualiza a soma dos pesos dos vertices da partição i:
                        soma_pesos_nos_clusters[i] += peso1;
                        // atualiza os vetores com as partições de cada vertice adicionado:
                        qual_cluster[id1] = i;
                        // remove a aresta do vetor de arestas:
                        arestas.erase(arestas.begin());
                    }
                    else
                    {
                        // remove a aresta do vetor de arestas:
                        arestas.erase(arestas.begin());
                    }
                }
                else
                {
                    // se a soma dos pesos dos vertices nao exceder o limite superior:
                    if ((soma_pesos_nos_clusters[i] + peso1) <= ccapacity)
                    {
                        // adiciona a aresta na partição i:
                        solucao_clusters[i].push_back(arestas.front());
                        // atualiza a soma dos pesos das arestas da partição i:
                        soma_pesos_arestas_clusters[i] += arestas.front()->getPeso();
                        // atualiza a soma dos pesos dos vertices da partição i:
                        soma_pesos_nos_clusters[i] += peso1;
                        // atualiza os vetores com as partições de cada vertice adicionado:
                        qual_cluster[id1] = i;
                        // remove a aresta do vetor de arestas:
                        arestas.erase(arestas.begin());
                    }
                    else
                    {
                        // remove a aresta do vetor de arestas:
                        arestas.erase(arestas.begin());
                    }
                }
            }

            // se o vertice 2 não está em nenhuma partição e o vertice 1 está na partição atual:
            else if ((qual_cluster[id1] == i) && (qual_cluster[id2] == -1))
            {
                if (ccapacity == -1)
                {
                    // se a soma dos pesos dos vertices nao exceder o limite superior:
                    if ((soma_pesos_nos_clusters[i] + peso2) <= this->U[i])
                    {
                        // adiciona a aresta na partição i:
                        solucao_clusters[i].push_back(arestas.front());
                        // atualiza a soma dos pesos das arestas da partição i:
                        soma_pesos_arestas_clusters[i] += arestas.front()->getPeso();
                        // atualiza a soma dos pesos dos vertices da partição i:
                        soma_pesos_nos_clusters[i] += peso2;
                        // atualiza os vetores com as partições de cada vertice adicionado:
                        qual_cluster[id2] = i;
                        // remove a aresta do vetor de arestas:
                        arestas.erase(arestas.begin());
                    }
                    else
                    {
                        // remove a aresta do vetor de arestas:
                        arestas.erase(arestas.begin());
                    }
                }
                else
                {
                    // se a soma dos pesos dos vertices nao exceder o limite superior:
                    if ((soma_pesos_nos_clusters[i] + peso2) <= ccapacity)
                    {
                        // adiciona a aresta na partição i:
                        solucao_clusters[i].push_back(arestas.front());
                        // atualiza a soma dos pesos das arestas da partição i:
                        soma_pesos_arestas_clusters[i] += arestas.front()->getPeso();
                        // atualiza a soma dos pesos dos vertices da partição i:
                        soma_pesos_nos_clusters[i] += peso2;
                        // atualiza os vetores com as partições de cada vertice adicionado:
                        qual_cluster[id2] = i;
                        // remove a aresta do vetor de arestas:
                        arestas.erase(arestas.begin());
                    }
                    else
                    {
                        // remove a aresta do vetor de arestas:
                        arestas.erase(arestas.begin());
                    }
                }
            }

            // se os dois vertices estão em partições diferentes:
            else if ((qual_cluster[id1] != qual_cluster[id2]) && (qual_cluster[id1] != -1) && (qual_cluster[id2] != -1))
            {
                // remove a aresta do vetor de arestas:
                arestas.erase(arestas.begin());
            }
        }
    }

    bool foiPossivel = true;
    for (int i = 0; i < n_clusters; i++)
    {
        if (ccapacity == -1)
        {
            if (soma_pesos_nos_clusters[i] < this->L[i])
                foiPossivel = false;
        }
        else
        {
            if (soma_pesos_nos_clusters[i] > ccapacity)
                foiPossivel = false;
        }
    }

    for (int i = 0; i < n_clusters; i++)
    {
        Grafo *subgrafo = subgrafoArestaInduzido(solucao_clusters[i]);
        clusters.push_back(subgrafo);
    }

    if (foiPossivel)
        std::cout << "Foi possível resolver o Problema da Clusterização Capacitada!" << endl
                  << "Resultado:" << endl
                  << endl;
    else if (!foiPossivel)
        std::cout << "Não foi possível resolver o Problema da Clusterização Capacitada com esse algoritmo, pois existem clusters com a soma dos pesos dos nós menor do que o limite inferior." << endl
                  << "Resultado:" << endl
                  << endl;
    else if (!verificaSolucao(solucao_clusters))
        std::cout << "Não foi possível resolver o Problema da Clusterização Capacitada com esse algoritmo, pois nem todos os nós do grafo estão presentes nas clusters." << endl
                  << "Resultado:" << endl
                  << endl;

    for (int i = 0; i < n_clusters; i++)
    {
        std::cout << "Cluster " << i + 1 << ":" << endl;
        if (ccapacity == -1)
        {
            std::cout << "Limite inferior: " << L[i] << endl;
            std::cout << "Limite superior: " << U[i] << endl;
        }
        else
        {
            std::cout << "Capacidade maxima: " << ccapacity << endl;
        }
        std::cout << "Soma dos pesos dos nós: " << soma_pesos_nos_clusters[i] << endl;
        std::cout << "Soma dos pesos das arestas: " << soma_pesos_arestas_clusters[i] << endl;
        clusters[i]->printGrafo();
        std::cout << endl;
    }

    float qualidade = 0;
    for (int i = 0; i < n_clusters; i++)
        qualidade += soma_pesos_arestas_clusters[i];

    delete[] solucao_clusters;

    return qualidade;
}

int Grafo::randomRange(int min, int max)
{
    static bool first = true;
    if (first)
    {
        if(ccapacity==-1) //se não for handover   
            seed=time(NULL);
        srand(seed);
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}

float Grafo::algoritmoGulosoRandomizadoAdaptativo(float alfa, int n_iter)
{
    vector<Aresta *> arestas;                                                  // vetor com todas as arestas do subgrafo, ordenadas em ordem crescente de pesos (candidatos)
    vector<Aresta *> *solucao_clusters = new vector<Aresta *>[n_clusters];     // vetor com os grupos de arestas para o algoritmo (solução)
    vector<Aresta *> *BESTsolucao_clusters = new vector<Aresta *>[n_clusters]; // vetor com os grupos de arestas para o algoritmo (solução)
    vector<Grafo *> clusters;                                                  // vetor com todas as clusters
    vector<float> soma_pesos_arestas_clusters;                                 // vetor com as somas dos pesos das arestas em cada cluster
    vector<float> BESTsoma_pesos_arestas_clusters;                             // vetor com as somas dos pesos das arestas em cada cluster da melhor solução
    vector<float> soma_pesos_nos_clusters;                                     // vetor com as somas dos pesos dos vertices em cada cluster
    vector<float> BESTsoma_pesos_nos_clusters;                                 // vetor com as somas dos pesos dos vertices em cada cluster da melhor solução
    vector<int> qual_cluster;                                                  // vetor de marcação dos vertices, registrando em qual partição ele se encontra
    vector<int> cont;                                                          // auxiliar

    for (int i = 0; i < n_clusters; i++)
        BESTsoma_pesos_arestas_clusters.push_back(0);

    for (int i = 0; i < n_clusters; i++)
        BESTsoma_pesos_nos_clusters.push_back(0);

    for (int n = 1; n <= n_iter; n++)
    {
        if (!arestas.empty())
            arestas.clear();

        for (int i = 0; i < n_clusters; i++)
            if (!solucao_clusters[i].empty())
                solucao_clusters[i].clear();

        if (!soma_pesos_arestas_clusters.empty())
            soma_pesos_arestas_clusters.clear();

        if (!soma_pesos_nos_clusters.empty())
            soma_pesos_nos_clusters.clear();

        if (!qual_cluster.empty())
            qual_cluster.clear();

        if (!cont.empty())
            cont.clear();

        // Adiciona todas as arestas do grafo no vetor arestas:
        for (int i = 0; i < this->getNumVertices(); i++)
        {
            for (int j = 0; j < this->nos_grafo[i]->getGrau(); j++)
            {
                if (!searchInVector<Aresta *>(arestas, this->nos_grafo[i]->getArestas()[j]))
                {
                    int no1_aux = this->nos_grafo[i]->getArestas()[j]->getNo1()->getId();
                    int no2_aux = this->nos_grafo[i]->getArestas()[j]->getNo2()->getId();
                    if (!verificaArestasIguais(arestas, no1_aux, no2_aux))
                        arestas.push_back(this->nos_grafo[i]->getArestas()[j]);
                }
            }
        }

        // Ordena o vetor arestas:
        std::sort(arestas.begin(), arestas.end(), compara);

        // inicializa o vetor de marcação
        for (int i = 0; i < this->getNumVertices(); i++)
            qual_cluster.push_back(-1);

        for (int i = 0; i < this->getNumVertices(); i++)
            cont.push_back(0);

        // inicializa o vetor de limites
        for (int i = 0; i < n_clusters; i++)
            soma_pesos_arestas_clusters.push_back(0);

        // inicializa o vetor de limites
        for (int i = 0; i < n_clusters; i++)
            soma_pesos_nos_clusters.push_back(0);

        // preenchimento do vetor de vetores solução
        while (!(arestas.empty()))
        {
            for (int i = 0; i < n_clusters; i++)
            {
                if (arestas.empty())
                    break;

                int k = randomRange(0, alfa * arestas.size() - 1);
                float peso1 = arestas[k]->getNo1()->getPesoNo();
                float peso2 = arestas[k]->getNo2()->getPesoNo();
                int id1 = arestas[k]->getNo1()->getId();
                int id2 = arestas[k]->getNo2()->getId();

                // se nenhum dos dois vertices está em nenhuma partição:
                if ((qual_cluster[id1] == qual_cluster[id2]) && (qual_cluster[id1] == -1))
                {
                    if (ccapacity == -1)
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso1 + peso2) <= this->U[i])
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso1 + peso2;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id1] = i;
                            qual_cluster[id2] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            cont[id1]++;
                            cont[id2]++;
                            if (cont[id1] >= n_clusters || cont[id2] >= n_clusters)
                            {
                                cont[id1] = 0;
                                cont[id2] = 0;
                                arestas.erase(arestas.begin() + k);
                            }
                        }
                    }
                    else
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso1 + peso2) <= this->ccapacity)
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso1 + peso2;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id1] = i;
                            qual_cluster[id2] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            cont[id1]++;
                            cont[id2]++;
                            if (cont[id1] >= n_clusters || cont[id2] >= n_clusters)
                            {
                                cont[id1] = 0;
                                cont[id2] = 0;
                                arestas.erase(arestas.begin() + k);
                            }
                        }
                    }
                }

                // se os dois nós estão na partição atual:
                else if ((qual_cluster[id1] == qual_cluster[id2]) && (qual_cluster[id1] == i))
                {
                    // adiciona a aresta na partição i:
                    solucao_clusters[i].push_back(arestas[k]);
                    // atualiza a soma dos pesos das arestas da partição i:
                    soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                    // remove a aresta do vetor de arestas:
                    arestas.erase(arestas.begin() + k);
                }

                // se o vertice 1 não está em nenhuma partição e o vertice 2 está na partição atual:
                else if ((qual_cluster[id1] == -1) && (qual_cluster[id2] == i))
                {
                    if (ccapacity == -1)
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso1) <= this->U[i])
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso1;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id1] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                    }
                    else
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso1) <= ccapacity)
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso1;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id1] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                    }
                }

                // se o vertice 2 não está em nenhuma partição e o vertice 1 está na partição atual:
                else if ((qual_cluster[id1] == i) && (qual_cluster[id2] == -1))
                {
                    if (ccapacity == -1)
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso2) <= this->U[i])
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso2;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id2] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                    }
                    else
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso2) <= ccapacity)
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso2;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id2] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                    }
                }

                // se os dois vertices estão em partições diferentes:
                else if ((qual_cluster[id1] != qual_cluster[id2]) && (qual_cluster[id1] != -1) && (qual_cluster[id2] != -1))
                {
                    // remove a aresta do vetor de arestas:
                    arestas.erase(arestas.begin() + k);
                }
            }
        }

        bool foiPossivel = true;
        for (int i = 0; i < n_clusters; i++)
        {
            if (ccapacity == -1)
            {
                if (soma_pesos_nos_clusters[i] < this->L[i] || !verificaSolucao(solucao_clusters))
                    foiPossivel = false;
            }
            else
            {
                if (soma_pesos_nos_clusters[i] > ccapacity || !verificaSolucao(solucao_clusters))
                    foiPossivel = false;
            }
        }

        // se a solução atual for possível e melhor do que a melhor solução:
        float soma_total_arestas = 0;
        float BESTsoma_total_arestas = 0;
        for (int i = 0; i < n_clusters; i++)
            soma_total_arestas += soma_pesos_arestas_clusters[i];

        for (int i = 0; i < n_clusters; i++)
            BESTsoma_total_arestas += BESTsoma_pesos_arestas_clusters[i];

        if (soma_total_arestas > BESTsoma_total_arestas && foiPossivel)
        {
            BESTsolucao_clusters->clear();
            BESTsolucao_clusters = solucao_clusters;

            BESTsoma_pesos_arestas_clusters.clear();
            BESTsoma_pesos_arestas_clusters = soma_pesos_arestas_clusters;

            BESTsoma_pesos_nos_clusters.clear();
            BESTsoma_pesos_nos_clusters = soma_pesos_nos_clusters;
        }
    }

    // bool foiPossivel = true;
    // for(int i = 0; i < n_clusters; i++)
    //     if(BESTsoma_pesos_nos_clusters[i] < this->L[i])
    //         foiPossivel = false;

    // for(int i = 0; i < n_clusters; i++)
    // {
    //     Grafo* subgrafo = subgrafoArestaInduzido(BESTsolucao_clusters[i]);
    //     clusters.push_back(subgrafo);
    // }

    // if(foiPossivel)
    //     std::cout << "Foi possível resolver o Problema da Clusterização Capacitada!" << endl << "Resultado:" << endl << endl;
    // else if(!foiPossivel)
    //     std::cout << "Não foi possível resolver o Problema da Clusterização Capacitada com esse algoritmo, pois existem clusters com a soma dos pesos dos nós menor do que o limite inferior." << endl << "Resultado:" << endl << endl;
    // else if(!verificaSolucao(solucao_clusters))
    //     std::cout << "Não foi possível resolver o Problema da Clusterização Capacitada com esse algoritmo, pois nem todos os nós do grafo estão presentes nas clusters." << endl << "Resultado:" << endl << endl;

    // for(int i = 0; i < n_clusters; i++)
    // {
    //     std::cout << "Cluster " << i+1 << ":" << endl;
    //     std::cout << "Limite inferior: " << L[i] << endl;
    //     std::cout << "Limite superior: " << U[i] << endl;
    //     std::cout << "Soma dos pesos dos nós: " << BESTsoma_pesos_nos_clusters[i] << endl;
    //     std::cout << "Soma dos pesos das arestas: " << BESTsoma_pesos_arestas_clusters[i] << endl;
    //     clusters[i]->printGrafo();
    //     std::cout << endl;
    // }

    float qualidade = 0;
    for (int i = 0; i < n_clusters; i++)
        qualidade += BESTsoma_pesos_arestas_clusters[i];

    delete[] BESTsolucao_clusters;
    // delete[] solucao_clusters;

    return qualidade;
}

void Grafo::atualizaCoeficientes(float coeficientes[], float melhorSolucao[], float mediaSolucoes[], int tamanho)
{
    // atualização dos coeficientes feito de acordo com o que foi dado pelo professor
    for (int i = 0; i < tamanho; i++)
        coeficientes[i] = melhorSolucao[i] / mediaSolucoes[i];
}

void Grafo::AtualizaProbabilidade(float probabilidade[], float coeficientes[], int tamanho)
{
    // atualização das probabilidades feito de acordo com o que foi dado pelo professor
    int somaCoeficientes = 0;
    for (int i = 0; i < tamanho; i++)
        somaCoeficientes += coeficientes[i];

    for (int i = 0; i < tamanho; i++)
        probabilidade[i] = coeficientes[i] / somaCoeficientes;
}

float Grafo::escolheAlfa(float probabilidade[], int tamanho)
{
    int limites[tamanho + 1];        // limites para o sorteio + 1 (limite minimo), demais limites são definidos apartir das probabilidades
    int sorte = randomRange(0, 100); // sorteia um numero aleatorio de 0 a 100
    limites[0] = 0;                  // define o limite minimo
    for (int i = 1; i < tamanho; i++)
        limites[i] = limites[i - 1] + probabilidade[i - 1] * 100; // define os limites intermediarios e o limite maximo (100)

    float retorno;
    // encontra qual alfa foi sorteado
    for (int i = 0; i < tamanho; i++)
    {
        if (i == tamanho)
        {
            retorno = (float)i;
            break;
        }

        if ((sorte > limites[i]) && (sorte < limites[i + 1]))
        {
            retorno = (float)i;
            break;
        }
    }
    return retorno;
}

float Grafo::algoritmoGulosoRandomizadoReativo(vector<float> alfaVec, int n_iter, int bloco, string arquivo_saida)
{
    vector<Aresta *> arestas;                                                  // vetor com todas as arestas do subgrafo, ordenadas em ordem crescente de pesos (candidatos)
    vector<Aresta *> *solucao_clusters = new vector<Aresta *>[n_clusters];     // vetor com os grupos de arestas para o algoritmo (solução)
    vector<Aresta *> *BESTsolucao_clusters = new vector<Aresta *>[n_clusters]; // vetor com os grupos de arestas para o algoritmo (solução)
    vector<Grafo *> clusters;                                                  // vetor com todas as clusters
    vector<float> soma_pesos_arestas_clusters;                                 // vetor com as somas dos pesos das arestas em cada cluster
    vector<float> BESTsoma_pesos_arestas_clusters;                             // vetor com as somas dos pesos das arestas em cada cluster da melhor solução
    vector<float> soma_pesos_nos_clusters;                                     // vetor com as somas dos pesos dos vertices em cada cluster
    vector<float> BESTsoma_pesos_nos_clusters;                                 // vetor com as somas dos pesos dos vertices em cada cluster da melhor solução
    vector<int> qual_cluster;                                                  // vetor de marcação dos vertices, registrando em qual partição ele se encontra
    float *melhorSolucao = new float[alfaVec.size()];                          // a melhor solucao para cada alfa
    float *mediaSolucoes = new float[alfaVec.size()];                          // media das solucoes de cada alfa
    float *somaSolucoes = new float[alfaVec.size()];                           // soma das solucoes de cada alfa
    float *NSolucoes = new float[alfaVec.size()];                              // o numer de solucoes encontradas de cada alfa
    float *coeficientes = new float[alfaVec.size()];                           // coeficiente que rege a probabilidade de um alfa ser escolhido
    float *probabilidade = new float[alfaVec.size()];                          // probabilidade de escolha de cada alfa
    vector<int> cont;                                                          // auxiliar
    int contaBloco = 1;
    float BESTalfa;

    for (int i = 0; i < n_clusters; i++)
        BESTsoma_pesos_arestas_clusters.push_back(0);

    for (int i = 0; i < n_clusters; i++)
        BESTsoma_pesos_nos_clusters.push_back(0);

    // inicializa os componentes necessarios para a parte reativa do algoritmo
    for (int i = 0; i < alfaVec.size(); i++)
    {
        melhorSolucao[i] = 1;
        mediaSolucoes[i] = 1;
        somaSolucoes[i] = 0;
        NSolucoes[i] = 0;
        probabilidade[i] = 1 / alfaVec.size();
    }

    for (int n = 1; n <= n_iter; n++)
    {
        // atualiza os coeficientes de cada alfa
        atualizaCoeficientes(coeficientes, melhorSolucao, mediaSolucoes, alfaVec.size());

        // atualiza as probabilidades de cada alfa
        if (contaBloco % bloco == 0)
            AtualizaProbabilidade(probabilidade, coeficientes, alfaVec.size());
        contaBloco++;

        // escolhe o alfa
        int alfaId = escolheAlfa(probabilidade, alfaVec.size());
        float alfa = alfaVec[alfaId];

        if (!arestas.empty())
            arestas.clear();

        for (int i = 0; i < n_clusters; i++)
            if (!solucao_clusters[i].empty())
                solucao_clusters[i].clear();

        if (!soma_pesos_arestas_clusters.empty())
            soma_pesos_arestas_clusters.clear();

        if (!soma_pesos_nos_clusters.empty())
            soma_pesos_nos_clusters.clear();

        if (!qual_cluster.empty())
            qual_cluster.clear();

        if (!cont.empty())
            cont.clear();

        // Adiciona todas as arestas do grafo no vetor arestas:
        for (int i = 0; i < this->getNumVertices(); i++)
        {
            for (int j = 0; j < this->nos_grafo[i]->getGrau(); j++)
            {
                if (!searchInVector<Aresta *>(arestas, this->nos_grafo[i]->getArestas()[j]))
                {
                    int no1_aux = this->nos_grafo[i]->getArestas()[j]->getNo1()->getId();
                    int no2_aux = this->nos_grafo[i]->getArestas()[j]->getNo2()->getId();
                    if (!verificaArestasIguais(arestas, no1_aux, no2_aux))
                        arestas.push_back(this->nos_grafo[i]->getArestas()[j]);
                }
            }
        }

        // Ordena o vetor arestas:
        std::sort(arestas.begin(), arestas.end(), compara);

        // inicializa o vetor de marcação
        for (int i = 0; i < this->getNumVertices(); i++)
            qual_cluster.push_back(-1);

        for (int i = 0; i < this->getNumVertices(); i++)
            cont.push_back(0);

        // inicializa o vetor de limites
        for (int i = 0; i < n_clusters; i++)
            soma_pesos_arestas_clusters.push_back(0);

        // inicializa o vetor de limites
        for (int i = 0; i < n_clusters; i++)
            soma_pesos_nos_clusters.push_back(0);

        // preenchimento do vetor de vetores solução
        while (!(arestas.empty()))
        {
            for (int i = 0; i < n_clusters; i++)
            {
                if (arestas.empty())
                    break;

                int k = randomRange(0, alfa * arestas.size() - 1);
                float peso1 = arestas[k]->getNo1()->getPesoNo();
                float peso2 = arestas[k]->getNo2()->getPesoNo();
                int id1 = arestas[k]->getNo1()->getId();
                int id2 = arestas[k]->getNo2()->getId();

                // se nenhum dos dois vertices está em nenhuma partição:
                if ((qual_cluster[id1] == qual_cluster[id2]) && (qual_cluster[id1] == -1))
                {
                    if (ccapacity == -1)
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso1 + peso2) <= this->U[i])
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso1 + peso2;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id1] = i;
                            qual_cluster[id2] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            cont[id1]++;
                            cont[id2]++;
                            if (cont[id1] >= n_clusters || cont[id2] >= n_clusters)
                            {
                                cont[id1] = 0;
                                cont[id2] = 0;
                                arestas.erase(arestas.begin() + k);
                            }
                        }
                    }
                    else
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso1 + peso2) <= ccapacity)
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso1 + peso2;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id1] = i;
                            qual_cluster[id2] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            cont[id1]++;
                            cont[id2]++;
                            if (cont[id1] >= n_clusters || cont[id2] >= n_clusters)
                            {
                                cont[id1] = 0;
                                cont[id2] = 0;
                                arestas.erase(arestas.begin() + k);
                            }
                        }
                    }
                }

                // se os dois nós estão na partição atual:
                else if ((qual_cluster[id1] == qual_cluster[id2]) && (qual_cluster[id1] == i))
                {
                    // adiciona a aresta na partição i:
                    solucao_clusters[i].push_back(arestas[k]);
                    // atualiza a soma dos pesos das arestas da partição i:
                    soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                    // remove a aresta do vetor de arestas:
                    arestas.erase(arestas.begin() + k);
                }

                // se o vertice 1 não está em nenhuma partição e o vertice 2 está na partição atual:
                else if ((qual_cluster[id1] == -1) && (qual_cluster[id2] == i))
                {
                    if (ccapacity == -1)
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso1) <= this->U[i])
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso1;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id1] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                    }
                    else
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso1) <= ccapacity)
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso1;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id1] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                    }
                }

                // se o vertice 2 não está em nenhuma partição e o vertice 1 está na partição atual:
                else if ((qual_cluster[id1] == i) && (qual_cluster[id2] == -1))
                {
                    if (ccapacity == -1)
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso2) <= this->U[i])
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso2;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id2] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                    }
                    else
                    {
                        // se a soma dos pesos dos vertices nao exceder o limite superior:
                        if ((soma_pesos_nos_clusters[i] + peso2) <= ccapacity)
                        {
                            // adiciona a aresta na partição i:
                            solucao_clusters[i].push_back(arestas[k]);
                            // atualiza a soma dos pesos das arestas da partição i:
                            soma_pesos_arestas_clusters[i] += arestas[k]->getPeso();
                            // atualiza a soma dos pesos dos vertices da partição i:
                            soma_pesos_nos_clusters[i] += peso2;
                            // atualiza os vetores com as partições de cada vertice adicionado:
                            qual_cluster[id2] = i;
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                        else
                        {
                            // remove a aresta do vetor de arestas:
                            arestas.erase(arestas.begin() + k);
                        }
                    }
                }

                // se os dois vertices estão em partições diferentes:
                else if ((qual_cluster[id1] != qual_cluster[id2]) && (qual_cluster[id1] != -1) && (qual_cluster[id2] != -1))
                {
                    // remove a aresta do vetor de arestas:
                    arestas.erase(arestas.begin() + k);
                }
            }
        }

        // avalia se as exigencias foram atendidas
        bool foiPossivel = true;
        for (int i = 0; i < n_clusters; i++)
        {
            if (ccapacity == -1)
            {
                if (soma_pesos_nos_clusters[i] < this->L[i] || !verificaSolucao(solucao_clusters))
                    foiPossivel = false;
            }
            else
            {
                if (soma_pesos_nos_clusters[i] > ccapacity)
                {
                    foiPossivel = false;
                }
            }
        }

        // se a solução atual for possível e melhor do que a melhor solução:
        float soma_total_arestas = 0;
        float BESTsoma_total_arestas = 0;
        for (int i = 0; i < n_clusters; i++)
            soma_total_arestas += soma_pesos_arestas_clusters[i];

        for (int i = 0; i < n_clusters; i++)
            BESTsoma_total_arestas += BESTsoma_pesos_arestas_clusters[i];

        if (soma_total_arestas > BESTsoma_total_arestas && foiPossivel)
        {
            BESTalfa = alfa;

            BESTsolucao_clusters->clear();
            BESTsolucao_clusters = solucao_clusters;

            BESTsoma_pesos_arestas_clusters.clear();
            BESTsoma_pesos_arestas_clusters = soma_pesos_arestas_clusters;

            BESTsoma_pesos_nos_clusters.clear();
            BESTsoma_pesos_nos_clusters = soma_pesos_nos_clusters;
        }

        // atualiza o historico necessario para a parte reativa do codigo
        somaSolucoes[alfaId] += soma_total_arestas;
        melhorSolucao[alfaId] += BESTsoma_total_arestas;
        NSolucoes[alfaId]++;
        mediaSolucoes[alfaId] = somaSolucoes[alfaId] / NSolucoes[alfaId];
    }

    bool foiPossivel = true;
    for (int i = 0; i < n_clusters; i++)
    {
        if (ccapacity == -1)
        {
            if (BESTsoma_pesos_nos_clusters[i] < this->L[i])
                foiPossivel = false;
        }
        else
        {
            if (BESTsoma_pesos_nos_clusters[i] > ccapacity)
                foiPossivel = false;
        }
    }

    // abrindo arquivo de saida
    ofstream arq_out;
    arq_out.open(arquivo_saida, ios::app);
    if (!arq_out.is_open())
    {
        std::cout << "FALHA AO ABRIR ARQUIVO DE SAIDA" << endl;
        exit(0);
    }

    // IMPRIMIR E ARQUIVAR O VALOR DE ALFA AQUI
    std::cout << "O valor de alfa para a melhor solução é " << BESTalfa << endl;
    arq_out << "O valor de alfa para a melhor solução é " << BESTalfa << endl;

    for (int i = 0; i < n_clusters; i++)
    {
        Grafo *subgrafo = subgrafoArestaInduzido(BESTsolucao_clusters[i]);
        clusters.push_back(subgrafo);
    }

    if (foiPossivel)
        std::cout << "Foi possível resolver o Problema da Clusterização Capacitada!" << endl
                  << "Resultado:" << endl
                  << endl;
    else if (!foiPossivel)
        std::cout << "Não foi possível resolver o Problema da Clusterização Capacitada com esse algoritmo, pois existem clusters com a soma dos pesos dos nós menor do que o limite inferior." << endl
                  << "Resultado:" << endl
                  << endl;
    else if (!verificaSolucao(solucao_clusters))
        std::cout << "Não foi possível resolver o Problema da Clusterização Capacitada com esse algoritmo, pois nem todos os nós do grafo estão presentes nas clusters." << endl
                  << "Resultado:" << endl
                  << endl;

    for (int i = 0; i < n_clusters; i++)
    {
        std::cout << "Cluster " << i + 1 << ":" << endl;
        if (ccapacity == -1)
        {
            std::cout << "Limite inferior: " << L[i] << endl;
            std::cout << "Limite superior: " << U[i] << endl;
        }
        else
        {
            std::cout << "Capacidade maxima: " << ccapacity << endl;
        }
        std::cout << "Soma dos pesos dos nós: " << BESTsoma_pesos_nos_clusters[i] << endl;
        std::cout << "Soma dos pesos das arestas: " << BESTsoma_pesos_arestas_clusters[i] << endl;
        clusters[i]->printGrafo();
        std::cout << endl;
    }

    float qualidade = 0;
    for (int i = 0; i < n_clusters; i++)
        qualidade += BESTsoma_pesos_arestas_clusters[i];

    delete[] BESTsolucao_clusters;
    // delete[] solucao_clusters;
    delete[] probabilidade;
    delete[] coeficientes;
    delete[] melhorSolucao;
    delete[] mediaSolucoes;
    delete[] somaSolucoes;
    delete[] NSolucoes;

    arq_out.close();

    return qualidade;
}