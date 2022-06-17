// 520021910607 Tanziming

#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

int hits = 0;
int misses = 0;
int evictions = 0;
int verbose = 0;

typedef struct cache_line
{
    int valid;
    int tag;
    int block;
    unsigned LRUcounter;
} CacheLine;

typedef struct _cache
{
    int S;
    int E;
    int B;
    CacheLine **line;  // line[S][E]
} Cache;

Cache *cache = NULL;
char t[2000];

void initCache(int s, int E, int b)
{
    int S = 1 << s;
    int B = 1 << b;
    cache = (Cache*)malloc(sizeof(Cache));
    cache->S = S;
    cache->B = B;
    cache->E = E;

    int i, j;
    cache->line = (CacheLine**)malloc(sizeof(CacheLine*) * S);
    for (i = 0; i < S; i++){
        cache->line[i] = (CacheLine*)malloc(sizeof(CacheLine) * E);
        for (j = 0; j < E; j++){
            cache->line[i][j].valid = 0;
            cache->line[i][j].block = 0;
            cache->line[i][j].tag = 0;
            cache->line[i][j].LRUcounter = -1;
        }
    }
}

void freeCache()
{
    int S = cache->S;
    for(int i = 0; i < S; ++i){
        free(cache->line[i]);
    }
    free(cache->line);
    free(cache);
}

void loadInCache(int tag, int index)
{
    CacheLine *set = cache->line[index];
    int E = cache->E;
    unsigned maxLRUcounter = 0;
    unsigned leastLRUcounter = -1;
    int leastLRUcounterIndex = -1;
    for(int i = 0; i < E; ++i){
        if(set[i].valid == 0) continue;
        // valid = 1, check LRUcounter
        maxLRUcounter = (set[i].LRUcounter > maxLRUcounter) ? set[i].LRUcounter : maxLRUcounter;
        leastLRUcounter = (set[i].LRUcounter > leastLRUcounter) ? leastLRUcounter : set[i].LRUcounter;
        leastLRUcounterIndex = (set[i].LRUcounter > leastLRUcounter) ? leastLRUcounterIndex : i;
    }

    // check is full or not
    for(int i = 0; i < E; ++i){
        if(set[i].valid == 1) continue;
        // not full, load here directly
        if(DEBUG) printf("\nloadInCache: set %d not full, load in line %d directly, LRUcounter %d", index, i, maxLRUcounter);
        set[i].LRUcounter = maxLRUcounter + 1;
        set[i].tag = tag;
        set[i].valid = 1;
        return;
    }

    // full, evict one which hasn't benn update most
    ++evictions;
    if(verbose){
        printf(" eviction");
    }

    if(DEBUG) printf("\nloadInCache: set %d full, evict line %d whose LRUcounter %d", index, leastLRUcounterIndex, leastLRUcounter);
    
    set[leastLRUcounterIndex].LRUcounter = maxLRUcounter + 1;
    set[leastLRUcounterIndex].tag = tag;
    set[leastLRUcounterIndex].valid = 1;
    
}

void updateLRUcounter(int indexSet, int indexLine)
{
    CacheLine *set = cache->line[indexSet];
    int E = cache->E;
    unsigned maxLRUcounter = 0;
    for(int i = 0; i < E; ++i){
        if(set[i].valid == 0) continue;
        // valid = 1, check LRUcounter
        maxLRUcounter = (set[i].LRUcounter > maxLRUcounter) ? set[i].LRUcounter : maxLRUcounter;
    }
    set[indexLine].LRUcounter = maxLRUcounter + 1;
}

void loadData(int tag, int index)
{
    CacheLine *set = cache->line[index];
    int E = cache->E;
    for(int i = 0; i < E; ++i){
        if(set[i].valid == 0) continue;

        // valid = 1, check tag
        if(set[i].tag == tag){
            // hit
            if(verbose)
                printf(" hit");
            ++hits;
            updateLRUcounter(index, i);
            return;
        }
    }

    // not found in cache, miss
    if(verbose)
        printf(" miss");
    ++misses;
    loadInCache(tag, index);
}


void storeData(int tag, int index)
{
    CacheLine *set = cache->line[index];
    int E = cache->E;
    for(int i = 0; i < E; ++i){
        if(set[i].valid == 0) continue;

        // valid = 1, check tag
        if(set[i].tag == tag){
            // hit
            if(verbose)
                printf(" hit");
            ++hits;
            updateLRUcounter(index, i);
            return;
        }
    }

    // not found in cache, miss
    if(verbose)
        printf(" miss");
    ++misses;
    loadInCache(tag, index);
}


void modifyData(int tag, int index)
{
    loadData(tag, index);
    storeData(tag, index);
}

void parseTraceFile(int s, int E, int b)
{
    FILE * pFile;
    pFile = fopen(t,"r");  // open file for reading

    char identifier;
    unsigned address;
    int size;

    // line look like this
    //   S 7ff000398,8
    //  I 0040051e,1
    while(fscanf(pFile, " %c %x,%d", &identifier, &address, &size) > 0){
        int tag = address >> (s+b);
        int index = (address >> b) & ~(((unsigned)-1) << s);
        if(verbose)
            printf("%c %x,%d", identifier, address, size);
        if(DEBUG) printf("parseTraceFile parsing %c %x,%d , tag %d, index %d\n", identifier,  address, size, tag, index);
        switch (identifier)
        {
        case 'I':
            break;
        case 'M':
            modifyData(tag, index);
            break;
        case 'L':
            loadData(tag, index);
            break;
        case 'S':
            storeData(tag, index);
            break;
        default:
            break;
        }
        if(verbose)
            printf("\n");

    }


    fclose(pFile);
}


int main(int argc, char** argv)
{
    int opt, s, E, b;

    while ((opt = getopt(argc,argv,"vs:E:b:t:")) != -1) {
        switch(opt) {
        case 'v':
            verbose = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            strcpy(t,optarg);
            break;
        default:
            printf("Wrong argument!");
            exit(1);
        }
    }

    if(DEBUG) printf("getopt FINISHED\n");

    // malloc space for cache
    initCache(s,E,b);

    if(DEBUG) printf("initCache FINISHED\n");

    parseTraceFile(s,E,b);

    // finally print summary
    printSummary(hits, misses, evictions);

    freeCache();

    return 0;
}
