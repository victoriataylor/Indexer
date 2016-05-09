/* ========================================================================== */
/* File: hashtable.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains the definitions for a hashtable of void pointers, 
 * Also contains definitions for the nodes that the hashtable will contain.
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"
// ---------------- Constants
#define MAX_HASH_SLOT 10000                  // number of "buckets"

// ---------------- Structures/Types

// typedef struct HashTableNode {
//     void *hashKey;                            
//     struct HashTableNode *next;              // pointer to next node
// } HashTableNode;


typedef struct DocumentNode {
  struct DocumentNode *next;         // pointer to the next member of the list.
  int doc_id;                        // document identifier
  int freq;                          // number of occurrences of the word
} DocumentNode;

typedef struct WordNode {
  struct WordNode *next;            // pointer to the next word (for collisions)
  char *word;                       // the word
  //int numDocs;
  struct DocumentNode *page;               // pointer to the first element of the page list.
} WordNode;

typedef struct HashTable {
    WordNode *table[MAX_HASH_SLOT];     // actual hashtable
} HashTable;

// ---------------- Prototypes/Macros
unsigned long JenkinsHash(const char *str, unsigned long mod);
int addWord(char *w, int docId, HashTable *Index, int remakeFlag, int freq);
int AddToHashTable(char *w, int docId, HashTable *Index, int freqflag, int freq);
int UpdateHashTable(char *w, int docId, HashTable *Index);
int InHashTable(char *w, HashTable *Index);
int cleanIndex(HashTable *Index);
#endif // HASHTABLE_H