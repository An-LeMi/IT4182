#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "BinaryTree.c"

#define STOP 17
#define MAX 100000


void printResult(TreeType tree, FILE *fr){
	if(tree != NULL){
    	printResult(tree->left, fr);
    	fprintf(fr, "%-25s: frequency: %d, line:", tree->key.ma, tree->key.frequency);
    	for (node *p = tree->key.line; p != NULL; p = p->next){
    		fprintf(fr, "%d,", p->element);
  		}
    	fprintf(fr, "\n");
    	printResult(tree->right, fr);
  	}
}


int checkStop(char stop[STOP][81], char *word){
	int checkStop = 0;
	for(int i = 0; i < STOP; i++){
		if (strcmp(stop[i], word) == 0){
			checkStop = 1;
			break;
			
		}
	}
	return checkStop;
}


int main() {
	FILE *fread, *fstop;
	// fread = fopen("completed/vanban.txt", "r");
	fread = fopen("completed/alice30.txt", "r");
	fstop = fopen("completed/stopw.txt", "r");

	if (fread == NULL || fstop == NULL) {
		printf("Cannot open file\n");
		return 1;
	}

	// Init Binary Tree
	TreeType root = NULL;
	TreeType tmp;


	// Read stopw
	char stop[STOP][81];
	for (int i = 0; i < STOP; i++) {
		fscanf(fstop,"%s\n", stop[i]);
		// printf("%s\n", stop[i]);
	}


	// Read text
	char a;
	char word[MAX][81];
	char cache[81];
	int numCache = 0, numWord = 0, numLine = 1, checkDot = 0;
	do {
		a = fgetc(fread);
		if (feof(fread))
			break;

		if (a == '\n')
			numLine++;

		if (a == '.')
			checkDot = 1;


		if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z')){
			cache[numCache] = a;
			numCache++;
		}
		else {
			cache[numCache] = '\0';

			if((cache[0] >= 'A' && cache[0] <= 'Z') && checkDot == 1){
				cache[0] = tolower(cache[0]);
				checkDot = 0;
			}

			if((cache[0] >= 'A' && cache[0] <= 'Z') && checkDot == 0)
			 	cache[0] = '\0';

			if(cache[0] != '\0' && checkStop(stop,cache) == 0){
				for(int i = 0; i < numCache; i++) {
					cache[i] = tolower(cache[i]);
				}
				strcpy(word[numWord],cache);
				if (Search(word[numWord], root) == NULL){
					KeyType new;
					strcpy(new.ma,word[numWord]);
					new.line = makenewNode(numLine);
					new.frequency = 1;
					insertNode(new, &root);
				}
				else {
					tmp = Search(word[numWord], root);
					insertTail(numLine,tmp->key.line);
					tmp->key.frequency++;
				}
				numWord++;
			}
			strcpy(cache,"");
			numCache = 0;
		}
	}  while(1);


	FILE *fResult = fopen("Result.txt","w");
	if (fResult == NULL){
		printf("Cannot open file\n");
		return 2;
	}
	printResult(root, fResult);


	fclose(fread);
	fclose(fstop);
	freetree(root);
	return 0;
}