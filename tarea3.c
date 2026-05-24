#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>

#define ARRIBA 0
#define ABAJO 1
#define IZQUIERDA 2
#define DERECHA 3


// Definición de la estructura para el estado del puzzle
typedef struct {
    int maze[N][N]; // Matriz NxN que representa el tablero
    int x;    // Posición x del agente
    int y;    // Posición x del agente
    int steps; // Pasos realizados hasta la posición actual
    List* actions; //Secuencia de movimientos para llegar al estado
} State;

int distancia_L1(State* state) {
    return abs(state->x - (N-1)) + abs(state->y - (N-1));
}

int is_final(State* state) 
{
    return (state->x == (N-1) && 
            state->y == (N-1));
}

// NUEVO ESTADO
State* transition(State* current, int pos)
{
    State* next = malloc(sizeof(State));
    next->x = current->x;
    next->y = current->y;
    next->steps = current->steps + 1;
    next->actions = list_create();
    int* primero = list_first(current->actions);

    while(primero != NULL)
    {
        list_pushBack(next->actions, primero);
        primero = list_next(current->actions);
    }

    list_pushBack(next->actions, (void*)(long)pos);
    
    for(int i = 0; i < N; i++)
    {
        for(int k = 0; k < N; k++)
        {
            next->maze[i][k] = current->maze[i][k];
        }
    }
    
    if(pos == ARRIBA) next->x--;
    else if(pos == ABAJO) next->x++;
    else if(pos == IZQUIERDA) next->y--;
    else if(pos == DERECHA) next->y++;
    
    return next;
}

// vecinos 
List* get_adjacent_nodes(State* current)
{
    List* vecinos = list_create();
    for(int pos = 0; pos < 4; pos++)
    {
        State* next = transition(current, pos);
        if(next->x < 0 || next->x >= N || next->y < 0 || next->y >= N)
        {
            free(next);
            continue;
        }
        
        if(current->maze[next->x][next->y] == 1)
        {
            free(next);
            continue;
        }

        list_pushBack(vecinos, next);
    }
    return vecinos;
}

// Función para imprimir el estado actual
void imprimirEstado(const State *estado) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (estado->x==i && estado->y==j) printf(" A ");
            else if (i == 0 && j == 0) printf(" I "); 
            else if (i == N-1 && j == N-1) printf(" M ");
            else if (estado->maze[i][j] == 0)
                printf(" . "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("[X]");
        }
        printf("\n");
    }
}

State crearEstadoInicial(int maze[N][N], int dificultad){
    State estado;
     // Copiar el laberinto generado al estado
    generate_maze(estado.maze,  dificultad);
    estado.x = 0;
    estado.y = 0;
    estado.steps = 0;
    estado.actions = list_create();
    return estado;
}


void mostrar(State* estado)
{
    char tablero[N][N];
    for(int i = 0; i < N; i++)
    {
        for(int k = 0; k < N; k++)
        {
            if(estado->maze[i][k] == 1)
            {
                tablero[i][k] = 'X';
            }
            else
            {
                tablero[i][k] = ' ';
            }
        }
    }

    int x = 0;
    int y = 0;

    tablero[x][y] = 'I';

    int* primero = list_first(estado->actions);
    while(primero != NULL)
    {
        int m = (long) primero;
        if(m == ARRIBA) x--;
        else if(m == ABAJO) x++;
        else if(m == DERECHA) y++;
        else if(m == IZQUIERDA) y--;

        if(x != N-1 || y != N-1) tablero[x][y] = '+';
        primero = list_next(estado->actions);
    }
    tablero[N-1][N-1] = 'M';
    printf("\nCAMINO TOMADO:\n\n");

    for(int i = 0; i < N; i++)
    {
        for(int k = 0; k < N; k++)
        {
            printf(" %c ", tablero[i][k]);
        }
        printf("\n");
    }
}


void dfs(State inicial)
{
    List* stack = list_create(); // Creamos la lista que funciona como pila

    State* inicio = malloc(sizeof(State));
    *inicio = inicial;

    list_pushBack(stack, inicio);
    int visitado[N][N] = {0};
    int iteraciones = 0;
    while(list_size(stack) > 0)
    {
        State* current = list_popBack(stack);
        iteraciones++;
        if(visitado[current->x][current->y])
        {
            continue;
        }
        visitado[current->x][current->y] = 1;
        if(is_final(current))
        {
            printf("SOLUCION DFS\n");
            printf("Cantidad de pasos: %d\n", current->steps);
            printf("Cantidad de Iteraciones: %d\n", iteraciones);
            mostrar(current);
            return;
        }

        List* vecinos = get_adjacent_nodes(current); // obtenemos a los vecinos
        State* next = list_first(vecinos);
        
        while(next != NULL)
        {
            list_pushBack(stack, next);
            next = list_next(vecinos);
        }
        list_clean(vecinos);
        free(vecinos);
    }
}

