// BCP.cpp : 定义控制台应用程序的入口点。
//#pragma once
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <stdio.h>
//#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <string.h>
#include <time.h>
#include <ctime>
#include <vector>
#include <math.h>
//#include <conio.h>
#include <ctype.h>
using namespace std;
/***************************************************************************/
/**************************** 1. Global variables **************************/
/***************************************************************************/
#define number_pop  20
#define max_iteration 6000

typedef struct Adjacent_Matrix{
         int neighbor ;
         struct Adjacent_Matrix *next ;
         }Adjacent ;

typedef struct POP_Class{
	int *p;
	int value;
	int distance;
	double G_x;
}POP_Class;

typedef struct PairSet{
	int i;
	int j;
	int number ; 
}Pair; 

Adjacent * *A_Matrix ;         
Adjacent *p1, *q1; 
std::string File_Name;
std::string Output_File_Name;
std::string Solution_File_Name;
int Current_Run;
unsigned int Current_Seed;
int N, K, G_K;  // node number and color number
int f, f_best, fp;
double total_time, starting_time; 
int * Color; // color array for each vertex
int * Best_Color ; // color array for each vertex
int * Best_Color_so_far;
int ** Delta_Matrix; 
int ** Delta_Matrix1;
int ** Delta_MatrixP; // incremental matrix for each vertex being colored each color
int ** Edge;   // adjacent matrix
int ** D;  //constraint matrix
int ** P; 
int ** TabuTenure;  // tabu tenure
int ** Freq; //
int maxFreq; 
int * V; //vertex 
int *Neibor_number;
POP_Class pop[number_pop];
POP_Class solution_best; 
POP_Class off_spring;
int **Pair_Set; 
Pair *pair_s; 
float *value_best, *value_ave;
int alpha0 ; 
int alpha ;  
double gema ; 
int T_max; 
float *t1;
int times;
int Nedges; 
double pho_e;
int max_p_e; 

/***************************************************************************/
/************************** 2. Inputing and outputing **********************/
/***************************************************************************/
void inputing()
{
     int i, x, y, x1, x2,d,v,vv;
	 ifstream FIC;
     FIC.open(File_Name.c_str());
     if ( FIC.fail() )
     {
           cout << "### Erreur open, File_Name " << File_Name << endl;
           exit(2);
     }
     char StrReading[100];
     FIC >> StrReading;
     if ( FIC.eof() )
     {
           cout << "### Error open, File_Name " << File_Name << endl;
           exit(2);
     }
     //int nb_vtx=0 ;
     int nb_edg=-1, max_edg=0;
     while ( ! FIC.eof() )
     {
           char bidon[50];
           if ( strcmp(StrReading, "p" )==0 )
           {
              FIC >> bidon >> N >> nb_edg;
              //cout << "Number of vertexes = " << N << endl;
              //cout << "Number of edges = " << nb_edg << endl;
              Nedges = nb_edg;
              Color = new int[N];
              Best_Color = new int[N];      
              Best_Color_so_far= new int [N];
              for( x = 0 ; x < N ; x++ ) 
                 {
                   Color[x] = rand() % K ;
                  // Move_Freq[ x ] = 0 ;
                 } 

              Edge=new int*[N];
              for (x = 0 ; x < N ; x++ ) 
                  Edge[x]= new int[N];
              D = new int*[N];
              for (x = 0 ; x < N ; x++ ) 
                  D[x]= new int[N];
              P = new int*[N];
              for (x = 0 ; x < N ; x++ ) 
                  P[x]= new int[N]; 
			  V=new int[N];
              for (x = 0 ; x < N ; x++ ) 
                V[x]=0;
              A_Matrix = new Adjacent *[N];
              for( i = 0 ; i < N; i ++ )
                 {
                   A_Matrix[ i ] = new Adjacent ;
                   A_Matrix[ i ]->neighbor = 0 ; 
                   A_Matrix[ i ]->next = NULL ;
                 }  
              
              Delta_Matrix=new int*[N];
              for (x=0;x<N;x++) Delta_Matrix[x]=new int[K];
              Delta_Matrix1=new int*[N];
			  for (x=0;x<N;x++) Delta_Matrix1[x]=new int[K];
              
              Delta_MatrixP=new int*[N];
              for (x=0;x<N;x++) Delta_MatrixP[x]=new int[K];
              
              TabuTenure=new int*[N];
              for (x=0;x<N;x++) TabuTenure[x]=new int[K];

			  Freq=new int*[N];
              for (x = 0; x < N; x++) Freq[x]=new int[K];

              for (x=0;x<N;x++)
                for (y=0;y<N;y++)
                    {
                      Edge[x][y]=0;
					  D[x][y]=0;
					  P[x][y] =0;
                    } 
			}
           
		   if ( strcmp(StrReading, "e")==0)
           {
                 FIC >> x1 >> x2>>d ;
                 // cout << x1 << x2 << endl;
                 x1--; x2--;  // one shift
                 if ( x1<0 || x2<0 || x1>=N || x2 >=N )
                 {
                       cout << "### Error of node : x1="
                            << x1 << ", x2=" << x2 << endl;
                       exit(2);
                 }
				 if(x1!=x2)
				 {
                    Edge[x1][x2]=Edge[x2][x1]=1;
				    D[x1][x2]=D[x2][x1]=d;
                 max_edg++;
                 // add x2 to x1's neighbor list
                 p1 = A_Matrix[ x1 ] ;
                 A_Matrix[ x1 ]->neighbor ++ ;
                 while( p1->next != NULL )
                      p1 = p1->next;   
                 q1 = new Adjacent ;
                 q1->neighbor = x2 ;
                 q1->next = NULL ;  
                 p1->next = q1 ;
                 
                 // add x1 to x2's neighbor list
                 p1 = A_Matrix[ x2 ] ;
                 A_Matrix[ x2 ]->neighbor ++ ;
                 while( p1->next != NULL )
                    p1 = p1->next;          
                 q1 = new Adjacent ;
                 q1->neighbor = x1 ;
                 q1->next = NULL ;  
                 p1->next = q1;
           }
		   }
	          if ( strcmp(StrReading, "n")==0 )
			  {
	              FIC >> v>>vv;
				  if (v < 1 || v > N)
                  {
                      cerr << "### Error of node weight index: " << v << endl;
                      exit(2);
                  }
				  V[v-1]=vv;
	          }

           FIC >> StrReading;
     }


	 //for(i=0;i<N;i++) printf(" %d\n ",A_Matrix[ i ]->neighbor);
    // cout << "Density = " << (float) 2 * max_edg/(N*(N-1)) << endl;
     if ( 0 && max_edg != nb_edg )
     {
           cout << "### Error de lecture du graphe, nbre aretes : annonce="
                 << nb_edg << ", lu=" << max_edg  << endl;
           exit(2);
     }
   
     FIC.close();
}

bool Valid_Coloring(const int *C)
{
    for (int i = 0; i < N; ++i)
        if (C[i] < 0 || C[i] >= K)
            return false;

    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (Edge[i][j] != 0 && abs(C[i] - C[j]) < D[i][j])
                return false;

    return true;
}

