#include <iostream.h>
#include <stdlib.h>
#include <conio.h>

void Swap(int&x,int&y)
{
 int temp=x;
 x=y;
 y=temp;
}

void getNext(int N,int Value[])
{
  int i = N - 1,j = N;
  while(Value[i-1] >= Value[i]) i--;
  while(Value[i-1] >= Value[j-1]) j--;
  Swap(Value[i-1],Value[j-1]);
  i++;
  j = N;
  while (i < j)
  {
    Swap(Value[i-1], Value[j-1]);
    i++;
    j--;
  }
}

void main()
{
 int Value[3];
 int N=3;
 int k,l;
 for(k=0;k<N;k++)
    Value[k]=k;
 for(k=0;k<6;k++)
   {
    for(l=0;l<N;l++)
    	cout<<Value[l];
    cout<<"\n";
    getNext(N,Value);
   }
 getch();
}



