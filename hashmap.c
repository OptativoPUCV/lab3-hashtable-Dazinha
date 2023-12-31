#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

/*
1.- Implemente la función *createMap* en el archivo hashmap.c. Esta función crea una variable de tipo HashMap, inicializa el arreglo de buckets con casillas nulas, inicializa el resto de variables y retorna el mapa. Inicialice el índice current a -1.
*/

HashMap * createMap(long capacity) {
  HashMap *newMap = (HashMap*) malloc(sizeof(HashMap));
  
  if (newMap == NULL)
  {
    return NULL;
  }

  newMap -> buckets = (Pair**) calloc(sizeof(Pair), capacity);
  newMap -> size = 0;
  newMap -> capacity = capacity;
  newMap -> current = -1;
  return newMap;
}

/*
2.- Implemente la función void insertMap(HashMap * map, char * key, void * value). Esta función inserta un nuevo dato (key,value) en el mapa y actualiza el índice current a esa posición.
Recuerde que para insertar un par (clave,valor) debe:

a - Aplicar la función hash a la clave para obtener la posición donde debería insertar el nuevo par

b - Si la casilla se encuentra ocupada, avance hasta una casilla disponible (*método de resolución de colisiones*). Una casilla disponible es una casilla nula, pero también una que tenga un par inválido (key==NULL).

c - Ingrese el par en la casilla que encontró.

No inserte claves repetidas. 
Recuerde que el arreglo es **circular**.
Recuerde actualizar la variable size.
*/
// insertMap(map,_strdup(words[i]),cont);
void insertMap(HashMap * map, char * key, void * value) {
  if (key == NULL || map == NULL)
  {
    return;
  }

  long capacity = map -> capacity;
  
  //Apicar función hash para obtener la posición
  long position = hash(key, capacity);

  //Si la casilla está ocupada, avanzar hasta una disponible
  while (map -> buckets[position] != NULL && map->buckets[position]->key != NULL)
  { 
    //llamar a funcion is_equal para evitar que sean iguales
    if (is_equal(map->buckets[position]->key, key)) 
    {
      return;
    }

    position = (position + 1) % (capacity);
  }

  
  //crear el nuevo par
  Pair *newPair = createPair(strdup(key), value);

  if (newPair == NULL)
  {
    return;
  }

  //actualizar el par
  map -> buckets[position] = newPair;
  map -> size ++;
  map -> current = position;
}


/*
3.- Implemente la función Pair * searchMap(HashMap * map,  char * key), la cual retorna el **Pair** asociado a la clave ingresada. 
Recuerde que para buscar el par debe:

a - Usar la función hash para obtener la posición donde puede encontrarse el par con la clave

b - Si la clave no se encuentra avance hasta encontrarla (*método de resolución de colisiones*)

c - Si llega a una casilla nula, retorne NULL inmediatamente (no siga avanzando, la clave no está)

Recuerde actualizar el índice current a la posición encontrada.
Recuerde que el arreglo es **circular**.
*/
//long hash( char * key, long capacity)
//is_equal(void* key1, void* key2)

Pair * searchMap(HashMap * map,  char * key) {   
  if (map == NULL || key == NULL)
  {
    return NULL;
  }

  //a
  long capacity = map -> capacity;  
  long position = hash(key, capacity);

  //b
  while (map -> buckets[position] != NULL)
  {
    if (is_equal(map->buckets[position]->key, key)) 
    {
      map -> current = position;
      return map -> buckets[position];
    }
      
    position = (position + 1) % (capacity);
  }

  //c)
  return NULL;
}

/*
4.- Implemente la función void eraseMap(HashMap * map,  char * key). Ésta función elimina el dato correspondiente a la clave key. Para hacerlo debe buscar el dato y luego *marcarlo* para que no sea válido.
**No elimine el par**, sólo invalídelo asignando NULL a la clave (pair->key=NULL).
Recuerde actualizar la variable size.
*/

