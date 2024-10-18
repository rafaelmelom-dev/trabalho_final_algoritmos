#include <stdbool.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define INTERVALO 5
#define LIVRO_ARQ "livros.bin"
#define CLIENTE_ARQ "clientes.bin"
#define ALUGUEL_ARQ "alugueis.bin"
#define QUANTIDADE_ARQ "quantidades.bin"
#define RELATORIO_ARQ "relatorio.txt"

// backup macros
#define LIVRO_ARQ_BACKUP "backup/livros.bin"
#define CLIENTE_ARQ_BACKUP "backup/clientes.bin"
#define ALUGUEL_ARQ_BACKUP "backup/alugueis.bin"
#define QUANTIDADE_ARQ_BACKUP "backup/quantidades.bin"

// definindo macros para limpar a tela e a entrada a depender do sistema
#ifdef _WIN32 // caso seja windows 32 bits
char command[] = "cls";
#define clean fflush // caso seja windows 64 bits
#elif _WIN64
char command[] = "cls";
#define clean fflush
#else
char command[] = "clear";
#define clean __fpurge
#endif

///////////////////////////
//                       //
//   ESTRUTURA AUXILIAR  //
//                       //
///////////////////////////

typedef struct {
  int dia;
  int mes;
  int ano;
} Data;

////////////////////
//                //
//   ESTRUTURA I  //
//                //
////////////////////

typedef struct {
  int id;
  char nome[64];
  int estoque;
  float preco;
  char genero;
} Livro;

////////////////////
//                //
//  ESTRUTURA II  //
//                //
////////////////////

typedef struct {
  char cpf[12];
  char nome[64];
  int idade;
  char email[32];
  bool estudante;
} Cliente;

////////////////////
//                //
//  ESTRUTURA III //
//                //
////////////////////

typedef struct {
  int id_aluguel;
  char id_cliente[12];
  int id_livro;
  Data data_aluguel;
  Data data_devolucao;
} Aluguel;

//////////////////////
//                  //
//  FUNCOES GERAIS  //
//                  //
//////////////////////
int menu_principal();
int menu_secundario();

void ler_quantidades(int *qtd_livro, int *qtd_cliente, int *qtd_aluguel);
void registrar_quantidades(int *qtd_livro, int *qtd_cliente, int *qtd_aluguel);

int validar_data(Data *data);
int comparar_data(Data *primeira_data, Data *segunda_data);
// COMPARANDO DATAS: retorna -1 se a primeira for menor, 0 se for igual e 1 se a segunda for maior
void ordenar_data(Aluguel *alugueis, int qtd_aluguel, int tipo);
// ORDENANDO DATAS: ordena datas a partir de um tipo:
// 1. ordena as datas de aluguel
// 2. ordena as datas de devolucao

void mostrar_todos(Livro *livros, int qtd_livro, Cliente *clientes, int qtd_cliente, Aluguel *alugueis, int qtd_aluguel);

void gerar_backup(Livro *livros, int *qtd_livro,Cliente *clientes, int *qtd_cliente, Aluguel *alugueis, int *qtd_aluguel);

//////////////////////
//                  //
//     LEITURA      //
//                  //
//////////////////////
Livro *ler_livro(int qtd_livros);
Cliente *ler_cliente(int qtd_clientes);
Aluguel *ler_aluguel(int qtd_aluguel);

//////////////////////
//                  //
//     ESCRITA      //
//                  //
//////////////////////
void escrever_livro(Livro *livros, int qtd_livro, char *modo);
void escrever_cliente(Cliente *clientes, int qtd_clientes, char *modo);
void escrever_aluguel(Aluguel *alugueis, int qtd_aluguel, char *modo);

//////////////////////
//                  //
//      BUSCA       //
//                  //
//////////////////////
int busca_livro(Livro *livros, int qtd_livro, int id);
int busca_cliente(Cliente *clientes, int qtd_cliente, char *id);
int busca_aluguel(Aluguel *alugueis, int qtd_aluguel, int id);
// retorna o indice da estrutura, procurando pelo id

//////////////////////
//                  //
//     REGISTRO     //
//                  //
//////////////////////
void registrar_livro(Livro **livros, int *qtd_livro); // duplo ponteiro para as estruturas que serao ralocadas
void registrar_cliente(Cliente **clientes, int *qtd_cliente);
void registrar_aluguel(Aluguel **alugueis, int *qtd_aluguel,Cliente *clientes, int *qtd_cliente, Livro *livros, int *qtd_livro);
// ponteiro para as quantidades para altera-las na funcao assim que necessario

//////////////////////
//                  //
//      EDICAO      //
//                  //
//////////////////////
void editar_livro(Livro *livros, int *qtd_livro);
void editar_cliente(Cliente *clientes, int *qtd_cliente);
void editar_aluguel(Aluguel *alugueis, int *qtd_aluguel,Cliente *clientes, int *qtd_cliente, Livro *livros, int *qtd_livro);

//////////////////////
//                  //
//     CONSULTA     //
//                  //
//////////////////////
void consultar_livro(Livro *livros, int qtd_livro);
void consultar_cliente(Cliente *clientes, int qtd_cliente);
void consultar_aluguel(Aluguel *alugueis, int qtd_aluguel, Cliente *clientes, int qtd_cliente, Livro *livros, int qtd_livro);

//////////////////////
//                  //
//     REMOVER      //
//                  //
//////////////////////
void remover_livro(Livro *livros, int *qtd_livro, Aluguel *alugueis, int *qtd_aluguel);
void remover_cliente(Cliente *clientes, int *qtd_cliente, Aluguel *alugueis, int *qtd_aluguel);
void remover_aluguel(Aluguel *alugueis, int *qtd_aluguel);

//////////////////////
//                  //
//     RELATORIO    //
//                  //
//////////////////////
void gerar_relatorio(Cliente *clientes, Livro *livros, Aluguel *alugueis, int qtd_livro, int qtd_cliente, int qtd_aluguel);

//////////////////
//              //
//     MAIN     //
//              //
//////////////////
int main() {
  setlocale(LC_ALL,"portuguese"); // permitindo os acentos

  // vetores para as estruturas
  Livro *livros;
  int qtd_livro;

  Cliente *clientes;
  int qtd_cliente;

  Aluguel *alugueis;
  int qtd_aluguel;

  int opt, tip; // opt = acao selecionada, tip = tipo dessa acao

  // lendo as quantidades de estruturas registradas
  ler_quantidades(&qtd_livro, &qtd_cliente, &qtd_aluguel);

  // carregando os dados que estavam salvos
  livros = ler_livro(qtd_livro);
  clientes = ler_cliente(qtd_cliente);
  alugueis = ler_aluguel(qtd_aluguel);

  while (1) {
    opt = menu_principal();

    switch (opt) {
    case 7: // antes de sair, liberar os ponteiros para as estruturas e gerar o backup
      if (qtd_livro % 5 != 0 && qtd_cliente % 5 != 0 && qtd_aluguel % 5 != 0) {
        gerar_backup(livros, &qtd_livro, clientes, &qtd_cliente, alugueis, &qtd_aluguel);
      }
      free(livros);
      free(clientes);
      free(alugueis);
      exit(0);
    case 6: 
        mostrar_todos(livros, qtd_livro, clientes, qtd_cliente, alugueis, qtd_aluguel); break;
    case 5:
      gerar_relatorio(clientes, livros, alugueis, qtd_livro, qtd_cliente, qtd_aluguel); break;
    default:
      tip = menu_secundario();
    }

    switch (tip) {
    case 1:
      // livros
      switch (opt) {
         case 1:
           registrar_livro(&livros, &qtd_livro);
           registrar_quantidades(&qtd_livro, &qtd_cliente, &qtd_aluguel);
           break;
        case 2:
          editar_livro(livros, &qtd_livro);
          break;
        case 3:
          consultar_livro(livros, qtd_livro);
          break;
        case 4:
          remover_livro(livros, &qtd_livro, alugueis, &qtd_aluguel);
          registrar_quantidades(&qtd_livro, &qtd_cliente, &qtd_aluguel);
          break;
      } break;
    case 2:
      // clientes
      switch (opt) {
        case 1:
          registrar_cliente(&clientes, &qtd_cliente);
          registrar_quantidades(&qtd_livro, &qtd_cliente, &qtd_aluguel);
          break;
        case 2:
           editar_cliente(clientes, &qtd_cliente);
           break;
        case 3:
          consultar_cliente(clientes, qtd_cliente);
          break;
        case 4:
          remover_cliente(clientes, &qtd_cliente, alugueis, &qtd_aluguel);
          registrar_quantidades(&qtd_livro, &qtd_cliente, &qtd_aluguel);
          break;
      } break;
    case 3:
      // alugueis
      switch (opt) {
        case 1:
          registrar_aluguel(&alugueis, &qtd_aluguel, clientes, &qtd_cliente, livros, &qtd_livro);
          registrar_quantidades(&qtd_livro, &qtd_cliente, &qtd_aluguel);
          break;
         case 2:
           editar_aluguel(alugueis, &qtd_aluguel, clientes, &qtd_cliente, livros, &qtd_livro);
           break;
        case 3:
          consultar_aluguel(alugueis, qtd_aluguel, clientes, qtd_cliente, livros, qtd_livro);
          break;
        case 4:
          remover_aluguel(alugueis, &qtd_aluguel);
          registrar_quantidades(&qtd_livro, &qtd_cliente, &qtd_aluguel);
          break;
      } break;
    default: break;
    }

    if (qtd_livro % 5 == 0 || qtd_cliente % 5 == 0 || qtd_aluguel % 5 == 0) { // verificando se ja chegou ao limite de algum, para nao escrever muitas vezes o backup
      gerar_backup(livros, &qtd_livro, clientes, &qtd_cliente, alugueis, &qtd_aluguel);
    }
  }

  return 0;
}

