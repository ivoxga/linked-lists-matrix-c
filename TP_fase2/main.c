#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

/**
 * @brief Estrutura que representa uma aresta no grafo.
 *
 * Esta estrutura é usada para definir uma aresta que conecta dois vértices do grafo.
 * Cada aresta possui um vértice de destino e um apontador para a próxima aresta na lista.
 */
typedef struct aresta {
    int vertice_destino;        ///< O vértice de destino desta aresta.
    struct aresta* seguinte;    ///< apontador para a próxima aresta na lista.
} Aresta;

/**
 * @brief Estrutura que representa um vértice no grafo.
 *
 * Esta estrutura é usada para definir um vértice no grafo.
 * Cada vértice possui um valor de origem e apontadores para as suas arestas associadas
 * e para o próximo vértice na lista.
 */
typedef struct vertice {
    int vertice_origem;                 ///< O vértice de origem.
    struct vertice* vertice_seguinte;   ///< apontador para o próximo vértice na lista.
    struct aresta* aresta_lado;         ///< apontador para a uma aresta deste vértice.
    struct aresta* aresta_abaixo;       ///< apontador para a uma outra aresta deste vértice.
} Vertice;

/**
 * @brief Verifica a existência e o conteúdo de um documento especificado pelo nome do arquivo na 'main()'.
 *
 * Esta função verifica se o documento especificado pelo nome do arquivo na
 *'main()' existe e se contém algum conteúdo.
 *
 * @param nomeFicheiro -> String que contém o nome do arquivo a ser analisado.
 *
 * @return -1 -> Se o documento não existir ou se estiver vazio.
 * @return 0 -> Se o documento existir e contiver conteúdo.
 */
int lerConteudo(const char nomeFicheiro[]){
    FILE* fp = fopen(nomeFicheiro, "r");    // Abre o arquivo para leitura.

    if (fp == NULL){
        printf("ERRO! Documento não encontrado.\n");
        return(-1);
    }

    else{
        // Verifica o tamanho do arquivo.
        fseek(fp, 0, SEEK_END);         // Mover o indicador de posição para o final do arquivo.
        long tamanhoDoc = ftell(fp);    // Obter a posição do indicador de posição (tamanho do arquivo).

        if (tamanhoDoc == 0){
            printf("ERRO! Documento vazio.\n");
            fclose(fp);
            return(-1);
        }

        else{
            fclose(fp);
            return(0);
        }
    }
}

/**
 * @brief Conta o número de linhas e colunas da matriz lida de um arquivo de texto.
 *
 * Esta função lê o arquivo passado por parâmetro e conta o número de linhas e colunas
 * da matriz com base nos caracteres de nova linha e ponto e vírgula.
 *
 * @param linhas -> Apontador para armazenar o número de linhas da matriz.
 * @param colunas -> Apontador para armazenar o número de colunas da matriz.
 */
void contaLC(int* linhas, int* colunas, const char nomeFicheiro[]){
    FILE *fp = fopen (nomeFicheiro, "r");
    char ch = getc(fp);     // A função getc retorna o caractere lido como um int e lê o documento caractere por caractere.
    int nL = 0;
    int nC = 1;

    while (ch != EOF){
        if (ch == '\n')
            nL++;               // +1 linha
        if (ch == ';'&& nL ==1)
            nC++;               // +1 coluna

        ch = getc (fp);
    }

    fclose(fp);
    *colunas=nC;    // Atualiza o valor de colunas.
    *linhas=nL;     // Atualiza o valor de linhas.
}

/**
 * @brief Insere um novo vértice na lista de vértices.
 *
 * Esta função aloca memória para um novo vértice, inicializa os seus campos e insere
 * o vértice na lista de vértices.
 * O novo vértice é inserido no final da lista.
 *
 * @param inicio -> Apontador para o início da lista de vértices.
 * @param vertice -> O valor do vértice a ser inserido.
 *
 * @return inicio -> Apontador para o início da lista de vértices.
 * @return 0 -> Se houver erro na alocação de memória.
 */
