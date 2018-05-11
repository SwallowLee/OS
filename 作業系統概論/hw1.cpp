
# include<unistd.h>
# include<iostream>
# include<string.h>
# include<stdlib.h>
# include<sys/wait.h>

using namespace std;
char str_char[100],*first,*second[1000];
string command;
int i,fin;
int process(char *prog,char **arg_list)
{
	pid_t child;
	child=fork();
	if(child<0)
		cout<<"error in fork!!!!"<<endl;
	else if(child>0)//parent
	{
		if(fin==0)
			wait(NULL);
		else
		{
			cout<<"[in process] "<<child<<endl;
		}
	}
	else//child
	{
		if(fin==1)
		{
			pid_t grand;
			grand=fork();
			if(grand<0)
				cout<<"grand error!!"<<endl;
			else if(grand>0)
				exit(0);
			else
			{
				execvp(prog,arg_list);
				exit(0);
			}
		}
		else
        {
            execvp(prog,arg_list);
            exit(0);
        }
	}
}
int main()
{
	fin=0;
	while(1)
	{
		cout<<">";
		fin=0;
		getline(cin,command);
		strcpy(str_char,command.c_str());
		char *slice=new char[strlen(str_char)];
		if(*command.rbegin()=='&')
		{
			fin=1;
		}
		if(strcmp(str_char,"exit")==0)
            return 0;
		slice=strtok(str_char," \n&");
		first=slice;
		for(i=0;slice!=NULL;i++)
		{
			second[i]=slice;
			slice=strtok(NULL," \n&");
		}
		second[i]=NULL;
		process(first,second);
	}
	return 0;
}
