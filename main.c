#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define HEAP_CAP 64000
#define HEAP_ALLOCED_CAP 1024
#define CHUNK_LIST_CAP 1024

typedef struct{
	char *start;
	size_t size;
} Chunk;

typedef struct{
	Chunk chunks[HEAP_ALLOCED_CAP];
	size_t count;
} Chunk_List;

char heap[HEAP_CAP] = {0};

Chunk_List alloced_chunks = {0};
Chunk_List freed_chunks = {
	.count = 1,
	.chunks ={[0]={.start = heap, .size = sizeof(heap)}},
};
Chunk_List tmp_chunks = {0};

int chunk_list_find(const Chunk_List *list,void *ptr);
void chunk_list_remove(Chunk_List *list, size_t index);

void chunk_list_merge(Chunk_List *tmp, Chunk_List *list){
void chunk_list_insert(Chunk_List *list, void *start, size_t size);
	tmp->count =0;
	
	for(size_t i=0;i<list->count;++i){
		const Chunk chunk = list->chunks[i];
		if(tmp->count>0){
			Chunk *top_chunk = &tmp->chunks[tmp->count-1].start;

			if(top_chunk->start + top_chunk->size == chunk.start){
				top_chunk->size+=chunk.size;
			}else{
				chunk_list_insert(tmp, chunk.start,chunk.size);

			}
		} else {
			chunk_list_insert(tmp, chunk.start,chunk.size);
		}
	}
}

int chunk_start_compar(const void* a,const void* b){
	const Chunk *a_chunk = a;
	const Chunk *b_chunk = b;
	return a_chunk->start - b_chunk->start;
}


int chunk_list_find(const Chunk_List *list,void *ptr){
	for(size_t i = 0;i<list->count;++i){
		if(list->chunks[i].start == ptr){
			return (int)i;
		}

	}
	return -1;
}
void chunk_list_remove(Chunk_List *list, size_t index){
	assert(index<list->count);
	for(size_t i = index;i<list->count- 1;++i ){
		list->chunks[i] = list->chunks[i+1];

	}
	list->count-=1;

}

void chunk_list_insert(Chunk_List *list, void *start, size_t size){
	assert(list->count < CHUNK_LIST_CAP);
	list->chunks[list->count].start = start;
	list->chunks[list->count].size = size;
	
	for(size_t i  = list->count;i>0 && list->chunks[i].start < list->chunks[i-1].start;--i){
		const Chunk t= list->chunks[i];
		list->chunks[i]  = list->chunks[i-1];
		list->chunks[i-1] = t;
	}
	list->count+=1;
}

void* heap_alloc(size_t size) {	
	if(size>0){
	chunk_list_merge(&tmp_chunks,&freed_chunks);
		freed_chunks = tmp_chunks;
		for(size_t i=0;i<freed_chunks.count;++i){
			const Chunk chunk  = freed_chunks.chunks[i];
		if(chunk.size >=size){
			chunk_list_remove(&freed_chunks,i);
			size_t tail_size = chunk.size - size;
			chunk_list_insert(&alloced_chunks,chunk.start, size);
			if(tail_size>0){
				chunk_list_insert(&freed_chunks,chunk.start+size,tail_size);
			
			}
			return chunk.start;
		}
	}
	}
	return NULL;
}

void chunk_list_dump(const Chunk_List* list){
	printf("chunks:\n");
	for(size_t i =0;i<list->count;++i){
		printf("start: %p, size: %zu\n"
		,list->chunks[i].start
		,list->chunks[i].size);
	}
}

void heap_free(void *ptr){
	if(ptr!=NULL){
	const int index = chunk_list_find(&alloced_chunks, ptr);
	assert(index>=0);
	chunk_list_insert(&freed_chunks,alloced_chunks.chunks[index].start,alloced_chunks.chunks[index].size);
	chunk_list_remove(&alloced_chunks, (size_t)index);
	}
}

void heap_collect(){ 


}

int main(){

	for(int i = 0;i<10;++i){
		void *p = heap_alloc(i);
			heap_free(p);
	}
	heap_alloc(10);

	chunk_list_dump(&alloced_chunks);
	chunk_list_dump(&freed_chunks);
	return 0;
}
