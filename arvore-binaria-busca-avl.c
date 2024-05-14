#include <stdio.h>
#include <stdlib.h>

typedef struct SArvore{
    int info;
    struct SArvore *esq;
    struct SArvore *dir;
    short altura;
} TArvore;


// void preOrdem(TArvore *no){
//     if (no == NULL)
//         return;
//     printf("%d ",no->info);
//     preOrdem(no->esq);
//     preOrdem(no->dir);
// }

// void ordem(TArvore *no){
//     if (no == NULL)
//         return;
//     ordem(no->esq);
//     printf("%d ",no->info);
//     ordem(no->dir);
// }

// void posOrdem(TArvore *no){
//     if (no == NULL)
//         return;
//     posOrdem(no->esq);
//     posOrdem(no->dir);
//     printf("%d ",no->info);
// }

short maior(short a, short b){
    return (a > b)? a : b;
}

short altura(TArvore *no){
    if (no == NULL)
        return -1;
    return no->altura;
}

short fatorBalanceamento(TArvore *no){
    if (no)
        return (altura(no->esq) - altura(no->dir));
    return -1;
}

TArvore *rotacaoEsquerda(TArvore *no){
    TArvore *x, *y;

    x = no->dir;
    y = x->esq;

    x->esq = no;
    no->dir = y;

    no->altura = maior(altura(no->esq), altura(no->dir)) + 1;
    x->altura = maior(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

TArvore *rotacaoDireita(TArvore *no){
    TArvore *x, *y;

    x = no->esq;
    y = x->dir;

    x->dir = no;
    no->esq = y;

    no->altura = maior(altura(no->esq), altura(no->dir)) + 1;
    x->altura = maior(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

TArvore *rotacaoDireitaEsquerda(TArvore *no){
    no->dir = rotacaoDireita(no->dir);

    return rotacaoEsquerda(no);
}

TArvore *rotacaoEsquerdaDireita(TArvore *no){
    no->esq = rotacaoEsquerda(no->esq);

    return rotacaoDireita(no);
}

TArvore* balancear(TArvore *raiz){
    short fb = fatorBalanceamento(raiz);

    // Rotação à esquerda
    if(fb < -1 && fatorBalanceamento(raiz->dir) <= 0)
        raiz = rotacaoEsquerda(raiz);

    // Rotação à direita
    else if(fb > 1 && fatorBalanceamento(raiz->esq) >= 0)
        raiz = rotacaoDireita(raiz);

    // Rotação dupla à esquerda
    else if(fb > 1 && fatorBalanceamento(raiz->esq) < 0)
        raiz = rotacaoEsquerdaDireita(raiz);

    // Rotação dupla à direita
    else if(fb < -1 && fatorBalanceamento(raiz->dir) > 0)
        raiz = rotacaoDireitaEsquerda(raiz);

    return raiz;
}

void remover(TArvore **no, int dado){
    if (*no == NULL)
        return;
    if ((*no)->info == dado){
        TArvore *temp = *no;
        if ((*no)->esq == NULL){
            *no = (*no)->dir;
        }
        else if ((*no)->dir == NULL){
            *no = (*no)->esq;
        }
        else{
            TArvore *min = (*no)->dir;
            while (min->esq != NULL){
                min = min->esq;
            }
            min->esq = (*no)->esq;
            *no = (*no)->dir;
        }
        free(temp);
        return;
    }
    remover(&((*no)->esq), dado);
    remover(&((*no)->dir), dado);

    (*no)->altura = maior(altura((*no)->esq), altura((*no)->dir)) + 1;
    *no = balancear(*no);
}

void imprimir(TArvore *raiz, int nivel){
    int i;
    if(raiz){
        imprimir(raiz->dir, nivel + 1);
        printf("     \n\n");

        for(i = 0; i < nivel; i++){
            printf("\t\t");
        }
        printf("%d", raiz->info);
        imprimir(raiz->esq, nivel + 1);
    }
}

TArvore * criarNo(int dado){
    TArvore *novo = malloc(sizeof(TArvore));
    novo->info = dado;
    novo->esq = novo->dir = NULL;
    novo->altura = 0;

    return novo;
}

TArvore* preencherABB(TArvore *no, int dado){
    if (no == NULL){
        return criarNo(dado);
    }
    if (dado < no->info){
        no->esq = preencherABB(no->esq, dado);
    }else if (dado > no->info){
        no->dir = preencherABB(no->dir, dado);
    }else{
        printf("Nao e possivel inserir um valor duplicado numa arvore binaria de busca.\n");
        return no;
    }
    no->altura = maior(altura(no->esq), altura(no->dir)) + 1;
    return balancear(no);
}

TArvore *buscaBinaria(TArvore *no, int key){
    if (no == NULL)
        return NULL;

    if (no->info == key)
        return no;
    else if (key < no->info){
        return buscaBinaria(no->esq, key);
    }else
        return buscaBinaria(no->dir, key);
}

int main() {

    int leitura;
    int aRemover;

    TArvore *raiz = malloc(sizeof(TArvore));
    printf("Informe o valor da raiz: ");
    scanf("%d",&raiz->info);
    raiz->esq = raiz->dir = NULL;
    raiz->altura = 0;

    while (1){
        printf("Insira o valor: ");
        scanf("%d",&leitura);
        if (leitura == 0)
            break;
        raiz = preencherABB(raiz, leitura);
    }

    imprimir(raiz, 1);

    while(1){
        printf("\ndeseja remover um no? ");
        scanf("%d", &aRemover);
        if (aRemover == 0){
            break;
        }else{
            remover(&raiz, aRemover);
        }
        imprimir(raiz, 1);
    }
    
    int chave;
    printf("\nInforme a chave da busca: ");
    scanf("%d",&chave);
    if (chave == 0){
        return (EXIT_SUCCESS);
    }
    
    TArvore *posicao = buscaBinaria(raiz, chave);
    if (posicao == NULL){
        printf("Chave nao encontrada.");
    }else{
        printf("Chave encontrada no endereco: %p", posicao);
    }

    return (EXIT_SUCCESS);
}