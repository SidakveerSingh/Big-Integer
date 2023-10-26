#include "BigInteger.h"

#include <stdio.h>
#include <stdlib.h>

// Function to insert a new node at the beginning of a linked list
struct node *insert(struct node *head, int val) {
  // Allocate memory for a new node
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  if (!temp) {
    printf("Memory Not Allocated!\n");
    exit(1);
  }
  temp->data = val;
  temp->next = head;
  return temp;
}

// Function to insert a new element at the end of a BigInteger
void insert_tail(struct BigInteger *n, int element) {
  // Allocate memory for a new node
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  if (temp == NULL) {
    printf("memory allocation failed\n");
    return;
  }
  temp->data = element;
  temp->next = NULL;
  n->length += 1;
  if (n->head == NULL) {
    n->head = temp;
    return;
  }
  struct node *itr = n->head;
  while (itr->next != NULL) {
    itr = itr->next;
  }
  itr->next = temp;
}

// Function to calculate the length of a linked list
long int length(struct node *head) {
  long int count = 0;
  while (head) {
    head = head->next;
    count++;
  }
  return count;
}

// Function to display a linked list in reverse order
void displayLL(struct node *link) {
  if (!link)
    return;
  displayLL(link->next);
  printf("%d", link->data);
}

// Function to display a BigInteger
void display(struct BigInteger b) {
  if (b.head == NULL) {
    printf("0");
    return;
  }
  if (b.sign == '-')
    printf("-");
  displayLL(b.head);
}

// Function to initialize a BigInteger from a string
struct BigInteger initialize(char *s) {
  struct BigInteger b;
  b.head = NULL;
  if (s[0] == '-')
    b.sign = '-';
  else
    b.sign = '+';
  int i;
  if (b.sign == '-')
    i = 1;
  else
    i = 0;
  int flag = 0;
  while (s[i] == '0' && s[i] != '\0')
    i++;
  for (int j = i; s[j] != '\0'; j++) {
    b.head = insert(b.head, s[j] - '0'); // Convert char to int
    flag = 1;
  }
  if (flag == 0) {
    b.head = insert(b.head, 0);
    b.sign = '+'; // Handle the special case where it's just "0"
  }
  b.length = length(b.head);
  return b;
}

// Function to reverse a linked list
struct node *reverse(struct node *head) {
  struct node *prev = NULL, *front = NULL, *current = head;
  while (current != NULL) {
    front = current->next;
    current->next = prev;
    prev = current;
    current = front;
  }
  return prev;
}

// Function to compare absolute values of BigIntegers
int compare(struct BigInteger n1, struct BigInteger n2) {
  if (n2.length > n1.length) {
    return -1; // Changed this to -1 for num2 greater
  } else if (n2.length == n1.length) {
    int return_val = 0;
    n1.head = reverse(n1.head);
    n2.head = reverse(n2.head);
    struct node *head1 = n1.head, *head2 = n2.head;
    while (head1 != NULL && head2 != NULL) {
      if (head2->data > head1->data) {
        return_val = -1; // -1 for num2 greater
        break;
      } else if (head1->data > head2->data) { 
        return_val = 1; //  1 for num1 greater
        break;
      }
      head1 = head1->next;
      head2 = head2->next;
    }
    n1.head = reverse(n1.head);
    n2.head = reverse(n2.head);
    return return_val;
  }
  return 1; // 1 for num1 greater
}

// Function to perform addition of two BigIntegers
struct BigInteger add(struct BigInteger n1, struct BigInteger n2) {
  struct BigInteger n3;
  n3.head = NULL;
  n3.sign = '+';
  n3.length = 0;

  if (n1.sign == '-' && n2.sign == '-') {
    n3.sign = '-';
  } else if (n1.sign == '-') {
    n1.sign = '+';
    n3 = sub(n2, n1);
    n1.sign = '-';
    return n3;
  } else if (n2.sign == '-') {
    n2.sign = '+';
    n3 = sub(n1, n2);
    n2.sign = '-';
    return n3;
  }

  struct node *l1 = n1.head, *l2 = n2.head;
  int sum, carry = 0;

  while (l1 != NULL || l2 != NULL || carry != 0) {
    int d1 = (l1 != NULL) ? l1->data : 0;
    int d2 = (l2 != NULL) ? l2->data : 0;

    sum = d1 + d2 + carry;
    carry = sum / 10;
    insert_tail(&n3, sum % 10);

    if (l1 != NULL)
      l1 = l1->next;
    if (l2 != NULL)
      l2 = l2->next;
  }
  // Remove trailing zeros
  n3.head = reverse(n3.head);
  while (n3.head != NULL && n3.head->data == 0) {
    struct node *temp = n3.head;
    n3.head = n3.head->next;
    free(temp);
  }
  n3.head = reverse(n3.head);
  n3.length = length(n3.head);
  return n3;
}

// Function to perform subtraction of two BigIntegers
struct BigInteger sub(struct BigInteger n1, struct BigInteger n2) {
  struct BigInteger n3;
  n3.head = NULL;
  n3.sign = '+';
  n3.length = 0;

