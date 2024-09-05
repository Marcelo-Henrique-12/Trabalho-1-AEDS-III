#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar informações de uma pessoa
typedef struct
{
  char nome_pessoa[200];
  int ano_titulacao;
  char grau_academico[50];
  long long codigo;
  int id_ies_titulacao;
  char nome_ies_titulacao[200];
  char pais_ies_titulacao[100];
  char nome_area_titulacao[200];
  char sigla_ies_titulacao[20];
} Pessoa;

// Estrutura para o nó da árvore
typedef struct
{
  Pessoa pessoa;
  int esq; // Indica a posição do nó esquerdo no arquivo
  int dir; // Indica a posição do nó direito no arquivo
} NoArvore;

typedef struct
{
  int raiz; // Raiz da árvore (índice no arquivo)
  int prox; // Próxima posição livre no arquivo
} CabecalhoArvore;

FILE *abre_arquivo(const char *nome)
{
  FILE *arq = fopen(nome, "rb"); // Abrir o arquivo em modo de leitura binária
  if (arq == NULL)
  {
    perror("Erro ao abrir o arquivo");
  }
  return arq;
}

// Função para abrir o arquivo da árvore
FILE *abreArquivoArvore(const char *nome)
{
  FILE *arq = fopen(nome, "r+b");
  if (!arq)
  {
    arq = fopen(nome, "w+b"); // Cria o arquivo se não existir
    if (!arq)
    {
      perror("Erro ao abrir/criar o arquivo");
      exit(1);
    }

    // Inicializa o cabeçalho
    CabecalhoArvore cab = {-1, 0};
    fwrite(&cab, sizeof(CabecalhoArvore), 1, arq);
  }
  return arq;
}

// Função para inserir um nó no arquivo
int insereNoArquivo(FILE *arq, Pessoa pessoa)
{
  fseek(arq, 0, SEEK_SET);

  // Carrega o cabeçalho
  CabecalhoArvore cab;
  fread(&cab, sizeof(CabecalhoArvore), 1, arq);

  NoArvore novoNo;
  novoNo.pessoa = pessoa;
  novoNo.esq = novoNo.dir = -1; // Inicialmente, sem filhos

  if (cab.raiz == -1)
  { // Se a árvore estiver vazia
    fseek(arq, sizeof(CabecalhoArvore), SEEK_SET);
    fwrite(&novoNo, sizeof(NoArvore), 1, arq);
    cab.raiz = cab.prox++;
  }
  else
  {
    // Percorre a árvore para inserir no local correto
    int atual = cab.raiz;
    int pai = -1;
    int direcao = 0;

    while (atual != -1)
    {
      NoArvore noAtual;
      fseek(arq, sizeof(CabecalhoArvore) + atual * sizeof(NoArvore), SEEK_SET);
      fread(&noAtual, sizeof(NoArvore), 1, arq);

      pai = atual;

      if (pessoa.codigo < noAtual.pessoa.codigo)
      {
        atual = noAtual.esq;
        direcao = 1;
      }
      else if (pessoa.codigo > noAtual.pessoa.codigo)
      {
        atual = noAtual.dir;
        direcao = 2;
      }
      else
      {
        return 0; // Código já existe na árvore
      }
    }

    // Insere o novo nó
    fseek(arq, sizeof(CabecalhoArvore) + cab.prox * sizeof(NoArvore), SEEK_SET);
    fwrite(&novoNo, sizeof(NoArvore), 1, arq);

    // Atualiza o nó pai
    fseek(arq, sizeof(CabecalhoArvore) + pai * sizeof(NoArvore), SEEK_SET);
    NoArvore noPai;
    fread(&noPai, sizeof(NoArvore), 1, arq);

    if (direcao == 1)
    {
      noPai.esq = cab.prox;
    }
    else if (direcao == 2)
    {
      noPai.dir = cab.prox;
    }

    fseek(arq, sizeof(CabecalhoArvore) + pai * sizeof(NoArvore), SEEK_SET);
    fwrite(&noPai, sizeof(NoArvore), 1, arq);

    cab.prox++;
  }

  // Atualiza o cabeçalho
  fseek(arq, 0, SEEK_SET);
  fwrite(&cab, sizeof(CabecalhoArvore), 1, arq);

  return 1;
}

// Função para buscar um nó no arquivo
int buscaNoArquivo(FILE *arq, long long codigo)
{
  fseek(arq, 0, SEEK_SET);

  // Carrega o cabeçalho
  CabecalhoArvore cab;
  fread(&cab, sizeof(CabecalhoArvore), 1, arq);

  int atual = cab.raiz;

  while (atual != -1)
  {
    NoArvore noAtual;
    fseek(arq, sizeof(CabecalhoArvore) + atual * sizeof(NoArvore), SEEK_SET);
    fread(&noAtual, sizeof(NoArvore), 1, arq);

    if (codigo == noAtual.pessoa.codigo)
    {
      printf("\nPessoa Encontrada: \n");
      printf("Nome: %s\n", noAtual.pessoa.nome_pessoa);
      printf("Ano de Titulação: %d\n", noAtual.pessoa.ano_titulacao);
      printf("Grau Acadêmico: %s\n", noAtual.pessoa.grau_academico);
      printf("Código: %lld\n", noAtual.pessoa.codigo);
      return 1;
    }
    else if (codigo < noAtual.pessoa.codigo)
    {
      atual = noAtual.esq;
    }
    else
    {
      atual = noAtual.dir;
    }
  }

  return 0; // Não encontrado
}

int main()
{

  FILE *pessoas = abre_arquivo("pessoas.bin");
  if (!pessoas)
  {
    return EXIT_FAILURE;
  }

  FILE *arvore = abreArquivoArvore("arvore_pessoas.bin");
  if (!arvore)
  {
    return EXIT_FAILURE;
  }


  Pessoa pessoa;
  while (fread(&pessoa, sizeof(Pessoa), 1, pessoas) == 1)
  {
    insereNoArquivo(arvore, pessoa);
  }

  // Busca na árvore
  long long codigo;
  printf("Digite o código para buscar: ");
  scanf("%lld", &codigo);

  if (!buscaNoArquivo(arvore, codigo))
  {
    printf("Código não encontrado.\n");
  }

  fclose(arvore);

  return 0;
}
