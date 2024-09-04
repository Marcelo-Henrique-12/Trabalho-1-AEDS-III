#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar informações de uma pessoa
typedef struct {
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

// Função para abrir o arquivo binário
FILE *abre_arquivo(char *nome) {
    FILE *arq = fopen(nome, "rb");  // Abrir o arquivo em modo de leitura binária
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
    }
    return arq;
}

int main() {
    FILE *file = abre_arquivo("pessoas.bin");
    if (!file) {
        return EXIT_FAILURE;
    }

    Pessoa pessoa;
    int count = 0;

    printf("Lendo os primeiros 5 registros do arquivo:\n");

    // Ler e imprimir os primeiros 5 registros

    while (fread(&pessoa, sizeof(Pessoa), 1, file) == 1 && count < 10) {
        printf("Nome: %s\n", pessoa.nome_pessoa);
        printf("Ano de Titulação: %d\n", pessoa.ano_titulacao);
        printf("Grau Acadêmico: %s\n", pessoa.grau_academico);
        printf("Código: %lld\n", pessoa.codigo);  // Usar %lld para long long
        printf("ID IES Titulação: %d\n", pessoa.id_ies_titulacao);
        printf("Nome IES Titulação: %s\n", pessoa.nome_ies_titulacao);
        printf("País IES Titulação: %s\n", pessoa.pais_ies_titulacao);
        printf("Nome Área Titulação: %s\n", pessoa.nome_area_titulacao);
        printf("Sigla IES Titulação: %s\n", pessoa.sigla_ies_titulacao);
        printf("----------------------\n");
        count++;
    }

    if (feof(file)) {
        printf("Fim do arquivo alcançado.\n");
    } else if (ferror(file)) {
        perror("Erro ao ler o arquivo");
    }

    printf("Total de registros lidos: %d\n", count);
    fclose(file);
    return 0;
}
