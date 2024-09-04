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
typedef struct TNo
{
  Pessoa pessoa; // Dados da pessoa
  struct TNo *esq, *dir;
} No;

// Função para abrir o arquivo binário
FILE *abre_arquivo(const char *nome)
{
  FILE *arq = fopen(nome, "rb"); // Abrir o arquivo em modo de leitura binária
  if (arq == NULL)
  {
    perror("Erro ao abrir o arquivo");
  }
  return arq;
}

// Função para inserir um nó na árvore
int insereNo(No **raiz, Pessoa pessoa)
{
  if (*raiz == NULL)
  {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL)
      return 0;
    novo->pessoa = pessoa;
    novo->esq = novo->dir = NULL;
    *raiz = novo;
  }
  else if (pessoa.codigo < (*raiz)->pessoa.codigo)
  {
    return insereNo(&(*raiz)->esq, pessoa);
  }
  else if (pessoa.codigo > (*raiz)->pessoa.codigo)
  {
    return insereNo(&(*raiz)->dir, pessoa);
  }
  return 1;
}

// Função para buscar um código na árvore e retornar a estrutura Pessoa
No *buscaNo(No *raiz, long long dado)
{
  if (raiz == NULL)
  {
    return NULL;
  }
  if (dado == raiz->pessoa.codigo)
  {
    return raiz;
  }
  else if (dado < raiz->pessoa.codigo)
  {
    return buscaNo(raiz->esq, dado);
  }
  else
  {
    return buscaNo(raiz->dir, dado);
  }
}

// Função para verificar as regras e inserir na árvore
int verificarEInserir(No **raiz, Pessoa pessoa)
{
  int ano_atual = 2024; // Supondo que o ano atual é 2024
  int idade_titulacao = ano_atual - pessoa.ano_titulacao;

  // Verificações
  if (pessoa.ano_titulacao <= 0 || pessoa.id_ies_titulacao <= 0 ||
      strcmp(pessoa.nome_ies_titulacao, "") == 0 ||
      strcmp(pessoa.pais_ies_titulacao, "") == 0)
  {
    return 0; // Dados faltando ou inválidos
  }

  if (strcmp(pessoa.pais_ies_titulacao, "Brasil") != 0 && strcmp(pessoa.nome_ies_titulacao, "Unimontes") != 0)
  {
    return 0; // Não é do Brasil e não é Unimontes
  }

  if (idade_titulacao < 2)
  {
    return 0; // Menos de 2 anos de titulação
  }

  // Determinando o destino
  int destino;
  if (strcmp(pessoa.grau_academico, "mestrado") == 0)
  {
    destino = 1;
  }
  else if (strcmp(pessoa.grau_academico, "doutorado") == 0)
  {
    destino = 2;
  }
  else if (strcmp(pessoa.grau_academico, "") != 0)
  {
    destino = 3;
  }
  else
  {
    destino = 0;
  }

  // Inserir o código na árvore se for válido
  if (destino > 0)
  {
    return insereNo(raiz, pessoa);
  }
  return 0;
}

// Função para liberar a memória da árvore
void liberaArvore(No *raiz)
{
  if (raiz != NULL)
  {
    liberaArvore(raiz->esq);
    liberaArvore(raiz->dir);
    free(raiz);
  }
}

int main()
{
  FILE *file = abre_arquivo("pessoas.bin");
  if (!file)
  {
    return EXIT_FAILURE;
  }

  Pessoa pessoa;
  No *raiz = NULL;

  // Ler e processar os registros
  while (fread(&pessoa, sizeof(Pessoa), 1, file) == 1)
  {
    verificarEInserir(&raiz, pessoa);
  }

  if (feof(file))
  {
    printf("Fim do arquivo alcançado.\n");
  }
  else if (ferror(file))
  {
    perror("Erro ao ler o arquivo");
  }

  fclose(file);

  long long codigo;
  printf("Digite o código a ser pesquisado: ");
  scanf("%lld", &codigo);

  // Verificar se o código está na árvore
  No *resultado = buscaNo(raiz, codigo);
  if (resultado)
  {
    // Encontrado, retornando destino

    printf("\nPessoa Encontrada: \n");
    printf("Nome: %s\n", resultado->pessoa.nome_pessoa);
    printf("Ano de Titulação: %d\n", resultado->pessoa.ano_titulacao);
    printf("Grau Acadêmico: %s\n", resultado->pessoa.grau_academico);
    printf("Código: %lld\n", resultado->pessoa.codigo); // Usar %lld para long long
    printf("ID IES Titulação: %d\n", resultado->pessoa.id_ies_titulacao);
    printf("Nome IES Titulação: %s\n", resultado->pessoa.nome_ies_titulacao);
    printf("País IES Titulação: %s\n", resultado->pessoa.pais_ies_titulacao);
    printf("Nome Área Titulação: %s\n", resultado->pessoa.nome_area_titulacao);
    printf("Sigla IES Titulação: %s\n", resultado->pessoa.sigla_ies_titulacao);
    printf("----------------------\n");
    int destino;
    if (strcmp(resultado->pessoa.grau_academico, "mestrado") == 0)
    {
      destino = 1;
    }
    else if (strcmp(resultado->pessoa.grau_academico, "doutorado") == 0)
    {
      destino = 2;
    }
    else
    {
      destino = 3;
    }
    printf("Destino: %d\n", destino);
  }
  else
  {
    // Não encontrado
    printf("Código não encontrado.\n");
  }

  // Liberar a memória da árvore
  liberaArvore(raiz);

  return 0;
}