Vertice* inserirVertice(Vertice* inicio, int vertice){
    Vertice* novo = (Vertice*)malloc(sizeof(Vertice));

    if (novo == NULL){
        printf("Erro ao alocar memória.\n");
        return(0);
    }

    novo->vertice_origem = vertice;
    novo->vertice_seguinte = NULL;  // Aponta o proximo vertice para NULL.
    novo->aresta_lado = NULL;       // Aponta a aresta para NULL.
    novo->aresta_abaixo = NULL;     // Aponta a aresta para NULL.

    if (inicio == NULL)
        return(novo);

    else{
        Vertice* temp = inicio;

        while (temp->vertice_seguinte != NULL){
            temp = temp->vertice_seguinte;
        }

        temp->vertice_seguinte = novo;
        return(inicio);
    }
}

/**
 * @brief Lê o documento da matriz e passa os valores para uma lista ligada.
 *
 * Esta função lê o arquivo passado por parâmetro e inser os valores inteiros numa lista ligada.
 *
 * @param nomeFicheiro -> String que contém o nome do arquivo a ser analisado.
 *
 * @return lista -> Apontador para o início da lista ligada preenchida com os valores lidos do documento.
 * @return 0 -> Se houver erro ao abrir o ficheiro.
 */
Vertice* leitura(const char nomeFicheiro[]){
    FILE* fp = fopen(nomeFicheiro, "r");
    Vertice* lista = NULL;
    int valor;

    if (fp == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return(0);
    }

    while (fscanf(fp, "%d;", &valor) == 1){
        lista = inserirVertice(lista, valor);
    }

    fclose(fp);
    return(lista);
}

/**
 * @brief Conecta os vértices ao seu adjacente à direita e abaixo.
 *
 * Esta função percorre a lista de vértices e cria arestas para conectar cada vértice
 * ao seu adjacente à direita (horizontalmente) e abaixo (verticalmente).
 * A função leva em consideração a estrutura da matriz representada pelos vértices.
 * O ultimo vertice de cada linha conecta ao primeiro da linha seguinte.
 *
 * @param inicio -> Apontador para o início da lista de vértices.
 * @param colunas -> O número de colunas na matriz.
 * @param linhas -> O número de linhas na matriz.
 *
 * @return 0 -> Se houver erro na alocação de memória.
 */
 void conectaDireitaBaixo(Vertice* inicio, int colunas, int linhas){
    Vertice* v1 = inicio;
    Vertice* vColuna = inicio;
    int j;
    int l=1; // Conta as linhas percorridas.
    int c=0; // Conta as colunas percorridas.

    while (v1 != NULL){
        Vertice* vDireita = v1->vertice_seguinte;

        // Conecta à direita (aresta do lado).
        if (vDireita != NULL){
            Aresta* novaArestaLado = (Aresta*)malloc(sizeof(Aresta));
            if (novaArestaLado == NULL){
                printf("Erro ao alocar memória.\n");
                return(0);
            }
            novaArestaLado->vertice_destino = vDireita->vertice_origem;
            novaArestaLado->seguinte = v1->aresta_lado;
            v1->aresta_lado = novaArestaLado;
        }

        // Conecta abaixo (aresta de baixo)
        if (v1->vertice_seguinte != NULL && l <= (linhas-1)){
            Vertice* vAbaixo = v1->vertice_seguinte;

            // Procura o vértice abaixo.
            for (j= 0; j < colunas; j++){
                vColuna = vColuna->vertice_seguinte;
            }

            if (vColuna != NULL && l < linhas){
                Aresta* novaArestaAbaixo = (Aresta*)malloc(sizeof(Aresta));

                if (novaArestaAbaixo == NULL){
                    printf("Erro ao alocar memória.\n");
                    return(0);
                }

                novaArestaAbaixo->vertice_destino = vColuna->vertice_origem;
                novaArestaAbaixo->seguinte = v1->aresta_abaixo;
                v1->aresta_abaixo = novaArestaAbaixo;
                novaArestaAbaixo->seguinte = NULL;
            }
            vColuna = vAbaixo;
            c++;
        }

        v1 = v1->vertice_seguinte;

        if (c == colunas){
            l++;
            c = 0;
        }
    }
}

