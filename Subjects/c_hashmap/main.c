/*
 * A unit test and example of how to use the simple C hashmap
 */


#include <assert.h>

#include "hashmap.h"

#define KEY_MAX_LENGTH (256)
#define KEY_PREFIX ("key")
#define KEY_COUNT (1024*256)

typedef struct data_struct_s
{
    char key_string[KEY_MAX_LENGTH];
    int number;
} data_struct_t;

void printMessage(char* str){
	printf("%s\n", str);
}

void destructHashmap(map_t);

map_t constructHashmap(){
	int index;
	int error;
	map_t mymap;
    	char key_string[KEY_MAX_LENGTH];
    	data_struct_t* value;

	mymap = hashmap_new();

    /* First, populate the hash map with ascending values */
    for (index=0; index<KEY_COUNT; index+=1)
    {
        /* Store the key string along side the numerical value so we can free it later */
        value = malloc(sizeof(data_struct_t));
        snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
        value->number = index;

        error = hashmap_put(mymap, value->key_string, value);
        if(error!=MAP_OK){
		destructHashmap(mymap);
		return NULL;
	}
    }

	return mymap;
}

void destructHashmap(map_t mymap){
    int error, index;
    char key_string[KEY_MAX_LENGTH];
    data_struct_t* value;

    /* Free all of the values we allocated and remove them from the map */
    for (index=0; index<KEY_COUNT; index+=1)
    {
        snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);

        error = hashmap_get(mymap, key_string, (void**)(&value));
        if(error==MAP_OK){
        	error = hashmap_remove(mymap, key_string);
        	assert(error==MAP_OK);
        	free(value);
	}        
    }
    
    /* Now, destroy the map */
    hashmap_free(mymap);
}

int getData(char* key, int* index){
    int error;
    map_t mymap;
    data_struct_t* value;
    
    mymap = constructHashmap();
    if(mymap==NULL) return -2;

    *index = hashmap_hash(mymap, key);
    error = hashmap_get(mymap, key, (void**)(&value));

    if (error==MAP_MISSING){
	return -1;
    }
    
    int result=value->number;

    destructHashmap(mymap);

    return result;
}

int main(int argc, char** argv)
{
	if(argc<2){
		printMessage("Invoke the program followed a key string.");
		return 0;
	}

	int* index=malloc(sizeof(int));
	*index=0;
	int data=getData(argv[1], index);
	char *message=(char*)malloc(64*sizeof(char));
	if(data==-2) message="Hashmap construction failed.";
	else if(data==-1) snprintf(message, 64, "Hashmap[%d]:\tData not match", *index);
	else snprintf(message, 64, "Hashmap[%d]:\t%d", *index, data);
	printMessage(message);
	return 0;
}
