#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_list
{
    int             num;
    struct s_list   *next;
} t_list;

char	*substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*sub;

	if (s == NULL)
		return (NULL);
	if (start > strlen(s))
		len = 0;
	else if (len > (strlen(s) - start))
		len = strlen(s) - start;
	sub = malloc(sizeof(char) * (len + 1));
	if (sub == NULL)
		return (NULL);
	i = 0;
	while (i < len && s[start] != '\0')
	{
		sub[i++] = s[start++];
	}
	sub[i] = '\0';
	return (sub);
}

size_t	count(const char *s, char c)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			count++;
			while ((s[i] != '\0') && s[i] != c)
				i++;
		}
		else if (s[i] == c)
			i++;
	}
	return (count);
}

size_t	word_len(const char *s, char c)
{
	size_t	i;

	i = 0;
	while (*(s + i) && *(s + i) != c)
		i++;
	return (i);
}

void	free_matrix(char **matrix, size_t i)
{
	while (i > 0)
	{
		i--;
		free(matrix[i]);
	}
	free(matrix);
}

char	**split2(const char *s, char c, char **matrix, size_t words)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < words)
	{
		while (s[j] != '\0' && s[j] == c)
			j++;
		matrix[i] = substr(s, j, word_len((&s[j]), c));
		if (matrix[i] == NULL)
		{
			free_matrix(matrix, i);
			return (NULL);
		}
		while (s[j] != '\0' && s[j] != c)
			j++;
		i++;
	}
	matrix[i] = NULL;
	return (matrix);
}

char **split(const char *s, char c)
{
    char **matrix;
	size_t words;

	if (s == NULL)
		return (NULL);
	words = count(s, c);
	matrix = malloc(sizeof(char *) * (words + 1));
	if (matrix == NULL)
		return (NULL);
	matrix = split2(s, c, matrix, words);
	return (matrix);
}

char *parse_data()
{
    int fd;
    int size;
    int b_read;
    char *buf;

    fd = open("data.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    size = lseek(fd, 0, SEEK_END);
    buf = malloc(sizeof(char) * (size + 1));
    lseek(fd, 0, SEEK_SET);
    b_read = read(fd, buf, size);
    if (b_read == -1)
    {
        perror("read");
        free(buf);
        close(fd);
        exit(1);
    }
    buf[b_read] = '\0';
    close(fd);
    for (int i = 0; buf[i]; i++)
    {
        if (buf[i] == '\n')
            buf[i] = ' ';
    }
    return (buf);
}

t_list *new_list(int data)
{
	t_list	*head;

	head = malloc(sizeof(t_list));
	if (!head)
		return (NULL);
	head->num = data;
	head->next = NULL;
	return (head);
}

void add_back_list(t_list **list, t_list *new)
{
    t_list	*tmp;

	if (!new)
		return ;
	if (*list == NULL)
		*list = new;
	else
	{
		tmp = *list;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void free_data(char **data)
{
    for (int i = 0; data[i]; i++)
        free(data[i]);
    free(data);
}

void get_data(t_list **list1, t_list **list2, char *str)
{
    char **data;
    t_list *t1;
    t_list *t2;

    data = split(str, ' ');
    t1 = *list1;
    t2 = *list2;
    for (int i = 0; data[i] && data[i + 1]; i += 2)
    {
        int val1 = atoi(data[i]);
        int val2 = atoi(data[i + 1]);
        t1 = new_list(val1);
        t2 = new_list(val2);
        add_back_list(list1, t1);
        add_back_list(list2, t2);
    }
    free_data(data);
}

int find_smallest_and_delete(t_list **list)
{
	if (*list == NULL)
		return (0);
    t_list *tmp;
    t_list *prev;
    t_list *smallest_prev;
    t_list *smallest;
    int s;

    tmp = *list;
    smallest = tmp;
    smallest_prev = NULL;
    prev = NULL;
    while (tmp)
    {
        if (tmp->num < smallest->num)
        {
            smallest = tmp;
            smallest_prev = prev;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    if (smallest_prev == NULL)
        *list = smallest->next;
    else
        smallest_prev->next = smallest->next;
    s = smallest->num;
    free(smallest);
    return s;
}

int	calculate_distance(t_list **list1, t_list **list2)
{
	int	dis[4096] = {0};
	int	i;
	int	res = 0;

	i = 0;
	while (*list1 != NULL && *list2 != NULL)
	{
		int l1 = (*list1 != NULL) ? find_smallest_and_delete(list1) : 0;
		int l2 = (*list2 != NULL) ? find_smallest_and_delete(list2) : 0;
		dis[i] = abs(l1 - l2);
		i++;
	}
	for (int j = 0; j < i; j++)
		res += dis[j];
	return (res);
}

int main(void)
{
    t_list  *list1;
    t_list  *list2;
	int		res;

    char *buf = parse_data();
    list1 = NULL;
    list2 = NULL;
    get_data(&list1, &list2, buf);
	res = calculate_distance(&list1, &list2);
	free(buf);
	printf("Result is: %d\n", res);
    return (0);
}
