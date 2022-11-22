/*
* Gustavo Rédua de Oliveira
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER_CHAR 15
#define MAX_LINE_CHAR 1000

// Enumeração que define se um dado
// é um operando ou uma operação 
typedef enum {
    NUM = 'N',
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    EXP = '^',
    LOG = 'L',
    FAT = '!',
    MOD = '%',
    FIM = '@'
} tipo_op;

// Define um registro do tipo celula 
// usado na pilha e lista. 
typedef struct cel {
    double valor;
    tipo_op tipo;
    struct cel *prox;
} celula;


// Funções de manipulação de estruturas de dados
void inserir_celula_na_fila (celula **fila, double pValor, tipo_op pTipo);
void inserir_celula_na_pilha (celula **pilha, celula cel);

celula *remove_celula_da_fila (celula **fila);
celula *remove_celula_da_pilha(celula **pilha);


// Funções matemáticas
double exponencial(double base, int expoente);
double logaritmo(double valor);
double fatorial (double n);
double piso(double valor);

// Calculadora de expressões RPN
double calcula_expressao (celula* filaCelula);


int main()
{

    int quantidadeTestes;
    int index;
    int indexAtual;

    double numeroAtual;

    char proximoCaracter;
    char stringNumero[MAX_NUMBER_CHAR];
    char *ponteiroAuxiliar;
    char linha[MAX_LINE_CHAR];


    // leitura
    scanf("%d", &quantidadeTestes);
    
    for(int i = 0; i < quantidadeTestes; i++){
        
        scanf("\r%[^\n]", linha);

        // variável para estruturas de dados;
        celula *filaCelula = NULL;        

        // inicializa variáveis auxiliares para a 
        // separação dos termos e operações da sentença

        // int
        index = 0;
        indexAtual = 0;


        // Funcionamento do processamento da string lida
        /*  o código abaixo percorrerá toda a string e por meio de padrões definidos identificará os números e operadores
            presentes na sentença RPN.
            
            para a explicação, tome os seguintes símbulos como:

                H : String com a expressão RPN;

                H': SubString gerada a partir de H contendo os caracter de um número;

                I : variável que controla o index da string que contem a 
                    expressão analizada;

                J : caracter espaço (' ');

                K : contador que é incrementado a cada pregressão realizada por I para 
                    determinar a quantidade de caracter presentes em determinado número;
                
                L : Número gerado da conversão de H' usando a função strtod().

                    
                    *   Elemento relevante são todos os caracter != J;
                    *   possíveis números ou operadores são todos os elementos relevantes.



            ->  Sabendo que o primeiro termo da expressão sempre será um número e que após qualquer número ou operando o próximo caracter é J:
                Para a identificação, separação e processamento de um número em H:
                
                a)  percorre H até encontrar o último algarismo de um número utilizando K e I;

                b)  quando chega ao último algarismo de um número:
                
                            -   utiliza da função strncpy() para gerar uma H' inicida no endereço de H + (I - K);

                            -   utiliza da função strtod() para converter a H' em um número no formato double;
                
                            -   Insere L na fila de elementos que compõe uma expressão RPN;

                            -   Neste momento H[i] == J, portanto, adiciona 1 em I para que avance para o próximo elemento relevante em H.



            -> Para a separação de operadores em H faz-se uma verificação no próximo caracter pois:
                
                a)  todo caracter do tipo operador é sucedido por J;

                b)  sabendo que sempre o primeiro elemento de H é um número e, depois que este é processado, todas as próximas verificações ocorrerão
                    já em um caracter relevante; Sabendo também, pela informações do trabalho, que todo número ocupara sempre 3 posições ou mais em H;
                    Logo: 
                            -   se o caracter de H[I] != J, este será um número ou um operador;

                            -   todo número ocupará um espaço >= 3, Portanto, para todo possivel número em H, H[I] até H[I + 2] != J;

                            -   induzindo I para a próxima localização em H, se H[I + 1] == J, sabe-se que o caracter atual é um operador;
        */ 
        
        while (linha[index] != FIM)
        {

            // diferencia operadores de números 
            proximoCaracter = linha[index + 1];

            if (proximoCaracter == ' ')
            {

                // adiciona  operador da equação lida
                // na fila de informações
                inserir_celula_na_fila(&filaCelula, 0, linha[index]);

                // incrementa a quantidade necessária
                // no index para avançar até o próximo char relevante
                index += 2;
            }
            else
            {         
                
                // lê a quantidade de caracter até
                // o próximo espaço na string 
                while (linha[index] != ' ')
                {
                    index++;
                    indexAtual++;
                }

                // cria uma substring contendo todos os 
                // caracter do número identificado dentro
                // do intervalo estabelecido
                strncpy(stringNumero, (linha + (index - indexAtual)), indexAtual);
                stringNumero[indexAtual] = '\0';
                numeroAtual = strtod(stringNumero, &ponteiroAuxiliar);

                // Adiciona valor encontrado na fila
                // de informações da equação lida
                inserir_celula_na_fila(&filaCelula, numeroAtual, NUM);
            
                // incrementa a quantidade de caracter
                // processados na variável index mais o
                // necessário para avançar até o próximo char relevante
                index ++;
                indexAtual = 0;
            }           
        }

        printf("%.2lf\n", calcula_expressao(filaCelula));
        free(filaCelula);
    }
    
    return 0;
}



