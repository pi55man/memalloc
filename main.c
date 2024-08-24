#include <stdio.h>
#include <assert.h>
#define HEAP_CAP 64000


char heap[HEAP_CAP];
size_t heap_size = 0;
void* heap_alloc(size_t size)
{	
	assert(heap_size+size<=HEAP_CAP);
	void* result = heap + heap_size;
	heap_size+=size;
	return result;
}




int main(){

	return 0; 
}