void WriteDate(int *C, const std::string &, int K, int f)
 { 
	if (!Valid_Coloring(C))
    {
        cerr << "Internal error: refusing to write an invalid coloring" << endl;
        exit(3);
    }
	ofstream output(Solution_File_Name.c_str(), ios::app);
    if (!output)
    {
        cerr << "Cannot open solution file: " << Solution_File_Name << endl;
        exit(2);
    }
    output << "run=" << Current_Run << " seed=" << Current_Seed
           << " k=" << K << " f=" << f << '\n';
	for(int i=0;i<N;i++)
        output << C[i] << (i + 1 == N ? '\n' : ' ');
 }//end writedate function
/***************************************************************************/
/************* 3. Assign and relax the memery needed ***********************/
/***************************************************************************/
void Assign_Memery(POP_Class pop[number_pop],POP_Class *solution_best, POP_Class *off_spring) 
{
     int i,j; 
    for(i=0;i<number_pop; i++)
	  pop[i].p= new int [N]; 
	(*solution_best).p = new int [N]; 
	(*off_spring).p = new int [N];  
    Pair_Set = new int * [number_pop]; 
	for(i=0;i<number_pop;i++) 
	    Pair_Set[i] = new int [number_pop]; 
    for(i=0; i<number_pop; i++)
       for(j=0;j<number_pop;j++) Pair_Set[i][j] = 0; 
    pair_s =new Pair [number_pop*number_pop/2]; 
    value_best = new float [max_iteration];
	for(i=0;i<max_iteration;i++) value_best[i]=0.0;
	value_ave = new float [max_iteration];
   for(i=0;i<max_iteration;i++) value_ave[i]=0.0;
	t1 = new float [max_iteration];
   for(i=0;i<max_iteration;i++) t1[i]=0.0;
}

void DeleteMemery(POP_Class pop[number_pop],POP_Class *solution_best, POP_Class *off_spring) 
{
    int i; 
    for(i=0;i<number_pop; i++)
	   delete [] pop[i].p; 
	delete [] (*solution_best).p; 
	delete [] (*off_spring).p ;  
    for(i=0; i<number_pop; i++) delete [] Pair_Set[i];
	delete [] Pair_Set;
	delete [] pair_s; 
	delete [] value_best;
	delete [] value_ave; 
	delete [] t1;

    for (i = 0; i < N; ++i)
    {
        delete [] Edge[i];
        delete [] D[i];
        delete [] P[i];
        delete [] Delta_Matrix[i];
        delete [] Delta_Matrix1[i];
        delete [] Delta_MatrixP[i];
        delete [] TabuTenure[i];
        delete [] Freq[i];

        Adjacent *node = A_Matrix[i];
        while (node != NULL)
        {
            Adjacent *next = node->next;
            delete node;
            node = next;
        }
    }
    delete [] Edge;
    delete [] D;
    delete [] P;
    delete [] Delta_Matrix;
    delete [] Delta_Matrix1;
    delete [] Delta_MatrixP;
    delete [] TabuTenure;
    delete [] Freq;
    delete [] A_Matrix;
    delete [] Color;
    delete [] Best_Color;
    delete [] Best_Color_so_far;
    delete [] V;
	delete [] Neibor_number;
}

