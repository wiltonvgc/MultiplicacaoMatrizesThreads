#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Wilton Vicente Goncalves da Cruz - RA:586889 - SO2 - UFSCar - 2016/2 */
/* Objetivo: Paralelizar multiplicacao de matrizes A e B com threads */
/* C = A * B */

#define M1 10 // linhas matriz 1
#define N1 10 //colunas matriz 1 
#define M2 10 //linhas matriz 2
#define N2 10 //colunas matriz 2
#define NUM_THREADS 5
#define LEN 128

int C[10][10];//matriz resultado C = A * B


/* Definicao de struct de parametros da funcao multiplicaMatriz */
typedef struct par{
	int *m1;
	int *m2;
	int l;
	int m;

}parametro;

/* Funcao que recebe duas matrizes, linhas que devem ser multiplicadas, limitadas por l e l, pela segunda matriz */
void multiplicaMatriz(parametro* p){
		int l = p->l;
		int m = p->m;
		int* m1 = p->m1;
                int* m2 = p->m2;		

		int i,j,k; //var para loop
		int soma=0; //guardara soma de mult de linha por coluna
		
		for(i=l;i<=m;i++){
			for(j=0;j<N2;j++){
				
				for(k=0;k<N1;k++){
				   soma = soma + (m1[i*N1+k] * m2[k*N2+j]);
				   
				}//fim for
			
				C[i][j] = soma;
				soma = 0;

			}//fim for
		
		}//fim for
}//fim multiplicaMatriz

/* Funcao principal: obtem matriz A e B,  cria threads para multiplicacao e escreve matriz C=A*B em arquivo */
void main(){
	
	int A[M1][N1], B[M2][N2];//declaracao de matrizes A e B com dimensaoes pre definidas
	
	char* path1 = "m1.txt"; //endereco arquivo matriz A
	char* path2 = "m2.txt"; //endereco arquivo de matriz B
	
	int fdA = fopen(path1,"r"); //abre arquivo de matriz A apenas para READ
	int fdB = fopen(path2,"r"); //abre arquivo de matriz B apenas para READ

	
	if(fdA==NULL || fdB==NULL){
		perror("Erro na abertura do arquivo de matriz A!");
	}

	
	/* Le elementos dos arquivos e armazena em matrizes A e B */
	for(int i = 0; i<M1; i++){
		for(int j=0; j<N1; j++){
			fscanf(fdA,"%d",&A[i][j]);
			fscanf(fdB,"%d",&B[i][j]);
		}
		
	}
	


	/* Criacao de threads para multiplicacao de A * B */
	pthread_t threads[NUM_THREADS];
	int t, status;
	char err_msg[LEN];
	
	/* seta parametros para funcao multiplicaMatriz */
	
	

	int d = M1/NUM_THREADS;//Numero de linhas de A dividido por NUM_THREADS para calculo das linhas de multiplicacao por thread
	parametro pars[NUM_THREADS];//cria um struct parametro para cada thread

	
	/* Criacao de threads para execucao de multiplicaMatriz() */
	for (t=0; t<NUM_THREADS; t++) {
		
		pars[t].m1 = A;
		pars[t].m2 = B;
		pars[t].l = d*t;
		pars[t].m = d*t + 1;
		

		status = pthread_create(&threads[t], NULL, multiplicaMatriz,(void*)&pars[t]);

		if (status) {
			strerror_r(status,err_msg,LEN);
			printf("Falha da criacao da thread %d (%d): %s\n",t,status,err_msg);
			exit(EXIT_FAILURE);
		} else {
			//
		}
	}

	
	/* Grava matriz resultado C = A * B em um arquivo resultado.txt */
	
	int fdC = fopen("resultado.txt","w");
	if(fdC==NULL){
		perror("Erro na abertura do arquivo de matriz C!");
	}

	char pulaLinha = '\n';
	for(int i = 0; i<10; i++)
		for(int j = 0; j<10; j++){
			fprintf(fdC,"%d   ",C[i][j]);
			if(j==9)
			   fprintf(fdC,"%c",pulaLinha);
		}
	
	fclose(fdA);
	fclose(fdB);
	fclose(fdC);

        pthread_exit(NULL);
	//return 0;
}









