/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>

int verifica_primo(unsigned long int primo);
void verifica_filho(unsigned long int primo, short index, int *buffer_compartilhado);

int main() {
	unsigned int i = 0, n = 0, j = 0;
  	char str[100], *s = str, *t = NULL;
	unsigned long int vet[100];
	int contador = 0;
	pid_t filho[64];
	int protection = PROT_READ | PROT_WRITE;
     int visibility = MAP_SHARED | MAP_ANON;
 
  	int  *buffer;	
	
	fgets(s, 100, stdin);

	// Remove \n da string
	while(str[i] != '\n') i++;
		str[i] = '\0';

	// Tokeniza e insere em um vetor de unsigned long int.
	while ((t = strtok(s, " ")) != NULL) {
		s = NULL;		
		vet[n++] = atoi(t);
	}

	/* Criacao de memoria compartilhada */
	buffer = (int *) mmap(NULL, sizeof(int)*n, protection, visibility, 0, 0);

	for(i=0; i < n; i++){
		buffer[i] = 0; 
	}

	 i = 0;
     j = 0;

    // Verifica a quantidade de numeros a ser processada e chama processos 
    //filhos conforme a quantidade verificada
    while((j < 4) && (i < n)){
        filho[j] = fork();
        if (filho[j] == 0){
            verifica_filho(vet[i], i, buffer);
        }
        else
            i++;
        j++;
    }    

    // Caso haja mais que 4 numeros, novos processos serao chamados 
    while(i < n){
        int status;
        wait(&status);
        filho[i] = fork();
        if (filho[i] == 0){
            verifica_filho(vet[i], i, buffer);
        }
        i++;
    }    

    // Cria tempo de espera para finalizacao dos processos 
    for(i = 0; i < 4; i++){
        int status;
        wait(&status);
    }
    for(i = 0; i < n; i++){
        contador +=  buffer[i];
    }
    printf("%d\n",contador);		

	return 0;

}

int verifica_primo(unsigned long int primo){
	unsigned long int i, div = 0; 
	for (i = 1; i <= primo; i++) {
    if (primo % i == 0)   
    	div++;
    if(primo % 2 == 0 && primo != 2)
    	return 0;
    if(div > 2)
    	return 0;
  }
    
  if (div == 2)
    return 1;
  else
    return 0;
  }
 void verifica_filho(unsigned long int primo, short index, int *buffer_compartilhado){
 	int flag;
 	flag = verifica_primo(primo);

 	if(flag == 1)
 		buffer_compartilhado[index]++;
 	exit(0);
 }
