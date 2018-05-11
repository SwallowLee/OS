# include<stdio.h>
# include<vector>
# include<string>
# include<algorithm>
# include<iostream>
# include<map>

using namespace std;
int main(void)
{
	FILE *text;
	int i, j, *place;
	long long int miss, hit, k, m;
	char ch, use[6];
	fpos_t pos;
	string replace;
	vector<string> frame;
	vector<string>::iterator it, it3;
	map<string, int> order;
	map<string, int>::iterator it2;
	
	text = fopen( "trace.txt", "r" );
	if( !text )
		printf("can't open the file.\n");
	fgetpos(text, &pos);
	
	//FIFO
	printf("FIFO---\n");
	printf("size                      miss                      hit                       page fault ratio\n");
	for(i = 64; i < 1024; i = i*2 )
	{
		miss = 0;
		hit = 0;
		place = &i; // whatever
		while( !feof(text) )
		{
			for( j = 0; j < 3; j++ )
				fscanf(text, "%c", &ch);
			for( j = 0; j < 5; j++ )
				fscanf(text, "%c", &use[j]);
			use[5] = '\0';
			if( !feof(text) )
			{
				string put(use);
				it2 = order.find(put);
				if( it2 == order.end() )
				{
					miss++;
					if(frame.size() == i )
					{
						replace = frame[0];
						frame.erase(frame.begin());
						order.erase(replace);
					}
					frame.push_back( put );
					order.insert( pair<string, int>(put, 0) );
				}
				else
				{	
					hit++;
				}
			}
			while(ch != '\n' )
				fscanf(text, "%c", &ch);
		}
		printf("%-26d%-26lld%-26lld%.9f\n",i, miss, hit, (double)miss/(miss + hit));
		
		frame.clear();
		order.clear();
		fsetpos(text, &pos);
	}

	//LRU
	printf("LRU---\n");
	printf("size                      miss                      hit                       page fault ratio\n");
	for(i = 64; i < 1024; i = i*2 )
	{
		miss = 0;
		hit = 0;
		k = 0;
		m = 0;
		while( !feof(text) )
		{
			for( j = 0; j < 3; j++ )
				fscanf(text, "%c", &ch);
			for( j = 0; j < 5; j++ )
				fscanf(text, "%c", &use[j]);
			use[5] = '\0';
			if( !feof(text) )
			{
				string put(use);
				it2 = order.find(put);
				if( it2 == order.end() )
				{
					miss++;
					if(frame.size() == i )
					{
						replace = frame[0];
						frame.erase(frame.begin());
						order.erase(replace);
						m++;
					}
					//replace = frame[ frame.size()-1 ];
					frame.push_back( put );		
					/*if( order.size() == 0 )
					{
						place = &k;
						order.insert( pair<string, int*>(put, place);
					}*/
					//it2 = order.find(replace);
					order.insert( pair<string, int>(put, k-m) );
					k++;
				}
				else
				{	
					hit++;
					it = find(frame.begin(), frame.end(), put);
					frame.erase(it);
					frame.push_back( put );
				}
			}
			while(ch != '\n' )
				fscanf(text, "%c", &ch);
		}
		printf("%-26d%-26lld%-26lld%.9f\n",i, miss, hit, (double)miss/(miss + hit));
		
		frame.clear();
		order.clear();
		fsetpos(text, &pos);
	}

	return 0;
}
