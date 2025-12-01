#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// ---------------------- STRUCT DA PEÇA ----------------------
typedef struct {
    char tipo;
    int id;
} Peca;

// ---------------------- FILA CIRCULAR ----------------------
typedef struct {
    Peca itens[MAX_FILA];
    int inicio, fim, total;
} Fila;

// ---------------------- PILHA ----------------------
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;


// ---------- Gera peça automaticamente ----------
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = rand() % 1000;
    return p;
}


// ---------- Fila: inicializar ----------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;

    for (int i = 0; i < MAX_FILA; i++) {
        f->itens[f->fim] = gerarPeca();
        f->fim = (f->fim + 1) % MAX_FILA;
        f->total++;
    }
}


// ---------- Fila: enqueue ----------
void enqueue(Fila *f) {
    if (f->total == MAX_FILA) return;

    f->itens[f->fim] = gerarPeca();
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}


// ---------- Fila: dequeue ----------
Peca dequeue(Fila *f) {
    Peca invalida = {'X', -1};
    if (f->total == 0) return invalida;

    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return p;
}


// ---------- Pilha: iniciar ----------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}


// ---------- Pilha: push ----------
void push(Pilha *p, Peca x) {
    if (p->topo == MAX_PILHA - 1) {
        printf("Pilha cheia! Não é possível reservar.\n");
        return;
    }
    p->itens[++p->topo] = x;
}


// ---------- Pilha: pop ----------
Peca pop(Pilha *p) {
    Peca invalida = {'X', -1};
    if (p->topo == -1) {
        printf("Pilha vazia! Nada para usar.\n");
        return invalida;
    }
    return p->itens[p->topo--];
}


// ---------- Exibir fila ----------
void exibirFila(Fila *f) {
    printf("\n=== FILA DE PEÇAS ===\n");
    int idx = f->inicio;
    for (int i = 0; i < f->total; i++) {
        printf("[%d] %c (%d)\n", i, f->itens[idx].tipo, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
}


// ---------- Exibir pilha ----------
void exibirPilha(Pilha *p) {
    printf("\n=== PILHA DE RESERVA ===\n");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%d] %c (%d)\n", i, p->itens[i].tipo, p->itens[i].id);
    }
    if (p->topo == -1)
        printf("(vazia)\n");
}


// ---------------------- MAIN ----------------------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opc;

    do {
        printf("\n===== MENU - NIVEL AVENTUREIRO =====\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça (push)\n");
        printf("3 - Usar peça reservada (pop)\n");
        printf("4 - Ver estruturas\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {

            case 1: { // Jogar peça
                Peca p = dequeue(&fila);
                if (p.id != -1) {
                    printf("Peça jogada: %c (%d)\n", p.tipo, p.id);
                    enqueue(&fila); // mantém a fila cheia
                }
                break;
            }

            case 2: { // Reservar
                Peca p = dequeue(&fila);
                if (p.id != -1) {
                    printf("Peça reservada: %c (%d)\n", p.tipo, p.id);
                    push(&pilha, p);
                    enqueue(&fila);
                }
                break;
            }

            case 3: { // Usar reservada
                Peca p = pop(&pilha);
                if (p.id != -1) {
                    printf("Peça usada da reserva: %c (%d)\n", p.tipo, p.id);
                }
                break;
            }

            case 4:
                exibirFila(&fila);
                exibirPilha(&pilha);
                break;
        }

    } while (opc != 0);

    return 0;
}
