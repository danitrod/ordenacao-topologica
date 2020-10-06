#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grafoLista.c"
#include "fila.c"

#define MAX_CHARS_INPUT 50

/*
  por Daniel Rodrigues, 31839010
  e   Matheus Masumoto de Carvalho, 31850741
  
  OBS: Feito usando formatador de código em C padrão do VS Code
*/

void topologicalSort(TGrafo *G, char **nomes);

int main(int argc, char const *argv[])
{
  FILE *f;
  char buff[MAX_CHARS_INPUT];

  f = fopen("./input.txt", "r");

  fgets(buff, MAX_CHARS_INPUT, f);
  int n = atoi(buff);

  TGrafo *grafo = Init(n);

  char *nomes[n];
  int i;
  for (i = 0; i < n; i++)
  {
    nomes[i] = malloc(MAX_CHARS_INPUT * sizeof(char));
    fgets(buff, MAX_CHARS_INPUT, f);
    strcpy(nomes[i], buff);
  }

  fgets(buff, MAX_CHARS_INPUT, f);
  int m = atoi(buff);

  int t1, t2;
  for (i = 0; i < m; i++)
  {
    fscanf(f, "%s", buff);
    t1 = atoi(buff) - 1;

    fscanf(f, "%s", buff);
    t2 = atoi(buff) - 1;

    insereA(grafo, t1, t2);
    if (detectaCiclo(grafo))
    {
      printf("Alerta: As tarefas (%d, %d) formam ciclo!\n", t1 + 1, t2 + 1);
      removeA(grafo, t1, t2);
    }
  }

  printf("\n");

  topologicalSort(grafo, nomes);

  // Finalizar
  fclose(f);
  libera(grafo);

  return 0;
}

void topologicalSort(TGrafo *G, char **nomes)
{
  // Ordenação topológica com Indegree
  int *indegree = calloc(G->V, sizeof(int));

  // Preencher indegree e armazenar nós com indegree 0 em uma fila
  int i = 0;
  Fila *fila = filaInit(G->V);
  for (; i < G->V; i++)
  {
    TNo *aux = G->adj[i];
    while (aux != NULL)
    {
      indegree[aux->w]++;
      aux = aux->prox;
    }
    if (!indegree[i])
    {
      enfilera(fila, i);
    }
  }

  // Printar a ordem topológica até a fila de indegree 0 terminar.
  while (fila->quant_elementos > 0)
  {
    int elem = desenfilera(fila);
    printf("Tarefa %d: %s", elem + 1, nomes[elem]);

    // Diminuir indegree dos que dependem da tarefa
    TNo *aux = G->adj[elem];
    while (aux != NULL)
    {
      indegree[aux->w]--;
      if (indegree[aux->w] == 0)
      {
        // Enfilerar nós com indegrees zerados
        enfilera(fila, aux->w);
      }
      aux = aux->prox;
    }
  }

  liberaFila(fila);
}