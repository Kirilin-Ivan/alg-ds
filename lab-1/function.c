#include "Function.h"

void MemoryFree(word_t* first);
///////////////////������ ��������� ����, �� ���� �������� ������ � ���������� ��������� �� ������ ��� �������///////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
word_t* Initialize(FILE* fp) {
  word_t* first = NULL, * current = NULL, * next = NULL;
  char* safe = NULL;
  char sym = 0;
  int symCounter = 0;

  first = (word_t*)malloc(sizeof(word_t));   //�������� ������ ��� ������ ������� ������ � ���������� ��������� �� ������ ��-�
  if (first == NULL)
    return NOT_ENOUGHT_MEMORY;
  current = first;                           //���������� ���� ������� �������
  current->next = NULL;
  sym = (char)getc(fp);                      //������ ������ ������ � �����

  while (sym != EOF) {

    current->word = (char*)malloc(sizeof(char)); //�������� ������ ��� ������� ������� ������
    if (current->word == NULL) {
      MemoryFree(first);
      return NOT_ENOUGHT_MEMORY;
    }

    while (sym != '\n' && sym != EOF) {
      safe = (char*)realloc(current->word, (symCounter + 2) * sizeof(char));
      if (safe == NULL) {
        current->size = symCounter;
        MemoryFree(first);
        return NOT_ENOUGHT_MEMORY;
      }
      current->word = safe;               //���������� ��� ����������� ������������� realloc
      current->word[symCounter++] = sym;
      sym = (char)getc(fp);               // ������ ��������� ������ ����� �����������
    }
    current->word[symCounter] = '\0';
    current->size = symCounter;
    if (sym == EOF) {
      current->next = NULL;
      break;
    }
    sym = (char)getc(fp); 
    if (sym == EOF) {
      current->next = NULL;
      break;
    }
    current->next = (struct word_t*)malloc(sizeof(word_t));
    current = (word_t*)current->next;
    current->next = NULL;
    symCounter = 0;
  }


  return first;
}

///////////////////��������� ������ �� �������� ASCII-Z/////////////
///////////////////////////////////////////////////////////////////
word_t* SortWords(word_t* first) {
  word_t* current = first, * next = NULL, * swap = NULL, * prev = NULL;
  check_t check = FACED;
  int i = 0;

  while (check == FACED) {
    check = NOTFACED;
    current = first;

    while (current->next != NULL) {
      next = (word_t*)current->next;

      for (i = 0; (i < current->size) && (i < next->size) && (current->word[i] == next->word[i]); i++);

      if (i == (current->size) || (next->word[i] > current->word[i])) { //���� ������� �� �� n �������� ��������� �� n �������� � �����, �� ������� ����� ������� || �� ��������� �������� ������� �� ���� ����� �� �������� ������
        prev = current;
        current = next;
        next = (word_t*)next->next;
        continue;
      }
      if ((i == (next->size)) || (next->word[i] < current->word[i])) {    //���� ������� ����� �� n �������� ��������� �� n �������� � ��, �� ������� �� �������, �� ������ �������
        if (current == first) {
          swap = first;
          first = (word_t*)current->next;
          current->next = next->next;
          next->next = (struct word_t*)swap;
          check = FACED;

          prev = first;
          current = next;
          next = (word_t*)next->next;
          continue;
        }
        swap = (word_t*)prev->next;         // ��������� ��
        prev->next = current->next;         // ������ �� �� �����
        current->next = next->next;         // ������ ��������� � �� �� ��������� � �����
        next->next = (struct word_t*)swap;  // ������ ����� ����� �� ��
        check = FACED;                      //��������, ��� ���� ������

        prev = current;
        current = next;
        next = (word_t*)next->next;
        continue;
      }
    }
  }
  return first;
}

///////////////////������� ���������� ������ � �������///////////////////
////////////////////////////////////////////////////////////////////////
void PrintWords(word_t* first) {
  word_t* current = first;
  int i = 0;

  while (current != NULL) {
    for (i = 0; i < current->size; i++) {
      printf("%c", current->word[i]);
    }
    printf("\n");
    current = (word_t*)current->next;
  }
}
///////////////////����������� ��� ������, ���������� �� ������///////////////////
/////////////////////////////////////////////////////////////////////////////////
void MemoryFree(word_t* first) {
  word_t* current = first, * next = NULL;

  while (current != NULL) {
    free(current->word);
    next = (word_t*)current->next;
    free(current);
    current = next;
  }
}