// Calculadora de expressões RPN
double calcula_expressao (celula* filaCelula)
{

    celula *pilha = NULL;
    celula *celulaRemovidaDaFila = NULL;
    celula celulaAuxiliar;

    double a;
    double b;

    celulaAuxiliar.tipo = NUM;
    celulaAuxiliar.prox = NULL;


    while (filaCelula)
    {
        celulaRemovidaDaFila = remove_celula_da_fila(&filaCelula);
        if (celulaRemovidaDaFila -> tipo == NUM)
        {
            inserir_celula_na_pilha(&pilha, *celulaRemovidaDaFila);
        }
        else
        {            
            switch (celulaRemovidaDaFila -> tipo)
            {
                case ADD:

                    a = remove_celula_da_pilha(&pilha) -> valor;
                    b = remove_celula_da_pilha(&pilha) -> valor;
                    celulaAuxiliar.valor = a + b;
                    inserir_celula_na_pilha(&pilha, celulaAuxiliar);  
                    break;

                case SUB:


                    b = remove_celula_da_pilha(&pilha) -> valor;
                    a = remove_celula_da_pilha(&pilha) -> valor;
                    celulaAuxiliar.valor = a - b;
                    inserir_celula_na_pilha(&pilha, celulaAuxiliar);
                    break;

                case MUL:
                   
                    a = remove_celula_da_pilha(&pilha) -> valor;
                    b = remove_celula_da_pilha(&pilha) -> valor;
                    celulaAuxiliar.valor = a * b;
                    inserir_celula_na_pilha(&pilha, celulaAuxiliar);
                    break;

                case DIV:

                    b = remove_celula_da_pilha(&pilha) -> valor;
                    a = remove_celula_da_pilha(&pilha) -> valor;
                    celulaAuxiliar.valor = a / b; 
                    inserir_celula_na_pilha(&pilha, celulaAuxiliar);
                    break;

                case MOD:

                    b = piso(remove_celula_da_pilha(&pilha) -> valor);
                    a = piso(remove_celula_da_pilha(&pilha) -> valor);
                    celulaAuxiliar.valor = (int)a % (int)b;
                    inserir_celula_na_pilha(&pilha, celulaAuxiliar);
                    break;

                case EXP:

                    b = remove_celula_da_pilha(&pilha) -> valor;
                    a = remove_celula_da_pilha(&pilha) -> valor;
                    celulaAuxiliar.valor = exponencial(a, b);
                    inserir_celula_na_pilha(&pilha, celulaAuxiliar);
                    break;

                case LOG:

                    a = remove_celula_da_pilha(&pilha) -> valor;
                    celulaAuxiliar.valor = logaritmo(piso(a));
                    inserir_celula_na_pilha(&pilha, celulaAuxiliar);
                    break;

                case FAT:

                    a = remove_celula_da_pilha(&pilha) -> valor;
                    celulaAuxiliar.valor = fatorial(piso(a));
                    inserir_celula_na_pilha(&pilha, celulaAuxiliar);
                    break;
            }
        }

    }

    return remove_celula_da_pilha(&pilha) -> valor;
}