//////////////////
//              //
//     MENUS    //
//              //
//////////////////

int menu_principal() {
  int opt;
  int flag;

  do {
    system(command);
    flag = 0;
    printf(" ============ MENU ========== \n");
    printf("1. Cadastrar\n");
    printf("2. Editar\n");
    printf("3. Consultar\n");
    printf("4. Remover\n");
    printf("5. Relatorio\n");
    printf("6. Mostrar todos os dados\n");
    printf("7. Sair\n");
    printf("Digite a opcao desejada: ");
    scanf("%d", &opt);
    clean(stdin);

    if (opt < 1 || opt > 7) {
      flag = 1;
      printf("\nOpcao invalida, pressione enter para continuar...");
      getchar();
    }
  } while (flag != 0);

  return opt;
}

int menu_secundario() {
  int tip;
  int flag;

  do {
    system(command);
    flag = 0;
    printf(" ============ MENU ========== \n");
    printf("1. Livros\n");
    printf("2. Clientes\n");
    printf("3. Alugueis\n");
    printf("4. Voltar\n");
    printf("Digite  a opcao desejada: ");
    scanf("%d", &tip);
    clean(stdin);

    if (tip < 1 || tip > 4) {
      flag = 1;
      printf("\nOpcao invalida, pressione enter para continuar...");
      getchar();
    }

  } while (flag != 0);

  return tip;
}

//////////////////
//              //
//     DATA     //
//              //
//////////////////
int validar_data (Data *data) {
  if (data->ano > 1900 && data->ano < 2200 && data->mes > 0 && data->mes < 13) {
    if (data->mes == 4 || data->mes == 6 || data->mes == 9 || data->mes == 11) { // meses de 30 dias
      if (data->dia > 0 && data->dia < 31) {
        return 1;
      }
    } else if (data->mes != 2) { // nao for fevereiro = meses de 31 dias
      if (data->dia > 0 && data->dia < 32) {
        return 1;
      }
    } else {  // fevereiro
      if ((data->dia > 0 && data->dia < 29) || (data->dia == 29 && data->ano % 4 == 0)) {
        return 1;
      }
    }
  }

  return 0; // retorna 0 se data nao for valida
}

int comparar_data(Data *primeira_data, Data *segunda_data) {
  if (primeira_data->ano > segunda_data->ano) return -1;
  else if (primeira_data->ano < segunda_data->ano) return 1;
  else  // ano igual
    if (primeira_data->mes > segunda_data->mes) return -1;
    else if (primeira_data->mes < segunda_data->mes) return 1;
    else // mes igual
      if (primeira_data->dia > segunda_data->dia) return -1;
      else if (primeira_data->dia < segunda_data->dia) return 1;
      else return 0; // data toda igual
}

void ordenar_data(Aluguel *alugueis, int qtd_aluguel, int tipo) {
    // tipo 1: data de aluguel
    // tipo 2: data de devolucao
    Aluguel aux;

    switch (tipo) {
        case 1: {
            for (int i = 0; i < qtd_aluguel - 1; i ++) {
                for (int j = i + 1; j < qtd_aluguel; j ++) {
                    if (comparar_data(&alugueis[i].data_aluguel, &alugueis[j].data_aluguel) == -1) {
                        aux = alugueis[j];
                        alugueis[j] = alugueis[i];
                        alugueis[i] = aux;
                    }
                }
            }
        } break;
        case 2: {
            for (int i = 0; i < qtd_aluguel - 1; i ++) {
                for (int j = i + 1; j < qtd_aluguel; j ++) {
                    if (comparar_data(&alugueis[i].data_devolucao, &alugueis[j].data_devolucao) == -1) {
                        aux = alugueis[j];
                        alugueis[j] = alugueis[i];
                        alugueis[i] = aux;
                    }
                }
            }
        } break;
        default: break;
    }
}

/////////////////////////
//                     //
//   TAMANHO DO VETOR  //
//                     //
/////////////////////////

void ler_quantidades (int *qtd_livro, int *qtd_cliente, int *qtd_aluguel) {
  FILE *file;

  file = fopen(QUANTIDADE_ARQ, "rb");

  // valor padrao
  *qtd_livro = 0;
  *qtd_cliente = 0;
  *qtd_aluguel = 0;

  if (file) {
    fread(qtd_livro, sizeof(int), 1, file);
    fread(qtd_cliente, sizeof(int), 1, file);
    fread(qtd_aluguel, sizeof(int), 1, file);
    fclose(file);
  } else {
    FILE *backup_file = fopen(QUANTIDADE_ARQ_BACKUP, "rb");

    if (backup_file) {
      fread(qtd_livro, sizeof(int), 1, backup_file);
      fread(qtd_cliente, sizeof(int), 1, backup_file);
      fread(qtd_aluguel, sizeof(int), 1, backup_file);

      FILE *file = fopen(QUANTIDADE_ARQ, "wb");

      if (file) {
          fwrite(qtd_livro, sizeof(int), 1, file);
          fwrite(qtd_cliente, sizeof(int), 1, file);
          fwrite(qtd_aluguel, sizeof(int), 1, file);
          fclose(file);
      }

      fclose(backup_file);
    } else {
      // caso nao exista um arquivo e nem um backup, entao criaremos ele com os valores em padrao, ou, inicial, que seria 0
      FILE *file_ = fopen(QUANTIDADE_ARQ, "wb");
      if (file_) {
        fwrite(qtd_livro, sizeof(int), 1, file_);
        fwrite(qtd_cliente, sizeof(int), 1, file_);
        fwrite(qtd_aluguel, sizeof(int), 1, file_);
        fclose(file_);
      }
    }
  }

}

void registrar_quantidades (int *qtd_livro, int *qtd_cliente, int *qtd_aluguel) {
  FILE *file;

  file = fopen(QUANTIDADE_ARQ, "wb");

  if (file) {
    fwrite(qtd_livro, sizeof(int), 1, file);
    fwrite(qtd_cliente, sizeof(int), 1, file);
    fwrite(qtd_aluguel, sizeof(int), 1, file);
    fclose(file);
  } else {
    printf("Erro ao escrever a quantidade de structs no arquivo.\n");
  }
}

/////////////////////////
//                     //
//        BACKUP       //
//                     //
/////////////////////////


