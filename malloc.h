#ifndef			__MALLOC_H__
#define			__MALLOC_H__

typedef	struct			s_malloc
{
  struct	s_malloc	*next;
  struct	s_malloc	*prev;
  size_t			size;
  char				isFree;
}				t_malloc;

#endif
