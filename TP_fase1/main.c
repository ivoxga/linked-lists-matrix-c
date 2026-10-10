/**
 * \file main.c
 * \brief Trabalho Pratico - fase 1.
 *
 * Objetivo:
 *  Usando o metodo de listas ligadas:
 *      Ler e imprimir a matriz default escrita num documento .txt;
 *      Possibilitar adicionar/remover colunas/linhas;
 *      Possibilitar a alteração de valores na matriz;
 *      Apresentar o maior somatório possível.
 *
 *  Nota: Função para calcular o maior somatório possível inacabada.
 *
 * Trabalho realizado por: Ivo Azevedo nº 15526
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

/**
 * \brief Estrutura da lista ligada.
 *
 * Esta estrutura define um registo da lista ligada, que contém um valor inteiro e um apontador
 * para o próximo registo da lista.
 */
typedef struct registo{
    int valor;                  /**< Valor inteiro que vai ser guardado no registo. */
    struct registo* seguinte;   /**< Apontador para o proximo registo de memória. */
}Lista;

/**
 * \brief Verifica a existência e o conteúdo de um documento especificado pelo nome do arquivo na 'main()'.
 *
 * Esta função verifica se o documento especificado pelo nome do arquivo na
 *'main()' existe e se contém algum conteúdo.
 *
 * \param nomeFicheiro -> String que contém o nome do arquivo a ser analisado.
 *
 * \return -1 -> Se o documento não existir ou se estiver vazio.
 * \return 0 -> Se o documento existir e contiver conteúdo.
 */