  if (n1.sign == '-' && n2.sign == '+') {
    n1.sign = '+';
    n3 = add(n1, n2);
    n1.sign = '-';
    n3.sign = '-';
    return n3;
  } else if (n1.sign == '+' && n2.sign == '-') {
    n2.sign = '+';
    n3 = add(n1, n2);
    n2.sign = '-';
    return n3;
  } else if (n1.sign == '-' && n2.sign == '-') {
    n2.sign = '+';
    n1.sign = '+';
    n3 = sub(n2, n1);
    n2.sign = '-';
    n1.sign = '-';
    return n3;
  } else if (compare(n1, n2) == -1) {
    n3.sign = '-'; 
    struct BigInteger temp = n1;
    n1 = n2;
    n2 = temp;
  }

  struct node *l1 = n1.head, *l2 = n2.head;
  int diff, borrow = 0;

  while (l1 != NULL || l2 != NULL) {
    int d1 = (l1 != NULL) ? l1->data : 0;
    int d2 = (l2 != NULL) ? l2->data : 0;

    diff = d1 - d2 - borrow;

    if (diff < 0) {
      diff += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }

    insert_tail(&n3, diff);

    if (l1 != NULL)
      l1 = l1->next;
    if (l2 != NULL)
      l2 = l2->next;
  }
  // Remove trailing zeros
  n3.head = reverse(n3.head);
  while (n3.head != NULL && n3.head->data == 0) {
    struct node *temp = n3.head;
    n3.head = n3.head->next;
    free(temp);
  }
  n3.head = reverse(n3.head);
  n3.length = length(n3.head);
  return n3;
}

// Function to perform multiplication of two BigIntegers
struct BigInteger mul(struct BigInteger n1, struct BigInteger n2) {
  struct node *a1 = n1.head, *a2 = n2.head;
  struct BigInteger n3 = initialize(""); // Initialize with string "0"
  n3.length = 0;
  if (a1 == NULL || a2 == NULL) {
    return n3;
  }
  struct BigInteger mid, result = initialize("");
  int carry = 0, product = 0, i = 0;
  while (a2 != NULL) {
    a1 = n1.head;
    carry = 0;
    mid = initialize("");
    for (int j = 0; j < i; j++) {
      insert_tail(&mid, 0);
    }
    while (a1 != NULL) {
      product = (a1->data) * (a2->data) + carry;
      insert_tail(&mid, product % 10);
      carry = product / 10;
      a1 = a1->next;
    }

    if (carry > 0) {
      insert_tail(&mid, carry);
    }

    result = add(mid, result);
    struct node *current = mid.head;
    while (current != NULL) {
      struct node *temp = current;
      current = current->next;
      free(temp);
    }

    a2 = a2->next;
    i++;
  }
  if (n1.sign != n2.sign) {
    result.sign = '-';
  }
  // Remove trailing zeros
  result.head = reverse(result.head);
  while (result.head != NULL && result.head->data == 0) {
    struct node *temp = result.head;
    result.head = result.head->next;
    free(temp);
  }
  result.head = reverse(result.head);
  result.length = length(result.head);
  return result;
}

struct BigInteger div1(struct BigInteger n1, struct BigInteger n2) {
  struct BigInteger n3 = initialize("0");
  n3.length = 0;
  n3.sign = '+';
  char sig = '+';
  int flag = 0;
  if (n2.length == 1 && n2.head->data == 0) {
    printf("Error division by zero\n");
    return n3;
  }
  if (n1.sign != n2.sign)
    sig = '-';
  char t1 = n1.sign;
  char t2 = n2.sign;
  n1.sign = '+';
  n2.sign = '+';
  struct BigInteger temp = add(n2, initialize("0"));
  while (compare(n1, temp) >= 0) {
    temp = add(temp, n2);
    n3 = add(n3, initialize("1"));
    flag = 1;
  }
  if (flag)
    n3.sign = sig;
  n1.sign = t1;
  n2.sign = t2;
  return n3;
}

struct BigInteger mod(struct BigInteger n1, struct BigInteger n2) {
  struct BigInteger n3 = initialize("0");
  n3.length = 0;
  n3.sign = '+';
  char sig = '+';
  int flag = 0;
  if (n2.length == 1 && n2.head->data == 0) {
    printf("Error division by zero\n");
    return n3;
  }
  if (n1.sign != n2.sign)
    sig = '-';
  char t1 = n1.sign;
  char t2 = n2.sign;
  n1.sign = '+';
  n2.sign = '+';
  struct BigInteger temp = add(n2, initialize("0"));
  while (compare(n1, temp) >= 0) {
    temp = add(temp, n2);
    n3 = add(n3, initialize("1"));
  }
  struct BigInteger temp2 = sub(temp, n2);
  struct BigInteger modu = sub(n1, temp2);
  n1.sign = t1;
  n2.sign = t2;
  return modu;
}