/**
 * @brief Conecta os vértices pares com os pares e os ímpares com os ímpares.
 *
 * Esta função percorre a lista de vértices e cria arestas para conectar cada
 * vértice par ao próximo vértice par e cada vértice ímpar ao próximo vértice ímpar.
 * A função aloca memória para as novas arestas e atualiza
 * as listas de arestas dos vértices correspondentes.
 *
 * @param inicio -> Apontador para o início da lista de vértices.
 *
 * @return 0 -> Se houver erro na alocação de memória.
 */
 void conectaParImpar(Vertice* inicio){
    Vertice* aux = inicio;
    Vertice* ultimoPar = NULL;
    Vertice* ultimoImpar = NULL;

    while (aux != NULL){
        if ((aux->vertice_origem) % 2 == 0){
            // Conectar o último par ao atual.
            if (ultimoPar != NULL){
                Aresta* arestaPar = (Aresta*)malloc(sizeof(Aresta));

                if (arestaPar == NULL){
                    printf("Erro ao alocar memória.\n");
                    return(0);
                }

                arestaPar->vertice_destino = aux->vertice_origem;
                arestaPar->seguinte = ultimoPar->aresta_lado;
                ultimoPar->aresta_lado = arestaPar;
            }
           ultimoPar = aux;
        }

        else{
            // Conectar o último ímpar ao atual.
            if (ultimoImpar != NULL){
                Aresta* arestaImpar = (Aresta*)malloc(sizeof(Aresta));

                if (arestaImpar == NULL){
                    printf("Erro ao alocar memória.\n");
                    return(0);
                }

                arestaImpar->vertice_destino = aux->vertice_origem;
                arestaImpar->seguinte = ultimoImpar->aresta_lado;
                ultimoImpar->aresta_lado = arestaImpar;
            }
            ultimoImpar = aux;
        }

        aux = aux->vertice_seguinte;
    }
}

/**
 * @brief Encontra um vértice na lista dado o valor do vértice de origem.
 *
 * Esta função percorre a lista de vértices e retorna o apontador para o vértice correspondente ao
 * vertice do valor fornecido.
 * Se o vértice não for encontrado, a função retorna NULL.
 *
 * @param inicio -> Apontador para o início da lista de vértices.
 * @param verticeInicial -> O valor do vértice de origem a ser encontrado.
 *
 * @return vertice -> Apontador para o vértice encontrado.
 * @return NULL -> Se o vértice não for encontrado.
 */
 Vertice* encontrarVertice(Vertice* inicio, int verticeInicial){
    Vertice* vertice = inicio;

    while (vertice != NULL){
        if (vertice->vertice_origem == verticeInicial)
            return(vertice);

        vertice = vertice->vertice_seguinte;
    }

    return(NULL);
}