// Funções de manipulação de estruturas de dados
void inserir_celula_na_fila (celula **fila, double pValor, tipo_op pTipo)
{
    celula *novaCelula = malloc(sizeof(celula));
    celula *celulaAux = malloc(sizeof(celula)); 
    if (novaCelula)
    {
        // insere dados no novo nó
        novaCelula -> valor = pValor;
        novaCelula -> tipo = pTipo;
        novaCelula -> prox = NULL;

        // verifica posição atual da fila para inserção do novo nó
        if(*fila)
        {
            celulaAux = *fila;
            while (celulaAux -> prox){ celulaAux = celulaAux -> prox;}
            celulaAux -> prox = novaCelula;
        }
        else
        {
            *fila = novaCelula;
        }
    }
    else
    {
        printf("\n\n\t\t\tErro ao alocar memoria para a criação de um novo item na fila\n");
    }
}


celula *remove_celula_da_fila (celula **fila)
{
    celula *removida = malloc(sizeof(celula));
    if (removida)
    {
        if (*fila)
        {
            removida = *fila;
            *fila = removida -> prox;
            return removida;
        }
        else
        {
            printf("\n\n\t\t\tALERTA DE REMOCAO DE ITENS DA FILA:\tFila vazia\n");
            return NULL;
        }
    
    }
    
    printf("\n\n\t\t\tALERTA DE REMOCAO DE ITENS DA FILA:\tErro de alocacao de momoria\n");
    return NULL;
    
}


void inserir_celula_na_pilha (celula **pilha, celula cel)
{
    celula *novaCelula = malloc(sizeof(celula));

    if (novaCelula)
    {
        novaCelula -> valor = cel.valor;
        novaCelula -> tipo =  cel.tipo;
        novaCelula -> prox = *pilha;
        *pilha = novaCelula;
        
    }
    else
    {
        printf("\n\n\t\t\tErro ao alocar memoria para a criação de um novo item na pilha\n");
    }

} 


celula *remove_celula_da_pilha(celula **pilha)
{
    
    if (*pilha)
    {
        celula *removida = malloc(sizeof(celula));
        if (removida)
        {
            removida = *pilha;
            *pilha = removida -> prox;
            return removida;
        }
        else
        {
            printf("ALERTA DE REMOCAO DE ITENS DA PILHA:\tErro de alocacao de momoria\n");
            return NULL;
        }

        
    }
    
    return NULL;
}



// funções matemáticas
double exponencial(double base, int expoente)
{

    double resposta = 1;
    
    for (int i = 0; i < abs(expoente); i++)
    {
        resposta *= base;
    }
    
    if (expoente < 0)
    {
        resposta = 1.0 / resposta;
    }
    
    return resposta;
}


double logaritmo(double valor)
{

    if (valor == 1 || valor <= 0)
    {
       return 0;
    }
    
    int n = valor / 2;

    if(n == 0)
    {
        return 0;
    }

    return 1+logaritmo(n);
    
}


double fatorial (double n) {

    if ((n==1) || (n==0))
    {
        return 1;
    }

    return fatorial(n-1)*n;

}


double piso(double valor){

    if (valor >= 0)
    {
        return (int)valor;
    }
    else
    {
         return (int)(valor + ((int)(-valor) + 1)) - ((int)(-valor)+1);
    }
    
    return valor;
}
