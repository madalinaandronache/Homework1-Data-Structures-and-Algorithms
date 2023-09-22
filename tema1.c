/* Andronache Madalina-Georgiana - 312CC */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 20
#define MAX_COMMAND 8

// definire nod lista dublu inlantuita
typedef struct nod {
    char data;
    struct nod *next;
    struct nod *prev;
} nod;

// structura pentru banda + deget
typedef struct list {
    nod *head;     // primul nod din lista
    nod *current;  // elementul curent/degetul
} list;

// definire nod coada
typedef struct nod_q {
    int data;  // memoreaza codul operatiei efectuate
    char ch;   // memoreaza un caracter daca operatia are nevoie
    struct nod_q *next;
} nod_q;

// definire structura coada
typedef struct queue {
    nod_q *first;
    nod_q *last;
} queue;

// definire nod stiva
typedef struct nod_s {
    nod *pointer;  // pointer la un nod din lista
    struct nod_s *next;
} nod_s;

// definire structura stiva
typedef struct stack {
    nod_s *top;
} stack;

// initializare lista cu santinela
list *initL(void)
{
    list *band = NULL;
    band = (list *)malloc(sizeof(list));

    nod *sentinel = (nod *)malloc(sizeof(nod));
    sentinel->data = '\0';
    sentinel->prev = NULL;
    sentinel->next = NULL;

    band->head = sentinel;
    band->current = NULL;

    nod *current = (nod *)malloc(sizeof(nod));
    current->prev = band->head;
    current->data = '#';
    current->next = NULL;

    band->head->next = current;  
    band->current = current;     
    band->current->prev = band->head;

    return band;
}

// initializare stiva
stack *initS(void)
{
    stack *s = NULL;
    s = (stack *)malloc(sizeof(stack));
    s->top = NULL;

    return s;
}

// initializare coada
queue *initQ(void)
{
    queue *q = NULL;
    q = (queue *)malloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;

    return q;
}

// OPERATII DE TIP UPDATE

// operatia de MOVE LEFT
nod *move_left(nod *current, nod *head)
{
    if (current->prev != head) {
        current = current->prev;
    }

    return current;
}

// operatia de MOVE RIGHT
nod *move_right(nod *current)
{
    if (current->next != NULL) {
        current = current->next;

        return current;
    } else {
        nod *newNode = (nod *)malloc(sizeof(nod));

        newNode->data = '#';
        current->next = newNode;
        newNode->prev = current;
        newNode->next = NULL;

        return newNode;
    }
}

// operatia de MOVE LEFT CHAR
nod *move_left_char(nod *current, nod *head, char c, FILE *fout)
{
    nod *ptr = current;

    if (ptr->data == c) {
        return ptr;
    }

    while (ptr-> prev != head) {
        ptr = ptr->prev;
        if (ptr->data == c) {
            return ptr;
        }
    }

    fprintf(fout, "ERROR\n");

    return current;
}

// operatia de MOVE RIGHT CHAR
nod *move_right_char(nod *current, char c)
{
    nod *ptr = current;

    if(ptr->data == c) {
        return ptr;
    }

    while (ptr->next) {
        ptr = ptr->next;
        if (ptr->data == c) {
            return ptr;
        }
    }

    nod *newNode = (nod *)malloc(sizeof(nod));

    newNode->data = '#';
    ptr->next = newNode;
    newNode->prev = ptr;
    newNode->next = NULL;

    return newNode;
}

// operatia de WRITE
void write(nod *current, char c)
{
    current->data = c;
}

// operatia de INSERT RIGHT
nod *insert_right(nod **current, char c)
{
    nod *q = *current;
    nod *newNode = (nod *)malloc(sizeof(nod));

    if (q->next) {
        nod *ptr = q->next;

        newNode->data = c;
        q->next = newNode;
        newNode->prev = q;
        newNode->next = ptr;
        ptr->prev = newNode;

        return newNode;
    } else {
        newNode->data = c;
        q->next = newNode;
        newNode->prev = q;
        newNode->next = NULL;

        return newNode;
    }
}

// operatia de INSERT LEFT
nod *insert_left(nod **current, nod *head, char c, FILE *fout)
{
    nod *q = *current;

    if (q->prev == head) {
        fprintf(fout, "ERROR\n");

        return q;
    } else {
        nod *ptr = q->prev;
        nod *newNode = (nod *)malloc(sizeof(nod));

        newNode->data = c;
        ptr->next = newNode;
        newNode->prev = ptr;
        newNode->next = q;
        q->prev = newNode;

        return newNode;
    }
}