/**
 * @brief Realiza uma busca em profundidade a partir de um vértice dado pelo utilizador e imprime
 *        os caminhos possiveis até ao final da matriz e a soma de cada caminho.
 *
 * Esta função implementa uma busca em profundidade (DFS) em um grafo representado por uma estrutura de vértices e arestas.
 * A função visita todos os vértices do grafo a partir de um vértice inicial, marcando cada vértice
 * visitado e calculando o número de caminhos possíveis e as somas dos valores dos vértices ao longo desses caminhos.
 *
 * @param vertice -> O vértice a ser explorado.
 * @param caminho -> Um array que armazena o caminho percorrido até o momento.
 * @param profundidade -> A profundidade atual na busca em profundidade.
 * @param visitados -> Um array que marca os vértices visitados durante a busca.
 * @param totalcaminhos -> Um apontador para o total de caminhos encontrados.
 * @param somaAtual -> A soma dos valores dos vértices ao longo do caminho atual.
 */
 void dfs(Vertice* vertice, int caminho[], int profundidade, bool* visitados, int* totalcaminhos, int somaAtual){
    caminho[profundidade] = vertice->vertice_origem;    // Adiciona o vértice atual ao caminho e marca como visitado.
    profundidade++;
    visitados[vertice->vertice_origem] = true;

    somaAtual += vertice->vertice_origem;   // Adiciona o valor do vértice atual à somaAtual.

    // Verifica se o vértice atual é uma folha (sem arestas).
    if (vertice->aresta_lado == NULL && vertice->aresta_abaixo == NULL){
        // Imprime o caminho atual.
        printf("Caminho [%d]: ", *totalcaminhos);
        for (int i = 0; i < profundidade; i++){
            printf("%d ", caminho[i]);
        }
        printf(" -> Soma = %d\n", somaAtual);
        *totalcaminhos += 1;     // Atualiza o número total de caminhos encontrados.
    }

    // Explora as arestas laterais.
    Aresta* arestaAtual = vertice->aresta_lado;
    while (arestaAtual != NULL){
        Vertice* proximoVertice = encontrarVertice(vertice, arestaAtual->vertice_destino);

        if (proximoVertice != NULL && !visitados[proximoVertice->vertice_origem])
            dfs(proximoVertice, caminho, profundidade, visitados, totalcaminhos, somaAtual);

        arestaAtual = arestaAtual->seguinte;
    }

    // Explora as arestas abaixo.
    arestaAtual = vertice->aresta_abaixo;
    while (arestaAtual != NULL){
        Vertice* proximoVertice = encontrarVertice(vertice, arestaAtual->vertice_destino);

        if (proximoVertice != NULL && !visitados[proximoVertice->vertice_origem])
            dfs(proximoVertice, caminho, profundidade, visitados, totalcaminhos, somaAtual);

        arestaAtual = arestaAtual->seguinte;
    }

    visitados[vertice->vertice_origem] = false;     // Desmarca o vértice atual (backtracking).
}

/**
 * @brief Realiza uma busca em profundidade em um grafo para encontrar o caminho de maior soma.
 *
 * Esta função implementa uma busca em profundidade (DFS) em um grafo representado por uma estrutura de vértices e arestas.
 * O objetivo é encontrar o caminho de maior soma a partir de um vértice inicial (primeiro vertice da matriz).
 * A função mantém o rastreamento do caminho e da soma atual, atualizando o caminho máximo e a soma máxima encontrada, se necessário.
 *
 * @param vertice -> O vértice a ser explorado.
 * @param visitado -> Um array que marca os vértices visitados durante a busca.
 * @param soma -> A soma atual dos valores dos vértices ao longo do caminho.
 * @param caminho -> Um apontador para o array que armazena o caminho percorrido até o momento.
 * @param profundidade -> A profundidade atual na busca em profundidade.
 * @param maxSoma -> Um apontador para a maior soma encontrada até o momento.
 * @param caminhoMax -> Um apontador para o array que armazena o caminho de maior soma encontrado.
 * @param profundidadeMax -> Um apontador para a profundidade do caminho de maior soma encontrado.
 */
 void dfsAll(Vertice* vertice, bool* visitado, int soma, int** caminho, int profundidade, int* maxSoma, int** caminhoMax, int* profundidadeMax){
    visitado[vertice->vertice_origem] = true;       // Marca o vértice atual como visitado e adiciona ao caminho.
    (*caminho)[profundidade] = vertice->vertice_origem;
    profundidade++;
    soma += vertice->vertice_origem;

    // Atualiza o caminho e a soma máxima se a soma atual for maior.
    if (soma > *maxSoma){
        *maxSoma = soma;
        *profundidadeMax = profundidade;

        // Copia o caminho atual para o caminho de maior soma.
        for (int i = 0; i < profundidade; i++){
            (*caminhoMax)[i] = (*caminho)[i];
        }
    }

    // Explora as arestas laterais.
    Aresta* aresta = vertice->aresta_lado;
    while (aresta != NULL){
        if (!visitado[aresta->vertice_destino])
            dfsAll(encontrarVertice(vertice, aresta->vertice_destino), visitado, soma, caminho, profundidade, maxSoma, caminhoMax, profundidadeMax);

        aresta = aresta->seguinte;
    }

    // Explora as arestas abaixo.
    aresta = vertice->aresta_abaixo;
    while (aresta != NULL){
        if (!visitado[aresta->vertice_destino])
            dfsAll(encontrarVertice(vertice, aresta->vertice_destino), visitado, soma, caminho, profundidade, maxSoma, caminhoMax, profundidadeMax);

        aresta = aresta->seguinte;
    }

    visitado[vertice->vertice_origem] = false;  // Desmarca o vértice atual (backtracking).
    profundidade--;
}

