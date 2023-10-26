#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <stdio.h>
#include <stdlib.h>

// Define a structure for a node in a linked list
struct node {
  int data;
  struct node *next;
};

// Define a structure for a BigInteger
struct BigInteger {
  struct node *head;
  long int length;
  char sign;
};

// Function declarations
struct node *insert(struct node *head, int val);
void insert_tail(struct BigInteger *n, int element);
long int length(struct node *head);
void displayLL(struct node *link);
void display(struct BigInteger b);
struct BigInteger initialize(char *s);
struct node *reverse(struct node *head);
int compare(struct BigInteger n1, struct BigInteger n2);
struct BigInteger add(struct BigInteger n1, struct BigInteger n2);
struct BigInteger sub(struct BigInteger n1, struct BigInteger n2);
struct BigInteger mul(struct BigInteger n1, struct BigInteger n2);
struct BigInteger div1(struct BigInteger n1, struct BigInteger n2);
struct BigInteger mod(struct BigInteger n1, struct BigInteger n2);
void freeL(struct BigInteger *n);

#endif // BIG_INTEGER_H