// OPERATII DE TIP QUERY

// operatia de SHOW CURRENT
void show_current(nod *current, FILE *fout)
{
    fprintf(fout, "%c\n", current->data);
}

// operatia de SHOW
void show(list *band, FILE *fout)
{
    nod *head = band->head;
    nod *p = head->next;
    nod *current = band->current;

    while (p->next) {
        if (p == current) {
            fprintf(fout, "|%c|", p->data);
        } else {
            fprintf(fout, "%c", p->data);
        }
        p = p->next;
    }

    if (!p->next) {
        if (p == current) {
            fprintf(fout, "|%c|", p->data);
        } else {
            fprintf(fout, "%c", p->data);
        }
    }

    fprintf(fout, "\n");
}

// OPERATII PE COADA

// introducere element in coada
void enqueue(queue **q, int x, char c)
{
    queue *k = *q;
    nod_q *newNode = (nod_q *)malloc(sizeof(nod_q));

    newNode->data = x;
    newNode->ch = c;
    newNode->next = NULL;

    if (!k->first) {
        k->first = newNode;
    }

    if (k->last) {
        k->last->next = newNode;
    }

    k->last = newNode;
}

// scoatere element din coada
int dequeue(queue **q, char *c)
{
    queue *k = *q;
    nod_q *ptr = k->first;
    int value = 0;

    if (ptr) {
        k->first = k->first->next;
        value = ptr->data;
        *c = ptr->ch;
        free(ptr);
        ptr = NULL;
    }

    if (!k->first) {
        k->last = NULL;
    }

    return value;
}

// OPERATII PE STIVA

// introducere element in stiva
void push(stack **s, nod *x)
{
    stack *k = *s;
    nod_s *newNode = (nod_s *)malloc(sizeof(nod_s));

    newNode->pointer = x;
    newNode->next = k->top;
    k->top = newNode;
}

// scoatere element din stiva
nod *pop(stack **s)
{
    stack *k = *s;
    nod_s *ptr = k->top;
    nod *data = ptr->pointer;

    k->top = k->top->next;

    free(ptr);
    ptr = NULL;

    return data;
}

// golire stiva
void empty_s(stack **s)
{
    stack *k = *s;
    nod_s *ptr = k->top;

    while (ptr) {
        k->top = k->top->next;
        free(ptr);
        ptr = k->top;
    }

    k->top = NULL;
}

// sparge in cuvant si caracter
char *transform(char *s, char *c)
{
    int i = 0;
    while (s[i] != ' ') {
        i++;
    }
    char *aux = NULL;
    aux = (char *)malloc(sizeof(char) * (i + 1));
    strncpy(aux, s, i);
    aux[i] = '\0';
    strcpy(s, aux);

    free(aux);

    *c = s[++i];

    return s;
}

// returneaza 1 daca este cuvant compus, 0 in caz contrar
int verify_word(char *s)
{
    int i = 0, ok = 0, lg = (int)strlen(s);

    while (i < lg) {
        if (s[i] == ' ') {
            ok = 1;
            break;
        }
        i++;
    }

    return ok;
}

/* returneaza 0 daca nu este operatie de UPDATE sau codul corespunzator 
codificarii operatiei de UPDATE */
int codific(char *s)
{
    int i = 0;
    char codif[MAX_COMMAND][MAX_CHAR] = {
        "MOVE_RIGHT\n",   "MOVE_LEFT\n",     "INSERT_LEFT", "INSERT_RIGHT",
        "MOVE_LEFT_CHAR", "MOVE_RIGHT_CHAR", "WRITE"};
    for (i = 0; i < MAX_COMMAND; i++) {
        if (strcmp(s, codif[i]) == 0) {
            return i + 1;
            break;
        }
    }

    return 0;
}

/* returneaza codul corespunzator pentru EXECUTE, SHOW, SHOW_CURRENT,
UNDO, REDO */