/***************************************************************************/
/************************ 4. Tabu Search procedure  ************************/
/***************************************************************************/
//4.1 Clear data for TS
void Clear_Delta_Matrix( )
{
   int x, v ;
   f = 0;
   for( x = 0 ; x < N ; x++ )
     for( v = 0 ; v < K ; v++ )
        Delta_Matrix[ x ][ v ] = 0 ;

   for( x = 0 ; x < N ; x++ )
     for( v = 0 ; v < K ; v++ )
	 {
		 TabuTenure[ x ][ v ] = 0 ;
	 }
   return ;     
}
//4.2 Build delta matrix
void Build_Delta_Matrix( )
{
   int i, j,s ;
   Clear_Delta_Matrix( ) ;
   for( i = 0 ; i < N ; i++ )
    for(s=0; s<K; s++)
	    for( j = 0 ; j <N ; j++ )
         if( (i!=j) && Edge[ i ][ j ] != 0 )
           {      
			     if(abs(Color[ j ] - s)<D[i][j])   Delta_Matrix[ i ][ s ]  +=  (D[i][j]-abs(Color[ j ] - s));
           }
    f=0;
    for( i = 0 ; i < N ; i++ )
       for( j = 0 ; j <i ; j++ )
          if((Edge[ i ][ j ] != 0)&&abs(Color[ i ] - Color[ j ])<D[i][j])
			{   
				//  printf("|ci-cj|=%d, dij=%d\n",abs(Color[ i ] - Color[ j ]),D[i][j]);
                  f += (D[i][j]-abs(Color[ i ] - Color[ j ]));
			}
      // printf("f = %d **********\n", f );
      return ;        
}
//4.3 Update Delta matrix
void One_Move_Update_Delta_Matrix(int i, int v0, int v1)
{
   int j , s;int start,end;
   p1 = A_Matrix[ i ] ;
   while( p1->next != NULL )    
      { 
		    p1 = p1->next ;
            j = p1->neighbor ;
			if((v0-D[i][j]+1)<0)start=0;
			else start=v0-D[i][j]+1;
			if((v0+D[i][j]-1)>K-1)end=K-1;
			else end= v0+D[i][j]-1;
	    	for(s=start; s<=end; s++)
		    {  
				Delta_Matrix[ j ][ s ] -= (D[j][i]-((s>v0)?(s-v0):v0-s));
            }
			if((v1-D[i][j]+1)<0)start=0;
			else start=v1-D[i][j]+1;
           if((v1+D[i][j]-1)>K-1)end=K-1;
			else end= v1+D[i][j]-1;
           for(s=start; s<=end; s++)
		    {
			    Delta_Matrix[ j ][ s ] += (D[j][i]-((s>v1)?(s-v1):v1-s));
            }
       }                                                                        
   return ;     
}
//4.4 Tabu Search Procedure with One Move
int One_Move_Tabu_Search(int Color_into[], int *value)
{
     int i ;
     int non_improve = 0 ;  // the stop condition of TS
     int num_tabu_best, num_best ;  // the number of tabu neighbors and non-tabu neighbors
     int best_x[ 50 ], best_v[ 50 ], x,  v;
     int tabu_best_x[ 50 ], tabu_best_v[ 50 ];
     int iter;
     int tabu_best_delta, best_delta, delt ;
     int old_color ;
    int select ;
	  	
    int t=0,p=0;
    const int p_max = 15; 
    int B[p_max] = {1,2,1,4,1,2,1,8,1,2,1,4,1,2,1};
    int A[p_max];
    int T[p_max];
    for(i=0;i<p_max;i++)
	 {
         A[i] = 4*T_max*B[i]/8; 
         T[i] =  T_max*B[i]/8; 
     }
     
	 //double total_time, starting_time;
	 //starting_time = clock();
    for(i=0;i<N;i++)
	 Color[i] = Color_into[i];
	 
    Build_Delta_Matrix( );
    f_best = f ;
    for (i = 0; i < N; ++i)
        Best_Color[i] = Color[i];
    if(f==0) return f;
    // printf("\n\n");
    // cout << endl << "One_Move :       iter       f       f_best      time " << endl;
   //  cout << "---------------------------------------------------" << endl; 
      
     iter = 0 ; 
     while( non_improve < alpha)
        {
          tabu_best_delta = 9999999 ; 
          best_delta = 9999999 ;
          num_tabu_best = 0 ; 
          num_best = 0 ;	
          for( x = 0 ; x < N ; x++ )
            if( Delta_Matrix[ x ][ Color[ x ] ] )
              {
                for( v = 0 ; v < K ; v++ )
                  if( v != Color[ x ] )
                    {
                      delt = Delta_Matrix[ x ][ v ] - Delta_Matrix[ x ][ Color[ x ] ];
                      if( TabuTenure[ x ][ v ] <= iter ) // if this is not tabued 
                        {
                          if( delt < best_delta )
                           {
                             best_x[ 0 ] = x ; 
                             best_v[ 0 ] = v ;
                             best_delta = delt ; 
                             num_best = 1 ;
                           }
                          else if( delt == best_delta && num_best < 50 )
                           {
                             best_x[ num_best ] = x ; 
                             best_v[ num_best ] = v ;
                             num_best ++ ;
                           }
                         }                                                    
                       else if( TabuTenure[ x ][ v ] > iter )// if it is tabu 
                           { 
                             if( delt < tabu_best_delta  )
                               {
                                 tabu_best_x[ 0 ] = x ; 
                                 tabu_best_v[ 0 ] = v ; 
                                 tabu_best_delta = delt ; 
                                 num_tabu_best = 1 ;
                               }
                             else if( delt == tabu_best_delta && num_tabu_best < 50 )
                               {
                                 tabu_best_x[ num_tabu_best ] = x ; 
                                 tabu_best_v[ num_tabu_best ] = v ; 
                                 num_tabu_best ++ ;
                               }                               
                           }
                     }
                 }        
                 if (num_best == 0 && num_tabu_best == 0)
                     break;
                 //choose the tabu best move if the tab aspiration criterion is satisfied
                 if( ( num_tabu_best > 0 && tabu_best_delta < best_delta && ( f + tabu_best_delta < f_best ) ) || num_best == 0 )  // aspiration criterion 
                   {
                     f += tabu_best_delta ;
                     select = rand( ) % num_tabu_best ;  
                    
                     
                     old_color = Color[ tabu_best_x[ select ] ] ;              
                     One_Move_Update_Delta_Matrix( tabu_best_x[ select ], old_color, tabu_best_v[ select ] );
                     Color[ tabu_best_x[ select ] ] = tabu_best_v[ select ] ;


                     
                     TabuTenure[ tabu_best_x[ select ] ][ old_color ] =   T[p] + rand()%3 ;  
				     TabuTenure[ tabu_best_x[ select ] ][ old_color ] += iter ; 
	                 t++;
			         if( t > A[p] ){  p=(p+1)% p_max;   t=0;  }					 
                     
                   } 
                 else //choose the best non tabu move
                   {
                     f += best_delta ; 
                     select = rand( ) % num_best ;    
                     old_color = Color[ best_x[ select ] ] ;              
                     One_Move_Update_Delta_Matrix( best_x[ select ], old_color, best_v[ select ] );
                     Color[ best_x[ select ] ] = best_v[ select ] ;

                     TabuTenure[ best_x[ select ] ][ old_color ] =  T[p] + rand()%3 ; 
				     TabuTenure[ best_x[ select ] ][ old_color ] += iter ; 	 
                     t++;
			         if( t > A[p] ){  p=(p+1)% p_max;   t=0;  }
                   } 
		
                 iter ++ ;
                 //Total_Iterations ++ ;
                 //total_time = (clock() - starting_time )/CLOCKS_PER_SEC ;
                 if( f <= f_best )
                   {
                     if( f < f_best )
                       {
                         f_best = f;
                         for( i = 0 ; i < N ; i ++ )
                              Best_Color[ i ] = Color[ i ] ;

                             //  printf("\n One_Move :  %8d       %3d       %3d       %5.3lf s", iter, f, f_best, total_time );
                         
                         non_improve = 0 ;
                        }  
                     else if ( f == f_best )   
                         non_improve ++ ;
                     if( f_best == 0 )
                        {
                            //printf("\n == %d      %d      %d  ", iter, f, f_best);
                            for(i=0;i<N;i++)
                            Color_into[i] = Best_Color[ i ];
                            *value = f_best; 
                          return f_best;
                        }  
                   }  
                 else   non_improve ++ ;   
     }
     
	 for(i=0;i<N;i++)
        Color_into[i] = Best_Color[ i ];
	 *value = f_best; 
     return f_best;
}

void One_Move_Update_Delta_Matrix1(int i, int v0, int v1)
{
   int j , s;int start,end;
   p1 = A_Matrix[ i ] ;
   while( p1->next != NULL )    
      { 
		    p1 = p1->next ;
            j = p1->neighbor ;
			if((v0-D[i][j]+1)<0)start=0;
			else start=v0-D[i][j]+1;
			if((v0+D[i][j]-1)>K-1)end=K-1;
			else end= v0+D[i][j]-1;
	    	for(s=start; s<=end; s++)
		    {  
				Delta_Matrix1[ j ][ s ] -= (D[j][i]-((s>v0)?(s-v0):v0-s));
            }
			if((v1-D[i][j]+1)<0)start=0;
			else start=v1-D[i][j]+1;
           if((v1+D[i][j]-1)>K-1)end=K-1;
			else end= v1+D[i][j]-1;
           for(s=start; s<=end; s++)
		    {
			    Delta_Matrix1[ j ][ s ] += (D[j][i]-((s>v1)?(s-v1):v1-s));
            }
       }                                                                        
   return ;     
}

//4.5 Tabu Search Procedure with One Move for optimizing the modified objective function

void Clear_Delta_Matrix_P( )
{
	int x, v ;
	for( x = 0 ; x < N ; x++ )
      for( v = 0 ; v < K ; v++ )
      Delta_MatrixP[ x ][ v ] = 0 ;
	  return ;     
}

