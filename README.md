## Descripcion
Este programa permite cargar un catálogo de películas y realizar búsquedas por género, director, década o ID. Además, incluye una watchlist donde el usuario puede agregar películas para ver más tarde, eliminarlas o visualizar las películas guardadas. Finalmente, el sistema permite calificar las películas vistas por los usuarios.

## Como ejecutar el codigo
Este programa fue realizado en replit asi que les explicare como hacerlo funcionar en esta plataforma.
Para ejecutar la tarea1 primero debemos seguir los siguientes pasos:

1. Accede al repositorio de este codigo, y crea un nuevo repositorio usando el template
````
https://github.com/sebastianvalenciad-collab/TAREA-3-EDD
````
2. Coloca en la opción Import code or design. Luego importa desde Github.
3. Pega la dirección de tu repositorio y luego impórtalo (Import from Github).
4. Agrega una pestaña “Shell”
5. y ejecuta los siguientes codigos:
````
gcc tdas/*.c tarea3.c -Wno-unused-result -o tarea3
````

Y luego ejecutar:
````
./tarea3
````

## Funcionalidades

1. Implementación de búsqueda en profundidad (DFS).
2. Implementación de búsqueda en anchura (BFS).
3. Implementación de búsqueda Best First / A* usando Heap.

### Posibles fallas:

1. El DFS (busqueda en profundidad) no garantiza el camino mas corto.

## Ejemplo de uso

1. Escoger dificultad del laberinto: El usuario puede escoger la dificultad del laberinto, puede ser de 0 a 100.
   <img width="465" height="16" alt="image" src="https://github.com/user-attachments/assets/5b3a4afc-7785-474c-946f-fb65d9e83664" />

2. Busqueda en profundidad: Si escogemos 1, usaremos la busqueda en profundidad.  El codigo nos mostrara la cantidad de pasos que tomo el camino, y la cantidad de nodos que visito, ademas el camino mapeado.
   <img width="183" height="202" alt="image" src="https://github.com/user-attachments/assets/0876a9c1-a832-470b-8613-eedd104d67c4" />

3. Busqueda  en anchura: Si escogemos 2, usaremos la busqueda en anchura. El codigo nos mostrara la cantidad de pasos que tomo el camino, y la cantidad de nodos que visito, ademas el camino mapeado.
   <img width="192" height="207" alt="image" src="https://github.com/user-attachments/assets/7d83722f-6606-4dd8-8b62-02a8e496903b" />

4. Busqueda Best First: Si escogemos 3, usaremos la busqueda Best First. El codigo nos mostrara la cantidad de pasos que tomo el camino, y la cantidad de nosod que visito, ademas del camino mapeado.
   <img width="186" height="206" alt="image" src="https://github.com/user-attachments/assets/27597299-2b89-4143-a6c3-7a5392971897" />

