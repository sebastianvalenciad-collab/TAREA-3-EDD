#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>

// Movimientos que se pueden hacer dentro de la matriz
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


int is_final(State* state) // Funcion que verifica si llegfamos a la meta
{
    return (state->x == (N-1) &&  // si la coordenada x e y son iguales a la llegada
            state->y == (N-1)); // es porque llegamos a la meta
}

// NUEVO ESTADO a partir del estado actual
State* transition(State* current, int pos)
{
    State* next = malloc(sizeof(State)); // asignamos memoria al nuevo estado
    next->x = current->x; // copiamos la informacion de current
    next->y = current->y;
    next->steps = current->steps + 1; // aumentamos la cantidad de pasos
    next->actions = list_create();  // creamos una lista de acciones
    int* primero = list_first(current->actions); // obtenemos la primera accion del estado actual

    while(primero != NULL)  // guardamos (copiamos) las acciones anteriores a next (siguiente estado)
    {
        list_pushBack(next->actions, primero);
        primero = list_next(current->actions);
    }

    list_pushBack(next->actions, (void*)(long)pos); // agregamos el nuevo movimiento al estado siguiente
    
    for(int i = 0; i < N; i++) // copiamos la matriz
    {
        for(int k = 0; k < N; k++)
        {
            next->maze[i][k] = current->maze[i][k];
        }
    }

    // aplicamos el movimiento segun la direccion que nos den
    if(pos == ARRIBA) next->x--;
    else if(pos == ABAJO) next->x++;
    else if(pos == IZQUIERDA) next->y--;
    else if(pos == DERECHA) next->y++;
    
    return next; // retornamos el nuevo estado
}

// OBTENER vecinos validos 
List* get_adjacent_nodes(State* current)
{
    List* vecinos = list_create(); // creamos la lista para guardar los vecinos
    for(int pos = 0; pos < 4; pos++) // recorremos las 4 direcciones posibles
    {
        State* next = transition(current, pos); // creamos un estado que se mueve hacia esa direccion
        if(next->x < 0 || next->x >= N || next->y < 0 || next->y >= N) // verificamos si el mov sale del tablero
        {
            free(next); // liberamos memoria del estado de la direccion
            continue; // siguiente iteracion
        }
        
        if(current->maze[next->x][next->y] == 1) // verificamos si es un 1 (pared)
        {
            free(next); // liberamos memoria del estado
            continue; // siguiente iteracion
        }

        list_pushBack(vecinos, next); // si es valido se agrega a la lista
    }
    return vecinos; // retornamos los vecinos
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
    char tablero[N][N]; // creamos una matriz auxiliar
    
    // recorremos la matriz original y colocamos X encaso de pared
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
                tablero[i][k] = ' '; // si no hay pared dejamos vacio
            }
        }
    }

    int x = 0; // posicion inicial
    int y = 0; // posicion inicial

    tablero[x][y] = 'I'; // inicio

    int* primero = list_first(estado->actions); // obtenemos la primera accion
    while(primero != NULL) // recorremos la secuencia de movimientos
    {
        int m = (long) primero; // convertimos el puntero a entero
        if(m == ARRIBA) x--; // aplicamos movimiento
        else if(m == ABAJO) x++; // aplicamos movimiento
        else if(m == DERECHA) y++; // aplicamos movimiento
        else if(m == IZQUIERDA) y--; // aplicamos movimiento

        if(x != N-1 || y != N-1) tablero[x][y] = '+'; // marcamos el camino recorrido
        primero = list_next(estado->actions); // avanzamos a la siguiente accion
    }
    tablero[N-1][N-1] = 'M'; // meta
    printf("\nCAMINO FINAL:\n\n");

    // printeamos el camino final
    for(int i = 0; i < N; i++)
    {
        for(int k = 0; k < N; k++)
        {
            printf(" %c ", tablero[i][k]);
        }
        printf("\n");
    }
}

