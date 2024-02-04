#include<stdio.h>
#include<conio.h>
#include<alloc.h>
#include<math.h>
#include<stdlib.h>

void print_array_symbols()
{
 clrscr();
 printf("    s1   s2  . . . sm\n\n");
 printf("d1  c11  c12 . . . c1m\n");
 printf("d2  c21  c22 . . . c2m\n");
 printf(".   .    .         .\n");
 printf(".   .    .         .\n");
 printf(".   .    .         .\n");
 printf("dn  cn1  cn2 . . . cnm\n");
 getch();
}

void print_array_values(float *C,int n,int m)
{
 int i,j;
 printf("\n");
 printf("   s1   s2  . . . sm\n");
 for(i=0;i<n;i++)
    {
     printf("\nd%d ",i+1);
     for(j=0;j<m;j++)
	printf("%1.1f\t",*(C+i*m+j));
    }
 getch();
}


void print_result_values(int *ans_final,int n,int m)
{
 int i,j;
 int *flag=(int*)malloc(m*sizeof(int));
 for(i=0;i<m;i++)
    flag[i]=0;
 printf("\n\nThe sites connected to each of the chosen concenterators:");
 for(j=0;j<n;j++)
   if(flag[ans_final[j]]==0)
       {
        flag[ans_final[j]]=1;
        printf("\nConcenterator%d:",ans_final[j]+1);
        printf("\nSite%d",j+1);
        for (i=j+1;i<n;i++)
           if (ans_final[i]==ans_final[j])
               printf("\nSite%d",i+1);
       }
 getch();
 free(flag);
}

int factorial(int n)
{
  if (n==1) return 1;
  return n*factorial(n-1);
}

void swap1(float *C,int m,int x,int y)
{
 float temp;
 for(int i=0;i<m;i++)
    {
     temp=*(C+x*m+i);
     *(C+x*m+i)=*(C+y*m+i);
     *(C+y*m+i)=temp;
    }
}

void swap2(int&x,int&y)
{
 int temp=x;
 x=y;
 y=temp;
}

void next_permutation(float *C,int *site_indecees,int n,int m)
{
  int i=n-1,j=n;
  while (site_indecees[i-1] >= site_indecees[i])   i--;
  while (site_indecees[i-1] >= site_indecees[j-1]) j--;
  swap1(C,m,i-1,j-1);
  swap2(site_indecees[i-1],site_indecees[j-1]);
  i++; j=n;
  while (i < j)
   {
    swap1(C,m,i-1,j-1);
    swap2(site_indecees[i-1],site_indecees[j-1]);
    i++;
    j--;
   }
}

