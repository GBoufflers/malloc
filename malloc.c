#include		<unistd.h>
#include		<stdlib.h>
#include		<stdio.h>
#include		<string.h>
#include		"malloc.h"

static t_malloc		*list = NULL;
static t_malloc		*end = NULL;

static void		*createShit(void *ptr, t_malloc *node, size_t size)
{
  t_malloc		*next;
  void			*s;

  s = ptr;
  ptr = malloc(size);
  memcpy(ptr, s, node->size);
  free(s);
  return (ptr);
}

static void		*fitAllocatedSpace(size_t size)
{
  t_malloc		*tmp;
  t_malloc		*bestNode;

  tmp = list;
  bestNode = NULL;
  while (tmp)
    {
      if (tmp->isFree == 1 && tmp->size > size &&
	  (!bestNode || bestNode->size < tmp->size))
	bestNode = tmp;
      tmp = tmp->next;
    }
  if (bestNode)
    {
      bestNode->size = size;
      bestNode->isFree = 0;
      return ((void*)bestNode + sizeof(t_malloc));
    }
  return (NULL);
}

void			*malloc(size_t size)
{
  t_malloc		*new;
  void			*p;

  if ((p = fitAllocatedSpace(size)) != NULL)
    return (p);
  new = sbrk(0);
  if (sbrk(sizeof(t_malloc) + size) == (void*) -1)
    return (NULL);
  new->size = size;
  new->isFree = 0;
  new->next = NULL;
  new->prev = NULL;
  if (end == NULL)
    list = new;
  else
    {
      end->next = new;
      new->prev = end;
    }
  end = new;
  return ((void*)new + sizeof(t_malloc));
}

static void		deleteNode(t_malloc  *ptr)
{
  if (ptr == end)
    {
      end = ptr->prev;
      ptr->prev->next = NULL;
    }
  else if (ptr->prev)
    {
      ptr->next->prev = ptr->prev;
      ptr->prev->next = ptr->next;
    }
}

void			free(void *ptr)
{
  t_malloc		*tmp;

  tmp = list;
  while (tmp)
    {
      if ((void*)tmp + sizeof(t_malloc) == ptr)
	{
	  tmp->isFree = 1;
	  if (tmp->prev && tmp->prev->isFree == 1)
	    {
	      tmp->prev->size += tmp->size + sizeof(t_malloc);
	      deleteNode(tmp);
	    }
	  if (tmp->next && tmp->next->isFree == 1)
	    {
	      tmp->size += tmp->next->size + sizeof(t_malloc);
	      deleteNode(tmp->next);
	    }
	  return ;
	}
      tmp = tmp->next;
    }
}

void			*realloc(void *ptr, size_t size)
{
  t_malloc		*node;

  if (!ptr)
    return(malloc(size));
  else if (size == 0)
    {
      free(ptr);
      return (NULL);
    }
  else
    {
      node = ptr - sizeof(t_malloc);
      if (node->size < size)
	return(createShit(ptr, node, size));
      return (ptr);
    }
}

void			show_alloc_mem()
{
  t_malloc		*tmp;

  tmp = list;
  printf("break: %p\n", sbrk(0));
  while (tmp)
    {
      if (tmp->isFree == 0)
	printf("%p - %p : %ld octets\n",
	       (void*)tmp + sizeof(t_malloc),
	       (void*)tmp + sizeof(t_malloc) + tmp->size,
	       tmp->size);
      tmp = tmp->next;
    }
}
