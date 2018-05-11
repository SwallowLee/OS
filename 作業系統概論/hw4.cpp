# include<pthread.h>
# include<semaphore.h>
# include<fstream>
# include<stdio.h>
# include<stdlib.h>
# include<pthread.h>
# include<sys/time.h>

sem_t layer, block, thread_lock, fin, protect;
int middle[7], run, count;

struct task
{
	int function;
	void *argument;
};

struct threadpool
{
	pthread_t *threads;
	task *queue;
	int thread_count;
};

struct str
{
	int *sorting;
	int start, end, num, com_num, now;
};

struct str p;

void *sort(void *data)
{
	struct str *p = (struct str*) data;
	int i,j,r;
	sem_post( &block );
	
	for( i = p->start; i < p->end; i++ )
	{
		for( j = i+1; j < p->end; j++ )
		{
			if( p->sorting[i] > p->sorting[j] )
			{
				r = p->sorting[i];
				p->sorting[i] = p->sorting[j];
				p->sorting[j] = r;
			}
		}
	}
	count++;
	if( count == 15 )
	{
		sem_post( &fin );
	}
}

//the function is to compare the number
void *compare(void *data)
{
	struct str *p = (struct str*) data;
	int i, j, to, str[p->num];
	sem_post( &block );

	to = p->end-1;
	for( i = p->start, j = p->start; i < p->end; i++ )
	{
		if( p->sorting[i] <= p->com_num )
		{
			str[j] = p->sorting[i];
			j++;
		}
		else
		{
			str[to] = p->sorting[i];
			to--;
		}
	}
	for( i=p->start; i<p->end; i++ )
	{
		p->sorting[i] = str[i];
	}
	if( p->now == 0 )

		middle[3] = to+1;

	else if( p->now == 1 )

		middle[1] = to+1;

	else if( p->now == 2 )

		middle[5] = to+1;

	else if( p->now == 3 )

		middle[0] = to+1;

	else if( p->now == 4 )

		middle[2] = to+1;

	else if( p->now == 5 )

		middle[4] = to+1;

	else if( p->now == 6 )
		
		middle[6] = to+1;

	count++;
	if( count == 1 || count == 3 || count == 7 )
	{
		sem_post( &layer );
	}
}

void *thread_routine( void* thread )
{
	threadpool *pool = ( threadpool * ) thread;

	while(1)
	{
		sem_wait( &protect );
		pool->thread_count--;
		sem_post( &protect );

		if( pool->thread_count < 0 )
		{
			sem_wait( &thread_lock );
		}
		sem_wait( &block );

		run++;
		int order = run;
		
		if( order == 1 || order == 3 || order == 7 )
		{
			sem_wait( &layer );
		}
		else if( order >= 15 )
		{
			break;
		}
		
		if( order < 7 )
		{
			if( order == 0)
			{
				p.start = 0;
				p.end = p.num;
				p.now = order;
			}
			else if( order == 1)
			{
				p.start = 0;
				p.end = middle[3];
				p.now = order;
			}
			else if( order == 2)
			{
				p.start = middle[3];
				p.end = p.num;
				p.now = order;
			}
			else if( order == 3)
			{
				p.start = 0;
				p.end = middle[1];
				p.now = order;
			}
			else if( order == 4)
			{
				p.start = middle[1];
				p.end = middle[3];
				p.now = order;
			}
			else if( order == 5)
			{
				p.start = middle[3];
				p.end = middle[5];
				p.now = order;
			}
			else if( order == 6)
			{
				p.start = middle[5];
				p.end = p.num;
				p.now = order;
			}	
		}
		else if( order < 15 )
		{
			if( order == 7 )
			{
				p.start = 0;
				p.end = middle[0];
				p.now = 7;
			}
			else if( order < 14 )
			{
				p.start = middle[ order-8 ];
				p.end = middle[ order-7 ];
				p.now = order;
			}
			else
			{
				p.start = middle[6];
				p.end = p.num;
				p.now = order;		
			}
		}
		
		pool->queue[order].argument = (void *)&p;
		if(pool->queue[order].function == 0)
		{
			compare( pool->queue[order].argument );
		}
		else
		{
			sort( pool->queue[order].argument );
		}

		pool->thread_count++;
		if( pool->thread_count == 0 )
		{
			sem_post( &thread_lock );
		}

	}
}

int main()
{
	int i, j, sec, usec, res, n;
	FILE *input, *output;
	char file[10000];
	struct threadpool pool;
	struct timeval start, end;

	input = fopen( "input.txt" , "r" );
	if( !input )
		printf( "error in open input file\n" );
	
	fscanf( input, "%d", &p.num );

	int string[p.num];
	p.sorting = ( int * ) malloc( p.num * sizeof(int) );
	for( i = 0; i < p.num; i++)
	{
		fscanf( input, "%d", &string[i] );
	}

	pool.queue = ( struct task* ) malloc( sizeof(struct task)* 15);
	
	for( i = 0; i < 7; i++ )
	{
		pool.queue[i].function = 0;
	}
	for( i = 7; i <15; i++ )
	{
		pool.queue[i].function = 1;
	}
	
	for( n = 1; n <= 8; n++ )
	{
		res = sem_init( &layer, 1, 0);
		if( res < 0 )
			printf("error in samphore mutex\n" );
		res = sem_init( &block, 1, 1);	
		if( res < 0 )
			printf("error in samphore block\n" );
		res = sem_init( &fin, 1, 0);	
		if( res < 0 )
			printf("error in samphore fin\n" );
		res = sem_init( &thread_lock, 1, 0);
		if( res < 0 )
			printf("error in samphore lock\n" );
		res = sem_init( &protect, 1, 1);
		if( res < 0 )
			printf("error in samphore protect\n" );
		for( i = 0; i < p.num; i++ )
		{
			p.sorting[i] = string[i];
		}

		gettimeofday( &start, 0 );
		
		run = -1;
		count = 0;
		pool.threads = ( pthread_t* ) malloc( sizeof(pthread_t) * n);
		pool.thread_count = n;
			
		for( i = 0; i < pool.thread_count; i++ )
		{
			pthread_create( &pool.threads[i], NULL, thread_routine, (void *) &pool);
		}
		sem_wait( &fin );
		
		if( n == 1)
			output = fopen( "output_1.txt", "w" );
		else if( n == 2)
			output = fopen( "output_2.txt", "w" );
		else if( n == 3)
			output = fopen( "output_3.txt", "w" );
		else if( n == 4)
			output = fopen( "output_4.txt", "w" );
		else if( n == 5)
			output = fopen( "output_5.txt", "w" );
		else if( n == 6)
			output = fopen( "output_6.txt", "w" );
		else if( n == 7)
			output = fopen( "output_7.txt", "w" );
		else if( n == 8)
			output = fopen( "output_8.txt", "w" );
		if( !output )
			printf( "error in open output file\n" );

		for( i = 0; i < p.num; i++ )
			fprintf( output, "%d ", p.sorting[i] );
		fprintf( output, "\n" );

		fclose( output );
		gettimeofday( &end, 0 );
		sec = end.tv_sec - start.tv_sec;
		usec = end.tv_usec - start.tv_usec;

		printf("%d thread\n", n);
		printf("elpased %f ms\n", sec*1000+(usec/1000.0) );

	}


	return 0;
}
