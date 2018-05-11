#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <string>

using namespace std;

int node = 0, size_min = 100000000, size_max = 0;
dirent *ptr;
long long int change = 1;
long double size;
struct stat buf;
char name[100] = "0";

void path(DIR *dir, char place[100000])
{
	int i, j;
	char next[100000];
	bool print, open;
	DIR *dir2;
	if(dir == NULL)
		printf("cant't open the directory\n");

	while((ptr = readdir(dir)) != NULL)
	{
		print = true;
		open = true;
		for(i = 0; place[i] != '\0'; i++)
			next[i] = place[i];
		if(place[i-1]!='/')
		{
			place[i] = '/';
			place[i+1] = '\0';
		}

		if(strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
		{
			if(next[i-1] != '/')
			{
				next[i] = '/';
				i++;
			}
			for(j = 0; ptr->d_name[j] != '\0'; i++, j++)
				next[i] = ptr->d_name[j];
		}
		else
			open = false;
		next[i] = '\0';

		if((dir2 = opendir(next)) != NULL && open == true)
		{
			path(opendir(next), next);
		}
		else
		{
			stat(next, &buf);
			size = (long double)buf.st_size/(long double)change;
			if(node != 0)
			{
				if(ptr->d_ino == (long unsigned int)node)
				{
					printf("%s%s %lu %.2LF MB\n", place, ptr->d_name, ptr->d_ino, size);
				}
			}

			if(strcmp(name, "0") != 0)
			{
				if(strcmp(ptr->d_name, name) != 0)
				{
					print = false;
				}

				if(size_max != 0)
				{
					if(size > size_max)//change the unit
						print = false;

					if(size_min != 100000000)
					{
						if(size < size_min)//change the unit
							print = false;

						if(print == true)
						{
							printf("%s%s %lu %.2LF MB\n", place, ptr->d_name, ptr->d_ino, size);
						}
					}
					else
					{
						if(print == true)
						{
							printf("%s%s %lu %.2LF MB\n", place, ptr->d_name, ptr->d_ino, size);
						}
					}
				}
				else
				{
					if(size_min != 100000000)
					{
						if(size < size_min)//change the unit
							print = false;

						if(print == true)
						{
							printf("%s%s %lu %.2LF MB\n", place, ptr->d_name, ptr->d_ino, size);
						}
					}
					else
					{
						if(print == true)
						{
							printf("%s%s %lu %.2LF MB\n", place, ptr->d_name, ptr->d_ino, size);
						}
					}
				}
			}
			else
			{
				if(size_max != 0)
				{
					if(size > size_max)//change the unit
						print = false;

					if(size_min != 100000000)
					{
						if(size < size_min)//change the unit
							print = false;

						if(print == true)
						{
							printf("%s%s %lu %.2LF MB\n", place, ptr->d_name, ptr->d_ino, size);
						}
					}
					else
					{
						if(print == true)
						{
							printf("%s%s %lu %.2LF MB\n", place, ptr->d_name, ptr->d_ino, size);
						}
					}
				}
				else
				{
					if(size_min != 100000000)
					{
						if(size < size_min)//change the unit
							print = false;

						if(print == true)
						{
							printf("%s%s %lu %.2LF MB\n", place, ptr->d_name, ptr->d_ino, size);
						}
					}
					else;
				}
			}
		}
	}
	return;
}

int main(int argc, char *argv[])
{
	int i, j;
	char place[100000];

	for(i = 0; i < 20; i++)
		change = change*2;

	for(i = 2; i < argc; i++ )
	{
		if(strcmp(argv[i], "-inode") == 0)
		{
			i++;
			for(j = 0; argv[i][j] != '\0'; j++)
				node = node * 10 + argv[i][j] - '0';
		}
		else if(strcmp(argv[i], "-name") == 0)
		{
			i++;
			for(j = 0; argv[i][j] != '\0'; j++)
				name[j] = argv[i][j];
			name[j] = '\0';
		}
		else if(strcmp(argv[i], "-size_max") == 0)
		{
			i++;
			for(j = 0; argv[i][j] != '\0'; j++)
				size_max = size_max * 10 + argv[i][j] - '0';
		}
		else if(strcmp(argv[i], "-size_min") == 0)
		{
			size_min = 0;
			i++;
			for(j = 0; argv[i][j] != '\0'; j++)
				size_min = size_min * 10 + argv[i][j] - '0';
		}
	}

	for(i = 0; argv[1][i] != '\0'; i++)
	{
		place[i] = argv[1][i];
	}
	place[i] = '\0';

	path(opendir(argv[1]), place);
	
	return 0;
}