int command(char *s)
{
    char exe[MAX_CHAR] = "EXECUTE\n";
    char show[MAX_CHAR] = "SHOW\n";
    char show_curr[MAX_CHAR] = "SHOW_CURRENT\n";
    char undo[MAX_CHAR] = "UNDO\n"; 
    char redo[MAX_CHAR] = "REDO\n";

    if (strcmp(s, exe) == 0) {
        return 1;
    } else if (strcmp(s, show) == 0) {
        return 2;
    } else if (strcmp(s, show_curr) == 0) {
        return 3;
    } else if (strcmp(s, undo) == 0) {
        return 4;
    } else if (strcmp(s, redo) == 0) {
        return 5;
    }

    return 0;
}

// executa operatia corespunzatoare
nod *execute(int code, char c, nod **actual, nod *head, stack **undo,
             FILE *fout)
{
    nod *current = *actual;

    switch (code) {
        case 1:  // move_right
            push(&(*undo), current); // adaugare in stiva de undo
            current = move_right(current);
            return current;
            break;
        case 2:  // move_left
            if(current != head->next) {
                push(&(*undo), current); // adaugare in stiva de undo
            }
            current = move_left(current, head);
            return current;
            break;
        case 3:  // insert_left
            current = insert_left(&current, head, c, fout);
            return current;
            break;
        case 4:  // insert_right
            current = insert_right(&current, c);
            return current;
            break;
        case 5:  // move_left_char
            current = move_left_char(current, head, c, fout);
            return current;
            break;
        case 6:  // move_right_char
            current = move_right_char(current, c);
            return current;
            break;
        case 7:  // write
            write(current, c);
            empty_s(&(*undo)); // se goleste stiva de undo
            empty_s(&(*undo)); // se goleste stiva de redo
            return current;
            break;
    }
    return current;
}

// ELIBERAREA MEMORIEI

// eliberare memorie lista
void freeList(list *band)
{
    nod *current = band->head;
    nod *next = current->next;

    while (next != NULL) {
        free(current);
        current = next;
        next = current->next;
    }

    free(current); // eliberarea ultimului nod
    free(band); // eliberarea structurii list
}

// eliberare memorie coada
void freeQueue(queue *q)
{
    nod_q *current = q->first;
    nod_q *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(q);
}

// eliberare memorie stiva
void freeStack(stack *s) {
    nod_s *current = s->top;
    nod_s *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(s);
}

int main(void)
{
    list *band = initL();
    queue *q = initQ();
    stack *undo = initS();
    stack *redo = initS();

    int n = -1, i = 0, cod = -1;
    char s[MAX_CHAR], c = '|';

    char *aux = (char *)malloc(sizeof(char) * MAX_CHAR);

    FILE *fin = NULL, *fout = NULL;

    fin = fopen("tema1.in", "r");    
    fout = fopen("tema1.out", "w"); 

    if(fscanf(fin, "%d", &n) != 1) {
        printf("Eroare. Numarul nu a putut fi citit.\n");
    }
    if(fscanf(fin, "%c", &c) != 1) {
        printf("Eroare. Caracterul nu a putut fi citit.\n");
    }

    for (i = 0; i < n; i++) {
        if(!fgets(s, MAX_CHAR, fin)) {
            printf("Eroare. Sirul de caractere nu a putut fi citit.\n");
        }
        c = '|';  // un caracter care nu poate fi folosit niciodata

        if (verify_word(s) == 1) {
            strcpy(aux, transform(s, &c));
            strcpy(s, aux);
        }

        if (codific(s) != 0) {  // daca este operatie de tip UPDATE
            enqueue(&q, codific(s), c);
        }

        if (command(s) == 1) { // daca este operatia EXECUTE
            c = '|'; 
            cod = dequeue(&q, &c);
            band->current =
                execute(cod, c, &(band->current), band->head, &undo, fout);
        } else if (command(s) == 2) { // daca este operatia de SHOW
            show(band, fout);
        } else if (command(s) == 3) { // daca este operatia de SHOW_CURRENT
            show_current(band->current, fout);
        } else if (command(s) == 4) {
            push(&redo, band->current); // se adauga in stiva de REDO
            band->current = pop(&undo);
        } else if (command(s) == 5) {
            push(&undo, band->current); // se adauga in stiva de UNDO
            band->current = pop(&redo);
        }
    }
    
    freeList(band);
    freeQueue(q);
    freeStack(undo);
    freeStack(redo);
    free(aux);

    fclose(fin); // inchidere fisier de intrare
    fclose(fout); // inchidere fisier de iesire
    
    return 0;
}