//2.2 Build delta matrix
void Build_Delta_Matrix_P( )
{
	int i, j,s ;
	Clear_Delta_Matrix_P( ) ;
	
	for( i = 0 ; i < N ; i++ )
		for(s=0; s<K; s++)
			for( j = 0 ; j <N ; j++ )
			{
				if( (i!=j) && Edge[ i ][ j ] != 0 )
				{      
					if(abs(Color[ j ] - s)<D[i][j])   Delta_MatrixP[ i ][ s ]  +=  P[i][j];
				}
			}
			fp=0;
			for( i = 0 ; i < N ; i++ )
				for( j = i+1 ; j <N ; j++ )
				{
					if((Edge[ i ][ j ] != 0)&&abs(Color[ i ] - Color[ j ])<D[i][j])
					{   
						
						fp += P[i][j];
					}
				}
				//printf("fp = %d ********** \n", fp);
				return ;        
}
void One_Move_Update_Delta_MatrixP(int i, int v0, int v1)
{
   int j , s;int start,end;
   p1 = A_Matrix[ i ] ;
   while( p1->next != NULL )    
      { 
		    p1 = p1->next ;
            j = p1->neighbor ;
			if((v0-D[i][j]+1)<0)start=0;
			else start=v0-D[i][j]+1;
			if((v0+D[i][j]-1)>K-1)end=K-1;
			else end= v0+D[i][j]-1;
	    	for(s=start; s<=end; s++)
		    {  
				Delta_MatrixP[ j ][ s ] -= P[j][i];
            }
			if((v1-D[i][j]+1)<0)start=0;
			else start=v1-D[i][j]+1;
           if((v1+D[i][j]-1)>K-1)end=K-1;
			else end= v1+D[i][j]-1;
           for(s=start; s<=end; s++)
		    {
			    Delta_MatrixP[ j ][ s ] +=  P[j][i];
            }
       }                                                                        
   return ;     
}