int lerConteudo(const char nomeFicheiro[]){
    FILE* fp = fopen(nomeFicheiro, "r");            /**< Abre o arquivo para leitura. */

    if (fp == NULL){
        printf("ERRO! Documento não encontrado.\n");
        return(-1);
    }

    else{
        // Verifica o tamanho do arquivo
        fseek(fp, 0, SEEK_END);                     /**< Mover o indicador de posição para o final do arquivo. */
        long tamanhoDoc = ftell(fp);                /**< Obter a posição do indicador de posição (tamanho do arquivo). */

        if (tamanhoDoc == 0) {
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
 * \brief Conta o número de linhas e colunas da matriz lida de um arquivo de texto.
 *
 * Esta função lê o arquivo passado por parâmetro e conta o número de linhas e colunas
 * da matriz com base nos caracteres de nova linha e ponto e vírgula.
 *
 * \param linhas -> Apontador para armazenar o número de linhas da matriz.
 * \param colunas -> Apontador para armazenar o número de colunas da matriz.
 */
void contaLC(int* linhas, int* colunas, const char nomeFicheiro[]){
    FILE *fp = fopen (nomeFicheiro, "r");
    char ch = getc(fp);     /**< A função getc retorna o caractere lido como um int e lê o documento caractere por caractere. */
    int nL = 0;
    int nC = 1;

    while(ch != EOF){
        if(ch == '\n')
            nL++;
        if(ch == ';'&& nL ==1)
            nC++;
        ch = getc (fp);
    }

    fclose(fp);
    *colunas=nC;
    *linhas=nL;
}

/**
 * \brief Inser um novo registo no inicio da lista ligada.
 *
 * Esta função insere um novo registo com o respetivo valor no inicio da lista ligada.
 *
 * \param inicio -> Apontador para o início da lista ligada onde o novo registo será inserido.
 * \param v -> Valor a ser inserido no novo registo.
 *
 * \return novo -> Apontador para o início da lista ligada após a inserção do novo registo.
 *         inicio -> Se a alocação de memória para o novo registo falhar, a função retorna o apontador
 *           para o início da lista ligada original.
 */
Lista* inserirRegisto(Lista* inicio, int v){
    Lista* novo = (Lista*) malloc(sizeof(Lista));

    if(novo!= NULL){
        novo->valor = v;
        novo->seguinte = inicio;
        return(novo);
    }
    else
        return(inicio);
}

/**
 * \brief Inser um novo registo no fim da lista ligada de forma recursiva.
 *
 * Esta função insere um novo registo com o respetivo valor no fim da lista ligada
 * utilizando abordagem recursiva.
 *
 * \param inicio -> Apontador para o início da lista ligada onde o novo registo será inserido.
 * \param v -> Valor a ser inserido no novo registo.
 *
 * \return novo -> Apontador para o início da lista ligada após a inserção do novo registo.
 *         inicio -> Se a alocação de memória para o novo registo falhar, a função retorna o apontador
 *           para o início da lista ligada original.
 */
Lista* inserirRegistoFim(Lista* inicio, int v){
    if(inicio == NULL){
        Lista* novo = (Lista*) malloc(sizeof(Lista));

        if(novo != NULL){
            novo->valor = v;
            novo->seguinte = NULL;
            return(novo);
        }
        else
            return(inicio);
    }

    else{
        inicio->seguinte = inserirRegistoFim(inicio->seguinte,v);  /**< Avança para o próximo registo na lista até encontrar o último registo. */
        return(inicio);
    }
}

/**
 * \brief Inser um novo registo na poisção passada por parâmetro na lista ligada.
 *
 * Esta função insere um novo registo com o respetivo valor na posição passada por parâmetro na lista ligada.
 * A posição de inserção é baseada em índices de 1 a N, onde 1 representa a primeira posição da lista e N+1
 * representa a posição após o último elemento da lista.
 *
 * \param inicio -> Apontador para o início da lista ligada onde o novo registo será inserido.
 * \param v -> Valor a ser inserido no novo registo.
 * \param poiscao -> Posição onde será inserido o novo registo na lista ligada.
 *
 * \return - Função que O apontador para o início da lista ligada após a inserção do novo registro.
 *         - Se a alocação de memória para o novo registro falhar, a função retorna o apontador
 *           para o início da lista ligada original.
 *         - Se a posição especificada for inválida (<1 ou maior que a quantidade de registos +1),
 *           a função retorna o apontador para o início da lista ligada original.
 */
Lista* inserirPosicao(Lista* inicio, int v, int posicao){
    if((posicao >= 1) && (posicao <= quantidade(inicio)+1)){
        Lista* anterior=inicio, *actual=inicio;

        if(posicao == 1)
            return(inserirRegisto(inicio, v));

        else{
            while((actual != NULL) && (posicao > 1)){
                anterior = actual;
                actual = actual->seguinte;
                posicao--;
            }

            Lista* novo = (Lista*) malloc(sizeof(Lista));
            novo->valor = v;
            anterior->seguinte = novo;
            novo->seguinte = actual;
            return(inicio);
        }
    }

    else
        return(inicio);
}

/**
 * \brief Conta o número de registos existentes na lista ligada referenciada por inicio.
 *
 * Esta função percorre a lista ligada do inicio ao fim e conta o número de registos existentes na mesma.
 *
 * \param inicio -> Apontador para o início da lista ligada.
 *
 * \return qt -> quantidade de registos existentes na lista ligada.
 */
int quantidade(Lista* inicio){
    int qt = 0;

    for( ; inicio != NULL; inicio = inicio->seguinte)
        qt++;

    return(qt);
}

/**
 * \brief Lê o documento da matriz e passa os valores para uma lista ligada.
 *
 * Esta função lê o arquivo passado por parâmetro e inser os valores inteiros numa lista ligada.
 *
 * \param nomeFicheiro -> String que contém o nome do arquivo a ser analisado.
 *
 * \return Um apontador para o início da lista ligada preenchida com os valores lidos do documento.
 */
Lista* leitura(char nomeFicheiro[]){
    FILE* fp = fopen(nomeFicheiro,"r");
    int valor, qt;
    Lista* lista = NULL;  /**< Lista ligada vazia. */

    do{
        do{
            qt = fscanf(fp,"%d;", &valor);  /**< Guadar os valores inteiros do ficheiro. */
            if (qt == 1)
                lista = inserirRegistoFim(lista,valor);
        }while (qt != 1);

        fscanf(fp,"\n");

    }while (!feof(fp));  /**< end of file  1 ou 0. */

    fclose(fp);
    return(lista);
}

/**
 * \brief Imprime na consola de forma tabular todos os valores registados na lista ligada.
 *
 * Esta função recebe o apontador para o inicio da lista ligada e com o número de linhas e colunas vai
 * imprimindo os valores da lista ligada de forma tabula.
 *
 * \param inicio -> Apontador para o início da lista ligada onde o novo registo será inserido.
 * \param linhas -> Número de linhas da matriz.
 * \param colunas -> Número de colunas da matriz.
 */
void listar(Lista* inicio, int linhas, int colunas){
    Lista* aux=inicio;
    int i = 0; // Colunas
    int j = 0; // Linhas

    printf("Matriz:\n");
    while(aux != NULL){
        printf("%d ", aux->valor);
        aux = aux->seguinte;
        i++;

        // Verifica se já chegou ao fim do número de colunas
        if(i >= colunas){
            printf("\n");
            i = 0;
            j++;

            // Verifica se já chegou ao fim do número de linhas
            if(j >= linhas)
                break;
        }
    }
    printf("\n");
}

/**
 * \brief Remove um registro da lista ligada dada a sua posição.
 *
 * Esta função remove um registro da lista ligada com base na posição passada por parâmetro.
 * Se a posição for igual a 1, o primeiro registro é removido.
 * Caso contrário, a função percorre recursivamente a lista ligada até encontrar
 * o registo na posição desejada e remove-o.
 *
 * \param inicio -> Apontador para o início da lista ligada.
 * \param pos -> Posição do registro a ser removido.
 *
 * \return aux -> O apontador para o início da lista ligada após a remoção do registro.
 *         NULL -> Se a lista estiver vazia, a função retorna NULL.
 */
Lista* removerRegisto(Lista* inicio, int pos){
    if (inicio==NULL)
        return(NULL);

    else if (pos==1){
        Lista* aux = inicio->seguinte;
        free(inicio);
        return(aux);
    }

    else{
        inicio->seguinte = removerRegisto(inicio->seguinte,pos-1);
        return(inicio);
    }
}

/**
 * \brief Adiciona uma nova linha à matriz representada pela lista ligada.
 *
 * Esta função permite ao usuário adicionar uma nova linha à matriz representada pela lista ligada.
 * É solicitado ao usuário para inserir os valores para cada coluna da nova linha.
 * A posição da nova linha pode ir de 1 ao número de linhas +1.
 *
 * \param inicio -> Apontador para o início da lista ligada que representa a matriz.
 * \param linhas -> Apontador para o número atual de linhas da matriz.
 * \param colunas -> Apontador para o número atual de colunas da matriz.
 *
 * \return inicio -> Apontador para o início da lista ligada com a nova linha adicionada.
 *         inicio -> Se o usuário inserir o número 99, a função retorna a lista sem qualquer alteração.
 */
Lista* adicionaLinha(Lista* inicio, int* linhas ,int* colunas){
    int adLinha;
    int novoValor;
    int erro;
    int j, i;

    printf("\n* Adicionar Linha *\n");
    printf("(assuma a contagem a começar em 1 até %d)\n", (*linhas)+1);
    printf("(para voltar ao menu digite 99 na seleção da linha)\n");

    do{
        erro = 0;
        printf("\nPosição da nova linha: ");
        // Verifica se é um numero
        if(scanf("%d", &adLinha) == 1){
            // Se quiser adicionar uma linha nova no inicio
            if(adLinha == 1){
                for(j = 1; j <= *colunas; j++){
                    printf(">Linha[%d] Coluna[%d]: ", adLinha, j);

                    if(scanf("%d", &novoValor) == 1){
                        if(j == 1)
                            inicio = inserirRegisto(inicio, novoValor);
                        else{
                            inicio = inserirPosicao(inicio, novoValor, j);
                        }
                    }

                    else{
                        printf("ERRO! Novo valor inválido\n\n");
                        j--;   // Para permanecer na linha
                        while(getchar() != '\n');
                    }
                }
                (*linhas)++;
                return(inicio);
            }
            // Se quiser adicionar uma linha nova no final
            else if(adLinha == (*linhas)+1){
                printf("\n");
                for(j = 1; j <= *colunas; j++){
                    printf(">Linha[%d] Coluna[%d]: ", adLinha, j);

                    if(scanf("%d", &novoValor) == 1)
                        inicio = inserirRegistoFim(inicio, novoValor);
                    else{
                        printf("ERRO! Novo valor inválido\n\n");
                        j--;
                        while(getchar() != '\n');
                    }
                }
                (*linhas)++;
                return(inicio);
            }

            // Se quiser adicionar uma linha nova no meio
            else if(adLinha > 1 && adLinha <= *linhas){
                printf("\n");
                // Calcula a posição na lista
                int n = ((adLinha-1) * (*colunas)) +1;

                for (int j = 1; j <= *colunas; j++){
                     printf(">Linha[%d] Coluna[%d]: ", adLinha, j);

                    if(scanf("%d", &novoValor) == 1){
                        inicio = inserirPosicao(inicio, novoValor, n);
                        n += 1;
                    }

                    else{
                        printf("ERRO! Novo valor inválido\n\n");
                        j--;
                        while(getchar() != '\n');
                    }
                }
                (*linhas)++;
                return(inicio);
            }

            // Caso seja uma linha inválida
             else if(adLinha < 1 || adLinha > ((*linhas)+1) && adLinha != 99){
                printf("ERRO! Linha inválida\n");
                erro = 1;
             }
        }

        // Se digitar caracteres
        else{
            printf("ERRO! Linha inválida\n");
            erro = 1;
            while(getchar() != '\n');
        }
    }while(erro == 1 || adLinha != 99);

    // Volta para o menu principal
    if(adLinha == 99)
        return(inicio);
}

/**
 * \brief Adiciona uma nova coluna à matriz representada pela lista ligada.
 *
 * Esta função permite ao usuário adicionar uma nova coluna à matriz representada pela lista ligada.
 * É solicitado ao usuário para inserir os valores para cada linha da nova coluna.
 * A posição da nova coluna pode ir de 1 ao número de colunas +1.
 *
 * \param inicio -> Apontador para o início da lista ligada que representa a matriz.
 * \param linhas -> Apontador para o número atual de linhas da matriz.
 * \param colunas -> Apontador para o número atual de colunas da matriz.
 *
 * \return inicio -> Apontador para o início da lista ligada com a nova coluna adicionada.
 *         inicio -> Se o usuário inserir o número 99, a função retorna a lista sem qualquer alteração.
 */
Lista* adicionaColuna(Lista* inicio, int* linhas ,int* colunas){
    int adColuna;
    int novoValor;
    int j = 1;
    int n = 0;
    int erro;
    int i;
    Lista* aux = inicio;

    printf("\n* Adicionar Coluna *\n");
    printf("(assuma a contagem a começar em 1 até %d)\n", (*colunas)+1);
    printf("(para voltar ao menu digite 99 na seleção da coluna)\n");

    do{
        erro = 0;
        printf("\nPosição da nova coluna: ");

        // Verifica se é um numero
        if(scanf("%d", &adColuna) == 1){
            // Se quiser adicionar um valor novo no inicio da lista
            if(adColuna == 1){
                printf("\n");
                do{
                    printf(">Linha[%d] Coluna[%d]: ", j, adColuna);

                    if(scanf("%d", &novoValor) == 1){
                        inicio = inserirPosicao(inicio, novoValor, 1);      /**< Inserir o novo valor da coluna para a posição 1 da lista. */
                        j = 2;
                    }

                    else{
                        printf("ERRO! Novo valor inválido\n\n");
                        j = 1;
                        while(getchar() != '\n');
                    }
                }while(j != 2);

                // Adicionar nas restantes linhas da coluna 0
                for (j = 2; j <= *linhas; j++){
                    printf(">Linha[%d] Coluna[%d]: ", j, adColuna);

                    if(scanf("%d", &novoValor) == 1){
                        n = (j -1) * (*colunas) + j ;                       /**< Posição para as linhas seguintes. */
                        for (i = 1; i < n; i++)                             /**< Procura posição. */
                            aux = aux->seguinte;

                        aux = inicio;                                       /**< Reset ao apontador para tornar a ler no inicio. */
                        inicio = inserirPosicao(inicio, novoValor, n);      /**< Insere na posição anterior ao n. */
                    }

                    else{
                        printf("ERRO! Novo valor inválido\n\n");
                        j--;
                        while(getchar() != '\n');
                    }
                }
                (*colunas)++;
                return(inicio);
            }

            // Se quiser adicionar uma coluna na posição > 0
            else if(adColuna > 1 && adColuna <= *colunas){
                printf("\n");
                n = adColuna;

                for (int j = 1; j <= *linhas; j++){
                     printf(">Linha[%d] Coluna[%d]: ", j, adColuna);

                    if(scanf("%d", &novoValor) == 1){
                            printf("n = %d\n", n);
                            inicio = inserirPosicao(inicio, novoValor, n);
                            n += *colunas +1;
                    }
                    else{
                        printf("ERRO! Novo valor inválido\n\n");
                        j--;
                        while(getchar() != '\n');
                    }
                }
                (*colunas)++;
                return(inicio);
            }

            // Se quiser adicionar uma coluna na ultima posição
            else if(adColuna == (*colunas)+1){
                printf("\n");

                for (j = 1; j <= *linhas; j++){
                    printf(">Linha[%d] Coluna[%d]: ", j, adColuna);

                    if(scanf("%d", &novoValor) == 1){
                        n = ((j -1) * (*colunas) + adColuna + j) - 1;       /**< Tenho que subtrair 1 porque a contagem das colunas começo em 0. */
                        for (int i = 1; i < n; i++)
                            aux = aux->seguinte;

                        aux = inicio;
                        inicio = inserirPosicao(inicio, novoValor, n);
                    }

                    else{
                        printf("ERRO! Novo valor inválido\n\n");
                        j--;
                        while(getchar() != '\n');
                    }
                }
                (*colunas)++;
                return(inicio);
            }

            // Caso seja uma coluna inválida
            else if(adColuna < 1 || adColuna > ((*colunas)+1) && adColuna != 99){
                printf("ERRO! Coluna inválida\n");
                erro = 1;
            }
        }
        // Se digitar caracteres
        else{
            printf("ERRO! Coluna inválida\n");
            while(getchar() != '\n');
            erro = 1;
        }

    }while(erro == 1 || adColuna != 99);

    // Volta para o menu principal
    if(adColuna == 99)
        return(inicio);
}

/**
 * \brief Alterar o valor de uma posição da matriz representada pela lista ligada.
 *
 * Esta função permite ao usuário alterar o valor de qualquer registo.
 * É solicitado ao usuário para inserir o número da linha e da coluna para obter a posição do valor a ser alterado.
 *
 * \param inicio -> Apontador para o início da lista ligada que representa a matriz.
 * \param linhas -> Número atual de linhas da matriz.
 * \param colunas -> Número atual de colunas da matriz.
 *
 * \return inicio -> Apontador para o início da lista ligada com o novo valor alterado.
 *         inicio -> Se o usuário inserir o número 99, a função retorna a lista sem qualquer alteração.
 */
Lista* alteraValor(Lista* inicio, int linhas, int colunas){
    int posicaoLinha;
    int posicaoColuna;
    int novoValor;
    int erro;

    printf("\n* Alterar valor *\n");
    printf("(assuma a contagem de linhas de [1 - %d] e das colunas de [1 - %d])\n", linhas, colunas);
    printf("(para voltar ao menu digite 99 na seleção da linha)\n");

    do{
        erro = 0;
        printf("\n>Número da linha: ");
        if(scanf("%d", &posicaoLinha) == 1){

            if(posicaoLinha == 99)     /**< Volta para o menu principal. */
                return(inicio);

            else if(posicaoLinha > linhas){         /**< Se a linha não existir. */
                printf("ERRO! Linha inválida\n");
                erro = 1;
            }

            else if(posicaoLinha == 0){
                printf("ERRO! Linha inválida\n");
                erro = 1;
            }

            else if(posicaoLinha <= linhas)     /**< Se o nº da linha existir avança para pedir o nº da coluna. */
                erro = 0;
        }
        // Se digitar caracteres
        else{
            printf("ERRO! Linha inválida\n");
            erro = 1;
            while(getchar() != '\n');
        }
    }while(erro == 1);

    do{
        erro = 0;
        if(erro == 0){
            printf("\n>Número da coluna: ");

            if(scanf("%d", &posicaoColuna) == 1){
                if(posicaoColuna > colunas){
                    printf("ERRO! Coluna inválida\n");
                    erro = 1;
                }

                else if(posicaoColuna == 0){            /**< Se a coluna não existir. */
                    printf("ERRO! Coluna inválida\n");
                    erro = 1;
                }

                else if(posicaoColuna <= colunas)       /**< Se o nº da coluna existir avança para pedir o novo valor. */
                    erro = 0;
            }
            // Se digitar caracteres
            else{
                printf("ERRO! Coluna inválida\n");
                erro = 1;
                while(getchar() != '\n');
            }
        }
    }while(erro == 1);

    int contador = ((posicaoLinha-1) * colunas + (posicaoColuna-1));        /**< Calcula quantas posições tem que andar na lista até chegar à posição desejada. */

    while ((inicio != NULL) && (contador >= 1)){
        inicio = inicio->seguinte;
        contador--;
    }
    if (inicio!=NULL){
        do{
            erro = 0;
            printf("\n>Alterar %d para: ", *inicio);

            if(scanf("%d", &novoValor) == 1)
                inicio->valor = novoValor;

            // Se digitar caracteres
            else{
                printf("ERRO! Novo valor inválido\n");
                erro = 1;
                while(getchar() != '\n');
            }
        }while(erro == 1);
    }
    else
        return(inicio);
}

/**
 * \brief Remover uma linha inteira à matriz representada pela lista ligada.
 *
 * Esta função permite ao usuário remover uma linha à matriz representada pela lista ligada.
 * É solicitado ao usuário para inserir a posição da linha que deseja remover.
 * A posição das linhas começa a contagem em 1 até ao número maximo de linhas da matriz.
 *
 * \param inicio -> Apontador para o início da lista ligada que representa a matriz.
 * \param linhas -> Apontador para o número atual de linhas da matriz.
 * \param colunas -> Apontador para o número atual de colunas da matriz.
 *
 * \return inicio -> Apontador para o início da lista ligada com a linha desejada removida.
 *         inicio -> Se o usuário inserir o número 99, a função retorna a lista sem qualquer alteração.
 */
Lista* removerLinha(Lista* inicio, int* linhas, int* colunas){
    int erro;
    int reLinha;
    int n = 0;
    Lista* aux = inicio;
    Lista* anterior = NULL;

    printf("\n* Remover Linha *\n");
    printf("(assuma a contagem a começar em 1 até %d)\n", (*linhas));
    printf("(para voltar ao menu digite 99 na seleção da linha)\n");

    do{
        erro = 0;
        printf("\nPosição da linha a remover: ");
        if(scanf("%d", &reLinha) == 1){

            // Volta ao menu inicial
            if(reLinha == 99)
                return(inicio);

            else if(reLinha > 0 && reLinha <= *linhas){
                n = ((reLinha) * (*colunas)) - (*colunas);       /**< Calcula a posição do elemento anterior da linha pretendida. */

                // Avança até a posição desejada
                for(int i = 1; i <= n; i++){
                    anterior = aux;
                    aux = aux->seguinte;
                }

                // Remove a linha
                for(int j = 1; j <= *colunas; j++){
                    Lista* proximo = aux->seguinte;
                    free(aux);
                    aux = proximo;
                }

                if (anterior != NULL)
                    anterior->seguinte = aux;   /**< Caso exista, atualiza o apontador do registo anterior. */

                else
                    inicio = aux;               /**< Se o registo anterior for NULL, significa que a cabeça da lista foi removida. */

                (*linhas)--;
                return(inicio);
            }

            else if(reLinha > *linhas || reLinha == 0){
                printf("ERRO! Linha inexistente\n");
                erro = 1;
            }
        }

        else{
            printf("ERRO! Linha inexistente\n");
            erro = 1;
            while(getchar() != '\n');
        }

    }while(erro == 1);
}

/**
 * \brief Remover uma coluna inteira à matriz representada pela lista ligada.
 *
 * Esta função permite ao usuário remover uma coluna à matriz representada pela lista ligada.
 * É solicitado ao usuário para inserir a posição da coluna que deseja remover.
 * A posição das colunas começa a contagem em 1 até ao número maximo de coluans da matriz.
 *
 * \param inicio -> Apontador para o início da lista ligada que representa a matriz.
 * \param linhas -> Apontador para o número atual de linhas da matriz.
 * \param colunas -> Apontador para o número atual de colunas da matriz.
 *
 * \return inicio -> Apontador para o início da lista ligada com a coluna desejada removida.
 *         inicio -> Se o usuário inserir o número 99, a função retorna a lista sem qualquer alteração.
 */
 Lista* removerColuna(Lista* inicio, int* linhas, int* colunas){
    int reColuna;
    Lista* aux = inicio;
    int j = 1;
    int n = 0;
    int valoresRemovidos = 0;
    int erro;


    printf("\n* Remover Coluna *\n");
    printf("(Assuma a contagem a começar em 1 até %d)\n", *colunas);
    printf("(Para voltar ao menu, digite 99 na seleção da coluna)\n");


     do{
        erro = 0;
        printf("\nPosição da coluna a remover: ");
        if(scanf("%d", &reColuna) == 1){

            if(reColuna == 99)       // Volta ao menu inicial
                return(inicio);

            else if(reColuna > 0 && reColuna <= (*colunas)){
                // Se quiser adicionar uma coluna na posição > 0
                for (j = 1; j <= *linhas; j++){

                    n = (j - 1) * (*colunas) + reColuna - valoresRemovidos;

                    for (int i = 1; i < n; i++){
                        aux = aux->seguinte;
                        printf("%d\n", n);
                    }

                    aux = inicio;                           /**< Reset ao apontador para tornar a ler no inicio. */
                    inicio = removerRegisto(inicio, n);
                    valoresRemovidos++;
                }

                (*colunas)--;
                return(inicio);
            }

            else if(reColuna > (*colunas) || reColuna == 0){
                printf("ERRO! Coluna inexistente\n");
                erro = 1;
            }
        }

        else{
            printf("ERRO! Coluna inexistente\n");
            erro = 1;
            while(getchar() != '\n');
        }

    }while(erro == 1);
}

/**
 * \brief Libertar toda a memória ocupada pela lista ligada.
 *
 * Esta função percorre toda a lista ligada e vai libertando registo a registo até chegar ao final da lista.
 *
 * \param inicio -> Apontador para o início da lista ligada.
 */
void libertarMemoria(Lista* inicio){
    Lista* aux;

    while (inicio != NULL){
        aux = inicio;
        inicio = inicio->seguinte;
        free(aux);
    }
}

/**
 * \brief Cria uma nova linha inteira de zeros na ultima linha da matriz.
 *
 * \param inicio -> Apontador para o início da lista ligada que representa a matriz.
 * \param linhas -> Apontador para o número atual de linhas da matriz.
 * \param colunas -> Apontador para o número atual de colunas da matriz.
 *
 * \return inicio -> Apontador para o início da lista ligada com a nova linha de zeros.
 */
Lista* adicionaLinhaZeros(Lista* inicio, int* linhas, int* colunas){
    int novoValor = 0;

    for (int j = 1; j <= *colunas; ++j) {
        inicio = inserirRegistoFim(inicio, novoValor);
    }
    (*linhas)++;

    return(inicio);
}

/**
 * \brief Cria uma nova coluna inteira de zeros na ultima coluna da matriz.
 *
 * \param inicio -> Apontador para o início da lista ligada que representa a matriz.
 * \param linhas -> Apontador para o número atual de linhas da matriz.
 * \param colunas -> Apontador para o número atual de colunas da matriz.
 *
 * \return inicio -> Apontador para o início da lista ligada com a nova coluna de zeros.
 */
Lista* adicionaColunaZeros(Lista* inicio, int* linhas, int* colunas){
    int novoValor = 0;
    int adColuna = (*colunas) + 1;
    Lista* aux = inicio;

    for (int j = 1; j <= *linhas; j++){

        int n = ((j -1) * (*colunas) + adColuna + j) - 1;      /**< Tenho que subtrair 1 porque a contagem das colunas começo em 0. */

        for (int i = 1; i < n; i++)
            aux = aux->seguinte;

        aux = inicio;                                           /**< Reset ao apontador para tornar a ler no inicio. */
        inicio = inserirPosicao(inicio, novoValor, n);          /**< Insere na posição anterior ao n. */
    }
    (*colunas)++;
    return(inicio);
 }

/**
 * \brief Apresenta o maior somatório possível pelo metodo do algoritmo Hungaro.
 *
 * Esta função apresnta passo a passo a forma para calcular o maior somatorio possivel atravez do
 * metodo do algoritmo Hungaro.
 * NOTA: A função ainda não esta acabada.
 *
 * \param inicio -> Apontador para o início da lista ligada que representa a matriz.
 * \param linhas -> Apontador para o número atual de linhas da matriz.
 * \param colunas -> Apontador para o número atual de colunas da matriz.
 */
int somatorio(Lista* inicio, int* linhas, int* colunas) {
    Lista* aux = inicio;
    int erro;

    // Negar todos os valores na matriz
    while (aux != NULL) {
        aux->valor *= -1;
        aux = aux->seguinte;
    }
    aux = inicio;
    printf("==============\n");
    printf("Matriz negada:\n");
    listar(aux, *linhas, *colunas);

    // Somar 900 a todos os valores
    while (aux != NULL) {
        aux->valor += 900;
        aux = aux->seguinte;
    }
    aux = inicio;
    printf("======================\n");
    printf("Matriz Somada com 900:\n");
    listar(aux, *linhas, *colunas);

    // Criar uma matriz quadrada
    if(*linhas < *colunas){
        aux = adicionaLinhaZeros(inicio, linhas, colunas);
        printf("==========================\n");
        printf("Matriz com linha de zeros:\n");
        listar(aux, *linhas, *colunas);
    }

    else if(*linhas > *colunas){
        aux = adicionaColunaZeros(inicio, linhas, colunas);
        printf("===========================\n");
        printf("Matriz com Coluna de zeros:\n");
        listar(aux, *linhas, *colunas);
    }

    printf("===========================\n");
}


/**
 * \brief Imprime o menu de opções para o usuário.
 *
 * Esta função imprime as opções disponíveis para o usuário interagir com a matriz representada pela lista ligada.
 * As opções incluem adicionar linha, adicionar coluna, remover linha, remover coluna, alterar valor de um número,
 * calcular o somatório dos valores na matriz e sair do programa.
 */
void Menu(){
    printf("1. Adicionar linha\n");
    printf("2. Adicionar coluna\n");
    printf("3. Remover linha\n");
    printf("4. Remover coluna\n");
    printf("5. Alterar valor de um número\n");
    printf("6. Somatório\n");
    printf("7. Sair\n");
}

/**
 * \brief Função principal para executar o programa de manipulação de matriz representada por lista ligada.
 *
 * Esta função principal executa o programa que permite ao usuário visualizar a matriz escrita num documento .txt,
 * realizar operações de adição / remoção de linhas e colunas, alterar valores e apresenta o maior somatório possívelde da mesma.
 *
 * \return 0 -> Após a conclusão bem-sucedida do programa.
 */
 int main(){
    const char nomeDoFicheiro[] = "Matriz_Default.txt";         /**< Nome do arquivo. */
    setlocale(LC_ALL, "Portuguese");                            /**< Para poder utilizar caracteres especiais. */
    Lista* inicio;                                              /**< Endereço de memória para um registo do tipo Lista */
    int verificar = lerConteudo(nomeDoFicheiro);
    int opcao;
    int erro = 1;
    int nL = 0;
    int nC = 1;

    // Verifica se exite documento e se não está vazio
    if(verificar != 0)
        return(0);

    else{
        inicio = leitura(nomeDoFicheiro);
        contaLC(&nL, &nC, nomeDoFicheiro);
        listar(inicio, nL, nC);
        Menu();
    }

    do{
        while(erro == 1 || opcao != 7){
            erro = 0;
            printf("\n> ");

            // Se a leitura for um número
            if(scanf("%d", &opcao) == 1){
                switch(opcao){

                    // Adicionar linha
                    case 1:
                        system("cls");
                        listar(inicio, nL, nC);
                        inicio = adicionaLinha(inicio, &nL, &nC);
                        system("cls");
                        listar(inicio, nL, nC);
                        Menu();
                        break;

                    // Adicionar coluna
                    case 2:
                        system("cls");
                        listar(inicio, nL, nC);
                        inicio = adicionaColuna(inicio, &nL , &nC);
                        system("cls");
                        listar(inicio, nL, nC);
                        Menu();
                        break;

                    // Remover linha
                    case 3:
                        system("cls");
                        listar(inicio, nL, nC);
                        inicio = removerLinha(inicio, &nL , &nC);
                        system("cls");
                        listar(inicio, nL, nC);
                        Menu();
                        break;

                    // Remover coluna
                    case 4:
                        system("cls");
                        listar(inicio, nL, nC);
                        inicio = removerColuna(inicio, &nL, &nC);
                        system("cls");
                        listar(inicio, nL, nC);
                        Menu();
                        break;

                    // Alterar valor
                    case 5:
                        system("cls");
                        listar(inicio, nL, nC);
                        alteraValor(inicio, nL, nC);
                        system("cls");
                        listar(inicio, nL, nC);
                        Menu();
                        break;

                    // Somatório
                    case 6:
                        system("cls");
                        listar(inicio, nL, nC);
                        somatorio(inicio, &nL, &nC);
                        listar(inicio, nL, nC);
                        Menu();
                        break;

                    // Sair
                    case 7:
                        printf("\nA sair");
                        usleep(300000);             /**< Pausa em microssegundos. */
                        printf(".");
                        usleep(300000);
                        printf(".");
                        usleep(300000);
                        printf(".");
                        usleep(300000);
                        printf("\n");
                        libertarMemoria(inicio);    /**< Liberta a memoria ocupada pela lista. */
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
    }while(opcao != 7);
    return(0);
}