/**
 * @brief Inicia uma busca em profundidade para encontrar o caminho de maior soma do grafo e imprime o resultado da procura.
 *
 * A função explora todos os vértices do grafo, chamando a função 'dfsAll()' e a partir dessa função
 * recebe o um array com caminho de maior soma e o resultado da soma.
 * No final imprime o caminho e o resultado do maior somatório.
 *
 * @param inicio -> apontador para o início da lista de vértices.
 * @param totalVertices -> O número total de vértices no grafo.
 *
 * @return 0 -> Se houver erro na alocação de memória.
 */
 void dfsMaiorSoma(Vertice* inicio, int totalVertices){
    Vertice* vertice = inicio;
    int* caminhoMax = (int*)malloc(sizeof(int) * totalVertices);
    int* caminho = (int*)malloc(sizeof(int) * totalVertices);
    bool* visitado = (bool*)malloc(sizeof(bool) * totalVertices);
    int maxSoma = 0;
    int profundidadeMax = 0;

    if (caminhoMax == NULL || caminho == NULL || visitado == NULL){
        printf("Erro ao alocar memória.\n");
        return(0);
    }

    // Percorre todos os vértices do grafo.
    while (vertice != NULL){
        memset(visitado, false, sizeof(bool) * totalVertices);      // Reinicia o array de vértices visitados para a próxima busca.
        dfsAll(vertice, visitado, 0, &caminho, 0, &maxSoma, &caminhoMax, &profundidadeMax);
        vertice = vertice->vertice_seguinte;
    }

    printf("\nCaminho de maior soma: ");
    for (int i = 0; i < profundidadeMax; i++){
        printf("%d", caminhoMax[i]);

        if (i < profundidadeMax - 1)
            printf(" -> ");         // Imprime "->" apenas se não for o último elemento.
    }
    printf("\nSoma: %d \n", maxSoma);

    free(caminhoMax);
    free(caminho);
    free(visitado);
}

/**
 * @brief Busca todos os caminhos a partir de um vértice dado pelo utilizador e imprime.
 *
 * Esta função busca todos os caminhos possíveis a partir de um vértice escolhido pelo utilizador.
 * Ela utiliza a função 'dfs()' para fazer uma busca em profundidade (DFS) e para imprimir os caminhos.
 *
 * @param inicio -> Apontador para o início da lista de vértices do grafo.
 * @param verticeInicial -> Vértice escolhido pelo utilizador para iniciar a busca.
 * @param totalVertices -> O número total de vértices no grafo.
 * @param maiorSoma -> Um apontador para a variável que armazena a maior soma encontrada.
 *
 * @return 0 -> Se houver erro na alocação de memória ou se o vertice escolhido não existir.
 */