void eraseMap(HashMap * map,  char * key) {    
  
  if (map == NULL || key == NULL)
  {
    return;
  }

  long capacity = map -> capacity;  
  long position = hash(key, capacity);
  
  while( map -> buckets[position] != NULL)
  {
      if (map -> buckets[position] -> key != NULL && is_equal(map -> buckets[position] -> key, key))
      {
        //asignar nulo 
        map -> buckets[position] -> key = NULL;
        map -> size --;
      }

    position = (position + 1) % (capacity);
  }
  return;
}

/*
5.- Implemente las funciones para recorrer la estructura: Pair * firstMap(HashMap * map) retorna el primer **Pair** válido del arreglo buckets. 
Recuerde actualizar el índice.
Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}
*/

Pair * firstMap(HashMap * map) {
  if (map == NULL)
  {
    return NULL;
  }

  long size = map -> size;

  for(long i = 0 ; i < size ; i++)
    {
      //Pair *newPair = createPair(strdup(key), value);
      Pair * newPair = map-> buckets[i];

      if(newPair != NULL && newPair -> key != NULL)
      {
        map -> current = i;    //long current; //indice del ultimo dato accedido
        return newPair;
      }
      
    }

  
  return NULL;
}

//Pair * nextMap(HashMap * map) retorna el siguiente **Pair** del arreglo buckets a partir índice current. 

Pair * nextMap(HashMap * map) {
  if (map == NULL || map -> current == -1)
  {
    return NULL;
  }

  long capacity = map -> capacity;

  for(long i = map -> current + 1; i < capacity ; i++)
    {
      //Pair *newPair = createPair(strdup(key), value);
      Pair *newPair = map-> buckets[i];

      if(newPair != NULL && newPair -> key != NULL)
      {
        map -> current = i;    //long current; //indice del ultimo dato accedido
        return newPair;
      }
      
    }

  map -> current = -1;
  return NULL;
}


/*
6.- Implemente la función void enlarge(HashMap * map). Esta función agranda la capacidad del arreglo buckets y reubica todos sus elementos. Para hacerlo es recomendable mantener referenciado el arreglo *actual/antiguo* de la tabla con un puntero auxiliar. Luego, los valores de la tabla se reinicializan con un nuevo arreglo con el **doble de capacidad**. Por último los elementos del arreglo antiguo se insertan en el mapa *vacío* con el método *insertMap*.
Puede seguir los siguientes pasos:

a - Cree una variable auxiliar de tipo Pair** para matener el arreglo map->buckets (*old_buckets*);

b - Duplique el valor de la variable capacity.

c - Asigne a map->buckets un nuevo arreglo con la nueva capacidad.

d - Inicialice size a 0.

e - Inserte los elementos del arreglo *old_buckets* en el mapa (use la función insertMap que ya implementó).
void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)

  map->capacity = 2;
  Pair  newBuckets = (Pair )malloc(sizeof(Pair) * map->capacity);
  if (newBuckets == NULL) {
       return;
  }

  for (int i = 0; i < map->capacity / 2; i++) {
      if (map->buckets[i] != NULL) {
          long cont = hash(map->buckets[i]->key,map->capacity) ;

          while (newBuckets[cont] != NULL) {
            cont = (cont + 1) % map->capacity;
          }
          newBuckets[cont] = map->buckets[i];
      }
    }

    free(map->buckets);
    map->buckets = newBuckets;
}


*/

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)

  map -> capacity *= 2;

  //c) Asigne a map->buckets un nuevo arreglo con la nueva capacidad.
  //newMap -> buckets = (Pair**) calloc(sizeof(Pair), capacity);
  Pair** newBuckets = (Pair**)malloc(sizeof(Pair) * map -> capacity);
  
  if (map -> buckets == NULL)
  {
    return;
  }
  
  for (long i = 0; i < map -> capacity / 2 ; i++) 
  {    
    if (map -> buckets[i] != NULL) 
    {
      long count = hash(map->buckets[i] -> key, map -> capacity) ;

      while (newBuckets[count] != NULL) 
      {
      count = (count + 1) % map -> capacity;
      }
      
      newBuckets[count] = map->buckets[i];
    }
    
  }

  free(map -> buckets);

  map -> buckets = newBuckets;
}

