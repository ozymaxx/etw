/*
 * Ozan Can Altiok
 * 0052709
 * COMP 508 - Computer Vision & Pattern Recognition
 * Project - Data Collection
 * trecord.c - Data Collection Functions Implementation (Bag of Frames)
 * */

#include "eat.h"
#include <assert.h>

// a function freeing the records
void freeRecords( char* fileName, struct trecord*** records, int* length) {
	// open the pipe to the output file
	FILE *output = fopen( fileName, "a+");
	
	assert(output != NULL);
	
	for ( int i = 0; i < *length; i++) {
		// write the remaining frames to the output file
		fprintf( output, "%d,%d,%d,%d,%llu,%d,%d\n", (*records)[i]->x, (*records)[i]->y, (*records)[i]->teamName, (*records)[i]->playerID, (*records)[i]->timestamp, (*records)[i]->minute, (*records)[i]->specialCase);
		// free the pointer to a record in the list
		free( (*records)[i] );
	}
	
	// close the output to the file
	fclose(output);
	
	// free the whole records list
	//printf("Freeing...\n");
	free(*records);
}

// a function allocating memory for records list
void allocRecords( struct trecord*** records, int* length) {
	*records = (struct trecord**) malloc( LENGTH * sizeof( struct trecord*) );
	*length = 0;
}

// a function adding the frame record to the records list
void addRecord( int x, int y, int tm, int id, unsigned long minute, unsigned long long timestamp, int specialCase, struct trecord*** records, int* index, char* fileName) {
	// if the list is full
	if ( (*index) >= LENGTH ) {
		//printf("Freeing..\n");
		// free the list
		freeRecords( fileName, records, index);
		// reallocate the list
		allocRecords( records, index);
	}
	
	// add a frame to the list
	(*records)[*index] = (struct trecord *) malloc( sizeof( struct trecord) );
	(*records)[*index]->x = x;
	(*records)[*index]->y = y;
	(*records)[*index]->playerID = id;
	(*records)[*index]->teamName = tm;
	(*records)[*index]->minute = minute;
	(*records)[*index]->timestamp = timestamp;
	(*records)[*index]->specialCase = specialCase;
	
	// increment the frame counter
	(*index)++;
}

// a function initializing the records bag, including the frame list
void initBag( trec** rb, char* fileName) {
	*rb = (trec*) malloc( sizeof(trec) );
	allocRecords( &((*rb)->records), &((*rb)->index));
	(*rb)->fileName = (char*) malloc( strlen(fileName) * sizeof(char) );
	strcpy( (*rb)->fileName, fileName);
}

// a function adding the frame contents to the bag
void addToBag( int x, int y, int tm, int id, unsigned long minute, unsigned long long ts, int specialCase, trec** rb) {
	addRecord( x, y, tm, id, minute, ts, specialCase, &((*rb)->records), &((*rb)->index), (*rb)->fileName);
}

// a function freeing the list inside the bag
void freeRemainder( trec** rb) {
	freeRecords( (*rb)->fileName, &((*rb)->records), &((*rb)->index) );
}

// a function freeing the bag
void freeBag( trec** rb) {
	//printf("Freeliyoruz...\n");
	free((*rb)->fileName);
	//printf("Freeliyoruz2...\n");
	free(*rb);
}
