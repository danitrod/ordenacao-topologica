typedef struct
{
  int inicio;
  int fim;
  int quant_elementos;
  int capacidade;
  int *valores;
} Fila;

Fila *filaInit(int cap)
{
  Fila *fila = (Fila *)malloc(
      sizeof(Fila));
  fila->capacidade = cap;
  fila->inicio = fila->quant_elementos = 0;

  fila->fim = cap - 1;
  fila->valores = (int *)malloc(cap * sizeof(int));

  return fila;
}

void enfilera(Fila *fila, int v)
{
  fila->fim = (fila->fim + 1) % fila->capacidade;
  fila->valores[fila->fim] = v;
  fila->quant_elementos = fila->quant_elementos + 1;
}

int desenfilera(Fila *fila)
{
  int v = fila->valores[fila->inicio];
  fila->inicio = (fila->inicio + 1) % fila->capacidade;
  fila->quant_elementos = fila->quant_elementos - 1;
  return v;
}

void liberaFila(Fila *fila)
{
  free(fila->valores);
  free(fila);
}