void gerar_backup (Livro *livros, int *qtd_livro,Cliente *clientes, int *qtd_cliente, Aluguel *alugueis, int *qtd_aluguel) {
  FILE *livro_file;
  FILE *cliente_file;
  FILE *aluguel_file;
  FILE *quantidades_file;

  livro_file = fopen(LIVRO_ARQ_BACKUP, "wb");
  cliente_file = fopen(CLIENTE_ARQ_BACKUP, "wb");
  aluguel_file = fopen(ALUGUEL_ARQ_BACKUP, "wb");
  quantidades_file = fopen(QUANTIDADE_ARQ_BACKUP, "wb");

  if (livro_file) {
    fwrite(livros, sizeof(Livro), *qtd_livro, livro_file);
    fclose(livro_file);
  }

  if (cliente_file) {
    fwrite(clientes, sizeof(Cliente), *qtd_cliente, cliente_file);
    fclose(cliente_file);
  }

  if (aluguel_file) {
    fwrite(alugueis, sizeof(Aluguel), *qtd_aluguel, aluguel_file);
    fclose(aluguel_file);
  }

  if (quantidades_file) {
    fwrite(qtd_livro, sizeof(int), 1, quantidades_file);
    fwrite(qtd_cliente, sizeof(int), 1, quantidades_file);
    fwrite(qtd_aluguel, sizeof(int), 1, quantidades_file);
    fclose(quantidades_file);
  }
}

