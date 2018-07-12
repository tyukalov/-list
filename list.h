/* list.h
 *  
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/* Igor 'sp_r00t' Tyukalov <tyukalov@bk.ru> */

#include <stdlib.h>

enum typelem{
  NONE,
  ATOM,
  LIST
};

enum busy {
  BUSY,
  NOTBUSY
};

typedef struct {
  void *data;
  enum typelem type;
} atomic;

struct list {
  atomic atom;
  struct list   *prev;
  struct list   *next;
  enum busy busyed;
};

void print(struct list*);

static inline struct list*
create(){
  struct list *res = (struct list*)calloc(1, sizeof(struct list));
  if (res){
    res->atom.type = NONE;
    res->busyed = NOTBUSY;
  }
  return res;
}

static inline struct list*
cdr(struct list* lst){
  return lst->next;
}

static inline atomic
car(struct list* lst){
  atomic res;
  if(lst)
    res = lst->atom;
  else
    res.type = NONE;
  return res;
}

static inline void
lfree(struct list *lst){
  if(lst){
    lst->busyed = BUSY;
    if(lst->next)
      if(lst->next->busyed == NOTBUSY)
	lfree(lst->next);
    if(lst->prev)
      if(lst->prev->busyed == NOTBUSY)
	lfree(lst->prev);
    if(lst->atom.data){
      if(lst->atom.type == ATOM)
	free(lst->atom.data);
      else if (lst->atom.type == LIST)
	lfree(lst->atom.data);
    }
    free(lst);
  }
}

static inline unsigned long
len(struct list* lst){
  unsigned long result = 0;
  do{
    if(lst->atom.type != NONE)
      result++;
    lst = lst->next;
  }while(lst);
  return result;
}

static inline void*
pop(struct list **plst){
  struct list *var = *plst;
  void *res;
  if(var){
    *plst = var->next;
    res = var->atom.data;
  } else{
    res = NULL;
  }
  return res;
}


#define get(atom) ((atom).data)

#define init_empty_element(e, t, lt, pl) {                              \
  if (((pl)->atom.type = lt) == LIST) {					\
    (pl)->atom.data = (void*)(e);					       	\
  } else {								\
    (pl)->atom.data = malloc(sizeof(t));			                \
    if((pl)->atom.data)							\
      *((t*)((pl)->atom.data)) = (e);				       	\
  }									\
}

#define push(elem, tp, ltype, plst) ({				\
    /* ltype - typelem */						\
    if ((plst)->atom.type == NONE) {						\
      init_empty_element(elem, tp, ltype, plst);			\
    } else {								\
      (plst)->prev = create();						\
      if ((plst)->prev) {						\
	init_empty_element(elem, tp, ltype, (plst)->prev);		\
	((plst)->prev)->next = (plst);					\
      }									\
    }									\
    ((plst)->prev)?                                                     \
      ((((plst)->prev)->atom.data)?((plst)->prev):NULL): \
      (((plst)->atom.data)?(plst):NULL);		 \
    })

