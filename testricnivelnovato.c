#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5

// ---------------------- STRUCT DA PEÇA ----------------------
typedef struct {
    char tipo;    // I, O, T, L
    int id;
} Peca;

// ---------------------- FILA CIRCULAR ----------------------
typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;


// ---------- Gera uma peça automaticamente ----------
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = rand() % 1000;
    return p;
}


// ---------- Inicializa a fila ----------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;

    for(int i = 0; i < MAX; i++) {
        f->itens[f->fim] = gerarPeca();
        f->fim = (f->fim + 1) % MAX;
        f->total++;
    }
}


// ---------- Enqueue ----------
void enqueue(Fila *f) {
    if (f->total == MAX) {
        printf("Fila cheia! Não é necessário inserir manualmente.\n");
        return;
    }
    f->itens[f->fim] = gerarPeca();
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}


// ---------- Dequeue ----------
Peca dequeue(Fila *f) {
    Peca p = {'X', -1};
    if (f->total == 0) {
        printf("Fila vazia!\n");
        return p;
    }
    p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
    return p;
}


// ---------- Exibir fila ----------
void exibirFila(Fila *f) {
    printf("\n=== FILA DE PEÇAS ===\n");
    int idx = f->inicio;
    for (int i = 0; i < f->total; i++) {
        printf("[%d] Tipo: %c | ID: %d\n", i, f->itens[idx].tipo, f->itens[idx].id);
        idx = (idx + 1) % MAX;
    }
}


// ---------------------- MAIN ----------------------
int main() {
    srand(time(NULL));

    Fila fila;
    inicializarFila(&fila);

    int opc;

    do {
        printf("\n===== MENU - NIVEL NOVATO =====\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("3 - Visualizar fila\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch(opc) {
            case 1: {
                Peca p = dequeue(&fila);
                if (p.id != -1) {
                    printf("Peça jogada: %c (%d)\n", p.tipo, p.id);
                    enqueue(&fila); // a fila deve ficar sempre cheia
                }
                break;
            }
            case 2:
                enqueue(&fila);
                break;

            case 3:
                exibirFila(&fila);
                break;
        }

    } while (opc != 0);

    return 0;
}
