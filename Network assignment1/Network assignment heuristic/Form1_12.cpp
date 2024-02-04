//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Form1_12.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
In_class *input ;
Calculate *calc ;
//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    input = new In_class() ;
    calc = new Calculate( input ) ;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnInputClick(TObject *Sender)
{
    bool fail = false ;
    String s ;
    int n ;

    input->Initialize() ;
    calc->Initialize() ;
    s = InputBox( "Enter # of concentrators" , "# of concentrators = " , "0" ) ;
    n = s.ToInt() ;
    if( n <= 0 )
        fail = true ;
    else
        input->num_C = n ;

    if( !fail )
    {
        s = InputBox( "Enter # of sites" , "# of sites = " , "0" ) ;
        n = s.ToInt() ;
        if( n <= 0 )
            fail = true ;
        else
            input->num_S = n ;
    }
    if( !fail )
    {
        input->A_Capacity = new int[input->num_C] ;
        for( int i=0 ; i<input->num_C ; i++ )
        {
            if( fail )
                break ;
            s = InputBox( "Enter Capacity" , "C" + String(i) + " Capacity = " , "0" ) ;
            n = s.ToInt() ;
            if( n < 0 )
                fail = true ;
            else
                input->A_Capacity[i] = n ;
        }
    }
    if( !fail )
    {
        input->A_Setup = new int[input->num_C] ;
        for( int i=0 ; i<input->num_C ; i++ )
        {
            if( fail )
                break ;
            s = InputBox( "Enter Setup Cost " , "C" + String(i) + " Setup = " , "0" ) ;
            n = s.ToInt() ;
            if( n < 0 )
                fail = true ;
            else
                input->A_Setup[i] = n ;
        }
    }
    if( !fail )
    {
        input->A_Cost = new InputRec*[input->num_S] ;
        for( int i=0 ; i<input->num_S ; i++ )
        {
            input->A_Cost[i] = new InputRec[input->num_C] ;
        }
        for( int i=0 ; i<input->num_S ; i++ )
        {
            for( int j=0 ; j<input->num_C ; j++ )
            {
                if( fail )
                    break ;
                s = InputBox( "Enter Cost" , "Cost[" + String(i) + "][" + String(j) + "] = " , "0" ) ;
                n = s.ToInt() ;
                if( n == 0 )
                    fail = true ;
                else
                {
                    input->A_Cost[i][j].cost = n ;
                    input->A_Cost[i][j].C = j ;
                }
            }
            if( fail )
                break ;
        }
    }
    if( fail )
    {
        input->Initialize() ;
        calc->Initialize() ;
    }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnSortClick(TObject *Sender)
{
    input->Sort() ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnShowClick(TObject *Sender)
{
    for( int i=0 ; i<input->num_S ; i++ )
    {
        for( int j=0 ; j<input->num_C ; j++ )
        {
            ShowMessage( "weight = " + String(input->A_Cost[i][j].cost) + String("\n") +"Con = " + String(input->A_Cost[i][j].C) ) ;
        }
    }
    for( int i=0 ; i<input->num_C ; i++ )
    {
        ShowMessage( "Capacity of C" + String( i ) + " = " + String(input->A_Capacity[i]) );
    }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnInitial_solClick(TObject *Sender)
{
    int j = 1 ;
    for( int i=0 ; i< input->num_C ; i++ )
    {
        j = j*2 ;
    }
    j-- ;
    calc->SetValues( j , input->num_C ) ;
    calc->Initial() ;
    for( int i=0 ; i<input->num_S ; i++ )
    {
        ShowMessage( "Site # = " + String(calc->A_Assign[i].site) + String("\n") + "cost   = " + String(calc->A_Assign[i].cost) + String("\n") + "Concentrator # = " + String(calc->A_Assign[i].Con) ) ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnMinClick(TObject *Sender)
{
    calc->Minimize() ;
    for( int i=0 ; i<input->num_S ; i++ )
    {
        ShowMessage( "Site # = " + String(calc->A_Assign[i].site) + String("\n") + "cost   = " + String(calc->A_Assign[i].cost) + String("\n") + "Concentrator # = " + String(calc->A_Assign[i].Con) ) ;
    }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnDropClick(TObject *Sender)
{
    input->Sort() ;
    int size ;
    DropRec* D = calc->Drop( size ) ;
    for( int i=0 ; i<size ; i++ )
    {
        String s = "Cost = " + String(D[i].cost) + String("\n") + "Code = " + String(D[i].comb) + String("\n") +  String(D[i].n);
        int j=1 ;
        for( int k=0 ; k< input->num_C ; k++ )
        {
            if( (D[i].comb ^ j) < D[i].comb )
                s = s + String("\n") + "C" + String( k ) ;
            j = j*2 ;
        }
        ShowMessage( s ) ;
    }
    calc->getLists( D , size ) ;

}
//---------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////
//-----------------class Calculate--------------------------------------

void Calculate::Initial()
{
    int *used = new int[in_obj->num_C];
    if( A_Assign )
        delete( A_Assign ) ;
    A_Assign = new CalcRec[in_obj->num_S];

    for( int i=0 ; i<in_obj->num_C ; i++ )
    {
        used[i] = 0 ;
    }
    for( int i=0 ; i<in_obj->num_S ; i++ )
    {
        int j=0 ;
        bool found = false ;
        do
        {
            int c = AA_Cost[i][j].C ;
            if( used[c] < in_obj->A_Capacity[c] )
            {
                A_Assign[i].site = i ;
                A_Assign[i].Con = c ;
                A_Assign[i].cost = AA_Cost[i][j].cost ;
                A_Assign[i].restrict = false ;
                used[c]++ ;
                found = true ;
            }
            else
                j++ ;
        } while(( j < num_CC )&&( !found ));
    }

}
//-------------------------------------------------------------------------
void Calculate::Minimize()
{

    CalcRec *another = NULL ;
    CalcRec *B = NULL ;
    bool change = false ;

    do
    {
        change = false ;
        for( int s = in_obj->num_S-1 ; s >= 0 ; s-- )
        {
            bool differ = false ;
            int x = getIndex(A_Assign[s] ) , n ;
            n = x ;
            while(( AA_Cost[A_Assign[s].site][n+1].cost == A_Assign[s].cost )&&( (n+1) < in_obj->num_S ))
                n++ ;
            if( A_Assign[s].cost > AA_Cost[A_Assign[s].site][0].cost )
            {
                B = new CalcRec[ in_obj->num_S ] ;
                Copy( A_Assign , B ) ;
                another = Put( A_Assign[s].site , 0 , B , 0 , x)  ;
                if( another )
                {
                    delete( A_Assign ) ;
                    A_Assign = ReOrder( another ) ;
                    change = true ;
                    differ = true ;
                    another = NULL ;
                }
            }
            if(( n > x )&& (!differ))
            {
                B = new CalcRec[ in_obj->num_S ] ;
                Copy( A_Assign , B ) ;
                another = Put( A_Assign[s].site , x+1 , B , 0 , n+1)  ;
                if( another )
                {
                    delete( A_Assign ) ;
                    A_Assign = ReOrder( another ) ;
                    change = true ;
                    differ = true ;
                    another = NULL ;
                }
            }
        }
    }while( change ) ;

}
//-----------------------------------------------------------------------

void Calculate::Copy( CalcRec *from , CalcRec *x )
{
    for( int i=0 ; i < in_obj->num_S ; i++ )
    {
        x[i].Con = from[i].Con ;
        x[i].cost = from[i].cost ;
        x[i].restrict = from[i].restrict ;
        x[i].site = from[i].site ;
    }

}
//-----------------------------------------------------------------------

CalcRec* Calculate::Put( int site , int start , CalcRec *A , int profit, int max )
{
    CalcRec* C = NULL ;
    CalcRec *B = new CalcRec[ in_obj->num_S ] ; // new A if succeed
    CalcRec *Aux = NULL ; // an auxillary array holding all sites with same concentrator as site
//    int count = 0 ; // holds number of other sites with same concentrator as site
    int j = 0 ; // holds current index in B ;
    int k = 0 ; // holds current index in Aux ;
    int *used = new int[in_obj->num_C ]; // holds current # of sites assigned to each concentrator

    for( int i=0 ; i<in_obj->num_C ; i++ ) // initialize used
    {
        used[i] = 0 ;
    }
    for( int i=0 ; i<in_obj->num_S ; i++ ) // calculate used
    {
        used[ A[i].Con ] ++ ;
    }

do
{
// the next is for one value of start
    k = 0 ;
    Aux = NULL ;
    InputRec R = AA_Cost[site][start] ;

//  if used[that con] < capacity then assign site to it, rebuild A and return
    int index = -1 ;
    for( int i=0 ; i < in_obj->num_S ; i++ )
    {
        if( A[i].site == site )
        {
            index = i ;
            break ;
        }
    }
    if( used[R.C] < in_obj->A_Capacity[R.C] )
    {
        A[index].Con = R.C ;
        A[index].cost = R.cost ;
        A[index].restrict = false ;
        delete( used ) ;
        Copy(A,B);
        return( B ) ;
    }

    Aux = new CalcRec[ used[ R.C ] ] ;


// start building B and Aux arrays
    B[0].site = site ;
    B[0].Con = R.C ;
    B[0].cost = R.cost ;
    B[0].restrict = true ;
    j = 1 ;
    int prof = 0 ;
    bool may_success = true ;
    for( int i=0 ; i < in_obj->num_S ; i++ )
    {
        if( A[i].site == site )
        {
            prof = profit + A[i].cost - R.cost ;
            if( prof <= 0 )
                may_success = false ;
        }
        else if( A[i].Con ==  R.C )
        {
            Aux[k] = A[i] ;
            k++ ;
        }
        else
        {
            B[j] = A[i] ;
            j++ ;
        }
    }
    if( !may_success )
        break ;

    // check if any of the auxillary sites can be moved with same cost to an unsaturated concentrator
    for( int i=0 ; i < used[ R.C ] ; i++ )
    {
        int n = getIndex( Aux[i] ) ;
        int m = n+1 ;
        while(( AA_Cost[ Aux[i].site ][m].cost == Aux[i].cost )&&( m < num_CC ))
        {
            if( (used[ AA_Cost[ Aux[i].site ][m].C ]) < (in_obj->A_Capacity[ AA_Cost[ Aux[i].site ][m].C ] ) )
            {
                B[0].restrict = false ;
                B[j].Con = AA_Cost[ Aux[i].site ][m].C ;
                B[j].cost = AA_Cost[ Aux[i].site ][m].cost ;
                B[j].site = Aux[i].site ;
                B[j].restrict = false ;
                j++ ;
                for( int l=0 ; l < used[ R.C ] ; l++ )
                {
                    if( l != i )
                    {
                        B[j] = Aux[l] ;
                        j++ ;
                    }
                }
                if( used )
                    delete used ;
                if( Aux )
                    delete Aux ;
                return B ;
            }
            m++ ;

        }
    }


    for( int i=0 ; i < used[ R.C ] ; i++ )
    {
        int y = j ;
        int g = getIndex(Aux[i])+1 ;
        if( g >= in_obj->num_C )
            continue ;
        int new_profit = prof - (AA_Cost[ Aux[i].site ][g].cost - Aux[i].cost) ;
        if( new_profit > 0 )
        {
            for( int k=0 ; k < used[ R.C ] ; k++ )
            {
                if( i == k )
                    continue ;
                B[y] = Aux[k] ;
                y++ ;
            }
            C = Put2( Aux[i].site , g , B  , prof  ) ;
            if( C )
            {

                for( int b=0 ; b < in_obj->num_S ; b++ )
                    C[b].restrict = false ;


                if( used != NULL )
                    delete used ;

                if( Aux != NULL)
                    delete Aux ;
                if( B != NULL)
                    delete( B ) ;

                return( C ) ;

            }
        }
    }

    start++ ;

}while( start < max );
    if( used )
        delete used ;
    if( Aux )
        delete Aux ;
    if( B )
        delete B ;
    return( NULL ) ;
}
//------------------------------------------------------------------------------
int Calculate::getIndex( CalcRec R)
{
    int site = R.site ;
    InputRec *In = AA_Cost[ site ] ;
    for( int i=0 ; i < num_CC ; i++ )
    {
        if( R.Con == In[i].C )
            return i ;
    }
    return -1;
}
//-------------------------------------------------------------------------

CalcRec* Calculate::ReOrder( CalcRec *A )
{
    CalcRec* B = new CalcRec[ in_obj->num_S ] ;
    Copy( A , B ) ;
    for( int i=0 ; i < in_obj->num_S ; i++ )
    {
        for( int j=0 ; j < in_obj->num_S ; j++ )
        {
            if( B[j].site == i )
            {
                A[i] = B[j] ;
            }
        }
    }
    delete( B ) ;
    return A ;

}
//-----------------------------------------------------------------------

CalcRec* Calculate::Put2( int site , int start , CalcRec *A , int profit  )
{
    CalcRec* C = NULL ;
    CalcRec *B = new CalcRec[ in_obj->num_S ] ; // new A if succeed
    CalcRec *Aux = NULL ; // an auxillary array holding all sites with same concentrator as site
//    int count = 0 ; // holds number of other sites with same concentrator as site
    int j = 0 ; // holds current index in B ;
    int k = 0 ; // holds current index in Aux ;
    int *used = new int[in_obj->num_C]; // holds current # of sites assigned to each concentrator

    for( int i=0 ; i<in_obj->num_C ; i++ ) // initialize used
    {
        used[i] = 0 ;
    }
    for( int i=0 ; i<in_obj->num_S-1 ; i++ ) // calculate used
    {
        used[ A[i].Con ] ++ ;
    }


// the next is for one value of start
do
{
    k = 0 ;
    Aux = NULL ;
    InputRec R = AA_Cost[site][start] ;

//  if used[that con] < capacity then assign site to it, rebuild A and return
    if( used[R.C ] < in_obj->A_Capacity[R.C ] )
    {
        A[in_obj->num_S-1].Con = R.C ;
        A[in_obj->num_S-1].cost = R.cost ;
        A[in_obj->num_S-1].restrict = false ;
        A[in_obj->num_S-1].site = site ;
        for( int x=0 ; x< in_obj->num_S ; x++ )
            A[x].restrict = false ;
        delete( used ) ;
        Copy(A,B);
        return( B ) ;    }

    int count = 0 ;
    for( int i=0 ; i< in_obj->num_S-1 ; i++ )
    {
        if((A[i].Con == R.C )&&( !A[i].restrict ))
            count++ ;
    }
    Aux = new CalcRec[ count ] ;


// start building B and Aux arrays
    B[0].site = site ;
    B[0].Con = R.C ;
    B[0].cost = R.cost ;
    B[0].restrict = true ;
    j = 1 ;


    int prof = profit + AA_Cost[site][start-1].cost - R.cost ;
    if( prof <= 0 )
        break ;
    for( int i=0 ; i < in_obj->num_S-1 ; i++ )
    {
        if(( A[i].Con ==  R.C )&&( !A[i].restrict ))
        {
            Aux[k] = A[i] ;
            k++ ;
        }
        else
        {
            B[j] = A[i] ;
            j++ ;
        }
    }

    // check if any of the auxillary sites can be moved with same cost to an unsaturated concentrator
    for( int i=0 ; i < count ; i++ )
    {
        int n = getIndex( Aux[i] ) ;
        int m = n+1 ;
        while(( AA_Cost[ Aux[i].site ][m].cost == Aux[i].cost )&&( m < num_CC ))
        {
            if( (used[ AA_Cost[ Aux[i].site ][m].C ]) < (in_obj->A_Capacity[ AA_Cost[ Aux[i].site ][m].C ] ) )
            {
                B[0].restrict = false ;
                B[j].Con = AA_Cost[ Aux[i].site ][m].C ;
                B[j].cost = AA_Cost[ Aux[i].site ][m].cost ;
                B[j].site = Aux[i].site ;
                B[j].restrict = false ;
                j++ ;
                for( int l=0 ; l < count ; l++ )
                {
                    if( l != i )
                    {
                        B[j] = Aux[l] ;
                        j++ ;
                    }
                }
                if( used )
                    delete used ;
                if( Aux )
                    delete Aux ;
                return B ;
            }
            m++ ;

        }
    }

    for( int i=0 ; i < count ; i++ )
    {
        int y = j ;
        int g = getIndex(Aux[i])+1 ;
        if( g >= in_obj->num_C )
            continue ;
        int new_profit = prof - (AA_Cost[ Aux[i].site ][g].cost - Aux[i].cost) ;
        if( new_profit > 0 )
        {
            for( int k=0 ; k < count ; k++ )
            {
                if( i == k )
                    continue ;
                B[y] = Aux[k] ;
                y++ ;
            }
            C = Put2( Aux[i].site , g , B  , prof ) ;
            if( C )
            {

                for( int b=0 ; b< in_obj->num_S ; b++ )
                    C[b].restrict = false ;


                if( used != NULL)
                    delete used ;

                if( Aux != NULL)
                    delete Aux ;

                if( B != NULL)
                    delete( B ) ;

                return( C ) ;

            }
        }
    }

    start++ ;
} while( start < num_CC ) ;

    if( used )
        delete used ;
    if( Aux )
        delete Aux ;
    if( B )
        delete B ;
    return( NULL ) ;
}
//-----------------------------------------------------------------
DropRec* Calculate::Drop( int &range )
{
    range = 1 ;
    int D_new_range = 0 ;
    DropRec *D = NULL ;
    DropRec *D_new = NULL ;
    DropRec *temp = NULL ;
    int cost , now ;
    int j = 1 ;
    for( int i=0 ; i< in_obj->num_C ; i++ )
    {
        j = j*2 ;
    }
    j-- ;
    now = j ;
    SetValues( now , in_obj->num_C ) ;
    Initial() ;
    Minimize() ;
    cost = CalcCost( A_Assign , now ) ;
    D = new DropRec[range] ;
    D[0].comb = now ;
    D[0].cost = cost ;
    D[0].n = in_obj->num_C ;
    bool did = false ;

    for( int num = in_obj->num_C ; num > 1 ; num-- ) // num: # of concentrators
    {
        did = false ;
        for( int k=0 ; k< range ; k++ )
        {
            if( D[k].n == num )
            {
                j = 1 ;
                for( int i=0 ; i< in_obj->num_C ; i++ )
                {
                    if(( D[k].comb ^ j )< D[k].comb )
                    {
                        now = D[k].comb^j ;
                        if( !CheckCapacity( now , num-1 ) )
                        {
                            j = j*2 ;
                            continue ;
                        }
                        SetValues( now , num-1 ) ;
                        Initial() ;
                        Minimize() ;
                        cost = CalcCost( A_Assign , now ) ;
                        if( cost <= D[k].cost )
                        {
                            did = true ;
                            temp = new DropRec[D_new_range+1] ;
                            for( int h=0 ; h< D_new_range ; h++ )
                                temp[h] = D_new[h] ;
                            delete( D_new ) ;
                            D_new = temp ;
                            D_new[D_new_range].comb = now ;
                            D_new[D_new_range].cost = cost ;
                            D_new[D_new_range].n = num-1 ;
                            D_new_range++ ;
                        }

                    }
                    j = j*2 ;
                }

            }
        }
        if( !did )
            break ;
        int mincost = D[0].cost ;
        int count = 0 ;
        // get mincost
        for( int k=1 ; k < range ; k++ )
        {
            if( D[k].cost < mincost )
                mincost = D[k].cost ;
        }
        for( int k=0 ; k < D_new_range ; k++ )
        {
            if( D_new[k].cost < mincost )
                mincost = D_new[k].cost ;
        }
        // get count of mincost entries
        for( int k=0 ; k < range ; k++ )
        {
            if( D[k].cost == mincost )
                count++ ;
        }
        for( int k=0 ; k < D_new_range ; k++ )
        {
            if( D_new[k].cost == mincost )
                count++ ;
        }
        // create new D array
        temp = new DropRec[count] ;
        int index = 0;
        for( int k=0 ; k < range ; k++ )
        {
            if( D[k].cost == mincost )
            {
                temp[index] = D[k] ;
                index++ ;
            }
        }
        for( int k=0 ; k < D_new_range ; k++ )
        {
            if( D_new[k].cost == mincost )
            {
                temp[index] = D_new[k] ;
                index++ ;
            }
        }
        if( D )
            delete( D ) ;
        D = NULL ;
        if( D_new )
            delete( D_new ) ;
        D_new = NULL ;
        D = temp ;
        range = count ;
        D_new_range = 0 ;

    }
    return D ;
}
//-------------------------------------------------------------

int Calculate::CalcCost( CalcRec *A , int comb )
{
    int FixedCost = 0 ;
    int varCost = 0 ;
    /*
    int *used = new int[in_obj->num_C ]; // holds current # of sites assigned to each concentrator

    for( int i=0 ; i<in_obj->num_C ; i++ ) // initialize used
    {
        used[i] = 0 ;
    }
    for( int i=0 ; i<in_obj->num_S ; i++ ) // calculate used
    {
        used[ A[i].Con ] ++ ;
    }
    for( int i=0 ; i<in_obj->num_C ; i++ ) // initialize used
    {
        if( used[i] )
            FixedCost += in_obj->A_Setup[i] ;
    }
    */
    int j=1 ;
    for( int i=0 ; i<in_obj->num_C ; i++ )
    {
        if(( comb ^ j )< comb )
            FixedCost += in_obj->A_Setup[i] ;
        j = j*2 ;
    }

    for( int i=0 ; i<in_obj->num_S ; i++ ) // initialize used
    {
        varCost += A[i].cost ;
    }
    return( varCost + FixedCost ) ;
}
//---------------------------------------------------------
bool Calculate::CheckCapacity( int xx , int num ) // xx:code of available concentrators
{                                  // num: number of available concentrators
    int j = 1 ;
    int Capacity = 0 ;
    for( int i=0 ; i< in_obj->num_C ; i++ )
    {
        if(( xx ^ j )< xx )
        {
            Capacity += in_obj->A_Capacity[i] ;
        }
        j=j*2;
    }
    if( Capacity >= in_obj->num_S )
        return true ;
    return false ;
}
//------------------------------------------------------------------

void Calculate::getLists( DropRec* D , int Dsize )
{
    for( int i=0 ; i<Dsize ; i++ )
    {
        SetValues( D[i].comb , D[i].n ) ;
        Initial() ;
//        Minimize() ;

        Equivalent( D[i] ) ;
    }
}
//---------------------------------------------------------
CalcRec** Calculate::Equivalent( DropRec D )
{
    CalcRec** hold = NULL ;
    CalcRec** temp = NULL ;
    int size = 0 ;

    Minimize() ;
    size++ ;
    hold = new CalcRec*[size] ;
    for( int i=0 ; i<size ; i++ )
    {
        hold[i] = new CalcRec[ in_obj->num_S ] ;
    }
    ReOrder( A_Assign ) ;
    Copy( A_Assign , hold[0] ) ;


    
    CalcRec *another = NULL ;
    CalcRec *B = NULL ;
    bool change = false ;

for( int i=0 ; i<size ;i++ )
{
    Copy( hold[i] , A_Assign ) ;
    do
    {
        change = false ;
        for( int s = in_obj->num_S-1 ; s >= 0 ; s-- )
        {
            bool differ = false ;
            int x = getIndex(A_Assign[s] ) , n ;
            n = x ;
            while(( AA_Cost[A_Assign[s].site][n+1].cost == A_Assign[s].cost )&&( (n+1) < in_obj->num_S ))
                n++ ;
            if(( A_Assign[s].cost >= AA_Cost[A_Assign[s].site][0].cost )&&( A_Assign[s].Con != AA_Cost[A_Assign[s].site][0].C ))
            {
                B = new CalcRec[ in_obj->num_S ] ;
                Copy( A_Assign , B ) ;
                another = PutE( A_Assign[s].site , 0 , B , 0 , x , hold , size ) ;
                if( another )
                {
                    if( CheckExist( hold , another , size) )
                    {
                        delete another ;
                        another = NULL ;
                    }
                    else
                    {
                        delete( A_Assign ) ;
                        A_Assign = ReOrder( another ) ;
                        change = true ;
                        differ = true ;
                        another = NULL ;
                        temp = new CalcRec*[size+1] ;
                        for( int i=0 ; i<size ; i++ )
                        {
                            temp[i] = new CalcRec[ in_obj->num_S ] ;
                            Copy( hold[i] , temp[i] ) ;
                        }
                        temp[size] = new CalcRec[ in_obj->num_S ] ;
                        ReOrder( A_Assign ) ;
                        Copy( A_Assign , temp[size] ) ;
                        for( int i=0 ; i<size ; i++ )
                            delete hold[i] ;
                        delete hold ;
                        hold = temp ;
                        size++ ;
                    }
                }
            }
            if(( n > x )&& (!differ))
            {
                B = new CalcRec[ in_obj->num_S ] ;
                Copy( A_Assign , B ) ;
                another = PutE( A_Assign[s].site , x+1 , B , 0 , n+1 , hold , size ) ;
                if( another )
                {
                    if( CheckExist( hold , another , size) )
                    {
                        delete another ;
                        another = NULL ;
                    }
                    else
                    {
                        delete( A_Assign ) ;
                        A_Assign = ReOrder( another ) ;
                        change = true ;
                        differ = true ;
                        another = NULL ;
                        temp = new CalcRec*[size+1] ;
                        for( int i=0 ; i<size ; i++ )
                        {
                            temp[i] = new CalcRec[ in_obj->num_S ] ;
                            Copy( hold[i] , temp[i] ) ;
                        }
                        temp[size] = new CalcRec[ in_obj->num_S ] ;
                        ReOrder( A_Assign ) ;
                        Copy( A_Assign , temp[size] ) ;
                        for( int i=0 ; i<size ; i++ )
                            delete hold[i] ;
                        delete hold ;
                        hold = temp ;
                        size++ ;
                    }
                }

            }
        }
    }while( change ) ;
}
String ss = "Cost = " + String(D.cost) + String("\n") + "Code = " + String(D.comb) + String("\n") +  String(D.n);
int j=1 ;
for( int k=0 ; k< input->num_C ; k++ )
{
    if( (D.comb ^ j) < D.comb )
        ss = ss + String("\n") + "C" + String( k ) ;
    j = j*2 ;
}
ShowMessage( ss ) ;

for( int i=0 ; i<size ; i++ )
{
    ss = "";
    for( int k=0  ;k<in_obj->num_S ; k++ )
        ss += "site: " + String( hold[i][k].site ) + " , Conc: " + String( hold[i][k].Con )+ " , cost: " + String( hold[i][k].cost ) + String( "\n" ) ;
    ShowMessage( ss ) ;
}
return hold ;


}
//-------------------------------------------------------------------------
CalcRec* Calculate::PutE( int site , int start , CalcRec *A , int profit, int max , CalcRec**hold , int size )
{
    CalcRec* C = NULL ;
    CalcRec *B = new CalcRec[ in_obj->num_S ] ; // new A if succeed
    CalcRec *Aux = NULL ; // an auxillary array holding all sites with same concentrator as site
//    int count = 0 ; // holds number of other sites with same concentrator as site
    int j = 0 ; // holds current index in B ;
    int k = 0 ; // holds current index in Aux ;
    int *used = new int[in_obj->num_C ]; // holds current # of sites assigned to each concentrator

    for( int i=0 ; i<in_obj->num_C ; i++ ) // initialize used
    {
        used[i] = 0 ;
    }
    for( int i=0 ; i<in_obj->num_S ; i++ ) // calculate used
    {
        used[ A[i].Con ] ++ ;
    }

do
{
// the next is for one value of start
    k = 0 ;
    Aux = NULL ;
    InputRec R = AA_Cost[site][start] ;

//  if used[that con] < capacity then assign site to it, rebuild A and return
    int index = -1 ;
    for( int i=0 ; i < in_obj->num_S ; i++ )
    {
        if( A[i].site == site )
        {
            index = i ;
            break ;
        }
    }
    if( used[R.C] < in_obj->A_Capacity[R.C] )
    {
        CalcRec *X = new CalcRec[ in_obj->num_S ] ;
        Copy( B , X ) ;

        Copy(A,B);
        B[index].Con = R.C ;
        B[index].cost = R.cost ;
        B[index].restrict = false ;
        if( !CheckExist( hold , B , size) )
        {
            delete( used ) ;
            delete X ;
            return( B ) ;
        }
        else
        {
            Copy( X , B ) ;
            delete X ;
        }
    }

    Aux = new CalcRec[ used[ R.C ] ] ;


// start building B and Aux arrays
    B[0].site = site ;
    B[0].Con = R.C ;
    B[0].cost = R.cost ;
    B[0].restrict = true ;
    j = 1 ;
    int prof = 0 ;
    bool may_success = true ;
    for( int i=0 ; i < in_obj->num_S ; i++ )
    {
        if( A[i].site == site )
        {
            prof = profit + A[i].cost - R.cost ;
            if( prof < 0 )
                may_success = false ;
        }
        else if( A[i].Con ==  R.C )
        {
            Aux[k] = A[i] ;
            k++ ;
        }
        else
        {
            B[j] = A[i] ;
            j++ ;
        }
    }
    if( !may_success )
        break ;

    // check if any of the auxillary sites can be moved with same cost to an unsaturated concentrator
    for( int i=0 ; i < used[ R.C ] ; i++ )
    {
        int n = getIndex( Aux[i] ) ;
        int m = n+1 ;
        while(( AA_Cost[ Aux[i].site ][m].cost == Aux[i].cost )&&( m < num_CC ))
        {
            if( (used[ AA_Cost[ Aux[i].site ][m].C ]) < (in_obj->A_Capacity[ AA_Cost[ Aux[i].site ][m].C ] ) )
            {
                int q = j ;
                CalcRec *X = new CalcRec[in_obj->num_S] ;
                Copy( B , X ) ;
                B[0].restrict = false ;
                B[j].Con = AA_Cost[ Aux[i].site ][m].C ;
                B[j].cost = AA_Cost[ Aux[i].site ][m].cost ;
                B[j].site = Aux[i].site ;
                B[j].restrict = false ;
                j++ ;
                for( int l=0 ; l < used[ R.C ] ; l++ )
                {
                    if( l != i )
                    {
                        B[j] = Aux[l] ;
                        j++ ;
                    }
                }
                if( !CheckExist( hold , B , size))
                {
                    if( used )
                        delete used ;
                    if( Aux )
                        delete Aux ;
                    delete X ;
                    return B ;
                }
                Copy( X , B ) ;
                delete X ;
                j = q ;
            }
            m++ ;

        }
    }


    for( int i=0 ; i < used[ R.C ] ; i++ )
    {
        int y = j ;
        int g = getIndex(Aux[i])+1 ;
        if( g >= in_obj->num_C )
            continue ;
        int new_profit = prof - (AA_Cost[ Aux[i].site ][g].cost - Aux[i].cost) ;
        if( new_profit >= 0 )
        {
            for( int k=0 ; k < used[ R.C ] ; k++ )
            {
                if( i == k )
                    continue ;
                B[y] = Aux[k] ;
                y++ ;
            }
            C = PutE2( Aux[i].site , g , B  , prof , hold , size ) ;
            if( C )
            {
                if( !CheckExist( hold , C , size))
                {
                    for( int b=0 ; b < in_obj->num_S ; b++ )
                        C[b].restrict = false ;

                    if( used != NULL )
                        delete used ;
                    if( Aux != NULL)
                        delete Aux ;
                    if( B != NULL)
                        delete( B ) ;
                    return( C ) ;
                }
            //    delete C ;
                C = NULL ;
            }
        }
    }

    start++ ;

}while( start < max );
    if( used )
        delete used ;
    if( Aux )
        delete Aux ;
    if( B )
        delete B ;
    return( NULL ) ;
}
//------------------------------------------------------------------------------
CalcRec* Calculate::PutE2( int site , int start , CalcRec *A , int profit , CalcRec**hold , int size)
{
    CalcRec* C = NULL ;
    CalcRec *B = new CalcRec[ in_obj->num_S ] ; // new A if succeed
    CalcRec *Aux = NULL ; // an auxillary array holding all sites with same concentrator as site
//    int count = 0 ; // holds number of other sites with same concentrator as site
    int j = 0 ; // holds current index in B ;
    int k = 0 ; // holds current index in Aux ;
    int *used = new int[in_obj->num_C]; // holds current # of sites assigned to each concentrator

    for( int i=0 ; i<in_obj->num_C ; i++ ) // initialize used
    {
        used[i] = 0 ;
    }
    for( int i=0 ; i<in_obj->num_S-1 ; i++ ) // calculate used
    {
        used[ A[i].Con ] ++ ;
    }


// the next is for one value of start
do
{
    k = 0 ;
    Aux = NULL ;
    InputRec R = AA_Cost[site][start] ;

//  if used[that con] < capacity then assign site to it, rebuild A and return
    if( used[R.C ] < in_obj->A_Capacity[R.C ] )
    {
        CalcRec* X = new CalcRec[in_obj->num_S] ;
        Copy( B , X ) ;
        Copy(A,B);
        B[in_obj->num_S-1].Con = R.C ;
        B[in_obj->num_S-1].cost = R.cost ;
        B[in_obj->num_S-1].restrict = false ;
        B[in_obj->num_S-1].site = site ;
        if( !CheckExist( hold , B , size))
        {
            for( int x=0 ; x< in_obj->num_S ; x++ )
                A[x].restrict = false ;
            delete( used ) ;
            delete X ;
            return( B ) ;
        }
        Copy( X , B );
        delete X ;
        return NULL ;
    }
    int count = 0 ;
    for( int i=0 ; i< in_obj->num_S-1 ; i++ )
    {
        if((A[i].Con == R.C )&&( !A[i].restrict ))
            count++ ;
    }
    Aux = new CalcRec[ count ] ;


// start building B and Aux arrays
    B[0].site = site ;
    B[0].Con = R.C ;
    B[0].cost = R.cost ;
    B[0].restrict = true ;
    j = 1 ;


    int prof = profit + AA_Cost[site][start-1].cost - R.cost ;
    if( prof < 0 )
        break ;
    for( int i=0 ; i < in_obj->num_S-1 ; i++ )
    {
        if(( A[i].Con ==  R.C )&&( !A[i].restrict ))
        {
            Aux[k] = A[i] ;
            k++ ;
        }
        else
        {
            B[j] = A[i] ;
            j++ ;
        }
    }

    // check if any of the auxillary sites can be moved with same cost to an unsaturated concentrator
    for( int i=0 ; i < count ; i++ )
    {
        int n = getIndex( Aux[i] ) ;
        int m = n+1 ;
        while(( AA_Cost[ Aux[i].site ][m].cost == Aux[i].cost )&&( m < num_CC ))
        {
            if( (used[ AA_Cost[ Aux[i].site ][m].C ]) < (in_obj->A_Capacity[ AA_Cost[ Aux[i].site ][m].C ] ) )
            {
                int q = j ;
                CalcRec *X = new CalcRec[in_obj->num_S] ;
                Copy( B , X ) ;
                B[0].restrict = false ;
                B[j].Con = AA_Cost[ Aux[i].site ][m].C ;
                B[j].cost = AA_Cost[ Aux[i].site ][m].cost ;
                B[j].site = Aux[i].site ;
                B[j].restrict = false ;
                j++ ;
                for( int l=0 ; l < count ; l++ )
                {
                    if( l != i )
                    {
                        B[j] = Aux[l] ;
                        j++ ;
                    }
                }
                if( !CheckExist( hold , B , size))
                {
                    if( used )
                        delete used ;
                    if( Aux )
                        delete Aux ;
                    delete X ;
                    return B ;
                }
                Copy( X , B ) ;
                delete X ;
                j = q ;
            }
            m++ ;

        }
    }

    for( int i=0 ; i < count ; i++ )
    {
        int y = j ;
        int g = getIndex(Aux[i])+1 ;
        if( g >= in_obj->num_C )
            continue ;
        int new_profit = prof - (AA_Cost[ Aux[i].site ][g].cost - Aux[i].cost) ;
        if( new_profit >= 0 )
        {
            for( int k=0 ; k < count ; k++ )
            {
                if( i == k )
                    continue ;
                B[y] = Aux[k] ;
                y++ ;
            }
            C = PutE2( Aux[i].site , g , B  , prof , hold, size ) ;
            if( C )
            {

                if( !CheckExist( hold , C ,size) )
                {
                    for( int b=0 ; b< in_obj->num_S ; b++ )
                        C[b].restrict = false ;
                    if( used != NULL)
                        delete used ;
                    if( Aux != NULL)
                       delete Aux ;
                    if( B != NULL)
                        delete( B ) ;
                    return( C ) ;
                }
//                delete C ;
                C = NULL ;
            }
        }
    }

    start++ ;
} while( start < num_CC ) ;

    if( used )
        delete used ;
    if( Aux )
        delete Aux ;
    if( B )
        delete B ;
    return( NULL ) ;
}
//-----------------------------------------------------------------

bool Calculate::CheckExist( CalcRec** hold , CalcRec *A , int size)
{
    CalcRec *B = new CalcRec[in_obj->num_S] ;
    Copy( A , B );
    ReOrder( B ) ;
    bool Eq = true ; ;
    for( int i=0 ; i<size ; i++ )
    {
        Eq = true ;
        for( int j=0 ; j < in_obj->num_S ; j++ )
        {
            if(( B[j].Con != hold[i][j].Con )||( B[j].cost != hold[i][j].cost )||( B[j].site != hold[i][j].site ))
            {
                Eq = false ;
                break ;
            }
        }
        if( Eq )
        {
            delete B ;
            return true ;
        }
    }
    delete B ;
    return false ;

}