void buscarCaminhos(Vertice* inicio, int verticeInicial, int totalVertices, int* maiorSoma) {
    Vertice* vertice = encontrarVertice(inicio, verticeInicial);

    if (vertice == NULL){
        printf("Vértice não encontrado.\n");
        return(0);
    }

    bool* visitado = (bool*)malloc(sizeof(bool) * totalVertices);
    int* caminho = (int*)malloc(sizeof(int) * totalVertices);
    int profundidade = 0;
    int totalcaminhos = 0;

    if (caminho == NULL || visitado == NULL){
        printf("Erro ao alocar memória.\n");
        return(0);
    }

    // Reinicia o array de vértices visitados para a busca atual.
    memset(visitado, false, sizeof(bool) * totalVertices);

    dfs(vertice, caminho, profundidade, visitado, &totalcaminhos, 0);

    free(visitado);
    free(caminho);
}

/**
 * @brief Lista as conexões de cada vértice do grafo.
 *
 * Esta função percorre a lista de vértices do grafo e lista as conexões
 * de cada vértice, tanto para as arestas laterais quanto para as arestas abaixo.
 *
 * @param inicio -> Apontador para o início da lista de vértices do grafo.
 */
 void listarArestas(Vertice* inicio){
    Vertice* aux = inicio;

    printf("Conexões dos vertices:\n");

    while (aux != NULL){
        printf("Vertice: %d -> ", aux->vertice_origem);
        Aresta* arestaAtual = aux->aresta_lado;

        if (arestaAtual != NULL){
            printf("Lado: ");
            while(arestaAtual != NULL){
                printf("%d ", arestaAtual->vertice_destino);
                arestaAtual = arestaAtual->seguinte;
            }
        }

        Aresta* arestaAbaixo = aux->aresta_abaixo;

        if (arestaAbaixo != NULL){
            printf("| Abaixo: ");
            while (arestaAbaixo != NULL){
                printf("%d ", arestaAbaixo->vertice_destino);
                arestaAbaixo = arestaAbaixo->seguinte;
            }
        }
        printf("\n");
        aux = aux->vertice_seguinte;
    }
}

/**
 * @brief Lista os valores da matriz representada como uma lista de vértices.
 *
 * Esta função percorre a lista de vértices do grafo e lista os valores
 * dos vértices em formato de matriz, considerando o número de linhas e colunas especificado.
 *
 * @param inicio -> Apontador para o início da lista de vértices do grafo.
 * @param linhas -> O número total de linhas da matriz.
 * @param colunas -> O número total de colunas da matriz.
 */
 void listarMatriz(Vertice* inicio, int linhas, int colunas){
    Vertice* aux = inicio;
    int i = 0; // Colunas.
    int j = 0; // Linhas.

    printf("Matriz:\n");
    while (aux != NULL){
        printf("%d ", aux->vertice_origem);
        aux = aux->vertice_seguinte;
        i++;

        if (i >= colunas){
            printf("\n");
            i = 0;
            j++;

            if (j >= linhas)
                break;
        }
    }
    printf("\n");
}

/**
 * @brief Liberta toda a memória alocada para os vértices e as arestas do grafo.
 *
 * Esta função percorre a lista de vértices do grafo e liberta a memória alocada
 * para cada vértice e as arestas associadas a ele nas listas 'aresta_lado' e 'aresta_abaixo'.
 *
 * @param inicio -> Apontador para o início da lista de vértices do grafo.
 */
 void libertarTodaMemoria(Vertice* inicio){
    Vertice* verticeAtual = inicio;
    while (verticeAtual != NULL){
        // Liberta a memória de todas as arestas na lista 'aresta_lado'.
        Aresta* arestaAtual = verticeAtual->aresta_lado;
        while (arestaAtual != NULL){
            Aresta* proximaAresta = arestaAtual->seguinte;
            free(arestaAtual);
            arestaAtual = proximaAresta;
        }

        // Liberta a memória de todas as arestas na lista 'aresta_abaixo'.
        arestaAtual = verticeAtual->aresta_abaixo;
        while (arestaAtual != NULL){
            Aresta* proximaAresta = arestaAtual->seguinte;
            free(arestaAtual);
            arestaAtual = proximaAresta;
        }

        // Liberta a memória do vértice atual.
        Vertice* proximoVertice = verticeAtual->vertice_seguinte;
        free(verticeAtual);
        verticeAtual = proximoVertice;
    }
}