void main( )
{
  int n,m,i,j;
  print_array_symbols();
  printf("\nEnter the number of sites 'n':");
  scanf("%d",&n);
  printf("\nEnter the number of concentrators 'm':");
  scanf("%d",&m);

  float *C;
  C=(float *)malloc(n*m*sizeof(float));

  int *max_no_of_dis_from_1src;
  max_no_of_dis_from_1src=(int*)malloc(m*sizeof(int));


  printf("\nEnter the cost matrix C,");
  printf(" where Cij is the cost of connecting customer site I to concentrator J:\n");
  for(i=0;i<n;i++)
     {
      printf("\nSite%d:\n",i+1);
      for(j=0;j<m;j++)
        {
       	 printf("c%d%d=",i+1,j+1);
         scanf("%f",C+i*m+j);
        }
     }

  print_array_values(C,n,m);

  printf("\n\nEnter the maximum number of sites that can be connected to each concentrator:");
  int sum=0;
  for (j=0;j<m;j++)
      {
       printf("\nConcentrator%d:",j+1);
       scanf("%d",max_no_of_dis_from_1src+j);
       sum+=max_no_of_dis_from_1src[j];
      }

  if(sum<n)
    {
     printf("\nThe sum of maximum no. of sites for each concenterator<no. of sites");
     getch();
     free(C);
     free(max_no_of_dis_from_1src);
     exit(0);
    }

  float *conc_cost;
  conc_cost=(float*)malloc(m*sizeof(float));

  int *site_indecees,*ans_perm,*ans_case,*ans_ith_concentrators,*ans_final;
  int *S,*sources,*min_sources_final;
  site_indecees=(int *)malloc(n*sizeof(int));
  ans_perm=(int *)malloc(n*sizeof(int));
  ans_case=(int *)malloc(n*sizeof(int));
  ans_ith_concentrators=(int*)malloc(n*sizeof(int));
  ans_final=(int*)malloc(n*sizeof(int));
  S=(int *)malloc(m*sizeof(int));
  sources=(int*)malloc(m*sizeof(int));
  min_sources_final=(int*)malloc(m*sizeof(int));

  printf("\nEnter the cost of each concentrator:");
  for(j=0;j<m;j++)
     {
      printf("\nCost of concentrator%d=",j+1);
      scanf("%f",conc_cost+j);
     }


  int min_cost,min_cost_perm,min_cost_case,min_cost_ith_concentrators,min_cost_final;
  int min_index,x,y,k,f=factorial(n);

  float *temp1;
  temp1=(float *)malloc(n*m*sizeof(float));
  for(x=0;x<m;x++)
      for(y=0;y<n;y++)
        *(temp1+y*m+x)=*(C+y*m+x);

  y=f;
  min_cost_case=10000;
  for (k=0;k<n;k++)
      site_indecees[k]=k;
  for (k=0;k<m;k++)
      sources[k]=k;
  do
    {
     min_cost_perm=0;
     for (k=0;k<m;k++)
         S[k]=0;
     for (k=0;k<n;k++)
         {
          for(i=0;i<m;i++)
              if (S[i]<max_no_of_dis_from_1src[i])
                 {
                  min_cost=*(temp1+k*m+i);
                  min_index=i;
                  break;
                 }
          for(x=i+1;x<m;x++)
              if (*(temp1+k*m+x)<min_cost && S[x]<max_no_of_dis_from_1src[x])
                  {
                   min_cost=*(temp1+k*m+x);
                   min_index=x;
                  }
          ans_perm[site_indecees[k]]= sources[min_index];
          S[min_index]++;
          min_cost_perm+=min_cost;
         }
     if (min_cost_perm<min_cost_case)
        {
         min_cost_case=min_cost_perm;
         for(i=0;i<n;i++)
            ans_case[i]=ans_perm[i];
        }
     if(y>1)
        next_permutation(temp1,site_indecees,n,m);
     y--;
    }
   while (y>0);
   for(j=0;j<m;j++)
      {
       min_cost_case+=conc_cost[j];
       min_sources_final[j]=sources[j];
      }
   min_cost_final=min_cost_case;
   for(i=0;i<n;i++)
       ans_final[i]=ans_case[i];
   free(S);
   free(sources);

   for(x=0;x<m;x++)
      for(y=0;y<n;y++)
        *(temp1+y*m+x)=*(C+y*m+x);

   int *max_dist_temp1;
   max_dist_temp1=(int*)malloc(m*sizeof(int));
   for(x=0;x<m;x++)
      max_dist_temp1[x]=max_no_of_dis_from_1src[x];

   float *temp2;
   int *max_dist_temp2,*min_sources_ith_concentrators,z;
   i=m-1; //current number of concenterators

   while(true)
       {
        S=(int*)malloc(i*sizeof(int));
        min_cost_ith_concentrators=10000;
        temp2=(float*)malloc(i*n*sizeof(float));
        max_dist_temp2=(int*)malloc(i*sizeof(int));
        sources=(int*)malloc(i*sizeof(int));
        min_sources_ith_concentrators=(int*)malloc(i*sizeof(int));

        for(j=1;j<=i;j++)
          {
            min_cost_case=10000;
            sum=0;
            for (x=0;x<=i;x++)
                 if(x!=j)
                   sum+=max_dist_temp1[x];
            if(sum<n) continue;
            for(x=0;x<j;x++)
               {
                sources[x]=x;
                max_dist_temp2[x]=max_dist_temp1[x];
                for(y=0;y<n;y++)
                  *(temp2+y*i+x)=*(temp1+y*(i+1)+x);
               }
            for(x=j+1;x<=i;x++)
               {
                sources[x-1]=x;
                max_dist_temp2[x-1]=max_dist_temp1[x];
                for(y=0;y<n;y++)
                  *(temp2+y*i+(x-1))=*(temp1+y*(i+1)+x);
               }

            y=f;
            for (k=0;k<n;k++)
                site_indecees[k]=k;
            do
              {
               min_cost_perm=0;
               for(k=0;k<i;k++)
                   S[k]=0;
               for(k=0;k<n;k++)
                   {
                    for(x=0;x<i;x++)
                       if(S[x]<max_dist_temp2[x])
                         {
                          min_cost=*(temp2+k*i+x);
                          min_index=x;
                          break;
                         }
                    for(z=x+1;z<i;z++)
                       if (*(temp2+k*i+z)<min_cost && S[z]<max_dist_temp2[z])
                          {
                           min_cost=*(temp2+k*i+z);
                           min_index=z;
                          }
                    ans_perm[site_indecees[k]]= sources[min_index];
                    S[min_index]++;
                    min_cost_perm+=min_cost;
                   }
               if (min_cost_perm<min_cost_case)
                  {
                   min_cost_case=min_cost_perm;
                   for(x=0;x<n;x++)
                      ans_case[x]=ans_perm[x];
                  }
               if(y>1)
                  next_permutation(temp2,site_indecees,n,i);
               y--;
              }
            while (y>0);
            for(x=0;x<i;x++)
               min_cost_case+=conc_cost[sources[x]];
            if(min_cost_case<min_cost_ith_concentrators)
              {
               min_cost_ith_concentrators=min_cost_case;
               for(x=0;x<n;x++)
                  ans_ith_concentrators[x]=ans_case[x];
               for(x=0;x<i;x++)
                  min_sources_ith_concentrators[x]=sources[x];
              }
          }
       free(S);
       free(temp1);
       free(temp2);
       free(max_dist_temp2);
       free(sources);
       if(min_cost_ith_concentrators<min_cost_final)
          {
           min_cost_final=min_cost_ith_concentrators;
           for(x=0;x<n;x++)
               ans_final[x]=ans_ith_concentrators[x];
           free(min_sources_final);
           min_sources_final=(int*)malloc(i*sizeof(int));
           for(x=0;x<i;x++)
               min_sources_final[x]=min_sources_ith_concentrators[x];
          }
       else  {free(min_sources_ith_concentrators); break;}
       temp1=(float*)malloc(n*i*sizeof(float));
       for(x=0;x<i;x++)
          for(y=0;y<n;y++)
            *(temp1+y*i+x)=*(C+y*m+min_sources_final[x]);
       i--;
       free(min_sources_ith_concentrators);
      };

   printf("\nThe chosen concenterators:");
   for(x=0;x<i+1;x++)
      printf("\nConcenterator%d",min_sources_final[x]+1);
   print_result_values(ans_final,n,m);
   printf("\n\nThe minimum cost is:%d",min_cost_final);
   getch();
   free(C);
   free(conc_cost);
   free(max_no_of_dis_from_1src);
   free(max_dist_temp1);
   free(temp1);
   free(site_indecees);
   free(ans_perm);
   free(ans_case);
   free(ans_ith_concentrators);
   free(ans_final);
   free(min_sources_final);
}

/*
  Calculate the minimum cost when there exist: m concentrators --> min. no. of concentrators

  minimum number of concentrators =
  ceil(number of sites /
       maximum number of sites that could be served by one concentrator )

  m concentrators --> 1 case         min. cost =c1
  m-1 concentrators -->m-1 case      min. cost =c2
  m-2 concentrators -->m-2 case      min. cost =c3
  .                                  min. cost =c4
  .
  .
  min. no. of concentrators

  note: if c1>c2>c3<c4  --> break the loop cause c3 is the lowest cost
        if there exists a specific site doesn't have a concentrator to be
        connected with -->break where the cost will be infinite

  There exists an array representing the no. of sites served by each source:
  initially S[i]=0;

  In each case:
  1-Get all the permutations of the rows.
  2-for each permutation find the minimum cost:
    for the first row,  choose the minimum (of the source i --> s[i]= 1)
    for the second row, choose the minimum
    for the third row,  choose the minimum

    for any row if s[i] (where i is the index of the minimum) is equal to
    the maximum number of sites that could be served by one concentrator
    then try the following minimum till s[i] < the maximum number of sites
    that could be served by one concentrator.
*/

