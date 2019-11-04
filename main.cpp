#include <iostream>
#include <pthread.h>
#include <time.h>

using namespace std;
#define MAX 50
int a[MAX][MAX];
int b[MAX][MAX];
int c[MAX][MAX] = {0};
int n1, m1, n2, m2;

const char* in_file = "input.txt";
const char* out_file = "output.txt";

typedef struct
{
    int row;
    int col;
} indices;

void * elemnt_mul(void * ind)
{
    indices* temp = (indices*)ind;
    int row = temp -> row;
    int col = temp -> col;
    for(int i = 0; i < m1; i++)
    {
        c[row][col] += a[row][i] * b[i][col];
    }

}

void *row_mul(void * index)
{
    int row = *((int *) index);
    for(int i = 0; i < m2; i++)
    {
        c[row][i] = 0;
        for(int j = 0; j < m1; j++)
        {
            c[row][i] += a[row][j] * b[j][i];
        }
    }
}

int main()
{
    freopen(in_file,"r", stdin);

    //reading first matrix.
    cin >> n1 >> m1;
    for(int i = 0; i < n1; i++)
    {
        for(int j = 0; j < m1; j++)
        {
            cin >> a[i][j];
        }
    }
    //reading second matrix.
    cin >> n2 >> m2;
    if(m1 != n2)
    {
        freopen(out_file,"w",stdout);
        cout << "The two matrices can't be multiplied.";
        return 0;
    }
    for(int i = 0; i < n2; i++)
    {
        for(int j = 0; j < m2; j++)
        {
            cin >> b[i][j];
        }
    }
    // element multiplication threads.
    pthread_t element_threads[n1][m2];
    clock_t t1 = clock();
    for(int i = 0; i < n1; i++)
    {
        for(int j = 0; j < m2; j++)
        {
            indices *ind = new indices;
            ind->row = i;
            ind->col = j;
            pthread_create(&element_threads[i][j], NULL, elemnt_mul,ind);
        }
    }
    for(int i = 0; i < n1; i++)
    {
        for(int j = 0; j < m2; j++)
        {
            pthread_join(element_threads[i][j], NULL);
        }
    }
    clock_t t2 =clock();
    double element_time = (t2 - t1) /(double) CLOCKS_PER_SEC;

    int temp_arr[n1][m2];
    for(int i = 0; i < n1; i++)
    {
        for(int j = 0; j < m2; j++)
        {
            temp_arr[i][j] = c[i][j];
        }
    }


    //row multiplication threads.
    pthread_t row_threads[n1];

    t1 = clock();
    for(int i = 0; i < n1; i++)
    {
        int * row = new int;
        * row = i;
        pthread_create(&row_threads[i], NULL,row_mul, row);
    }
    for(int i = 0; i < n1; i++)
    {
        pthread_join(row_threads[i], NULL);
    }
    t2 = clock();
    double row_time = (t2 - t1) /(double) CLOCKS_PER_SEC;


    //writing the output file.
    freopen(out_file,"w",stdout);
    for(int i = 0; i < n1; i++)
    {
        for(int j = 0; j < m2; j++)
        {
            cout << temp_arr[i][j] << " ";
        }
        cout << endl;
    }
    cout  << element_time;
    cout << endl;
    for(int i = 0; i < n1; i++)
    {
        for(int j = 0; j < m2; j++)
        {
            cout << c[i][j] << " ";
        }
        cout << endl;
    }
    cout  << row_time;
    return 0;
}
