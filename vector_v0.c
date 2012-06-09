#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define vec_data_ptr ( (**vec).data + ( (*(**vec).block_count-1) * *(**vec).block_sz ) + *(**vec).count_within_block- 1 )
#define vec_data_ptr_generic(x) ( (**x).data + ( (*(**x).block_count-1) * *(**x).block_sz ) + *(**x).count_within_block- 1 )

//helper for vector_push_back
//#define next_valid_vector_pos(x) ( (**x).data + (*(**x).count_within_block- 1) * *(**x).type_sz ) 

#define BYTE char

struct vector_mem_block
{
  void * data;
  struct vector_mem_block * next;
};

struct vector
{
  size_t * type_sz;
  size_t * block_sz;
  char * count_within_block;

  struct vector_mem_block * head;
  struct vector_mem_block * last_block;
};

typedef struct vector vector;
typedef struct vector_mem_block vector_mem_block;

void vector_allocate_block(vector ** vec);

vector * vector_managed_init(size_t block_sz_param, size_t type_sz_param)
{
  vector * ret = NULL;

  ret = malloc(sizeof(vector));
  ret->block_sz = malloc(sizeof(size_t));
  ret->type_sz = malloc(sizeof(size_t));
  ret->count_within_block= (char *) malloc(sizeof(char));

  *ret->type_sz = type_sz_param;
  *ret->block_sz = block_sz_param;
  *ret->count_within_block= 0;
  ret->head = NULL;
  ret->last_block = ret->head;

  //allocate first block
  vector_allocate_block(&ret); 

  return ret;
}

/*void vector_managed_destroy(vector ** vec)
{
  free((**vec).data);
  free((**vec).type_sz);
  free((**vec).block_sz);
  free((**vec).count_within_block;
  free(vec);
}*/

void vector_managed_destroy_everything(vector ** vec)
{
  //gets rid of everything vector elements
  //point to if they are pointers
}

void vector_allocate_block(vector ** vec)
{
  vector_mem_block * temp = NULL, * iter = NULL;

  ////prepare the vector_mem_block first
  temp = (vector_mem_block *) malloc(sizeof(vector_mem_block));
  temp->data = malloc( *(**vec).type_sz * *(**vec).block_sz );
  temp->next = NULL; //critical

  ////plug the vector_mem_block into vec
  
  if((**vec).last_block == NULL)
  {
    //then we're dealing with the first block
    (**vec).head = temp;
  }
  else
  {
    (**vec).last_block->next = temp;
  }

  //update the vector's utility last_block pointer
  (**vec).last_block = temp;

  //set the per-block incrementer to 0
  *(**vec).count_within_block= 0;
}

void * vector_get_next_valid_position(vector * vec)
{
  return ((BYTE *)vec->last_block->data + *vec->type_sz * *vec->count_within_block);
}


void vector_push_back(vector ** vec, void * data)
{
  //assuming data is of the same type as
  //whatever vec was instantiated as in vector_init()
  //warning: no type checking! don't be stupid!

  
  if(*(**vec).count_within_block >=/*==*/ *(**vec).block_sz)
  //then we have need to allocate a new block
  //and of course set vec.count_within_blockto 0
  {
    vector_allocate_block(vec);
  }

  //#define vec_data_ptr ( (**vec).data + ( (*(**vec).block_count-1) * *(**vec).block_sz ) + *(**vec).count_within_block- 1 );
  //bypass this if it's of pointer type
  memcpy(vector_get_next_valid_position(*vec), data, *(**vec).type_sz);
  //*( ((BYTE *)(**vec).data) + *(**vec).count_within_block4 ) = data;
  
  *(**vec).count_within_block += 1;
}

void vector_for_each
(
  vector * vec,
  void (*your_manip)(BYTE * /*element_ptr*/, size_t /*block_size*/, size_t /*element_size*/)
)
{
  BYTE * begin = NULL, * end = NULL;
  vector_mem_block * next = NULL;
  int i = 0;
  
  next = vec->head;

  for(;
    next != NULL;
    next = next->next, ++i)
  //printf("%d\n", i);
  {
    for(i = 0; i < *vec->block_sz; ++i)
    {
      //printf("%d\n", *( ((int *) next->data) + i ));
      (*your_manip)((BYTE *) next->data, *vec->block_sz, *vec->type_sz);
    }
  }
}

void print(BYTE * data, size_t block_sz, size_t type_sz)
{
  int i;

  printf( "%d\n", *( ((int *) data) + 0 ) );
}

int main()
{
  vector * my_vec = NULL;
  int hurr = 984, durr=438, derp = 9799, i = 0;

  my_vec = vector_managed_init(4, sizeof(int));
  vector_push_back(&my_vec, &hurr);
  vector_push_back(&my_vec, &durr);
  vector_push_back(&my_vec, &derp);
  vector_push_back(&my_vec, &derp);
  vector_push_back(&my_vec, &derp);
  vector_push_back(&my_vec, &derp);
  vector_push_back(&my_vec, &derp);
  vector_push_back(&my_vec, &derp);

  //printf("%d\n", *( (int *) (*my_vec).data + ( *my_vec->block_count - 1 ) * *my_vec->block_sz + *my_vec->count_within_block- 1 ) );
  //printf("%d\n", *( (int *) (*my_vec).data + 1 ) );
  //printf("%d\n", *( (int *)  ( (**x).data + ( (*(**x).block_count-1) * *(**x).block_sz ) + *(**x).count_within_block- 1 ) );

  printf("%d\n\n", *( (int *) my_vec->last_block->data ) );
  vector_for_each(my_vec, &print);
  

  //vector_managed_destroy(&my_vec);

  return 0;
}
