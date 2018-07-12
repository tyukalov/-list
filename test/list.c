#include <stdio.h>
#include <stdlib.h>
#include "../list.h"
  
int
main() {
  struct list *a = create();
  struct list *b = create();
  struct list *r1;
  int r2;
  b = push(11, int, ATOM, b);
  if(!b){
    printf("Error\n");
    return -1;
  }
  b = push(13, int, ATOM, b);
  if(!b){
    printf("Error\n");
    return -1;
  }
  a = push(17, int, ATOM, a);
  if(!a){
    printf("Error\n");
    return -1;
  }
  b = push(a, struct list*, LIST, b);
  if(!b){
    printf("Error\n");
    return -1;
  }
  printf("a.len = %d\nb.len = %d\n", len(a), len(b));
  print(b);
  fputs("========================\n", stdout);
  printf("car(b = )");
  print((struct list*)get(car(b)));
  printf("car(cdr(b)) = %d\n", *((int*)get(car(cdr(b)))));
  r1 = (struct list*)pop(&b);
  fputs("Print b\n", stdout);
  print(b);
  fputs("Print r1\n", stdout);
  print(r1);
  r2 = *((int*)(pop(&b)));
  printf("r2 = %d\n", r2);
  fputs("Print b\n", stdout);
  print(b);
  lfree(b);
  /* lfree(a); */
  return 0;
}

void print(struct list *lst){
  if (!lst){
    fputs("Error - zero pointer!\n", stdout);
    return;
  }
  do{
    if (lst->atom.type == ATOM){
      printf("%d\n", (int)*((int *)(lst->atom.data)));
    }
    else if (lst->atom.type == LIST){
      print(lst->atom.data);
    }
    lst = lst->next;
  } while(lst);
}

