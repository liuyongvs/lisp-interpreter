#ifndef __TYPE_H
#define __TYPE_H

struct Atom {
	enum {
		ATOM_NIL,
		ATOM_SYMBOL,
		ATOM_INTEGER,
		ATOM_PAIR
	}type;

	union {
		const char *symbol;
		int integer;
		struct Pair *pair;
	}value;
};

struct Pair {
	struct Atom atom[2];
};

typedef enum {
	ERROR_OK,
	ERROR_SYNTAX,
	ERROR_ARGS,
	ERROR_UNBOUND,
	ERROR_TYPE
}Error;
typedef struct Atom Atom;
typedef struct Pair Pair;

static Atom nil = {0};

#define car(p) ((p).value.pair->atom[0])
#define cdr(p) ((p).value.pair->atom[1])

#define nilp(p) (p.type == ATOM_NIL)

#define true  1
#define false 0

#endif
