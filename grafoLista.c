#include <stdio.h>
#include <stdlib.h>

typedef struct _TNo
{
  int w;
  struct _TNo *prox;
} TNo;

typedef struct
{
  int V;
  int A;
  TNo **adj;
} TGrafo;

TGrafo *Init(int V);
void insereA(TGrafo *G, int v, int w);
void libera(TGrafo *G);
int temCiclo(TGrafo *G, int v, int *visitados, int *pilha);

// Inicializa o grafo
TGrafo *Init(int V)
{
  int i;
  TGrafo *g;
  g = (TGrafo *)calloc(1, sizeof(TGrafo));
  g->V = V;
  g->A = 0;
  g->adj = (TNo **)calloc(V, sizeof(TNo *));
  for (i = 0; i < g->V; i++)
    g->adj[i] = NULL;

  return g;
}

// Insere uma aresta de v para w
void insereA(TGrafo *G, int v, int w)
{
  if (v == w)
  {
    printf("Alerta: Tarefas dependentes devem ser diferentes! (%d, %d)\n ", v + 1, w + 1);
    return;
  }
  if (v >= G->V || w >= G->V || v < 0 || w < 0)
  {
    printf("Detectada inserção de aresta em vértice inexistente.\n");
    exit(1);
  }

  TNo *aux = G->adj[v];
  if (aux == NULL)
  {
    // Se o nó ainda não houver arestas, fazer a nova aresta ser a cabeça
    aux = (TNo *)calloc(1, sizeof(TNo));
    aux->w = w;
    aux->prox = NULL;
    G->A++;
    G->adj[v] = aux;
    return;
  }

  if (w < aux->w)
  {
    // Inserir aresta na cabeça
    TNo *novo = (TNo *)calloc(1, sizeof(TNo));
    novo->w = w;
    novo->prox = aux;
    G->adj[v] = novo;
    G->A++;
    return;
  }

  while (aux->prox != NULL && aux->prox->w <= w)
  {
    // Percorrer lista de arestas até a posição correta
    if (aux->w == w)
    {
      printf("Alerta: Tarefa (%d, %d) já inserida!\n", v + 1, w + 1);
      return;
    }
    aux = aux->prox;
  }
  if (aux->w == w)
  {
    printf("Alerta: Tarefa (%d, %d) já inserida!\n", v + 1, w + 1);
    return;
  }

  // Inserir a nova aresta na posição correta
  TNo *novo = (TNo *)calloc(1, sizeof(TNo));
  novo->w = w;
  novo->prox = aux->prox;
  aux->prox = novo;
  G->A++;
}

// Remove aresta de v para w
void removeA(TGrafo *G, int v, int w)
{
  TNo *aux = G->adj[v];
  if (aux == NULL)
    return;
  if (aux->w == w)
  {
    // Se a aresta for a cabeça, fazer a lista apontar para a próxima
    G->adj[v] = aux->prox;
    free(aux);
    G->A--;
    return;
  }
  while (aux->prox != NULL)
  {
    // Percorrer a lista até encontrar a aresta procurada
    if (aux->prox->w == w)
    {
      // Remover a aresta procurada
      TNo *prox = aux->prox->prox;
      free(aux->prox);
      aux->prox = prox;
      G->A--;
      return;
    }
    aux = aux->prox;
  }
}

int detectaCiclo(TGrafo *G)
{
  // Retorna 1 se houver ciclo, 0 se não.
  int *visitados = calloc(G->V, sizeof(int));
  int *pilha = calloc(G->V, sizeof(int));

  int i = 0;
  for (; i < G->V; i++)
  {
    if (temCiclo(G, i, visitados, pilha))
    {
      free(visitados);
      free(pilha);
      return 1;
    }
  }
  free(visitados);
  free(pilha);
  return 0;
}

int temCiclo(TGrafo *G, int v, int *visitados, int *pilha)
{
  if (!visitados[v])
  {
    visitados[v] = 1;
    pilha[v] = 1;

    TNo *aux = G->adj[v];
    while (aux != NULL)
    {
      if (!visitados[aux->w])
      {
        if (temCiclo(G, aux->w, visitados, pilha))
          return 1;
      }
      else if (pilha[aux->w])
      {
        return 1;
      }
      aux = aux->prox;
    }
  }

  pilha[v] = 0;
  return 0;
}

// Libera a memória utilizada pelo grafo
void libera(TGrafo *G)
{
  int i;

  for (i = 0; i < G->V; i++)
    free(G->adj[i]);

  free(G->adj);
  free(G);
}

