
#define _GNU_SOURCE
#define __USE_GNU
#include <dlfcn.h>
#include <stdint.h>
#include <malloc.h>
#include <pthread.h>

static void* (*systemMalloc)( size_t ) = NULL;
static void  (*systemFree)  ( void * ) = NULL;

static char  bootstrapBuffer[ 10 << 19 ];
static char *cursor = bootstrapBuffer;

int pePreloaded = 1;

pthread_key_t mallocCountKey;
pthread_key_t mallocSizeKey;
pthread_key_t freeCountKey;

void initPePreload()
{
    pthread_key_create( &mallocCountKey, 0 );
    pthread_key_create( &mallocSizeKey , 0 );
    pthread_key_create( &freeCountKey  , 0 );
    
    pthread_setspecific( mallocCountKey, 0 );
    pthread_setspecific( mallocSizeKey , 0 );
    pthread_setspecific( freeCountKey  , 0 );

    systemMalloc = ( void* (*)( size_t ) ) dlsym( RTLD_NEXT, "malloc" );
    systemFree   = ( void (*)(void *) )    dlsym( RTLD_NEXT, "free" );
}

void* malloc( size_t sz )
{
    if( systemFree == NULL )
    {
        char * tmp = cursor;
        cursor += sz + ( 8 - ( sz % 8 ) );
        return tmp;
    }

    pthread_setspecific( mallocCountKey, (void *)( (uint64_t)pthread_getspecific( mallocCountKey ) + 1 ) );
    pthread_setspecific( mallocSizeKey , (void *)( (uint64_t)pthread_getspecific( mallocSizeKey ) + sz ) );

    if( sz == 0 )
    {
        return NULL;
    }
    return (*systemMalloc)( sz );
}

void free( void *ptr )
{
    if( systemFree == NULL ||  ptr - (void*)bootstrapBuffer < sizeof(bootstrapBuffer) )
    {
        return;
    }

    pthread_setspecific( freeCountKey, (void *)( (uint64_t)pthread_getspecific( freeCountKey ) + 1 ) );
    if( ptr == NULL )
    {
        return;
    }

    (*systemFree)( ptr );
}
