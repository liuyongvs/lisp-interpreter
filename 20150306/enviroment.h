#ifndef __ENVIROMENT_H
#define __ENVIROMENT_H

Atom env_create(Atom parent);
int env_set(Atom env, Atom symbol, Atom value);
int env_find(Atom env, Atom symbol, Atom *result);

#endif