void bfs(State inicial)
{
    List* busqueda = list_create();

    State* inicio = malloc(sizeof(State));
    *inicio = inicial;
    
    list_pushBack(busqueda, inicio);
    int visitado[N][N] = {0};
    int iteraciones = 0;
    while(list_size(busqueda) != 0)
    {
        State* current = list_popFront(busqueda); // 4
        iteraciones++;
        if(visitado[current->x][current->y]) continue;
        visitado[current->x][current->y] = 1; // mARCAmos

        if(is_final(current))
        {
            printf("SOLUCION BFS\n");
            printf("Cantidad de pasos: %d\n", current->steps);
            printf("Cantidad de Iteraciones: %d\n", iteraciones);
            mostrar(current);
            return;
        }

        List* vecinos = get_adjacent_nodes(current);
        State* next = list_first(vecinos);
        while(next != NULL)
        {
            list_pushBack(busqueda, next);
            next = list_next(vecinos);
        }
        list_clean(vecinos);
        free(vecinos);
    }
}


void best_first(State inicial)
{
    Heap* heap = heap_create();

    State* inicio = malloc(sizeof(State));
    *inicio = inicial;

    int prioridad_inicial = -(inicio->steps + distancia_L1(inicio));
    heap_push(heap, inicio, prioridad_inicial);
    int visitados[N][N] = {0};
    int iteraciones = 0;
    while(heap_top(heap) != NULL)
    {
        State* current = (State*) heap_top(heap);
        heap_pop(heap);
        iteraciones++;
        if(visitados[current->x][current->y]) continue;
        visitados[current->x][current->y] = 1;

        if(is_final(current))
        {
            printf("PUNTO ENCONTRADO\n");
            printf("Cantidad de pasos: %d\n", current->steps);
            printf("Cantidad de Iteraciones: %d\n", iteraciones);
            mostrar(current);
            return;
        }

        List* vecinos = get_adjacent_nodes(current);
        State* next = list_first(vecinos);
        while(next != NULL)
        {
            int prioridad = -(next->steps + distancia_L1(next));
            heap_push(heap, next, prioridad);
            next = list_next(vecinos);
        }
        list_clean(vecinos);
        free(vecinos);
    }
}

int main() {
    // Inicializar la semilla de aleatoriedad
    srand(time(NULL));

    int maze[N][N];
    int dificultad;

    // Solicitar la dificultad al usuario con validación
    do {
        printf("Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): ");
        scanf("%d", &dificultad);
        if (dificultad < 0 || dificultad > 100) {
            printf("Error: Por favor ingrese un valor válido entre 0 y 100.\n");
        }
    } while (dificultad < 0 || dificultad > 100);

    // Estado inicial del puzzle
    // Laberinto generado con la dificultad ingresada por el usuario
    State estado_inicial = crearEstadoInicial(maze, dificultad);

    // Imprime el estado inicial
    printf("\nEstado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    printf("Distancia L1: %d\n", distancia_L1(&estado_inicial));

    // Ejemplo de heap (cola con prioridad)
    printf("\n***** EJEMPLO USO DE HEAP ******\nCreamos un Heap e insertamos 3 elementos con distinta prioridad\n");
    Heap* heap = heap_create();
    char* data = strdup("Cinco");
    printf("Insertamos el elemento %s con prioridad -5\n", data);
    heap_push(heap, data, -5 /*prioridad*/);
    data = strdup("Seis");
    printf("Insertamos el elemento %s con prioridad -6\n", data);
    heap_push(heap, data, -6 /*prioridad*/);
    data = strdup("Siete");
    printf("Insertamos el elemento %s con prioridad -7\n", data);
    heap_push(heap, data, -7 /*prioridad*/);

    printf("\nLos elementos salen del Heap ordenados de mayor a menor prioridad\n");
    while (heap_top(heap) != NULL){
        printf("Top: %s\n", (char*) heap_top(heap));      
        heap_pop(heap);
    }
    printf("No hay más elementos en el Heap\n");

    char opcion;
    do {
        printf("\n***** EJEMPLO MENU ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");

        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");

        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
        case '1':
          dfs(estado_inicial);  
          break;
        case '2':
          bfs(estado_inicial);
          break;
        case '3':
          best_first(estado_inicial);
          break;
        }

        // Evitamos pausar y limpiar pantalla si el usuario eligió salir
        if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }

  } while (opcion != '4');

  return 0;
}
