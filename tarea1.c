#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Planificador de Tareas");
  puts("========================================");

  puts("1) Nueva Categoría");
  puts("2) Eliminar Categoría");
  puts("3) Mostrar Categorías");
  puts("4) Registrar Nueva Tarea");
  puts("5) Atender Siguiente");
  puts("6) Visualización del Tablero General");
  puts("7) Filtrado por Categoría");
  puts("8) Salir");
}

void registrar_categorias(List *categorias) {
  printf("Registrar nueva categoría\n");
  // Aquí implementarías la lógica para registrar una nueva categoría
}

void mostrar_categorias(List *categorias) {
  // Mostrar categorías
  printf("Categorías:\n");
  // Aquí implementarías la lógica para mostrar las categorías
}

int main() {
  char opcion;
  List *categorias = list_create(); // Lista para almacenar categorías

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) {
    case '1':
      registrar_categorias(categorias);
      break;
    case '2':
      // Lógica para eliminar una categoría
      break;
    case '3':
      mostrar_categorias(categorias);
      break;
    case '4':
      break;
    case '5':
      break;
    case '6':
      break;
    case '7':
      break;
    case '8':
      puts("Saliendo del sistema...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  // Liberar recursos, si es necesario
  list_clean(categorias);

  return 0;
}
