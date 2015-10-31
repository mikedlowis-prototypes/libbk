#include <bk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************************************************************************
 * GC Algorithm Setup
 *****************************************************************************/
const GCAPI* GC_Algorithm = &DeferredRC;

/******************************************************************************
 * Utility Functions
 *****************************************************************************/
static char* strclone(char* str) {
    char* newstr = (char*)alloc(strlen(str)+1, NULL);
    strcpy(newstr, str);
    return newstr;
}

/******************************************************************************
 * Pairs and Lists
 *****************************************************************************/
typedef struct {
    void* car;
    void* cdr;
} Pair;

static void pair_free(void* ptr) {
    Pair* p = (Pair*)ptr;
    release(p->car);
    release(p->cdr);
}

Pair* cons(void* a, void* b) {
    Pair* pair = (Pair*)alloc(sizeof(Pair), pair_free);
    pair->car = retain(a);
    pair->cdr = retain(b);
    return pair;
}

static void* car(Pair* p) { return p->car; }
static void* cdr(Pair* p) { return p->cdr; }

static Pair* reverse(Pair* p) {
    Pair* newlist = NULL;
    while (p != NULL) {
        newlist = cons(car(p), newlist);
        p = cdr(p);
    }
    return newlist;
}

//static void foreach(Pair* list, void (*fn)(void*)) {
//    while(list != NULL) {
//        fn(car(list));
//        list = cdr(list);
//    }
//}

/******************************************************************************
 * Record Definition
 *****************************************************************************/
typedef struct {
    int amount;
    char* person;
    char* thing;
} Record;

static void record_free(void* ptr) {
    Record* rec = (Record*)ptr;
    assign(rec->person, NULL);
    assign(rec->thing, NULL);
}

Record* record(int amt, char* p, char* t) {
    Record* rec = (Record*)alloc(sizeof(Record), record_free);
    rec->amount = amt;
    rec->person = retain(p);
    rec->thing  = retain(t);
    return rec;
}

Record* parse_record(char* line) {
    char* amount = strtok(line, ",");
    char* person = strtok(NULL, ",");
    char* thing  = strtok(NULL, ",");
    thing[strlen(thing)-1] = '\0';
    return record(atoi(amount), strclone(person), strclone(thing));
}

/******************************************************************************
 * Record Definition
 *****************************************************************************/
/*
    The main is a close analogue of this Ruby code:

    dkp_log = File.foreach("dkp.log").map { |line|
      amount, person, thing = line.strip.split(",")
      [ amount.to_i, person, thing ]
    }
    standings = dkp_log.group_by { |trans| trans[1] }.map { |person, history|
      [ person, history.reduce(0) { |sum, trans| sum + trans[0] } ]
    }.sort { |a, b| b[1] <=> a[1] }
*/
#include <stdbool.h>

//void printrec(void* ptr) {
    //Record* rec = (Record*)ptr;
    //printf("%d : %s : %s\n", rec->amount, rec->person, rec->thing);
//}

int main(int argc, char** argv) {
    const char* infile = (argc == 2 && argv[1]) ? argv[1] : "tests/input.log";
    /* Read the data in from the file */
    char linebuf[1024];
    FILE* data = fopen(infile, "r");
    Pair* entries = NULL;
    bool discard = false;
    while(fgets(linebuf, 1023, data)) {
        Record* rec = parse_record(linebuf);
        if (!discard)
            entries = cons(rec, entries);
        discard = !discard;
    }
    entries = reverse(entries);
    //foreach(entries, printrec);
    fclose(data);
    /* */
    return 0;
}
