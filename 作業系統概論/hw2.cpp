# include<unistd.h>
# include<sys/ipc.h>
# include<sys/shm.h>
# include<sys/wait.h>
# include<sys/time.h>
# include<iostream>
# include<stdlib.h>

using namespace std;

int shmC,dim;
unsigned int *matrix,sum,i,j,part,num,row,col,m,k;

int process(int start,int end)
{	

	pid_t child;
	child=fork();

	if(child<0)
		cout<<"error in fork"<<endl;
	else if(child==0)
	{
		for( j=start ; j<end ; j++ ) //number j in matrix
		{
			row=j/dim;
			for( m=row*dim , col=j%dim ; m<dim*( row+1 ) ; m++ , col=col+dim )
			{
				matrix[j]=matrix[j]+m*col;
				//cout<<"matrix "<<matrix[j]<<" "<<m<<" "<<col<<" "<<m*col<<endl;
			}
		}
		if( end<dim*dim && (end+part)>dim*dim )
			for( j=j ; j<dim*dim ;j++ )
			{
				row=j/dim;
				for( m=row*dim , col=j%dim ; m<dim*( row+1 ) ; m++ , col=col+dim )
				{
					matrix[j]=matrix[j]+m*col;
					//cout<<"matrix "<<matrix[j]<<" "<<m<<" "<<col<<" "<<m*col<<endl;
				}
			
			}
		exit(0);

	}
	else
	{
		if( end+part<=dim*dim )
			process(end,end+part);
		wait(NULL);
	}
	return 0;
}

int main()
{
	cout<<"Input the matrix dimension: ";
	cin>>dim;
	
	for(int i=1;i<17;i++)
	{
		struct timeval start,end;
		gettimeofday(&start,0);
		
		shmC=shmget(0,dim*dim*sizeof(unsigned int),IPC_CREAT|0666);
		matrix=(unsigned int *)shmat(shmC,NULL,0);
		
		for( j=0 ; j<dim*dim ; j++ )
		{
			matrix[j]=0;
		}

		part=(dim*dim)/i;		
		j=0;
		num=0;
		
		process(0,part);
		
		sum=0;
		for( j=0 ; j<dim*dim ; j++)
		{
			sum=sum+matrix[j];
		}

		shmdt(matrix);
		shmctl(shmC,IPC_RMID,0);

		gettimeofday(&end,0);
		int sec=end.tv_sec-start.tv_sec;
		int usec=end.tv_usec-start.tv_usec;
		
		cout<<"Multiplying matrices using "<<i<<" process"<<endl;
		cout<<"Elapsed time: "<<sec*1000+(usec/1000.0)<<" ms, Checksum: "<<sum<<endl;
	}
}
