/* ========================================================================== */
/* File: indexer.c - Tiny Search Engine web crawler
 *
 * Author: Victoria M. Taylor
 * Date: August 9, 2015
 *
 * Input:
 *
 * Command line options:
 *
 * Output:
 *
 * Error Conditions:
 *
 * Special Considerations:
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf                     
#include <string.h>                          //to use strlen()
#include <stdlib.h>
#include <limits.h>
#include "file.h"
#include "web.h"
#include "hashtable.h"

// ---------------- Local includes  e.g., "file.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
char *LoadDocument (char *fileName);
int GetDocumentId (char *fileName);
int SaveIndexToFile(HashTable *Index, char *indexfile);
HashTable *FileToIndex(char *indexfile);
void inplace_rev( char * s );

/* ========================================================================== */

int main(int argc, char* argv[])
{	

	//checking arguments
	if (argc < 3 || argc > 4) {
		printf("Incorrect number of args\n");
		exit(1);
	}
	if (!IsDir(argv[1])){
		printf("First argument must be valid directory\n");
		exit(1);
	}

	// initialize variables
	char **filenames = NULL;
	char *indexfile = argv[2];

	// Create index and initialize each slot to NULL
	struct HashTable *Index = malloc(sizeof(HashTable));
	for (int i = 0; i < MAX_HASH_SLOT; i++){
		Index->table[i] = NULL;
	}

	// get files from directory
	int numFiles = GetFilenamesInDir(argv[1], &filenames);
	
	//If there are no files, inform user and quit
	if (numFiles <0){
		printf("Failed to get files\n");
		exit(1);
	}
	//Otherwise iterate through all files in directory and add their words to the Index
	else {
		printf("Building Index.\n");
		for (int i = 0; i < numFiles; i++) {
			// Get filepath

			char *filepath = malloc(strlen(filenames[i]) + strlen(argv[1]) + 3);
			sprintf(filepath, "%s/%s", argv[1], filenames[i]);

			//Get DocId, reset position, and load document into doc
			int docId = GetDocumentId(filenames[i]);
			char *doc = LoadDocument(filepath);
			int pos = 0;
			char *word;

			//Loop through all words and add to Index
			while ((pos = GetNextWord(doc, pos, &word)) > 0) {
				NormalizeWord(word);
			 	addWord(word, docId, Index, 0, 0);

			}
			//Free filepath, filenames, and loaded string file
			free(filenames[i]);
			filenames[i] = NULL;
			free(filepath);
			filepath = NULL;
			free(doc);
			doc = NULL;
		}
		free(filenames);
		SaveIndexToFile(Index, indexfile);
		printf("Done!\n");
		cleanIndex(Index);

		// If testing
		if (argc == 4){ 
			printf("Testing Index\n");
			struct HashTable *Index = FileToIndex(indexfile);
			SaveIndexToFile(Index, argv[3]);
			printf("Test Complete\n");
			cleanIndex(Index);
		}
	}
	return 1;
}

/* Takes a filename, opens that file, skips the first two lines, and
 * loads the rest of the file into a char string, which is returns.
 */
char *LoadDocument (char *filename){
	char *filestring;
	FILE *fp = fopen(filename, "r");
	if (fp != NULL) {

		//find size of file
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, SEEK_SET, 0);

		// //Set pointer at third line
		char line[10000];
		fgets(line, sizeof(line), fp);
		fgets(line, sizeof(line), fp);
		fseek(fp, 0, SEEK_CUR);

		// //load pointer into filestring and return it
		filestring = calloc(size, sizeof(char));
		fread(filestring, 1, size, fp);
		fclose(fp);
		return filestring;
	}
	else {
		return 0;
	}
}

/*Takes a filename (a character string of an int) and returns 
 * the int equivalent. 
 */
int GetDocumentId (char *fileName){
	int docId = atoi(fileName);
	return docId;
}

/* Takes in an Index. Writes a line to the indexfile for each word found in the Index. 
 * Format = word docNum docId docFreq docId docFreq etc
 */
int SaveIndexToFile(HashTable *Index, char *indexfile)
{
	FILE *indexfp = fopen(indexfile, "w");

	for (int x = 0; x < MAX_HASH_SLOT; x++){
		WordNode *word = Index->table[x];

		while (word != NULL){

			DocumentNode *doc = word->page;
			int docnum = 0;
			while (doc != NULL){
				docnum++;
				doc = doc->next;
			}

			fprintf(indexfp, "%s %d ", word->word, docnum);
			doc = word->page;
			while (doc != NULL){
				fprintf(indexfp, "%d %d ", doc->doc_id, doc->freq);
				doc = doc->next;
			}
			fprintf(indexfp, "\n");
			word = word->next;
		}
	}

	fclose(indexfp);
	return 1;
}

/* Takes the name of an indexfile and reconstructs an Index from it.
 * Returns the Index.
 */
HashTable *FileToIndex(char *indexfile) 
{
	printf("Creating Index");
	// Construct Index and initialize everything to NULL
	struct HashTable *Index = malloc(sizeof(HashTable));
	for (int i = 0; i < MAX_HASH_SLOT; i++){
		Index->table[i] = NULL;
	}
	char *token, *wordInfo, *word;
	int docnum, docId, docfreq, offset;
	FILE *fp = fopen(indexfile, "r");
	char *line = calloc(10000, sizeof(char));

	while (fgets(line, 10000, fp) != NULL){
		token = strtok(line, " ");
		word = calloc(75, sizeof(char));
		strcpy(word, token);
		//line = strtok(NULL, "\n");
		wordInfo = strtok(NULL, "\n");
		
		//sscanf(wordInfo, "%d%n", &docnum, &offset);
		sscanf(wordInfo, "%d%n", &docnum, &offset);
		wordInfo += offset;	
		
		inplace_rev(wordInfo); //reverse the string so that indexfiles will be written in same order
		while (sscanf(wordInfo, "%d%d%n", &docfreq, &docId, &offset) == 2) {
			addWord(word, docId, Index, 1, docfreq);
			wordInfo += offset;
		}
	}
	free(line);
	fclose(fp);
	return Index;
}

/* Function to reverse a string in place. 
 * Takes pointer to a character string.
 */
void inplace_rev( char * s ) {
  char t, *e = s + strlen(s);
  while ( --e > s ) { t = *s;*s++=*e;*e=t; }
}
