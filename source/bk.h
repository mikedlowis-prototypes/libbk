#ifndef BK_H
#define BK_H

#include <stddef.h>

#ifdef BK_DEBUG_TRACE
    #include <bktrace.h>
#else
    /* Macros to log collector startup */
    #define TRACE_INIT_START()
    #define TRACE_INIT_END()

    /* Macros to log collector shutdown */
    #define TRACE_DEINIT_START()
    #define TRACE_DEINIT_END()

    /* Macros to log collection cycle */
    #define TRACE_COLLECT_START()
    #define TRACE_COLLECT_END()

    /* Macros to log an allocation using the collector */
    #define TRACE_ALLOC_START(ptr,size)
    #define TRACE_ALLOC_END(ptr,size)

    /* Macros to log an allocation using the collector */
    #define TRACE_DEALLOC_START(ptr)
    #define TRACE_DEALLOC_END(ptr)

    /* Macros to log an reference increment using the collector */
    #define TRACE_ADDREF_START(ptr)
    #define TRACE_ADDREF_END(ptr)

    /* Macros to log an reference decrement using the collector */
    #define TRACE_DELREF_START(ptr)
    #define TRACE_DELREF_END(ptr)
#endif

/* Destructor function pointer type. */
typedef void (*dtor_t)(void*);

/* Collector API definition structure */
typedef struct {
    void (*init)(void** stkbtm);
    void (*deinit)(void);
    void (*collect)(void);
    void* (*alloc)(size_t sz, dtor_t dtor);
    void (*dealloc)(void* ptr);
    void (*addref)(void* ptr);
    void (*delref)(void* ptr);
} GCAPI;

/* Single-threaded naive reference counting algorithm */
extern const GCAPI NaiveRC;

/* Single-threaded deferred reference counting algorithm */
extern const GCAPI DeferredRC;

/* Atomic reference counting algorithm */
extern const GCAPI AtomicRC;

/* The currently selected GC algorithm */
extern const GCAPI* GC_Algorithm;

/* Initialize the garbage collector */
static inline void gc_init(void** stkbtm) {
    TRACE_INIT_START();
    GC_Algorithm->init(stkbtm);
    TRACE_INIT_END();
}

/* Deinitialize the garbage collector */
static inline void gc_deinit(void) {
    TRACE_DEINIT_START();
    GC_Algorithm->deinit();
    TRACE_DEINIT_END();
}

/* Trigger a garbage collection cycle immediately */
static inline void gc_collect(void) {
    TRACE_COLLECT_START();
    GC_Algorithm->collect();
    TRACE_COLLECT_END();
}

/* Allocate a garbage collected object */
static inline void* gc_alloc(size_t sz, dtor_t dtor) {
    void* ptr;
    TRACE_ALLOC_START((ptr = GC_Algorithm->alloc(sz, dtor)),sz);
    TRACE_ALLOC_END(ptr,sz);
    return ptr;
}

/* Deallocate a garbage collected object */
static inline void gc_dealloc(void* ptr) {
    TRACE_DEALLOC_START(ptr);
    GC_Algorithm->dealloc(ptr);
    TRACE_DEALLOC_END(ptr);
}

/* Increment the reference count for a garbage collected object */
static inline void gc_addref(void* ptr) {
    TRACE_ADDREF_START(ptr);
    GC_Algorithm->addref(ptr);
    TRACE_ADDREF_END(ptr);
}

/* Decrement the reference count for a garbage colected object */
static inline void gc_delref(void* ptr) {
    TRACE_DELREF_START(ptr);
    GC_Algorithm->delref(ptr);
    TRACE_DELREF_END(ptr);
}

/* Replace the value of a reference and adjust the reference counts accordingly */
static inline void gc_swapref(void** var, void* val) {
    void* oldref = *var;
    gc_addref(val);
    *var = val;
    gc_delref(oldref);
}

/* Allocate a new garbage collected object */
#define alloc(obj,dtor) gc_alloc(obj,dtor)

/* Increment the reference count for a garbage collected object */
#define retain(obj) (gc_addref(obj), obj)

/* Decrement the reference count for a garbage colected object */
#define release(obj) (gc_delref(obj), (void*)0)

/* Replace the value of a reference and adjust the reference counts accordingly */
#define assign(var,val) gc_swapref((void**)&(var), val)

/*
 * Make sure we use the built-in main which initializes the collector and calls user_main
 */
#ifndef NO_MAIN_WRAPPER
    #define main user_main
#endif
int user_main(int, char**);

#endif