/**
 * @brief Liberta a memória alocada para as arestas de todos os vértices do grafo.
 *
 * Esta função percorre a lista de vértices do grafo e liberta a memória alocada
 * para todas as arestas associadas a cada vértice nas listas 'aresta_lado' e 'aresta_abaixo'.
 * Após libertar todas as arestas de um vértice, os apontadores 'aresta_lado' e 'aresta_abaixo' são definidos como NULL.
 *
 * @param inicio -> Apontador para o início da lista de vértices do grafo.
 */
 void libertarTodasAsArestas(Vertice* inicio) {
    Vertice* verticeAtual = inicio;
    while (verticeAtual != NULL) {
        // Liberta todas as arestas na lista 'aresta_lado'.
        Aresta* arestaAtual = verticeAtual->aresta_lado;
        while (arestaAtual != NULL) {
            Aresta* proximaAresta = arestaAtual->seguinte;
            free(arestaAtual);
            arestaAtual = proximaAresta;
        }

        verticeAtual->aresta_lado = NULL;  // Define como NULL após libertar todas as arestas.

        // Liberta todas as arestas na lista 'aresta_abaixo'.
        arestaAtual = verticeAtual->aresta_abaixo;
        while (arestaAtual != NULL) {
            Aresta* proximaAresta = arestaAtual->seguinte;
            free(arestaAtual);
            arestaAtual = proximaAresta;
        }

        verticeAtual->aresta_abaixo = NULL;  // Define como NULL após libertar todas as arestas.
        verticeAtual = verticeAtual->vertice_seguinte;
    }
}

/**
 * @brief Imprime o menu principal do programa.
 *
 * Este menu exibe duas opções de regras de conexão e uma opção para terminar o programa.
 */
 void Menu(){
    printf("1. Regra de conexão 1 - Conecta para a direita e para baixo individualmente\n");
    printf("2. Regra de conexão 2 - Conecta pares com pares e impares com impares\n");
    printf("3. Sair\n");
}

/**
 * @brief Imprime o submenu do programa.
 *
 * Este submenu exibe as opções disponíveis depois da regra de conexão ter sido escolhida.
 */
void subMenu(){
    printf("\n");
    printf("1. Procura em profundidade\n");
    printf("2. Maior somatorio possivel\n");
    printf("3. Voltar\n");
}

/**
 * @brief Função principal do programa.
 *
 * Esta função gere as interações com o usuário, incluindo o menu principal e os submenus.
 */
