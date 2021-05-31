#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Define Struct da Arvore 
typedef struct nodoArvore{
    char valor;
    char codigoMorse[10];
    struct nodoArvore *esquerda; 
    struct nodoArvore *direita;
}nodoArvore;

nodoArvore *constroiArvore(nodoArvore *root, char Lcode[], int tam, char valor, int pos);
void PreOrdem(nodoArvore *root);
void EmOrdem(nodoArvore *root);
void PosOrdem(nodoArvore *root);
void Traduz(nodoArvore **root,char morseCode[],int pos,int tam);


//percorre toda a arvore alocando nodos até satisfazer todas as linhas da tabela de codigo morse
//ps: * é usado para marcar nodos sem letras alocadas
nodoArvore *constroiArvore(nodoArvore *root, char Lcode[], int tam, char valor, int pos){
    if (root == NULL){
        root = (nodoArvore*) malloc(sizeof(nodoArvore));
        root->esquerda = NULL;
        root->direita = NULL;
        root->valor = '*';
    }

    if (pos < tam){
        if (Lcode[pos] == '.'){
            root->esquerda = constroiArvore(root->esquerda, Lcode, tam, valor, pos+1);
        }else{
            root->direita = constroiArvore(root->direita, Lcode, tam, valor, pos+1);
        } 
    }else{
        if (root->valor == '*'){
            root->valor = valor;
            Lcode[pos] = '\0';
            strcpy(root->codigoMorse,Lcode);
        }else{
            puts("ERROR - CANT LOAD TREE FILE");
            printf("%c\n",valor);
        }   
    }
    return root;
}

// caminhamentos pela arvore
void PreOrdem(nodoArvore *root){
    if (root != NULL){
        if(root->valor != '*')
            printf("%s %c\n", root->codigoMorse,root->valor);
    
        PreOrdem(root->esquerda);
        PreOrdem(root->direita);
    }
}

void EmOrdem(nodoArvore *root){
    if (root != NULL){
        EmOrdem(root->esquerda);
        if(root->valor != '*')
            printf("%s %c\n", root->codigoMorse,root->valor);
        EmOrdem(root->direita);
    }
}

void PosOrdem(nodoArvore *root){
    if (root != NULL){
        PosOrdem(root->esquerda);
        PosOrdem(root->direita);
        if(root->valor != '*')
            printf("%s %c\n", root->codigoMorse,root->valor);
    }
}


//percorre a arvore até o nodo da letra que vai ser traduzida
void Traduz(nodoArvore **root, char morseCode[], int pos, int tam){
    nodoArvore *node;
    node = *root;
    if (pos < tam){
        if (morseCode[pos] == '.'){
            Traduz(&(node)->esquerda,morseCode,pos+1,tam);
        } else if(morseCode[pos] == '-'){
            Traduz(&(node)->direita,morseCode,pos+1,tam);
        }

    }else{
        if (node->valor != '*')
            printf("%c", node->valor);
    }
}

int main(){

    char Lcode[10],symbol,letter;
    int count;
    nodoArvore *root;
    
    //alocando raiz vazia
    root = (nodoArvore*) malloc(sizeof (nodoArvore) );
    root->esquerda = NULL;
    root->direita = NULL;
    root->valor = '*';
    FILE *arqv;
    arqv = fopen("LoadMorseTab.txt", "r");
    if (arqv == NULL)
        printf("ERROR - CANT OPEN FILE.\n");
    
    count = 0 ;
    while (fscanf(arqv, "%c", &symbol) != EOF){
        if (symbol == ' '){
            fscanf(arqv, "%c", &letter);
            root = constroiArvore(root, Lcode, count, letter,0);
            count = 0;
        }else if (symbol != '\n'){
            Lcode[count++] = symbol;
        }
    }
    fclose(arqv);

    int op, i;
    char morseCode[10], cod;
    do{
        puts("\nEscolha uma opcao: \n1- Caminhamento PreOrdem\n2- Caminhamento EmOrdem\n3- Caminhamento PosOrdem\n4- Transcricao\n0- Sair");
        scanf("%d", &op);

        switch (op){
        case 1:
            printf("\n");
            PreOrdem(root);
            break;
        case 2:
            printf("\n");
            EmOrdem(root);
            break;
        case 3:
            printf("\n");
            PosOrdem(root);
            break;
        case 4:
            printf("\n");
            count = 0;
            arqv= fopen("InputMorseCode.txt", "r");
            if (arqv == NULL){
                printf("ERROR - CAN´T OPEN FILE.\n");
            }
            while (fscanf(arqv, "%c",&cod) != EOF){
                if ((cod != ' ') && (cod != '/') && (cod != '\n')){
                    morseCode[count++] = cod;
                }else if (cod == '/'){
                    printf(" ");
                }else if ((cod == ' ') || (cod == '\n')){
                    Traduz(&root,morseCode,0,count);
                    count = 0;
                }
                if (cod == '\n'){
                    puts(" ");
                }
            }
            printf("\n");
            break;
        default:
            break;
        }
    }while (op != 0);

    return 0;
}