#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3
#define MAX_UNDO 20   // histórico de desfazer

// ---------------------- STRUCT DA PEÇA ----------------------
typedef struct {
    char tipo;
    int id;
} Peca;

// ---------------------- ACTION (para undo) ----------------------
typedef struct {
    int tipo; // 1=jogar, 2=reserva, 3=usar reserva, 4=trocar, 6=inverter
    Peca peca1, peca2; // peças antes/depois dependendo do caso
} Acao;


// ---------------------- FILA CIRCULAR ----------------------
typedef struct {
    Peca itens[MAX_FILA];
    int inicio, fim, total;
} Fila;

// ---------------------- PILHA PRINCIPAL ----------------------
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ---------------------- PILHA DE UNDO ----------------------
typedef struct {
    Acao itens[MAX_UNDO];
    int topo;
} PilhaUndo;


// ---------- gerar peca ----------
Peca gerarPeca() {
    char tipos[] = {'I','O','T','L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = rand() % 1000;
    return p;
}


// ---------- fila ----------
void inicializarFila(Fila *f) {
    f->inicio = f->fim = f->total = 0;
    for(int i=0; i<MAX_FILA; i++){
        f->itens[f->fim] = gerarPeca();
        f->fim = (f->fim + 1) % MAX_FILA;
        f->total++;
    }
}

Peca dequeue(Fila *f){
    Peca inv = {'X', -1};
    if(f->total == 0) return inv;

    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return p;
}

void enqueue(Fila *f){
    if(f->total == MAX_FILA) return;
    f->itens[f->fim] = gerarPeca();
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}


// ---------- pilha ----------
void inicializarPilha(Pilha *p){ p->topo = -1; }

void push(Pilha *p, Peca x){
    if(p->topo == MAX_PILHA - 1) {
        printf("Pilha cheia!\n");
        return;
    }
    p->itens[++p->topo] = x;
}

Peca pop(Pilha *p){
    Peca inv = {'X', -1};
    if(p->topo == -1) return inv;
    return p->itens[p->topo--];
}


// ---------- pilha undo ----------
void inicializarUndo(PilhaUndo *u){ u->topo = -1; }

void pushUndo(PilhaUndo *u, Acao a){
    if(u->topo < MAX_UNDO - 1)
        u->itens[++u->topo] = a;
}

Acao popUndo(PilhaUndo *u){
    Acao a = {-1};
    if(u->topo == -1) return a;
    return u->itens[u->topo--];
}


// ---------- inverter fila e pilha ----------
void inverter(Fila *f, Pilha *p){
    Peca tempFila[MAX_FILA];
    Peca tempPilha[MAX_PILHA];
    int tFila = f->total;
    int tPilha = p->topo + 1;

    // copia fila
    int idx = f->inicio;
    for(int i=0; i<tFila; i++){
        tempFila[i] = f->itens[idx];
        idx = (idx + 1) % MAX_FILA;
    }

    // copia pilha
    for(int i=0; i<tPilha; i++)
        tempPilha[i] = p->itens[i];

    // FILA recebe pilha
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;

    for(int i = 0; i < tPilha && i < MAX_FILA; i++){
        f->itens[f->fim] = tempPilha[i];
        f->fim = (f->fim + 1) % MAX_FILA;
        f->total++;
    }

    // PILHA recebe fila
    p->topo = -1;
    for(int i=0; i<tFila && i<MAX_PILHA; i++){
        p->itens[++p->topo] = tempFila[i];
    }
}


// ---------- exibir ----------
void exibirFila(Fila *f){
    printf("\n=== FILA ===\n");
    int idx = f->inicio;
    for(int i=0; i<f->total; i++){
        printf("[%d] %c (%d)\n", i, f->itens[idx].tipo, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
}

void exibirPilha(Pilha *p){
    printf("\n=== PILHA ===\n");
    for(int i=p->topo; i>=0; i--)
        printf("[%d] %c (%d)\n", i, p->itens[i].tipo, p->itens[i].id);
    if(p->topo == -1) printf("(vazia)\n");
}


// ---------------------- MAIN ----------------------
int main(){
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    PilhaUndo undo;

    inicializarFila(&fila);
    inicializarPilha(&pilha);
    inicializarUndo(&undo);

    int opc;

    do {
        printf("\n===== MENU - NIVEL MESTRE =====\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar fila <-> pilha\n");
        printf("5 - Desfazer\n");
        printf("6 - Inverter fila e pilha\n");
        printf("7 - Ver estruturas\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch(opc){

            case 1: { // jogar peça
                Peca p = dequeue(&fila);
                if(p.id != -1){
                    Acao a = {1, p};
                    pushUndo(&undo, a);

                    printf("Jogou: %c (%d)\n", p.tipo, p.id);
                    enqueue(&fila);
                }
                break;
            }

            case 2: { // reservar peça
                Peca p = dequeue(&fila);
                if(p.id != -1){
                    push(&pilha, p);
                    Acao a = {2, p};
                    pushUndo(&undo, a);
                    enqueue(&fila);
                    printf("Reservou: %c (%d)\n", p.tipo, p.id);
                }
                break;
            }

            case 3: { // usar reserva
                Peca p = pop(&pilha);
                if(p.id != -1){
                    Acao a = {3, p};
                    pushUndo(&undo, a);
                    printf("Usou reserva: %c (%d)\n", p.tipo, p.id);
                }
                break;
            }

            case 4: { // troca
                if(fila.total > 0 && pilha.topo >= 0){
                    Peca frente = fila.itens[fila.inicio];
                    Peca topo = pilha.itens[pilha.topo];

                    Acao a = {4, frente, topo};
                    pushUndo(&undo, a);

                    fila.itens[fila.inicio] = topo;
                    pilha.itens[pilha.topo] = frente;

                    printf("Troca realizada!\n");
                }
                break;
            }

            case 5: { // desfazer
                Acao a = popUndo(&undo);
                if(a.tipo == -1){
                    printf("Nada para desfazer!\n");
                    break;
                }

                if(a.tipo == 1){ // desfaz jogar
                    fila.inicio = (fila.inicio - 1 + MAX_FILA) % MAX_FILA;
                    fila.itens[fila.inicio] = a.peca1;
                    fila.total++;
                    printf("Desfeito: jogar\n");
                }

                else if(a.tipo == 2){ // desfaz reservar
                    Peca temp = pop(&pilha);
                    fila.inicio = (fila.inicio - 1 + MAX_FILA) % MAX_FILA;
                    fila.itens[fila.inicio] = temp;
                    fila.total++;
                    printf("Desfeito: reservar\n");
                }

                else if(a.tipo == 3){ // desfaz usar reserva
                    push(&pilha, a.peca1);
                    printf("Desfeito: usar reserva\n");
                }

                else if(a.tipo == 4){ // desfaz troca
                    Peca frente = fila.itens[fila.inicio];
                    Peca topo = pilha.itens[pilha.topo];

                    fila.itens[fila.inicio] = a.peca1;
                    pilha.itens[pilha.topo] = a.peca2;

                    printf("Desfeito: troca\n");
                }

                break;
            }

            case 6: {
                Acao a = {6};
                pushUndo(&undo, a);
                inverter(&fila, &pilha);
                printf("Fila e pilha invertidas!\n");
                break;
            }

            case 7:
                exibirFila(&fila);
                exibirPilha(&pilha);
                break;
        }

    } while(opc != 0);

    return 0;
}