int One_Move_Tabu_Search_Penalty(int Color_into[], int *value)
{
     int i ;
     int non_improve = 0 ;  // the stop condition of TS
     int num_tabu_best, num_best ;  // the number of tabu neighbors and non-tabu neighbors
     int best_x[ 50 ], best_v[ 50 ], x,  v;
     int tabu_best_x[ 50 ], tabu_best_v[ 50 ];
     int iter;
     int tabu_best_delta, best_delta, delt , delt1, delt2;
     int old_color ;
     int select ;
	 
	int t=0,p=0;
    const int p_max = 15; 
    int B[p_max] = {1,2,1,4,1,2,1,8,1,2,1,4,1,2,1};
    int A[p_max];
    int T[p_max];
    for(i=0;i<p_max;i++)
	 {
         A[i] = 4*T_max*B[i]/8; 
         T[i] =  T_max*B[i]/8; 
     }
    
    //double total_time, starting_time;
	// starting_time = clock();
     for(i=0;i<N;i++)
	 Color[i] = Color_into[i];
     Build_Delta_Matrix( );
     Build_Delta_Matrix_P( );
     f = f + fp;
     f_best = f;
     for (i = 0; i < N; ++i)
         Best_Color[i] = Color[i];
     // printf("\n");
     // cout << endl << "One_Move :       iter       f       f_best      time " << endl;
     //  cout << "---------------------------------------------------" << endl; 
      
     iter = 0 ; 
     while( non_improve < alpha0)
        {
          tabu_best_delta = 9999999 ; 
          best_delta = 9999999 ;
          num_tabu_best = 0 ; 
          num_best = 0 ;	
          for( x = 0 ; x < N ; x++ )
            if( Delta_Matrix[ x ][ Color[ x ] ]!=0 || Delta_MatrixP[ x ][ Color[ x ]]!=0 )
              {
                for( v = 0 ; v < K ; v++ )
                  if( v != Color[ x ] )
                    {
                      delt1 = Delta_Matrix[ x ][ v ] - Delta_Matrix[ x ][ Color[ x ] ];
                      delt2 = Delta_MatrixP[ x ][ v ] - Delta_MatrixP[ x ][ Color[ x ] ];
                      delt = delt1 + delt2;
                      if( TabuTenure[ x ][ v ] <= iter ) // if this is not tabued 
                        {
                          if( delt < best_delta )
                           {
                             best_x[ 0 ] = x ; 
                             best_v[ 0 ] = v ;
                             best_delta = delt ; 
                             num_best = 1 ;
                           }
                          else if( delt == best_delta && num_best < 50 )
                           {
                             best_x[ num_best ] = x ; 
                             best_v[ num_best ] = v ;
                             num_best ++ ;
                           }
                         }                                                    
                       else if( TabuTenure[ x ][ v ] > iter )// if it is tabu 
                           { 
                             if( delt < tabu_best_delta  )
                               {
                                 tabu_best_x[ 0 ] = x ; 
                                 tabu_best_v[ 0 ] = v ; 
                                 tabu_best_delta = delt ; 
                                 num_tabu_best = 1 ;
                               }
                             else if( delt == tabu_best_delta && num_tabu_best < 50 )
                               {
                                 tabu_best_x[ num_tabu_best ] = x ; 
                                 tabu_best_v[ num_tabu_best ] = v ; 
                                 num_tabu_best ++ ;
                               }                               
                           }
                     }
                 }        
                 if (num_best == 0 && num_tabu_best == 0)
                     break;
                 //choose the tabu best move if the tab aspiration criterion is satisfied
                 if( ( num_tabu_best > 0 && tabu_best_delta < best_delta && ( f + tabu_best_delta < f_best ) ) || num_best == 0 )  // aspiration criterion 
                   {
                     f += tabu_best_delta ;
                     select = rand( ) % num_tabu_best ;  
              
                     
                     old_color = Color[ tabu_best_x[ select ] ] ;              
                     One_Move_Update_Delta_Matrix( tabu_best_x[ select ], old_color, tabu_best_v[ select ] );
                     One_Move_Update_Delta_MatrixP( tabu_best_x[ select ], old_color, tabu_best_v[ select ] );
                     Color[ tabu_best_x[ select ] ] = tabu_best_v[ select ] ;

                     TabuTenure[ tabu_best_x[ select ] ][ old_color ] =   T[p] + rand()%3 ;  
				     TabuTenure[ tabu_best_x[ select ] ][ old_color ] += iter ; 
	                 t++;
			         if( t > A[p] ){  p=(p+1)% p_max;   t=0;  }	
                   } 
                 else //choose the best non tabu move
                   {
                     f += best_delta ; 
                     select = rand( ) % num_best ;    
                     old_color = Color[ best_x[ select ] ] ;              
                     One_Move_Update_Delta_Matrix( best_x[ select ], old_color, best_v[ select ] );
                     One_Move_Update_Delta_MatrixP( best_x[ select ], old_color, best_v[ select ] );
                     Color[ best_x[ select ] ] = best_v[ select ] ;

                     TabuTenure[ best_x[ select ] ][ old_color ] =  T[p] + rand()%3 ; 
				     TabuTenure[ best_x[ select ] ][ old_color ] += iter ; 	 
                     t++;
			         if( t > A[p] ){  p=(p+1)% p_max;   t=0;  }
                   } 
		
                 iter ++ ;
                 //Total_Iterations ++ ;
                 
                // total_time = (clock() - starting_time )/CLOCKS_PER_SEC ;

                 if( f <= f_best )
                   {
                     if( f < f_best )
                       {
                         f_best = f;
                         for( i = 0 ; i < N ; i ++ )
                              Best_Color[ i ] = Color[ i ] ;

                       // printf("\n One_Move :  %8d      %3d      %3d     %5.3lf ", iter, f, f_best, total_time );
                         
                         non_improve = 0 ;
                       }  
                     else if ( f == f_best )   
                         non_improve ++ ;
                     if( f_best == 0 )
                        {
                            //printf("\n == %d      %d      %d  ", iter, f, f_best);
                            for(i=0;i<N;i++)
                            Color_into[i] = Best_Color[ i ];
                            *value = f_best; 
                          return f_best;
                        }  
                   }  
                 else   
                    non_improve ++ ;   
     }
	 for(i=0;i<N;i++)
        Color_into[i] = Best_Color[ i ];
	 *value = f_best; 
     return f_best;
}
/***************************************************************************/
/************************** 5. Updating penalty function *******************/
/***************************************************************************/
void updatingP(int C[])
{
    int i,j;
    int p_max = 0;
    
    for(i=0;i<N;i++)
    for(j=i+1;j<N;j++)
       if((Edge[ i ][ j ] != 0)&&abs(C[ i ] - C[ j ]) < D[i][j]){ P[i][j] += 1; P[j][i] += 1;}
       
    for(i=0;i<N;i++)
    for(j=i+1;j<N;j++)   
        if((Edge[ i ][ j ] != 0)&& P[i][j] > p_max) p_max = P[i][j]; 
    if(p_max > max_p_e  )
    {
      for(i=0;i<N;i++)
        for(j=i+1;j<N;j++) 
        { 
            P[i][j] =  (int) (pho_e*P[i][j]) ;  
            P[j][i] = P[i][j];
        } 
    }
  
}
/***************************************************************************/
/************************** 6. Relinking operators **************************/
/***************************************************************************/
void Path_Relinking(int *x,int *y, int *off_spring)
{
	int f_min = 9999999;
	int i;
	int j; 
	int *NC;
	int *PV;
	int *FI;
	int k=0;
	int r=0;
	int delta=0;
	int min_delta=99999999;
	NC = new int [N+1];
	for(i=0;i<N;i++) NC[i]=0; 
	PV = new int [N+1];
	for(i=0;i<=N;i++)PV[i]=-1;
	FI = new int [N+1];
	for(i=0;i<=N;i++)FI[i]=0;
	for(i=0;i<N;i++)Color[i]=x[i];
	Build_Delta_Matrix( );
	FI[0]=f;
    for(i=0;i<N;i++) if(x[i]!=y[i]){ r++; NC[i]=1;}
	for(i=1;i <= r;i++)
	{
	  min_delta = 99999999;
      for(j=0;j<N;j++) 
	  {    
		  if(NC[j]!=0)
		  {
			  delta = Delta_Matrix[j][y[j]] - Delta_Matrix[j][x[j]]; 
			  if(delta < min_delta) { k = j; min_delta = delta;}
		  }	  
	  }
	  FI[i] = FI[i-1] + min_delta;
      One_Move_Update_Delta_Matrix( k, x[k], y[k]); 
      NC[k] = 0; 
	  PV[i] = k;

	}
	int s = 0; 
	f_min=9999999;

	for(i=(int)(gema*r);i<(int)((1.0-gema)*r);i++) 
	{   
		if(FI[i] < f_min){ f_min = FI[i]; s = i;}
	}

	for(i=0;i<N;i++) off_spring[i]=x[i];
	for(i=1;i<=s;i++) off_spring[PV[i]]=y[PV[i]]; 

	delete [] NC;
	delete [] FI;
	delete [] PV;
}
void Path_Relinking1(int *x,int *y, int *off_spring)
{
 
	int f_min = 9999999;
	int i;
	int *NC;
	int *PV;
	int *FI;
	int k=0;
	int r=0;
	int delta=0;
	NC = new int [N+1];
	for(i=0;i<N;i++) NC[i]=0; 
	PV = new int [N+1];
	for(i=0;i<=N;i++)PV[i]=-1;
	FI = new int [N+1];
	for(i=0;i<=N;i++)FI[i]=0;
	for(i=0;i<N;i++)Color[i]=x[i];
	Build_Delta_Matrix( );
	FI[0]=f;
    for(i=0;i<N;i++) if(x[i]!=y[i]){ r++; NC[i]=1;}
	for(i=1;i <= r;i++)
	{

      while(1)
	  {   
		  k = rand() % N;
		  if(NC[k]!=0)
		  { 
			delta = Delta_Matrix[k][y[k]] - Delta_Matrix[k][x[k]];
			FI[i] = FI[i-1] + delta;
            One_Move_Update_Delta_Matrix( k, x[k], y[k]); 
            NC[k] = 0; 
	        PV[i] = k; 
            break;
		  }

	  }

	}

	int s = 0; 
	f_min=9999999;
   
	for(i=(int)(gema*r);i<(int)((1.0-gema)*r);i++) 
	{   
		if(FI[i] < f_min){ f_min = FI[i]; s = i;}
	}
	//for(i=1;i<=r;i++) { if(i%10==0)printf("\n");  printf ("f(x[%d])= %d ", i, FI[i]);}
    //printf("f_min = %d \ n ", f_min); 
	for(i=0;i<N;i++) off_spring[i]=x[i];
	for(i=1;i<=s;i++) off_spring[PV[i]]=y[PV[i]]; 
     
	delete [] NC;
	delete [] FI;
	delete [] PV;
}
void Path_Relinking2(int *x,int *y, int *off_spring)
{
	int f_min = 9999999;
	int i,j;
	int *NC;
	int *NC1;
	int *PV;
	int *FI;
	int k=0;
	int r=0;
	int delta=0;
	NC = new int [N+1];
	for(j=0;j<N;j++) NC[j]=0; 
	NC1 = new int [N+1];
    for(j=0;j<=N;j++) NC1[j]=0; 
	int count_number = 0; 
	PV = new int [N+1];
	for(j=0;j<=N;j++)PV[j]=-1;
	FI = new int [N+1];
	for(j=0;j<=N;j++)FI[j]=0;
	for(j=0;j<N;j++)Color[j]=x[j];
	Build_Delta_Matrix( );
	FI[0]=f;
    for(j=0;j<N;j++) if(x[j]!=y[j]){ r++; NC[j]=1;}
	for(i=1;i <= r;i++)
	{    
		count_number = 0;
		for(j=0;j<N;j++) if( NC[j]!=0 ) { NC1[count_number] = j; count_number++; }
		k = NC1[rand() % count_number]; 
		if(NC[k]!=0)
		{ 
			delta = Delta_Matrix[k][y[k]] - Delta_Matrix[k][x[k]];
			FI[i] = FI[i-1] + delta;
			One_Move_Update_Delta_Matrix( k, x[k], y[k]); 
			NC[k] = 0; 
			PV[i] = k; 	
		}
	//	printf("f= %d \n",FI[i]);
	}
	
	int s = 0; 
	f_min=9999999;
	
	for(i=(int)(gema*r);i<(int)((1.0-gema)*r);i++) 
	{   
		if(FI[i] < f_min){ f_min = FI[i]; s = i;}
	}

	for(i=0;i<N;i++) off_spring[i]=x[i];
	for(i=1;i<=s;i++) off_spring[PV[i]]=y[PV[i]]; 
	
	delete [] NC;
	delete [] FI;
	delete [] PV;
}

