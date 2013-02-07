#ifndef			__MALLOC_H__
#define			__MALLOC_H__

typedef	struct			s_malloc
{
  struct	s_malloc	*next;
  size_t			size;
  int				isFree;
}				t_malloc;

#endif