/////////////////////////
//                     //
//   MOSTRANDO TUDO    //
//                     //
/////////////////////////
void mostrar_todos(Livro *livros, int qtd_livro, Cliente *clientes, int qtd_cliente, Aluguel *alugueis, int qtd_aluguel) {
  int opt;
  int flag;
  char estudante[6];
  
  system(command);

  do {
    flag = 0;
    system(command);
    printf("Mostrando todos os elementos\n");
    printf("Opcoes: \n");
    printf("1. Livros\n");
    printf("2. Clientes\n");
    printf("3. Alugueis\n");
    printf("4. Voltar\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opt);
    clean(stdin);

    if (opt < 1 || opt > 4) {
      printf("Opcao invalida, pressione enter para continuar.\n");
      getchar();
      flag = 1;
    }
  } while (flag != 0);

  system(command);
  switch (opt) {
    case 1: {
      printf("Dados dos livros: \n\n");

      for (int i = 0; i < qtd_livro; i++) {
        flag = 1;
        printf("Nome: %s - Genero: %c\n", livros[i].nome, livros[i].genero);
        printf("ID: %d - Preco: %.2f - Estoque: %d\n\n", livros[i].id, livros[i].preco, livros[i].estoque);
        printf("++++++++++++++++++++\n\n");
      }
    } break;
    case 2: {
      printf("Dados dos clientes: \n\n");

      for (int i = 0; i < qtd_cliente; i++) {
        flag = 1;
        if (clientes[i].estudante) {
          strcpy(estudante, "Sim");
        } else {
          strcpy(estudante, "Nao");
        }

        printf("Nome: %s - Idade: %d\n", clientes[i].nome, clientes[i].idade);
        printf("CPF: %s\n", clientes[i].cpf);
        printf("Estudante: %s\n\n", estudante);
        printf("++++++++++++++++++++\n\n");
      } 
    } break;
    case 3: {
      printf("Dados dos alugueis: \n\n");

      for (int i = 0; i < qtd_aluguel; i ++) {
        flag = 1;
        printf("ID Aluguel: %d\n", alugueis[i].id_aluguel);
        printf("ID Livro: %d - CPF: %s\n", alugueis[i].id_livro, alugueis[i].id_cliente);
        printf("Data de Aluguel: %d/%d/%d\n", 
               alugueis[i].data_aluguel.dia,
               alugueis[i].data_aluguel.mes,
               alugueis[i].data_aluguel.ano);
        printf("Data de Devolucao: %d/%d/%d\n",
               alugueis[i].data_devolucao.dia,
               alugueis[i].data_devolucao.mes,
               alugueis[i].data_devolucao.ano);
        printf("++++++++++++++++++++\n\n");
      }
    } break;
    default: break;
  }

  if (flag == 1) { // nao ha nenhum dado no tipo escolhido
    printf("Nao foi encontrado nenhum dado no vetor escolhido!!!\n"); 
  }

  clean(stdin);
  printf("Pressione enter para continuar...");
  getchar();
}


//////////////////////
//                  //
//     LEITURA      //
//                  //
//////////////////////
Livro *ler_livro (int qtd_livro) {
  FILE *file = fopen(LIVRO_ARQ, "rb");
  Livro *livros = (Livro *)malloc(sizeof(Livro) * (INTERVALO + qtd_livro));

  if (file) {
    fread(livros, sizeof(Livro), qtd_livro, file);
    fclose(file);
  } else {
    FILE *backup_file = fopen(LIVRO_ARQ_BACKUP, "rb");

    if (backup_file) {
      fread(livros, sizeof(Livro), qtd_livro, backup_file);

      // escrever o backup no arquivo original para ficar tudo sincronizado
      FILE *file = fopen(LIVRO_ARQ, "wb");

      if (file) {
          fwrite(livros, sizeof(Livro), qtd_livro, file);
          fclose(file);
      }

      fclose(backup_file);
    }
  }

  return livros;
}

Cliente *ler_cliente (int qtd_cliente) {
  FILE *file = fopen(CLIENTE_ARQ, "rb");
  Cliente *clientes = (Cliente *)malloc(sizeof(Cliente) * (INTERVALO + qtd_cliente));

  if (file) {
    fread(clientes, sizeof(Cliente), qtd_cliente, file);
    fclose(file);
  } else {
    FILE *backup_file = fopen(CLIENTE_ARQ_BACKUP, "rb");

    if (backup_file) {
      fread(clientes, sizeof(Cliente), qtd_cliente, backup_file);

      // escrever no arquivo original para ficar tudo sincronizado
      FILE *file = fopen(CLIENTE_ARQ, "wb");

      if (file) {
          fwrite(clientes, sizeof(Cliente), qtd_cliente, file);
          fclose(file);
      }

      fclose(backup_file);
    }
  }

  return clientes;
}

Aluguel *ler_aluguel (int qtd_aluguel) {
  FILE *file = fopen(ALUGUEL_ARQ, "rb");
  Aluguel *alugueis = (Aluguel *)malloc(sizeof(Aluguel) * (INTERVALO + qtd_aluguel));

  if (file) {
    fread(alugueis, sizeof(Aluguel), qtd_aluguel, file);
    fclose(file);
  } else {
    FILE *backup_file = fopen(ALUGUEL_ARQ_BACKUP, "rb");

    if (backup_file) {
      fread(alugueis, sizeof(Aluguel), qtd_aluguel, backup_file);

      // escrever no arquivo original para ficar tudo sincronizado
      FILE *file = fopen(ALUGUEL_ARQ, "wb");

      if (file) {
          fwrite(alugueis, sizeof(Aluguel), qtd_aluguel, file);
          fclose(file);
      }

      fclose(backup_file);
    }
  }

  return alugueis;
}

//////////////////////
//                  //
//     ESCRITA      //
//                  //
//////////////////////
void escrever_livro (Livro *livros, int qtd_livro, char *modo) {
  FILE *file = fopen(LIVRO_ARQ, modo);

  if (file) {
    fwrite(livros, sizeof(Livro), qtd_livro, file);
    fclose(file);
  }
}

void escrever_cliente (Cliente *clientes, int qtd_cliente, char *modo) {
  FILE *file = fopen(CLIENTE_ARQ, modo);

  if (file) {
    fwrite(clientes, sizeof(Cliente), qtd_cliente, file);
    fclose(file);
  }
}

void escrever_aluguel (Aluguel *alugueis, int qtd_aluguel, char *modo) {
  FILE *file = fopen(ALUGUEL_ARQ, modo);

  if (file) {
    fwrite(alugueis, sizeof(Aluguel), qtd_aluguel, file);
    fclose(file);
  }
}

//////////////////////
//                  //
//      BUSCA       //
//                  //
//////////////////////
int busca_livro(Livro *livros, int qtd_livro, int id) {
  for (int i = 0; i < qtd_livro; i ++) {
    if (livros[i].id == id){
        return i;
    }
  }
  return -1;
}

int busca_cliente(Cliente *clientes, int qtd_cliente, char *id) {
  for (int i = 0; i < qtd_cliente; i ++) {
    if (strcmp(clientes[i].cpf, id) == 0){
        return i;
    }
  }
  return -1;
}

int busca_aluguel(Aluguel *alugueis, int qtd_aluguel, int id) {
  for (int i = 0; i < qtd_aluguel; i ++) {
    if (alugueis[i].id_aluguel == id){
        return i;
    }
  }
  return -1;
}

//////////////////////
//                  //
//     REGISTRO     //
//                  //
//////////////////////
void registrar_livro(Livro **livros, int *qtd_livro){ //n�o � necessario tamanho de cliente e aluguel
    int flag;
    Livro aux;
    system(command);
    printf("Selecionado cadastro de livro.\n");
    printf("Iniciando cadastro...\n");

    do{
        flag = 0;
        printf("\nO codigo do livro deve conter 4 digitos inteiros e positivos.\n");
        printf("Digite o id do Livro: ");
        clean(stdin);
        scanf("%d", &aux.id);
        if(aux.id > 999 && aux.id < 10000){ //Verifica se o id � v�lido
          if (busca_livro(*livros, *qtd_livro, aux.id) != -1) { // verifica se a id ja existe
            printf("Essa ID j� existe, digite outra ID.\n");
            flag = 1;
          }
        } else {
            printf("ID invalido.\n");
            flag = 1;
        }
    } while (flag != 0);
    printf("ID armazendada.\n");

    do {
      printf("\nO nome do livro deve conter ate 64 caracteres.\n");
      printf("Digite o nome do livro: ");
      clean(stdin);
      fgets(aux.nome,64,stdin);
      aux.nome[strlen(aux.nome) - 1] = '\0';
      if (strlen(aux.nome) == 0) {
        printf("O nome nao pode ser um campo vazio.\n");
        flag = 1;
      } else {
        printf("Nome armazenado com sucesso.\n");
      }
    } while (flag != 0);

    do{
        flag = 0;
        printf("\nO estoque deve ser um numero inteiro positivo ate 10.000\n");
        printf("Digite o estoque do livro: ");
        clean(stdin);
        scanf("%d",&aux.estoque);
        if(aux.estoque < 0 || aux.estoque > 10000){
            printf("Quantidade invalida.\n");
            flag=1;
        }
    } while (flag != 0);
    printf("Estoque armazenado com sucesso.\n");

    do{
        flag = 0;
        printf("\nGeneros disponoveis\n");
        printf("A - Acao e Aventura\n");
        printf("C - Comedia\n");
        printf("D - Drama\n");
        printf("F - Ficcao Cientifica\n");
        printf("T - Terror\n");
        printf("Digite a letra respectiva ao genero: ");
        clean(stdin);
        scanf("%c", &aux.genero);
        aux.genero = toupper(aux.genero);
        if (aux.genero != 'A' && aux.genero != 'C' && aux.genero != 'D' && aux.genero != 'F' && aux.genero != 'T'){
            printf("Genero invalido.\n");
            flag = 1;
        }
    } while (flag != 0);
    printf("Genero armazenado com sucesso.\n");

    do{
        flag = 0;
        printf("\nO preco deve ser um numero positivo.\n");
        printf("Digite o preco: ");
        clean(stdin);
        scanf("%f", &aux.preco);
        if(aux.preco < 0){
            printf("Preco invalido.\n");
            flag = 1;
        }
    } while (flag != 0);

    (*livros)[*qtd_livro] = aux;
    (*qtd_livro)++;

    escrever_livro(&(*livros)[*qtd_livro - 1], 1, "ab");

  if (*qtd_livro % 5 == 0) {  // aumentando os espacos no vetor conforme necessidade, ou seja multiplo de 5 == vetor lotado, e entao aloca-se mais cinco
    *livros = realloc(*livros, sizeof(Livro) * (*qtd_livro + INTERVALO));
  }

  Livro *temp = *livros;
  *livros = realloc(temp, sizeof(Livro) * (*qtd_livro + INTERVALO));

  clean(stdin);
  printf("\nLivro cadastrado com sucesso!!!");
  printf("\nPressione enter para continuar ...");
  getchar();
}

void registrar_cliente(Cliente **clientes, int *qtd_cliente){
    int flag=0;
    Cliente aux;
    char verify;
    printf("Selecionado cadastro de cliente.\n");
    printf("Iniciando cadastro...\n");
    do{
        flag=0;
        printf("\nO CPF deve conter apenas 11 numeros inteiros.\n");
        clean(stdin);
        fgets(aux.cpf, 12, stdin);
        for(int i=0;i<strlen(aux.cpf);i++){
            if(!isdigit(aux.cpf[i])){
                flag = 1;
            }
        }

        if (strlen(aux.cpf) != 11 || flag == 1){
            printf("CPF invalido.\n");
            flag = 1;
        }else{
            flag = busca_cliente(*clientes,*qtd_cliente, aux.cpf);
            if(flag!=-1){
                printf("CPF ja cadastrado, tente outro CPF.\n");
            }
        }
    }while (flag !=-1);
    printf("CPF armazenado com sucesso.\n");
    do {
      flag = 0;
      printf("\nO nome do cliente deve conter no maximo 63 caracteres.\n");
      printf("Digite o nome do cliente: ");
      clean(stdin);
      fgets(aux.nome,64,stdin);
      aux.nome[strlen(aux.nome) - 1] = '\0';

      if (strlen(aux.nome) == 0) {
        printf("O nome nao pode ser um campo vazio.\n");
        flag = 1;
      } else {
        printf("Nome armazenado com sucesso.\n");
      }
    } while (flag != 0);
    do{
        flag=0;
        printf("Digite a idade do cliente: ");
        clean(stdin);
        scanf("%d",&aux.idade);
        if(aux.idade<0 || aux.idade>122){ //Idade da pessoa mais velha foi 122 anos.
            printf("Idade invalida.\n");
            flag=1;
        }
    } while (flag != 0);
    printf("Idade armazenada com sucesso.\n");

    do {
      printf("\nO email deve conter no maximo 31 caracteres.\n");
      printf("Digite o email: ");
      clean(stdin);
      fgets(aux.email,32,stdin);
      aux.email[strlen(aux.email) - 1] = '\0';

      if (strlen(aux.email) == 0) {
        printf("O email nao pode ser um campo vazio.\n");
        flag = 1;
      } else {
        printf("Email armazenado com sucesso.\n");
      }
    } while (flag != 0);

    do{
        flag=0;
        printf("\nInforme se o cliente e estudante(S/N): ");
        scanf("%c",&verify);
        verify = toupper(verify);
        if(verify=='S'){
            aux.estudante= true;
        }else{
            if(verify=='N'){
                aux.estudante= false;
            }else{
                printf("Resposta nao correspondente, digite apenas S/N.\n");
                flag=1;
            }
        }
  }while(flag != 0);

  (*clientes)[*qtd_cliente] = aux;
  (*qtd_cliente)++;


  escrever_cliente(&(*clientes)[*qtd_cliente - 1], 1, "ab");

  if (*qtd_cliente % 5 == 0) {  // aumentando os espacos no vetor conforme necessidade, ou seja multiplo de 5 == vetor lotado, e entao aloca-se mais cinco
    *clientes = realloc(*clientes, sizeof(Cliente) * (*qtd_cliente + INTERVALO));
  }



  clean(stdin);
  printf("\nCliente cadastrado com sucesso!!!");
  printf("\nPressione enter para continuar ...");
  getchar();
}

void registrar_aluguel(Aluguel **alugueis, int *qtd_aluguel, Cliente *clientes, int *qtd_cliente, Livro *livros, int *qtd_livro){
    int flag=0;
    Aluguel aux;
    int livro_ind; // indice do livro para diminuir o estoque
    printf("Selecionado cadastro de aluguel.\n");
    printf("Iniciando cadastro...\n");
    do{
        flag=0;
        printf("\nO ID do aluguel deve conter 4 numeros inteiros e positivos.\n");
        clean(stdin);
        scanf("%d", &aux.id_aluguel);
        if(aux.id_aluguel < 1000 || aux.id_aluguel > 9999){ //Verifica se o id e valido
            printf("ID invalido.\n");
            flag = 1;
        } else {
            if (busca_aluguel(*alugueis, *qtd_aluguel, aux.id_aluguel) != -1){
              printf("ID ja existente.\n");
              flag = 1;
            }
        }
    }while (flag != 0);
    printf("ID armazenada com sucesso.\n");
    do{
        flag = 0;
        printf("\nO CPF do cliente deve estar cadastrado para realizar o aluguel.\n");
        printf("\nO CPF deve conter 11 numeros inteiros e positivos.\n");
        printf("Digite o CPF do cliente: ");
        clean(stdin);
        fgets(aux.id_cliente,12,stdin);
        for(int i=0;i<strlen(aux.id_cliente);i++){
            if(!isdigit(aux.id_cliente[i])){
                flag = 1;
                break;
            }
        }
        if(strlen(aux.id_cliente) != 11 || flag==1){
            printf("CPF invalido.\n");
            flag=1;
        }else{
            flag = busca_cliente(clientes, *qtd_cliente, aux.id_cliente);
            if(flag==-1){
                printf("CPF nao cadastrado.\n");
            }else{
                flag=0;
            }
        }
    }while (flag!=0);
    printf("CPF armazenado com sucesso.\n");

    do{
        flag=0;
        printf("\nO codigo do livro deve conter 4 digitos inteiros e positivos.\n");
        printf("Digite o codigo do livro: ");
        clean(stdin);
        scanf("%d",&aux.id_livro);
        if(aux.id_livro < 1000 || aux.id_livro > 9999){ //Verifica se o id e valido
            printf("ID invalido.\n");
            flag = 1;
        }else{
            livro_ind = busca_livro(livros, *qtd_livro, aux.id_livro);
            if(livro_ind == -1){ // pegando o indice do livro que o aluguel sera registrado
              printf("ID nao cadastrada.\n");
              flag = 1;
            } else if (livros[livro_ind].estoque == 0) {
              printf("O livro nao esta em estoque.\n");
              flag = 1;
            } else {
              livros[livro_ind].estoque--;
              flag = 0;
            }
        }
    }while(flag!=0);

  do {
    printf("\nDigite a data de aluguel: \n");
    printf("Dia: ");
    scanf("%d", &aux.data_aluguel.dia);
    printf("Mes: ");
    scanf("%d", &aux.data_aluguel.mes);
    printf("Ano: ");
    scanf("%d", &aux.data_aluguel.ano);

    flag = validar_data(&aux.data_aluguel);

    if (flag == 0) { // flag == 0, entao data nao e valida
      printf("Data invalida.\n");
    }
  } while (flag == 0);

  do {
    printf("\nDigite a data de devolucao: \n");
    printf("Dia: ");
    scanf("%d", &aux.data_devolucao.dia);
    printf("Mes: ");
    scanf("%d", &aux.data_devolucao.mes);
    printf("Ano: ");
    scanf("%d", &aux.data_devolucao.ano);

    flag = validar_data(&aux.data_devolucao);

    if (flag == 0) {
      printf("Data invalida.\n");
    } else {
      flag = comparar_data(&aux.data_aluguel, &aux.data_devolucao);

      if (flag < 1) {
        printf("A data de devolucao nao pode ser a mesma ou anterior a data de aluguel.\n");
      }
    }
  } while (flag < 1);  // menor que 1, caso a data nao seja valida

  (*alugueis)[*qtd_aluguel] = aux;
  (*qtd_aluguel)++;

  escrever_aluguel(&(*alugueis)[*qtd_aluguel - 1], 1, "ab");
  escrever_livro(livros, *qtd_livro, "wb"); // salvando o estoque atualizado no arquivo

  if (*qtd_aluguel % 5 == 0) {  // aumentando os espacos no vetor conforme necessidade, ou seja multiplo de 5 == vetor lotado, e entao aloca-se mais cinco
    *alugueis = realloc(*alugueis, sizeof(Aluguel) * (*qtd_aluguel + INTERVALO));
  }

  clean(stdin);
  printf("\nAluguel cadastrado com sucesso!!!");
  printf("\nPressione enter para continuar ...");
  getchar();
}

//////////////////////
//                  //
//      EDICAO      //
//                  //
//////////////////////
void editar_livro(Livro *livros, int *qtd_livro) {
  int flag = 0; // flag para validade
  int livro_ind; // indice do livro a ser alterado
  int id; // id do livro a ser editado
  int opt; // opcao do menu

  // campos a serem editados
  char nome[64];
  char genero;
  float preco;
  int estoque;

  printf("Selecionado edicao de livro.\n");
  printf("Iniciando edicao...\n");
  // lendo o id do usuario
  do {
    flag = 0;
    printf("Digite o id do livro: ");
    scanf("%d", &id);

    if (id < 1000 || id > 9999) {
      printf("ID invalido.\n");
      flag = 1;
    }
  } while (flag != 0);
  // id lido

  // procurando pelo livro
  livro_ind = busca_livro(livros, *qtd_livro, id);

  if (livro_ind != -1) {
    do {
      flag = 0;
      printf("\nOpcoes a ser escolhida para editar:\n");
      printf("1. Nome\n");
      printf("2. Genero\n");
      printf("3. Preco\n");
      printf("4. Estoque\n");
      printf("5. Voltar ao menu\n");
      printf("Digite a opcao desejada: ");
      scanf("%d", &opt);

      if (opt < 1 || opt > 5) {
        printf("Opcao invalida!\n");
        flag = 1;
      }
    } while (flag != 0);

    switch (opt) {
      case 1: {
        do {
          flag = 0;
          printf("Digite o nome a ser editado: ");
          clean(stdin);
          fgets(nome, 64, stdin);
          nome[strlen(nome) - 1] = '\0';

          if (strlen(nome) == 0) {
            printf("O nome nao pode ser um campo vazio!\n");
            flag = 1;
          }
        } while (flag != 0);

        strcpy(livros[livro_ind].nome, nome);
      } break;
      case 2: {
        do {
          flag = 0;
          printf("\nGeneros disponoveis\n");
          printf("A - Acao e Aventura\n");
          printf("C - Comedia\n");
          printf("D - Drama\n");
          printf("F - Ficcao Cientifica\n");
          printf("T - Terror\n");
          printf("Digite o genero a ser editado: ");
          scanf("%c", &genero);

          genero = toupper(genero);

          if (genero != 'A' && genero != 'C' && genero != 'D' && genero != 'F' && genero != 'T') {
            printf("Genero invalido!\n");
            flag = 1;
          }
        } while (flag != 0);

        livros[livro_ind].genero = genero;
      } break;
      case 3: {
        do {
          flag = 0;
          printf("Digite o preco a ser editado: ");
          scanf("%f", &preco);

          if (preco < 0) {
            printf("O preco nao pode ser negativo!\n");
            flag = 1;
          }
        } while (flag != 0);

        livros[livro_ind].preco = preco;
      } break;
      case 4: {
        do {
          flag = 0;
          printf("Digite o estoque a ser editado: ");
          scanf("%d", &estoque);

          if (estoque < 0 || estoque > 10000) {
            printf("Quantidade invalida.\n");
            flag = 1;
          }
        } while (flag != 0);

        livros[livro_ind].estoque = estoque;
      } break;
      default: {
        flag = 1; // opcao 5 selecionada, entao nao tem livro editado
        break;
      }
    }

  } else {
    printf("Nao existe livro com esse id.\n");
  }

  clean(stdin);
  if (flag == 0) {
    escrever_livro(livros, *qtd_livro, "wb");

    printf("\nLivro editado com sucesso!!!");
  }
  printf("\nPressione enter para continuar ...");
  getchar();
}

void editar_cliente(Cliente *clientes, int *qtd_cliente) {
  int flag = 0; // flag para validade
  int cliente_ind; // indice do cliente a ser alterado
  char cpf[12]; // id do cliente a ser editado
  int opt; // opcao do menu

  // campos a serem editados
  char nome[64];
  int idade;
  char email[32];
  bool estudante;

  printf("Selecionado edicao de clientes.\n");
  printf("Iniciando edicao...\n");
  // pedindo o id ao usuario
  do {
    flag = 0;
    printf("Digite o CPF do cliente: ");
    fgets(cpf, 12, stdin);

    if (strlen(cpf) != 11) {
      printf("CPF invalido.\n");
      flag = 1;
    } else {
      for (int i = 0; i < strlen(cpf); i ++) {
        if (!isdigit(cpf[i])) {
          flag = 1;
          break;
        }
      }
    }
  } while (flag != 0);

  // procurando pelo livro
  cliente_ind = busca_cliente(clientes, *qtd_cliente, cpf);

  if (cliente_ind != -1) {
    do {
      flag = 0;
      printf("\nOpcoes a ser escolhida para editar:\n");
      printf("1. Nome\n");
      printf("2. Idade\n");
      printf("3. Email\n");
      printf("4. Estudante\n");
      printf("5. Voltar ao menu\n");
      printf("Digite a opcao desejada: ");
      scanf("%d", &opt);

      if (opt < 1 || opt > 5) {
        printf("Opcao invalida!\n");
        flag = 1;
      }
    } while (flag != 0);

   switch (opt) {
      case 1: {
        do {
          flag = 0;
          printf("Digite o nome a ser editado: ");
          clean(stdin);
          fgets(nome, 64, stdin);
          nome[strlen(nome) - 1] = '\0';

          if (strlen(nome) == 0) {
            printf("O nome nao pode ser um campo vazio!\n");
            flag = 1;
          }
        } while (flag != 0);

        strcpy(clientes[cliente_ind].nome, nome);
      } break;
      case 2: {
        do {
          flag = 0;
          printf("Digite a idade a ser editada: ");
          scanf("%d", &idade);

          if (idade < 0) {
            printf("A idade nao pode ser negativa!\n");
            flag = 1;
          }else if (idade > 122) {
            printf("Idade invalida.\n");
            flag=1;
          }
        } while (flag != 0);

        clientes[cliente_ind].idade = idade;
      } break;
      case 3: {
      do {
          flag = 0;
          printf("Digite o email a ser editado: ");
          clean(stdin);
          fgets(email, 32, stdin);
          email[strlen(email) - 1] = '\0';

          if (strlen(email) == 0) {
            printf("O email nao pode ser um campo vazio!\n");
            flag = 1;
          }
        } while (flag != 0);

        strcpy(clientes[cliente_ind].email, email);
      } break;
      case 4: {
        do{
            char verify;
            flag=0;
            printf("\nInforme se o cliente e estudante(S/N): ");
            clean(stdin);
            scanf("%c",&verify);
            verify = toupper(verify);
            if(verify=='S'){
                estudante= true;
            }else{
                if(verify=='N'){
                    estudante= false;
                }else{
                    printf("Resposta nao correspondente, digite apenas S/N.\n");
                    flag=1;
                }
            }
        }while(flag != 0);
        clientes[cliente_ind].estudante = estudante;
      } break;
      default: {
        flag = 1; // opcao 5 selecionada, entao nao tem cliente editado
        break;
      }
    }
  } else {
    printf("Nao existe cliente com esse cpf.\n");
  }
  clean(stdin);
  if (flag == 0) {
    escrever_cliente(clientes, *qtd_cliente, "wb");

    printf("\nCliente editado com sucesso!!!");
  }
  printf("\nPressione enter para continuar ...");
  getchar();
}

void editar_aluguel(Aluguel *alugueis, int *qtd_aluguel,Cliente *clientes, int *qtd_cliente, Livro *livros, int *qtd_livro) {
    int flag = 0; // flag para validade
    int aluguel_ind; // indice do aluguel a ser alterado
    int opt; // opcao do menu

    // campos a serem editados
    char id_cliente[12];
    int id_livro;
    int id_aluguel;
    Data data_aluguel;
    Data data_devolucao;

    printf("Selecionado edicao de alugueis.\n");
    printf("Iniciando edicao...\n");
    // pedindo o id ao usuario
    do {
        flag = 0;
        printf("Digite o id do aluguel: ");
        scanf("%d", &id_aluguel);

        if (id_aluguel < 1000 || id_aluguel > 9999) {
          printf("ID invalido.\n");
          flag = 1;
        }
    } while (flag != 0);
    // id lido

    // procurando pelo livro
    aluguel_ind = busca_aluguel(alugueis, *qtd_aluguel, id_aluguel);

    if (aluguel_ind != -1) {
        do {
          flag = 0;
          printf("\nOpcoes a ser escolhida para editar:\n");
          printf("1. ID Cliente\n");
          printf("2. ID Livro\n");
          printf("3. Data Aluguel\n");
          printf("4. Data Devolução\n");
          printf("5. Voltar ao menu\n");
          printf("Digite a opcao desejada: ");
          scanf("%d", &opt);

          if (opt < 1 || opt > 5) {
            printf("Opcao invalida!\n");
            flag = 1;
          }
        } while (flag != 0);

        switch (opt) {
          case 1: {
            do{
                flag = 0;
                printf("\nO CPF do cliente deve estar cadastrado para realizar o aluguel.\n");
                printf("\nO CPF deve conter 11 numeros inteiros e positivos.\n");
                printf("Digite o CPF do cliente: ");
                clean(stdin);
                fgets(id_cliente,12,stdin);
                for(int i=0;i<strlen(id_cliente);i++){
                    if(!isdigit(id_cliente[i])){
                        flag = 1;
                        break;
                    }
                }
                if(strlen(id_cliente) != 11 || flag==1){
                    printf("CPF invalido.\n");
                    flag=1;
                }else{
                    flag = busca_cliente(clientes, *qtd_cliente,id_cliente);
                    if(flag==-1){
                        printf("CPF nao cadastrado.\n");
                    }else{
                        flag=0;
                    }
                }
            }while (flag!=0);

            strcpy(alugueis[aluguel_ind].id_cliente, id_cliente);
          } break;
          case 2: {
            int livro_ind;
            do{
                flag=0;
                printf("\nO codigo do livro deve conter 4 digitos inteiros e positivos.\n");
                printf("Digite o codigo do livro: ");
                clean(stdin);
                scanf("%d",&id_livro);
                if(id_livro < 1000 || id_livro > 9999){ //Verifica se o id e valido
                    printf("ID invalido.\n");
                    flag = 1;
                }else{
                    livro_ind = busca_livro(livros, *qtd_livro, id_livro);
                    if(livro_ind == -1){ // pegando o indice do livro que o aluguel sera registrado
                      printf("ID nao cadastrada.\n");
                      flag = 1;
                    } else if (livros[livro_ind].estoque == 0) {
                      printf("O livro nao esta em estoque.\n");
                      flag = 1;
                    } else {
                      livros[livro_ind].estoque--;
                      flag = 0;
                    }
                }
            }while(flag!=0);

            livros[alugueis[aluguel_ind].id_livro].estoque++; //confere se isso funciona, tenho que incrementar 1 no estoque do livro antigo

            alugueis[aluguel_ind].id_livro = id_livro;
              } break;
          case 3: {
            do {
                flag = 0;
                printf("\nDigite a data de aluguel a ser alterada: \n");
                printf("Dia: ");
                scanf("%d", &data_aluguel.dia);
                printf("Mes: ");
                scanf("%d", &data_aluguel.mes);
                printf("Ano: ");
                scanf("%d", &data_aluguel.ano);

                if (!validar_data(&data_aluguel)) {
                    printf("Data invalida\n");
                    flag = 1;
                }
            } while (flag != 0);

            alugueis->data_aluguel = data_aluguel;
          } break;
          case 4: {
              do {
                  flag = 0;
                  printf("\nDigite a data de devolucao a ser alterada: \n");
                  printf("Dia: ");
                  scanf("%d", &data_devolucao.dia);
                  printf("Mes: ");
                  scanf("%d", &data_devolucao.mes);
                  printf("Ano: ");
                  scanf("%d", &data_devolucao.ano);

                  if (!validar_data(&data_devolucao)) {
                      printf("Data invalida\n");
                      flag = 1;
                  }
              } while (flag != 0);

              alugueis->data_devolucao = data_devolucao;
          } break;
          default: {
            flag = 1; // opcao 5 selecionada, entao nao tem aluguel editado
            break;
          }
        }
    } else {
        printf("Nao existe aluguel com esse id.\n");
    }

  clean(stdin);
  if (flag == 0) {
    escrever_aluguel(alugueis, *qtd_aluguel, "wb");

    printf("\nAluguel editado com sucesso!!!");
  }
  printf("\nPressione enter para continuar ...");
  getchar();
}

/*char id_cliente[12];
  int id_livro;
  Data data_aluguel;
  Data data_devolucao;
*/

//////////////////////
//                  //
//     CONSULTA     //
//                  //
//////////////////////
void consultar_livro(Livro *livros, int qtd_livro) {
  int opt, flag;
  int livro_ind; // indice do livro retornardo pela funcao de busca

  // campos para busca
  char nome[64];
  int id;
  char genero;

  system(command);
  printf("Selecionado a consulta de livros ...\n");
  // selecionando a opcao de consulta
  do {
    flag = 0;
    printf("Escolha um campo para a consuta: \n");
    printf("1. Nome\n");
    printf("2. ID\n");
    printf("3. Genero\n");
    printf("Digite a opcao desejada: ");
    clean(stdin);
    scanf("%d", &opt);

    if (opt < 1 || opt > 3) {
      printf("Voce escolheu uma opcao que nao esta no menu. \n\n");
      flag = 1;
    }
  } while (flag != 0);

  switch (opt) {
    case 1: {
      do {
        flag = 0;
        printf("Digite o nome a ser procurado: ");
        clean(stdin);
        fgets(nome, 64, stdin);
        nome[strlen(nome) - 1] = '\0';

        if (strlen(nome) == 0) {
          printf("O nome nao pode ser um campo vazio.\n\n");
          flag = 1;
        }
      } while (flag != 0);

      printf("\nResultados encontrados:\n\n");
      for (int i = 0; i < qtd_livro; i ++) {
        if (strstr(livros[i].nome, nome)) {
            flag = 1;
            printf("\nNome: %s - Genero: %c\n", livros[i].nome, livros[i].genero);
            printf("ID: %d - Preco: %.2f - Estoque: %d\n", livros[i].id, livros[i].preco, livros[i].estoque);
            printf("++++++++++++++++++++\n");
        }
      }

      if (flag == 0) {
        printf("Nenhum resultado foi encontrado.\n");
      }

      printf("Pressione enter para continuar ...");
      getchar();
    } break;
    case 2: {
      do {
        flag = 0;
        printf("Digite o id do livro: ");
        clean(stdin);
        scanf("%d", &id);
        clean(stdin);

        if (id < 1000 || id > 9999) {
          printf("ID nao e valido.\n\n");
          flag = 1;
        }
      } while (flag != 0);

      livro_ind = busca_livro(livros, qtd_livro, id);

      if (livro_ind == -1) {
        printf("Nao existe livro com esse id.\n");
      } else {
        printf("\nResultado encontrado:\n");
        printf("\nNome: %s - Genero: %c\n", livros[livro_ind].nome, livros[livro_ind].genero);
        printf("ID: %d - Preco: %.2f - Estoque: %d\n\n", livros[livro_ind].id, livros[livro_ind].preco, livros[livro_ind].estoque);
      }

      printf("\nPressione enter para continuar ...");
      getchar();
    } break;
    case 3: {
      do {
        flag = 0;
        printf("\nGeneros disponoveis\n");
        printf("A - Acao e Aventura\n");
        printf("C - Comedia\n");
        printf("D - Drama\n");
        printf("F - Ficcao Cientifica\n");
        printf("T - Terror\n");
        printf("Digite o genero do livro: ");
        clean(stdin);
        scanf("%c", &genero);
        clean(stdin);

        genero = toupper(genero);

        if (genero != 'A' && genero != 'C' && genero != 'D' && genero != 'F' && genero != 'T') {
          printf("Genero invalido.\n\n");
          flag = 1;
        }
      } while (flag != 0);

      printf("\nResultados encontrados:\n\n");
      for (int i = 0; i < qtd_livro; i ++) {
        if (livros[i].genero == genero) {
            flag = 1;
            printf("\nNome: %s - Genero: %c\n", livros[i].nome, livros[i].genero);
            printf("ID: %d - Preco: %.2f - Estoque: %d\n", livros[i].id, livros[i].preco, livros[i].estoque);
            printf("++++++++++++++++++++\n");
        }
      }

      if (flag == 0) {
        printf("Nenhum resultado foi encontrado.\n");
      }

      printf("\nPressione enter para continuar ...");
      getchar();
    } break;
    default: break;
  }
}

void consultar_cliente(Cliente *clientes, int qtd_cliente) {
  int opt, flag;
  int cliente_ind;
  char estudante[6];

  // campos para busca
  char nome[64];
  char cpf[12];
  char aluno;
  bool is_aluno; // guardar valor de true ou false pra comparar na busca

  system(command);
  printf("Selecionado a consulta de clientes ...\n");

  do {
    flag = 0;
    printf("Escolha um campo para a consuta: \n");
    printf("1. Nome\n");
    printf("2. CPF\n");
    printf("3. Aluno\n");
    printf("Digite a opcao desejada: ");
    clean(stdin);
    scanf("%d", &opt);

    if (opt < 1 || opt > 3) {
      printf("Voce escolheu uma opcao que nao esta no menu. \n\n");
      flag = 1;
    }
  } while (flag != 0);

  switch (opt) {
    case 1: {
      do {
        flag = 0;
        printf("Digite o nome a ser procurado: ");
        clean(stdin);
        fgets(nome, 64, stdin);
        nome[strlen(nome) - 1] = '\0';

        if (strlen(nome) == 0) {
          printf("O nome nao pode ser um campo vazio.\n\n");
          flag = 1;
        }
      } while (flag != 0);

      printf("\nResultados encontrados:\n\n");
      for (int i = 0; i < qtd_cliente; i ++) {
        if (strstr(clientes[i].nome, nome)) {
          if (clientes[i].estudante) {
            strcpy(estudante, "Sim");
          } else {
            strcpy(estudante, "Nao");
          }
          flag = 1;
          printf("\nNome: %s - Idade: %d\n", clientes[i].nome, clientes[i].idade);
          printf("CPF: %s\n", clientes[i].cpf);
          printf("Email: %s\n", clientes[i].email);
          printf("Estudante: %s\n", estudante);
          printf("++++++++++++++++++++\n");
        }
      }

      if (flag == 0) {
        printf("Nenhum resultado foi encontrado.\n");
      }

      printf("Pressione enter para continuar ...");
      getchar();
    } break;
    case 2: {
      do {
        flag = 0;
        printf("Digite o cpf do cliente: ");
        clean(stdin);
        fgets(cpf, 12, stdin);
        clean(stdin);

        for (int i = 0; i < strlen(cpf); i++) {
          if (!isdigit(cpf[i])) {
            flag = 1;
          }
        }

        if (strlen(cpf) != 11 || flag == 1) {
          printf("CPF invalido.\n");
        }
      } while (flag != 0);

      cliente_ind = busca_cliente(clientes, qtd_cliente, cpf);

      if (cliente_ind == -1) {
        printf("Nao existe cliente com esse cpf.\n");
      } else {
        if (clientes[cliente_ind].estudante) {
          strcpy(estudante, "Sim");
        } else {
          strcpy(estudante, "Nao");
        }

        printf("\nResultado encontrado:\n");
        printf("\nNome: %s - Idade: %d\n", clientes[cliente_ind].nome, clientes[cliente_ind].idade);
        printf("CPF: %s\n", clientes[cliente_ind].cpf);
        printf("Email: %s\n", clientes[cliente_ind].email);
        printf("Estudante: %s\n", estudante);
      }

      printf("\nPressione enter para continuar ...");
      getchar();
    } break;
    case 3: {
      do {
        flag = 0;
        printf("O cliente e um estudante (S/N): ");
        clean(stdin);
        scanf("%c", &aluno);
        clean(stdin);

        aluno = toupper(aluno);

        if (aluno != 'S' && aluno != 'N') {
          printf("Opcao invalida.\n\n");
          flag = 1;
        }
      } while (flag != 0);

      if (aluno == 'S') {
        is_aluno = true;
      } else {
        is_aluno = false;
      }

      printf("\nResultados encontrados:\n\n");
      for (int i = 0; i < qtd_cliente; i ++) {
        if (clientes[i].estudante == is_aluno) {
            if (clientes[i].estudante) {
              strcpy(estudante, "Sim");
            } else {
              strcpy(estudante, "Nao");
            }
            flag = 1;
            printf("\nNome: %s - Idade: %d\n", clientes[i].nome, clientes[i].idade);
            printf("CPF: %s\n", clientes[i].cpf);
            printf("Email: %s\n", clientes[i].email);
            printf("Estudante: %s\n", estudante);
            printf("++++++++++++++++++++\n");
        }
      }

      if (flag == 0) {
        printf("Nenhum resultado foi encontrado.\n");
      }

      printf("\nPressione enter para continuar ...");
      getchar();
    } break;
    default: break;
  }
}

void consultar_aluguel(Aluguel *alugueis, int qtd_aluguel, Cliente *clientes, int qtd_cliente, Livro *livros, int qtd_livro) {
  system(command);
  printf("Selecionado a consulta de alugueis ...\n\n");

  int flag = 0;
  int livro_ind;
  int cliente_ind;

  ordenar_data(alugueis, qtd_aluguel, 2);

  for (int i = 0; i < qtd_aluguel; i ++) {
    flag = 1;
    livro_ind = busca_livro(livros, qtd_livro, alugueis[i].id_livro);
    cliente_ind = busca_cliente(clientes, qtd_cliente, alugueis[i].id_cliente);

    printf("Data de devolucao: %d/%d/%d\n", alugueis[i].data_devolucao.dia, alugueis[i].data_devolucao.mes, alugueis[i].data_devolucao.ano);
    printf("Nome do cliente: %s\n", clientes[cliente_ind].nome);
    printf("Nome do livro: %s\n\n", livros[livro_ind].nome);
    printf("++++++++++++++++++++\n");
  }

  if (flag == 0) {
    printf("Nenhum resultado foi encontrado.\n");
  }
  printf("\nPressione enter para continuar ...");
  getchar();
}

//////////////////////
//                  //
//     REMOVER      //
//                  //
//////////////////////
void remover_livro(Livro *livros, int *qtd_livro, Aluguel *alugueis, int *qtd_aluguel) {
  int id, flag;
  int livro_ind;

  system(command);
  printf("Selecionado a remocao de livro...\n");

  do {
    flag = 0;
    printf("Digite o id do livro a ser removido: ");
    clean(stdin);
    scanf("%d", &id);

    if (id < 1000 || id > 9999) {
      printf("ID invalido.\n\n");
      flag = 1;
    }
  } while (flag != 0);

  for (int i = 0; i < *qtd_aluguel; i ++) {
    if (alugueis[i].id_livro == id) {
      flag = 1;
      break;
    }
  }

  if (flag == 1) {
    printf("O livro nao pode ser excluido, devido a estar cadastrado em algum aluguel.\n");
  } else {
    livro_ind = busca_livro(livros, *qtd_livro, id);

    if (livro_ind != -1) {
      for (int i = livro_ind; i < *qtd_livro - 1; i ++) {
        livros[i] = livros[i + 1];
      }
      (*qtd_livro)--;

      escrever_livro(livros, *qtd_livro, "wb"); // salvando um livro a menos

      printf("Livro removido com sucesso.\n");
    } else {
      printf("Nao existe livro com esse id.\n");
    }
  }

  clean(stdin);
  printf("\nPressione enter para continuar ...");
  getchar();
}

void remover_cliente(Cliente *clientes, int *qtd_cliente, Aluguel *alugueis, int *qtd_aluguel) {
  int flag;
  char cpf[12];
  int cliente_ind;

  system(command);
  printf("Selecionado a remocao do cliente...\n");

  do {
    flag = 0;
    printf("Digite o cpf do cliente a ser removido: ");
    clean(stdin);
    fgets(cpf, 12, stdin);

    for (int i = 0; i < strlen(cpf); i ++) {
      if (!isdigit(cpf[0])) {
        flag = 1;
        break;
      }
    }

    if (strlen(cpf) != 11 || flag == 1) {
      printf("CPF invalido.\n");
      flag = 1;
    }
  } while (flag != 0);

  for (int i = 0; i < *qtd_aluguel; i ++) {
    if (strcmp(alugueis[i].id_cliente, cpf) == 0) {
      flag = 1;
      break;
    }
  }

  if (flag == 1) {
    printf("O cliente nao pode ser excluido, devido a estar cadastrado em algum aluguel.\n");
  } else {
    cliente_ind = busca_cliente(clientes, *qtd_cliente, cpf);

    if (flag == 1) {
      printf("O cliente nao pode ser excluido, devido a estar cadastrado em algum aluguel.\n");
    } else {
      cliente_ind = busca_cliente(clientes, *qtd_cliente, cpf);

      if (cliente_ind != -1) {
        for (int i = cliente_ind; i < *qtd_cliente - 1; i ++) {
          clientes[i] = clientes[i + 1];
        }
        (*qtd_cliente)--;

        escrever_cliente(clientes, *qtd_cliente, "wb"); // salvando um cliente a menos

        printf("Cliente removido com sucesso.\n");
      } else {
        printf("Nao existe cliente com esse cpf.\n");
      }
    }
  }

  clean(stdin);
  printf("\nPressione enter para continuar ...");
  getchar();
}

void remover_aluguel(Aluguel *alugueis, int *qtd_aluguel) {
  int id, flag;
  int aluguel_ind;

  system(command);
  printf("Selecionado a remocao de aluguel...\n");

  do {
    flag = 0;
    printf("Digite o id do aluguel a ser removido: ");
    clean(stdin);
    scanf("%d", &id);

    if (id < 1000 || id > 9999) {
      printf("ID invalido.\n\n");
      flag = 1;
    }
  } while (flag != 0);


  aluguel_ind = busca_aluguel(alugueis, *qtd_aluguel, id);

  if (aluguel_ind != -1) {
    for (int i = aluguel_ind; i < *qtd_aluguel - 1; i ++) {
      alugueis[i] = alugueis[i + 1];
    }
    (*qtd_aluguel)--;

    escrever_aluguel(alugueis, *qtd_aluguel, "wb"); // salvando um aluguel a menos

    printf("Aluguel removido com sucesso.\n");
  } else {
    printf("Nao existe aluguel com esse id.\n");
  }

  clean(stdin);
  printf("\nPressione enter para continuar ...");
  getchar();
}

//////////////////////
//                  //
//     RELATORIO    //
//                  //
//////////////////////
void gerar_relatorio(Cliente *clientes, Livro *livros, Aluguel *alugueis, int qtd_livro, int qtd_cliente, int qtd_aluguel) {
  Data data_inicial;
  int data_valida, cliente_ind, livro_ind;
  char estudante[6];

  system(command);
  printf("Gerando o relatorio, escolha uma data inicial: \n");

  clean(stdin);
  do {
    printf("Digite o dia: ");
    scanf("%d", &data_inicial.dia);
    printf("Digite o mes: ");
    scanf("%d", &data_inicial.mes);
    printf("Digite o ano: ");
    scanf("%d", &data_inicial.ano);

    data_valida = validar_data(&data_inicial);

    if (!data_valida) {
      printf("\nDigite uma data valida.\n");
    }
  } while (!data_valida);

  FILE *file = fopen(RELATORIO_ARQ, "w");

  fprintf(file, " ========== RELATORIO ========== \n");
  fprintf(file, "Data de inicio: %d/%d/%d\n\n\n", data_inicial.dia, data_inicial.mes, data_inicial.ano);

  ordenar_data(alugueis, qtd_aluguel, 1);

  for (int i = 0; i < qtd_aluguel; i ++) {
    if (comparar_data(&data_inicial, &alugueis[i].data_aluguel) >= 0) {
      livro_ind = busca_livro(livros, qtd_livro, alugueis[i].id_livro);
      cliente_ind = busca_cliente(clientes, qtd_cliente, alugueis[i].id_cliente);

      if (clientes[cliente_ind].estudante)
        strcpy(estudante, "Sim");
      else
        strcpy(estudante, "Nao");

      // cliente
      fprintf(file, "Nome: %s - Idade: %d\n", clientes[cliente_ind].nome, clientes[cliente_ind].idade);
      fprintf(file, "CPF: %s\n", clientes[cliente_ind].cpf);
      fprintf(file, "Email: %s\n", clientes[cliente_ind].email);
      fprintf(file, "Estudante: %s\n\n", estudante);

      // livro
      fprintf(file, "Nome: %s - Genero: %c\n", livros[livro_ind].nome, livros[livro_ind].genero);
      fprintf(file, "ID: %d - Preco: %.2f - Estoque: %d\n\n", livros[livro_ind].id, livros[livro_ind].preco, livros[livro_ind].estoque);

      // data do aluguel
      fprintf(file, "Data do Aluguel: %d/%d/%d\n\n", alugueis[i].data_aluguel.dia, alugueis[i].data_aluguel.mes, alugueis[i].data_aluguel.ano);
      fprintf(file, "++++++++++++++++++++\n\n");
    }
  }

  fclose(file);

  clean(stdin);
  printf("\nRelatorio gerado com sucesso!!!");
  printf("\nPressione enter para continuar ...");
  getchar();
}
