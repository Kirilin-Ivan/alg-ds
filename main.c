#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#pragma warning (disable:4996)

#define SUCCESS 1
#define FAIL -1
#define SIZE 500

typedef enum {
  FREE,
  NOT_FREE,
  DELETED
}T_STATUS;

typedef struct {
  char* str;
  int status;
}T_HASHTABLE;

T_HASHTABLE* Create(int size) {
  T_HASHTABLE* table;

  table = (T_HASHTABLE*)malloc(sizeof(T_HASHTABLE) * size);
  if (table == NULL)
    return NULL;

  for (int i = 0; i < size; i++)
    table[i].status = FREE;

  return table;
}

static int Hash(char* str, int size) {
  int index = 0;

  for (int i = 0; str[i] != 0; i++)
    index += str[i];

  return index % size;
}

void Insert(T_HASHTABLE* table, char* elem, int size) {
  int key = Hash(elem, size);
  int index = key;
  for (int pos = 0; pos < size; pos++) {
    if (table[index].status == FREE || table[index].status == DELETED) {
      table[index].str = (char*)malloc((strlen(elem) + 1) * sizeof(char));
      strcpy(table[index].str, elem);
      table[index].status = NOT_FREE;
      return;
    }
    else
      if (table[index].status == NOT_FREE && strcmp(table[index].str, elem) == 0)
        return;
    index = (index + 2 * pos + 1) % size;
  }
}

int Search(T_HASHTABLE* table, char* elem, int size) {
  int key = Hash(elem, size);
  int index = key;
  for (int pos = 0; pos < size; pos++) {
    if (table[index].status == NOT_FREE && strcmp(table[index].str, elem) == 0)
      return SUCCESS;
    else
      if (table[index].status == FREE)
        return FAIL;
    index = (index + 2 * pos + 1) % size;
  }
  return FAIL;
}

void Delete(T_HASHTABLE* table, char* elem, int size) {
  int key = Hash(elem, size);
  int index = key;
  for (int pos = 0; pos < size; pos++) {
    if (table[index].status == NOT_FREE && strcmp(table[index].str, elem) == 0) {
      free(table[index].str);
      table[index].status = DELETED;
      return;
    }
    else
      if (table[index].status == FREE)
        return;
    index = (index + 2 * pos + 1) % size;
  }
}

void Clear(T_HASHTABLE* table, int size) {
  for (int i = 0; i < size; i++)
    if (table[i].status == NOT_FREE)
      free(table[i].str);
  free(table);
}

int main() {

  T_HASHTABLE* table;
  table = Create(SIZE);
  char key[100];
  char command;
  char symbol;

  while (1) {
    command = getchar();
    if (command == EOF)
      break;
    while (command == '\0' || command == '\n' || isspace(command) != 0)
      symbol = getchar();
    scanf("%s", &key);
    switch (command) {
    case('a'):
      Insert(table, key, SIZE);
      break;
    case('r'):
      Delete(table, key, SIZE);
      break;
    case('f'):
      Search(table, key, SIZE) == SUCCESS ? puts("yes") : puts("no");
      break;
    default:
      break;
    }
    symbol = getchar();
  }
  Clear(table, SIZE);
  return 0;
}