int main(){
    const char nomeDoFicheiro[] = "Matriz_Default.txt";
    setlocale(LC_ALL, "Portuguese");
    Vertice* inicio;
    int verificar = lerConteudo(nomeDoFicheiro);
    int nL = 0, nC = 1, total;
    int verticeProf;
    int erro = 1;
    int opcaoMenu, opcaoSubMenu, avanca;
    int* maiorSoma = 0; // Inicialize a maior soma como '0'

    // Verifica se o arquivo de matriz tem conteúdo.
    if (verificar != 0)
        return(0);

    else{
        inicio = leitura(nomeDoFicheiro);
        contaLC(&nL, &nC, nomeDoFicheiro);
        total = nL*nC;
        listarMatriz(inicio, nL, nC);
        Menu();
    }

    do{
        while(erro == 1 || opcaoMenu != 3){
            erro = 0;
            system("cls");
            listarMatriz(inicio, nL, nC);
            Menu();
            printf("\n> ");

            // Se a leitura for um número.
            if(scanf("%d", &opcaoMenu) == 1){
                switch(opcaoMenu){

                    // Regra 1.
                    case 1:
                        if(inicio!=NULL){
                            system("cls");
                            conectaDireitaBaixo(inicio, nC, nL);
                            listarMatriz(inicio, nL, nC);
                            listarArestas(inicio);
                            subMenu();
                            printf("\n> ");
                            scanf("%d", &opcaoSubMenu);

                            switch(opcaoSubMenu){
                                // Profundidade
                                case 1:
                                    printf("\nVertice para consultar profundidade: ");
                                    scanf("%d", &verticeProf);
                                    printf("\n");

                                    buscarCaminhos(inicio, verticeProf, total, &maiorSoma);
                                    libertarTodasAsArestas(inicio);

                                    printf("\nPremir uma tecla para recuar até ao menu principal. ");
                                    getchar();                                                      // Consumir o '\n' deixado no buffer pelo scanf
                                    avanca = getchar();
                                    break;

                                // Soma maxima
                                case 2:
                                    dfsMaiorSoma(inicio, total);
                                    libertarTodasAsArestas(inicio);
                                    printf("\nPremir uma tecla para recuar até ao menu principal. ");
                                    getchar();              // Consumir o '\n' deixado no buffer pelo scanf
                                    avanca = getchar();     // Esperar pela interação do usuário
                                    break;

                                // Sair para o menu principal
                                case 3:
                                    libertarTodasAsArestas(inicio);
                                    break;

                                default:
                                    libertarTodasAsArestas(inicio);
                                    break;
                            }
                        }
                        break;

                    // Regra 2
                    case 2:
                        if(inicio!=NULL){
                            system("cls");
                            conectaParImpar(inicio);
                            listarMatriz(inicio, nL, nC);
                            listarArestas(inicio);
                            subMenu();
                            printf("\n> ");
                            scanf("%d", &opcaoSubMenu);

                            switch(opcaoSubMenu){
                                // Profundidade
                                case 1:
                                    printf("\nVertice para consultar profundidade: ");
                                    scanf("%d", &verticeProf);
                                    printf("\n");

                                    buscarCaminhos(inicio, verticeProf, total, &maiorSoma);
                                    libertarTodasAsArestas(inicio);

                                    printf("\nPremir uma tecla para recuar até ao menu principal. ");
                                    getchar();
                                    avanca = getchar();
                                    break;

                                // Soma maxima
                                case 2:
                                    dfsMaiorSoma(inicio, total);
                                    libertarTodasAsArestas(inicio);

                                    printf("\nPremir uma tecla para recuar até ao menu principal. ");
                                    getchar();              // Consumir o '\n' deixado no buffer pelo scanf
                                    avanca = getchar();     // Esperar pela interação do usuário
                                    break;

                                // Sair para o menu principal
                                case 3:
                                    libertarTodasAsArestas(inicio);
                                    break;

                                default:
                                    libertarTodasAsArestas(inicio);
                                    break;
                            }
                        }
                        break;

                    // Sair
                    case 3:
                        printf("\nA sair");
                        usleep(300000);             /**< Pausa em microssegundos. */
                        printf(".");
                        usleep(300000);
                        printf(".");
                        usleep(300000);
                        printf(".");
                        usleep(300000);
                        printf("\n");
                        libertarTodaMemoria(inicio);;    /**< Liberta a memoria ocupada pela lista. */
                        break;

                    default:
                        printf("ERRO! Insira um número válido!\n");
                        erro = 1;
                }
            }

            // Se a opção não for um número
            else{
                printf("ERRO! Insira um número válido!\n");
                while(getchar() != '\n');       /**< Limpar o buffer de entrada para evitar loops infinitos em caso de entrada não numérica. */
                erro = 1;
            }
        }
        libertarTodasAsArestas(inicio);
    }while(opcaoMenu != 3);

    return(0);
}
