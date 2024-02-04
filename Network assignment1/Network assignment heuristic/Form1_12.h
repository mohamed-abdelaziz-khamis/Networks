//---------------------------------------------------------------------------

#ifndef Form1_12H
#define Form1_12H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TButton *btnInput;
    TButton *btnDrop;
    TButton *btnShow;
    void __fastcall btnInputClick(TObject *Sender);
    void __fastcall btnSortClick(TObject *Sender);
    void __fastcall btnShowClick(TObject *Sender);
    void __fastcall btnInitial_solClick(TObject *Sender);
    void __fastcall btnMinClick(TObject *Sender);
    void __fastcall btnDropClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};

struct InputRec{
    int C ;
    int cost ;
};

struct CalcRec{
    int site ;
    int cost ;
    int Con ;
    bool restrict ;
};

struct DropRec{
    int comb ;// concentrator combination
    int cost ;
    int n ; // number of conentrators
};

class In_class{
public:
    int num_C ;
    int num_S ;
    InputRec **A_Cost ;
    int *A_Capacity ;
    int *A_Setup ;

    In_class()
    {
        num_C = 0 ;
        num_S = 0 ;
        A_Cost = NULL ;
        A_Capacity = NULL ;
        A_Setup = NULL ;
    }
    ////////////////////////////
    void Initialize()
    {
        num_C = 0 ;
        num_S = 0 ;
        if( A_Cost )
        {
            for( int i=0 ; i<num_S ; i++ )
            {
                delete( A_Cost[i] ) ;
            }
            delete( A_Cost ) ;
        }
        A_Cost = NULL ;
        if( A_Capacity )
            delete( A_Capacity ) ;
        A_Capacity = NULL ;
        if( A_Setup )
            delete( A_Setup ) ;
        A_Setup = NULL ;

    }
    /////////////////////////////////
    void Sort()
    {
        for( int i=0 ; i<num_S ; i++ )
            Bubble( i ) ;
    }

private:
    void Bubble( int row )
    {
        int i , j = num_C ;
        bool swap = false ;
        do
        {
            swap = false ;
            for( i=0 ; i<j-1 ; i++ )
            {
                if( A_Cost[row][i].cost > A_Cost[row][i+1].cost )
                {
                    Swap( A_Cost[row] ,i ,i+1 ) ;
                    swap = true ;
                }
            }
            j-- ;
        } while ((j>0)&&(swap)) ;

    }
    ///////////////////////////////////////////
    void Swap( InputRec *Cost , int x , int y )
    {
        InputRec temp = Cost[x] ;
        Cost[x] = Cost[y] ;
        Cost[y] = temp ;
    }

};

class Calculate{

public:
    In_class *in_obj ;
    CalcRec *A_Assign ;
    InputRec **AA_Cost ;
    int num_CC ;
    int c_av ;

    Calculate( In_class *In )
    {
        A_Assign = NULL ;
        AA_Cost = NULL ;
        in_obj = In ;
    }
    void Initialize()
    {
        if( A_Assign )
            A_Assign = NULL ;
        if( AA_Cost ) 
            AA_Cost = NULL ;
    }
    void SetValues( int xx , int num ) // xx:code of available concentrators
    {                                  // num: number of available concentrators
        num_CC = num ;
        c_av = xx ;
        AA_Cost = new InputRec*[in_obj->num_S] ;
        for( int i=0 ; i<in_obj->num_S ; i++ )
        {
            AA_Cost[i] = new InputRec[num_CC] ;
        }
        for( int i=0 ; i<in_obj->num_S ; i++ )
        {
            int col = 0 ;
            for( int j=0 ; j<in_obj->num_C ; j++ )
            {
                int k = 1 ;
                for( int l = 0 ; l< in_obj->A_Cost[i][j].C ; l++ )
                {
                    k = k*2 ;
                }
                if(( k ^ c_av ) < c_av )
                {
                    AA_Cost[i][col] = in_obj->A_Cost[i][j] ;
                    col++ ;
                }
            }
        }

    }
    void Initial() ;   // initial assignment
    void Minimize() ;
    CalcRec* Put( int site , int start , CalcRec* A , int profit , int max ) ;
    CalcRec* Put2( int site , int start , CalcRec* A , int profit ) ;
    CalcRec* PutE( int site , int start , CalcRec* A , int profit , int max , CalcRec** hold , int size ) ;
    CalcRec* PutE2( int site , int start , CalcRec* A , int profit , CalcRec** hold , int size ) ;
    void Copy( CalcRec *from , CalcRec *to ) ;
    int getIndex( CalcRec R ) ;
    CalcRec* ReOrder( CalcRec *A ) ;
    DropRec* Drop(int &range) ;
    int CalcCost( CalcRec *A , int comb ) ;
    bool CheckCapacity( int xx , int num ) ;// xx:code of available concentrators
                      // num: number of available concentrators};
    void getLists( DropRec *D , int size ) ;
    CalcRec** Equivalent( DropRec D ) ;
    bool CheckExist( CalcRec** hold , CalcRec* A , int size) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
