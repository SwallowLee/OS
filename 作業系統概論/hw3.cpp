# include<pthread.h>
# include<semaphore.h>
# include<fstream>
# include<stdio.h>
# include<stdlib.h>
# include<pthread.h>
# include<sys/time.h>

sem_t mutex[15],block;
pthread_t T[15];
int middle[7];

struct str
{
	int *sorting;
	int start, end, num, times, run, com_num, now;
};

void *sort(void *data)
{
	struct str *p = (struct str*) data;
	int from,to,i,j,r,now;
	from = p->start;
	to = p->end;
	now = p->now;
	sem_post( &block );
	
	for( i = from; i < to; i++ )
	{
		for( j = i+1; j < to; j++ )
		{
			if( p->sorting[i] > p->sorting[j] )
			{
				r = p->sorting[i];
				p->sorting[i] = p->sorting[j];
				p->sorting[j] = r;
			}
		}
	}
	sem_post( &mutex[now] );
}

//the function is to compare the number
void *compare(void *data)
{
	struct str *p = (struct str*) data;
	int i, j, from, to, str[p->num], now, com, initial;
	from = p->start;
	to = p->end-1;
	now = p->now;
	initial = p->end;
	com = p->com_num;
	sem_post( &block );
	for( i = from, j = from; i < initial; i++ )
	{
		if( p->sorting[i] <= com )
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
	for( i=from; i<initial; i++ )
	{
		p->sorting[i] = str[i];
	}
	if( now == 0 )

		middle[3] = to+1;

	else if( now == 1 )

		middle[1] = to+1;

	else if( now == 2 )

		middle[5] = to+1;

	else if( now == 3 )

		middle[0] = to+1;

	else if( now == 4 )

		middle[2] = to+1;

	else if( now == 5 )

		middle[4] = to+1;

	else if( now == 6 )
		
		middle[6] = to+1;
	sem_post( &mutex[now] );
}

int main()
{
	int i, j, sec, usec, res;
	FILE *input, *output;
	char file[10000];
	struct str p;
	struct timeval start, end;

	scanf( "%s", file);
	input = fopen( file , "r" );
	if( !input )
		printf( "error in open input file\n" );
	
	fscanf( input, "%d", &p.num );

	p.sorting = ( int * ) malloc( p.num * sizeof(int) );
	for( i = 0; i < p.num; i++)
	{
		fscanf( input, "%d", &p.sorting[i] );
	}

	gettimeofday( &start, 0 );

	//multi thread
	for( i = 0; i<15; i++ )
	{
		res = sem_init( &mutex[i], 1, 1 );
		if( res < 0 )
			printf( "error in samphore mutex %d\n", i );
	}	

	p.com_num = 500000;
	p.start = 0;
	p.end = p.num;
	p.now=0;
	sem_wait( &mutex[0] );
	pthread_create( &T[0], NULL, compare, (void *) &p );
	
	sem_wait( &mutex[0] );
	p.start = 0;
	p.end = middle[3];
	p.now = 1;
	p.com_num = 25000;
	sem_wait( &block );
	for( i = 1; i < 3; i++ )
	{
		sem_wait( &mutex[i] );
		pthread_create( &T[i], NULL, compare, (void *) &p );
		
		sem_wait( &block );
		p.now = 2;
		p.start = middle[3];
		p.end = p.num;
		p.com_num = 75000; 
	}

	sem_wait( &mutex[1] );
	sem_wait( &mutex[2] );
	p.start = 0;
	p.end = middle[1];
	p.now = 3;
	p.com_num = 12500;
	for( i = 3; i < 5; i++ )
	{
		sem_wait( &mutex[i] );
		pthread_create( &T[i], NULL, compare, (void *) &p );
		
		sem_wait( &block );
		p.now = 4;
		p.start = middle[1];
		p.end = middle[3];
		p.com_num = 37500; 
	}

	p.start = middle[3];
	p.end = middle[5];
	p.now = 5;
	p.com_num = 62500;
	for( i = 5; i < 7; i++ )
	{
		sem_wait( &mutex[i] );
		pthread_create( &T[i], NULL, compare, (void *) &p );
		
		sem_wait( &block );
		p.now = 6;
		p.start = middle[5];
		p.end = p.num;
		p.com_num = 87500; 
	}
	
	//sorting
	sem_wait( &mutex[3] );
	sem_wait( &mutex[4] );
	sem_wait( &mutex[5] );
	sem_wait( &mutex[6] );
	p.start = 0;
	p.end = middle[0];
	p.now = 7;
	for( i = 1; i < 9; i++ )
	{
		sem_wait( &mutex[ i+6 ] );
		pthread_create( &T[i], NULL, sort, (void *) &p );
		
		sem_wait( &block );
		if( i < 7 )
		{
			p.start = middle[i-1];
			p.end = middle[i];
			p.now = i+7;
		}
		else
		{
			p.start = middle[6];
			p.end = p.num;
			p.now = i+7;
		}
	}	
	
	sem_wait( &mutex[7] );
	sem_wait( &mutex[8] );
	sem_wait( &mutex[9] );
	sem_wait( &mutex[10] );
	sem_wait( &mutex[11] );
	sem_wait( &mutex[12] );
	sem_wait( &mutex[13] );
	sem_wait( &mutex[14] );
	output = fopen( "output1.txt", "w" );
	if( !output )
		printf( "error in open output file\n" );

	for( i = 0; i < p.num; i++ )
		fprintf( output, "%d ", p.sorting[i] );
	fprintf( output, "\n" );

	fclose( output );

	gettimeofday( &end, 0 );
	sec = end.tv_sec - start.tv_sec;
	usec = end.tv_usec - start.tv_usec;
	
	printf( "multi-thread\n" );
	printf( "elapsed %f ms\n", sec*1000+(usec/1000.0) );

	//single thread
	gettimeofday( &start, 0 );

	p.start = 0;
	p.end = p.num;
	p.times = 1;
	p.run = 0;
	sort( (void *) &p );
	
	output = fopen( "output2.txt", "w" );
	if( !output )
		printf( "error in open output file\n" );

	for( i = 0; i < p.num; i++ )
		fprintf( output, "%d ", p.sorting[i] );
	fprintf( output, "\n" );

	fclose( output );

	gettimeofday( &end, 0 );
	sec = end.tv_sec - start.tv_sec;
	usec = end.tv_usec - start.tv_usec;
	
	printf( "single thread\n" );
	printf( "elapsed %f ms\n", sec*1000+(usec/1000.0) );

	return 0;
}