// BUSQUEDA en profundidad
void dfs(State inicial)
{
    List* stack = list_create(); // Creamos la lista que funciona como pila

    State* inicio = malloc(sizeof(State)); // reservamos memoria para el estado inicial
    *inicio = inicial; // copaimos el estado inicial

    list_pushBack(stack, inicio); // insertamos el estado inicial a la pila
    int visitado[N][N] = {0}; // posiciones visitadas
    int iteraciones = 0; // iteraciones
    while(list_size(stack) > 0) // mientras la pila no este vacia el while funciona
    {
        State* current = list_popBack(stack); // desapilamos la pila (sacamos el ultimo elemento)
        iteraciones++; // sumamos 1 iteracion
        if(visitado[current->x][current->y]) // si la posicion ya fue visitada pasamas la siguiente
        {
            continue;
        }
        visitado[current->x][current->y] = 1; // marcamos la posicion
        if(is_final(current)) // verificamos si llegamos al final
        {
            printf("SOLUCION DFS\n");
            printf("Cantidad de pasos: %d\n", current->steps);
            printf("Cantidad de Iteraciones: %d\n", iteraciones);
            mostrar(current);
            return;
        }

        List* vecinos = get_adjacent_nodes(current); // obtenemos a los vecinos validos del estado actual
        State* next = list_first(vecinos); // primer vecino
        
        while(next != NULL) // recorremos los vecinos
        {
            list_pushBack(stack, next); // insertamos el vecino a la pila
            next = list_next(vecinos); // avanzamos al siguiente vecino
        }
        list_clean(vecinos); // liberamos vecinos
        free(vecinos);
    }
}

// busqueda en anchura
void bfs(State inicial)
{
    List* busqueda = list_create();  // creamos una lista que funciona como cola

    State* inicio = malloc(sizeof(State)); // mewmoria para el estado inicial
    *inicio = inicial; // copiamos el estado inicial
    
    list_pushBack(busqueda, inicio); //insertamos el estado inicial a la cola
    int visitado[N][N] = {0}; // visitados (empiezan todos en 0)
    int iteraciones = 0; // iteraciones
    while(list_size(busqueda) != 0) // si la cola no esta vacia sigue
    {
        State* current = list_popFront(busqueda); // sacamos el primer elemento de la cola
        iteraciones++; // iteraciones + 1
        if(visitado[current->x][current->y]) continue; //si la pos fue visitada pasamos a la siguiente iteracion
        visitado[current->x][current->y] = 1; // mARCAmos la posicion

        if(is_final(current)) // verificanmos si llegamos al final
        {
            printf("SOLUCION BFS\n");
            printf("Cantidad de pasos: %d\n", current->steps);
            printf("Cantidad de Iteraciones: %d\n", iteraciones);
            mostrar(current);
            return;
        }

        List* vecinos = get_adjacent_nodes(current); // obtenbemos todos los nodos vecinos validos
        State* next = list_first(vecinos); // obtenemos el primero
        while(next != NULL) // recorremos los vecinos
        {
            list_pushBack(busqueda, next); // insertamos los vecinos al final de la cola
            next = list_next(vecinos); // pasamos al siguiente
        }
        list_clean(vecinos); // liberamos vecinos
        free(vecinos);
    }
}

//busqueda fbest first
void best_first(State inicial)
{
    Heap* heap = heap_create(); // creamos una cola de prioridad

    State* inicio = malloc(sizeof(State)); // reservamos memoria para el estado inicial
    *inicio = inicial; // copiamos el estado inicial

    int prioridad_inicial = -(inicio->steps + distancia_L1(inicio)); //calculamos la prioridad incial con los pasos realizados + distancia
    heap_push(heap, inicio, prioridad_inicial); // insertamos el estado inicial en la cola
    int visitados[N][N] = {0}; // posiciones visitadas
    int iteraciones = 0; // iteraciones
    while(heap_top(heap) != NULL) // mientras el heap no este vacio
    {
        State* current = (State*) heap_top(heap); // obtenemos el estado con la mayor prioridad
        heap_pop(heap); // eliminamos el elemento del heap
        iteraciones++; // sumamos 1 a iteraciones
        if(visitados[current->x][current->y]) continue; // si la pos ya fue visitada avanzamos a la sgte iteracion
        visitados[current->x][current->y] = 1; // marcamos la posicion

        if(is_final(current)) // verificamos si llegamos al final
        {
            printf("PUNTO ENCONTRADO\n");
            printf("Cantidad de pasos: %d\n", current->steps);
            printf("Cantidad de Iteraciones: %d\n", iteraciones);
            mostrar(current);
            return;
        }

        List* vecinos = get_adjacent_nodes(current); // obtenemos los vecinos 
        State* next = list_first(vecinos); // primer vecino 
        while(next != NULL) // recorremos todos los vecinos
        {
            int prioridad = -(next->steps + distancia_L1(next)); // calculamos la prioridad del vecino
            heap_push(heap, next, prioridad); // insertamos el vecino en la cola
            next = list_next(vecinos); // pasamos al siguiente
        }
        list_clean(vecinos); // liberamos la lista de vecinos
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