void Mixed_Path_Relinking(int *x,int *y, int *off_spring)
{
	
	int count1 = 0, count2 = 0; 
	int i;
	int j; 
	int *NC;
	int *PV;
	int *PV1;
	int *FI;
	int *FI1;
	int k=0;
	int r=0;
	int delta=0;
	int min_delta=99999999;
	NC = new int [N+1];
	for(i=0;i<N;i++) NC[i]=0; 
	PV = new int [N+1];
	for(i=0;i<=N;i++)PV[i]=-1;
	PV1 = new int [N+1];
	for(i=0;i<=N;i++)PV1[i]=-1;
	FI = new int [N+1];
	for(i=0;i<=N;i++)FI[i]=0;
	FI1 = new int [N+1];
	for(i=0;i<=N;i++)FI1[i]=0;

	for(i=0;i<N;i++)Color[i]=x[i];
	Build_Delta_Matrix( );
	for(i=0;i<N;i++)
		for(j=0;j<K;j++) 
			Delta_Matrix1[i][j] = Delta_Matrix[i][j]; 
	FI1[0]=f;
	
	for(i=0;i<N;i++)Color[i]=y[i];
	Build_Delta_Matrix( );
	FI[0]=f;
	
	for(i=0;i<N;i++) if(x[i]!=y[i]){ r++; NC[i]=1;}
	for(i=1;i <= r;i++)
	{
		min_delta = 99999999;

		if(i%2==1)
		{
			for(j=0;j<N;j++) 
			{    
				if(NC[j]!=0)
				{
					delta = Delta_Matrix1[j][y[j]] - Delta_Matrix1[j][x[j]]; 
					if(delta < min_delta) { k = j; min_delta = delta;}
				}	  
			}
			FI1[count1+1] = FI1[count1] + min_delta;
			One_Move_Update_Delta_Matrix1( k, x[k], y[k]); 
			NC[k] = 0; 
			PV1[count1] = k;
			count1++;
			
		}
		else 
		{
			for(j=0;j<N;j++) 
			{    
				if(NC[j]!=0)
				{
					delta = Delta_Matrix[j][x[j]] - Delta_Matrix[j][y[j]]; 
					if(delta < min_delta) { k = j; min_delta = delta;}
				}	  
			}
			FI[count2 + 1] = FI[count2] + min_delta;
			One_Move_Update_Delta_Matrix( k, y[k], x[k]); 
			NC[k] = 0; 
			PV[count2] = k;
			count2++; 
			
		}

	}
//	for(i=0;i<count1;i++)  printf("x[%d]= %d \n",i,FI1[i]);
	
 //   for(i=count2;i>=0;i--) printf("y[%d]= %d \n",i,FI[i]);

	for(i=0;i<N;i++) off_spring[i]=x[i];
	for(i=1;i<=count1;i++) off_spring[PV1[i]] = y[PV1[i]]; 
 //   printf("r=%d,   count1= %d \n", r, count1);
//	r1=0;
//	for(i=0;i<N;i++)if(off_spring[i] != x[i]) r1++;
//	printf("distance to x is %d \n",r1);
//	r1=0;
//	for(i=0;i<N;i++)if(off_spring[i] != y[i])r1++;
//	printf("distance to y is %d \n",r1);

	delete [] NC; NC=NULL;
	delete [] FI; FI=NULL;
	delete [] FI1; FI1= NULL;
	delete [] PV; PV = NULL;
	delete [] PV1; PV1= NULL; 
}

/***************************************************************************/
/************************** 7. population initilization ********************/
/***************************************************************************/
void  pop_initilization() 
{
  
   int i,j;  
   int count;
   int f_max;
   int f_min=99999999;
   int k;
   int k_min;
   int s;
   POP_Class xx; 
   xx.p = new int [N];
  
   for(i=0;i<number_pop;i++)
	 for(j=0;j<N;j++)   pop[i].p[j] = rand()%K;	
	 for(i=0;i<number_pop;i++) 
	 { 
		 One_Move_Tabu_Search(pop[i].p, &(pop[i].value)); 
		 if(pop[i].value==0) {for(j=0;j<N;j++)solution_best.p[j] = pop[i].p[j]; solution_best.value = pop[i].value; delete [] xx.p; return; }
		 
	 } 
   count=0;
   while(count < 2*number_pop)
   {   
	  f_max = -1;
	  k = 0 ;
      for(i=0;i<number_pop;i++) 
	  {
		  if(pop[i].value > f_max) { k=i; f_max = pop[i].value; }
	  }
     for(j=0;j<N;j++)  xx.p[j] = rand()%K;
     One_Move_Tabu_Search(xx.p, &(xx.value)); 
     if(xx.value==0)
	 {
		 for(j=0;j<N;j++)solution_best.p[j] = xx.p[j];
         solution_best.value = xx.value; 
         delete [] xx.p;
		 return; 
	 }
	 if(xx.value < f_max) 
	 {
       for(j=0;j<N;j++)
		   pop[k].p[j]=xx.p[j];
	   pop[k].value = xx.value; 
	 }

	  count++; 
   }

   for(i=0;i<number_pop;i++) 
   {
	   if(pop[i].value < f_min) { k_min = i; f_min = pop[i].value;  }
   }
  
   for(j=0;j<N;j++) solution_best.p[j] = pop[k_min].p[j];   
   solution_best.value = f_min; 


   for(i=0;i<number_pop;i++)
	 for(j=i+1;j<number_pop;j++) Pair_Set[i][j] = 1;
   s=0; 
   for(i=0;i<number_pop;i++)
	   for(j=i+1;j<number_pop;j++) 
	   if(Pair_Set[i][j] ==1) {pair_s[s].i =i; pair_s[s].j =j; s++;} 
   
   pair_s[0].number = s;     
   for(j=0;j<N;j++)  off_spring.p[j] = rand()%K;  
	     off_spring.value = 9999999; 
   delete [] xx.p; 
}

void  pop_initilization1() 
{
  
   int i,j;  
   int count;
   int f_max;
   int fp_max=-99999;
   int k;
   int s;
   int k_max;
   POP_Class xx; 
   xx.p = new int [N];
  
   for(i=0;i<number_pop;i++)
	 for(j=0;j<N;j++)   pop[i].p[j] = rand()%K;	
	 for(i=0;i<number_pop;i++) 
	 { 
		 One_Move_Tabu_Search(pop[i].p, &(pop[i].value)); 
		 if(pop[i].value==0) {for(j=0;j<N;j++) solution_best.p[j] = pop[i].p[j];solution_best.value = pop[i].value; delete [] xx.p; return; }
		 
	 } 
   count=0;
   while(count < 2*number_pop)
   {   
	  f_max = -1;
	  k = 0 ;
      for(i=0;i<number_pop;i++) 
	  {
		  if(pop[i].value > f_max) { k=i; f_max = pop[i].value; }
	  }
     for(j=0;j<N;j++)  xx.p[j] = rand()%K;
     One_Move_Tabu_Search(xx.p, &(xx.value)); 
     if(xx.value==0)
	 {
		 for(j=0;j<N;j++)solution_best.p[j] = xx.p[j];
         solution_best.value = xx.value; 
         delete [] xx.p;
		 return; 
	 }
	 if(xx.value < f_max) 
	 {
       for(j=0;j<N;j++)
		   pop[k].p[j]=xx.p[j];
	   pop[k].value = xx.value; 
	 }

	  count++; 
   }

   for(i=0;i<number_pop;i++) 
   {
	   if(pop[i].value > fp_max) { k_max = i; fp_max = pop[i].value;  }
   }
   
   for(j=0;j<N;j++) pop[k_max].p[j] = solution_best.p[j];   
   pop[k_max].value = solution_best.value ;
   

   for(i=0;i<number_pop;i++)
	   for(j=i+1;j<number_pop;j++) Pair_Set[i][j] = 1;
   s=0; 
   for(i=0;i<number_pop;i++)
	   for(j=i+1;j<number_pop;j++) 
	   if(Pair_Set[i][j] ==1) {pair_s[s].i =i; pair_s[s].j =j; s++;} 
   
   pair_s[0].number = s;    

   for(j=0;j<N;j++)  off_spring.p[j] = rand()%K;  
	     off_spring.value = 9999999; 

   delete [] xx.p; 
   
}

/***************************************************************************/
/****************************** 8. Pool updating ***************************/
/***************************************************************************/
int pop_updating(POP_Class pop[number_pop],POP_Class *off_spring,int *position)
{
   
   int i,j;
   int f_max = -999999; 
   int f_min = 9999999; 
   int k;
   int count;
   int count_min = 999999;
   
   for(i=0;i<number_pop;i++) 
   {
	   if(pop[i].value > f_max) { k=i; f_max = pop[i].value; }
	   if(pop[i].value < f_min) {  f_min = pop[i].value; }
   }
   if((*off_spring).value >= f_max ) return 0;
   
   for(i=0;i<number_pop;i++)
   {
	   count =0;
	   for(j=0;j<N;j++) if(pop[i].p[j]!=(*off_spring).p[j]) count++; 
	   if( count < count_min ) count_min = count ;  	   
   }  
   if( count_min < 0.1*N &&(*off_spring).value >= f_min )  return 0; 
   
   if((*off_spring).value < f_max) 
   {
	  for(j=0;j<N;j++) pop[k].p[j]= (*off_spring).p[j]; 
	  pop[k].value =  (*off_spring).value; 
      (*position) = k;
   }

   if((*off_spring).value < f_max) return 1; 
   else return 0;  
}
int pop_updating2(POP_Class pop[number_pop],POP_Class *off_spring,int *position)
{
	
	int i,j;
	int f_max = -99999; 
	int f_min = 99999;
	int k_max;
	int k_closest;
	int count;
	int similarity=99999;
	
    for(i=0;i<number_pop;i++)
     {
       if(pop[i].value > f_max) { k_max = i; f_max = pop[i].value; } 
       if(pop[i].value < f_min) { f_min = pop[i].value; }
     } 
	if((*off_spring).value > f_max) return 0; // don't need to update 
	
	for(i=0;i<number_pop;i++) 
	{
		count =0;
		for(j=0;j<N;j++) if(pop[i].p[j]!=(*off_spring).p[j]) count++; 
		if( count < similarity ) { similarity= count; k_closest=i;} 	   
	} // find out the most similar solution 
	
	if((*off_spring).value < f_min) 
	{
	    for(j=0;j<N;j++) pop[k_closest].p[j]= (*off_spring).p[j]; 
		pop[k_closest].value =  (*off_spring).value; 
		(*position) = k_closest;    
        return 1;              
    }// deplace the most simlilar solution by the offspring if the offspring is better than the best solution in the population
    if(((*off_spring).value < pop[k_closest].value)&&(similarity <= 0.25*N))
    {
        for(j=0;j<N;j++) pop[k_closest].p[j]= (*off_spring).p[j]; 
		pop[k_closest].value =  (*off_spring).value; 
		(*position) = k_closest;    
        return 1;                                                
    }
    if(((*off_spring).value < f_max)&&(similarity > 0.25*N))
    {
        for(j=0;j<N;j++) pop[k_max].p[j]= (*off_spring).p[j]; 
		pop[k_max].value =  (*off_spring).value; 
		(*position) = k_max;    
        return 1;                                                
    }
    
   return 0;  
}
/***************************************************************************/
/****************************** 9. Updating PairSet ************************/
/***************************************************************************/
void updating_PairSet(int x,int y)
{
   int i,j;
   int s=0;
   Pair_Set[x][y] = 0 ; 
   for(i=0;i<number_pop;i++)
	   for(j=i+1;j<number_pop;j++) 
	   if(Pair_Set[i][j] ==1) {pair_s[s].i =i; pair_s[s].j =j; s++;} 
   
   pair_s[0].number = s;  
}

void updating_PairSet_pop(int position)
{
	int i,j,s=0;
	for(i=position+1;i<number_pop;i++) Pair_Set[position][i] = 1;
	for(j=0;j<position;j++) Pair_Set[j][position] = 1; 
	
	for(i=0;i<number_pop;i++)
	   for(j=i+1;j<number_pop;j++) 
	   if(Pair_Set[i][j] ==1) {pair_s[s].i =i; pair_s[s].j =j; s++;} 
   
    pair_s[0].number = s;  
}
/***************************************************************************/
/*********************** 10. Global parameters Setting *********************/
/***************************************************************************/ 
 void ParameterSetting()
 {
      alpha0  = 2000; 
      alpha   = 10000;  
      gema    = 0.4; 
      pho_e   = 0.4;
	  if(N<=150)
	  { T_max = 50;  max_p_e = 30; }
      else 
	  { T_max = 100;  max_p_e = 50; } 
 }
/***************************************************************************/
/****************************** 11. PR  Algorithm  *************************/
/***************************************************************************/ 
 int PR_Algo()
 {
     int x1,x2; 
	 int x,v;
	 int flag = 0; 
	 int IterationNumber = 0; 
	 int p; 
	 int j;
	 int position;
	
     for(x=0;x<N;x++)
       for(v=0;v<N;v++) P[x][v] = 0;    
     IterationNumber = 0;
	 flag =0; 
	 
	 do{
         //1.initilizing the population
	     if(flag==0)
	     {
           pop_initilization();
           flag = 1; 
	     }
	     else 
	      { 
            pop_initilization1(); 
	      }	 
         IterationNumber++; 
         if(solution_best.value==0)
	      {    
            WriteDate(solution_best.p,File_Name,K,solution_best.value);   
            return 1;       
          }        
	     //2. starting iterate  
	     do{ 
		          p = rand()%pair_s[0].number; 
				  x1= pair_s[p].i;
				  x2= pair_s[p].j;
                  updating_PairSet(x1,x2); 
				  //1. x1------->x2
				  Path_Relinking(pop[x1].p,pop[x2].p,off_spring.p);				
		          One_Move_Tabu_Search_Penalty(off_spring.p, &(off_spring.value)); 
		          One_Move_Tabu_Search(off_spring.p, &(off_spring.value));
    	          updatingP(off_spring.p);  
                  if(off_spring.value < solution_best.value) 
				   {
                    for(j=0; j<N; j++)  solution_best.p[j] = off_spring.p[j];  
		            solution_best.value = off_spring.value;  	
			    	printf("inproved!******"); printf("best=%d\n",solution_best.value);
				  }	
				  
                  if(solution_best.value==0)  
                   {
                     WriteDate(solution_best.p,File_Name,K,solution_best.value);  
                     return 1;  
		           }  
                  if(pop_updating(pop,&off_spring,&position)) 
				  {  
					  updating_PairSet_pop(position);
                  }
                 //2. x2------->x1
                  Path_Relinking(pop[x2].p,pop[x1].p,off_spring.p);				
		          One_Move_Tabu_Search_Penalty(off_spring.p, &(off_spring.value)); 
		          One_Move_Tabu_Search(off_spring.p, &(off_spring.value));
    	          updatingP(off_spring.p);  	
				
                  if(off_spring.value < solution_best.value) 
	              {
                    for(j=0; j<N; j++)  solution_best.p[j] = off_spring.p[j];  
		            solution_best.value = off_spring.value;  	
			    	printf("*****inproved!******"); printf("best=%d \n",solution_best.value);
				  }		
                  if(solution_best.value==0)  
                  {
                     WriteDate(solution_best.p,File_Name,K,solution_best.value);  
                     return 1; 
                  }  
                  if(pop_updating(pop,&off_spring,&position)) 
				  {  
				     updating_PairSet_pop(position); 
                  }	  
                  IterationNumber++; 
         }while(pair_s[0].number!=0 && IterationNumber < 5000);   
			  
        // if(solution_best.value==0) return 1; 

     }while (IterationNumber < 5000);  
     
    return 0; 
 }

/***************************************************************************/
/****************************** 12. Main  Scheme  **************************/
/***************************************************************************/ 
int  main(int argc, char **argv)
{
    if (argc < 4 || argc > 6)
    {
        cerr << "Usage: " << argv[0]
             << " INSTANCE K RUNS [BASE_SEED] [RUN_CSV]" << endl;
        return 2;
    }

    char *end = NULL;
    errno = 0;
    long parsed_k = strtol(argv[2], &end, 10);
    if (errno != 0 || *argv[2] == '\0' || *end != '\0' ||
        parsed_k <= 0 || parsed_k > numeric_limits<int>::max())
    {
        cerr << "K must be a positive integer: " << argv[2] << endl;
        return 2;
    }

    errno = 0;
    long parsed_runs = strtol(argv[3], &end, 10);
    if (errno != 0 || *argv[3] == '\0' || *end != '\0' ||
        parsed_runs <= 0 || parsed_runs > numeric_limits<int>::max())
    {
        cerr << "RUNS must be a positive integer: " << argv[3] << endl;
        return 2;
    }

    unsigned long parsed_seed = static_cast<unsigned long>(time(NULL));
    if (argc >= 5)
    {
        errno = 0;
        parsed_seed = strtoul(argv[4], &end, 10);
        if (errno != 0 || *argv[4] == '\0' || *end != '\0' ||
            parsed_seed > numeric_limits<unsigned int>::max())
        {
            cerr << "BASE_SEED must be an unsigned integer: " << argv[4] << endl;
            return 2;
        }
    }

    File_Name = argv[1];
    K = static_cast<int>(parsed_k);
    times = static_cast<int>(parsed_runs);
    const unsigned int base_seed = static_cast<unsigned int>(parsed_seed);

    if (argc >= 6)
        Output_File_Name = argv[5];
    else
    {
        ostringstream name;
        name << filesystem::path(File_Name).filename().string()
             << "-k" << K << "-seed" << base_seed << ".runs.csv";
        Output_File_Name = name.str();
    }
    Solution_File_Name = Output_File_Name + ".solutions.txt";

    ofstream run_output(Output_File_Name.c_str(), ios::trunc);
    if (!run_output)
    {
        cerr << "Cannot open run CSV: " << Output_File_Name << endl;
        return 2;
    }
    run_output << "run,seed,success,cpu_seconds,wall_seconds\n";
    ofstream solution_output(Solution_File_Name.c_str(), ios::trunc);
    if (!solution_output)
    {
        cerr << "Cannot open solution file: " << Solution_File_Name << endl;
        return 2;
    }
    solution_output.close();

    srand(base_seed);
    inputing();
    Assign_Memery(pop,&solution_best,&off_spring);
    ParameterSetting();

    int succ = 0;
    double successful_cpu_time = 0.0;
    const chrono::steady_clock::time_point all_wall_start = chrono::steady_clock::now();

    for (int i = 0; i < times; ++i)
    {
        Current_Run = i + 1;
        Current_Seed = base_seed + static_cast<unsigned int>(i);
        srand(Current_Seed);

        const clock_t cpu_start = clock();
        const chrono::steady_clock::time_point wall_start = chrono::steady_clock::now();
        const int ret = PR_Algo();
        const chrono::steady_clock::time_point wall_end = chrono::steady_clock::now();
        const clock_t cpu_end = clock();

        const double cpu_seconds = static_cast<double>(cpu_end - cpu_start) / CLOCKS_PER_SEC;
        const double wall_seconds = chrono::duration<double>(wall_end - wall_start).count();
        const bool success = (ret == 1);

        if (success)
        {
            if (!Valid_Coloring(solution_best.p))
            {
                cerr << "Internal error: run " << Current_Run
                     << " returned an invalid coloring" << endl;
                DeleteMemery(pop,&solution_best,&off_spring);
                return 3;
            }
            ++succ;
            successful_cpu_time += cpu_seconds;
        }

        run_output << Current_Run << ',' << Current_Seed << ','
                   << (success ? 1 : 0) << ',' << fixed << setprecision(9)
                   << cpu_seconds << ',' << wall_seconds << '\n';
        run_output.flush();
    }

    const double total_wall_time = chrono::duration<double>(
        chrono::steady_clock::now() - all_wall_start).count();
    cout << "LPR_SUMMARY"
         << " instance=" << File_Name
         << " k=" << K
         << " runs=" << times
         << " successes=" << succ
         << " base_seed=" << base_seed
         << " successful_cpu_total=" << fixed << setprecision(9)
         << successful_cpu_time
         << " successful_cpu_average=";
    if (succ > 0)
        cout << successful_cpu_time / succ;
    else
        cout << "NA";
    cout << " total_wall=" << total_wall_time
         << " run_csv=" << Output_File_Name
         << " solutions=" << Solution_File_Name << endl;

    DeleteMemery(pop,&solution_best,&off_spring);
    return 0;
}
