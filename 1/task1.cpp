#include <stdio.h>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

vector<int> myvector;
vector<int>::iterator it;

void create_vector(const char *name)
{
     FILE * myfile;
     myfile = fopen(name, "r");
     if(myfile == NULL)
     {
               printf("ERROR! cannot open file \"%s\"\n", name);
               return;
     }

     int i;
     int a;
     try
     {
        while((a = fscanf(myfile, "%d", &i)) != EOF)
        if(a > 0)
        myvector.push_back(i);
        else
        {
            printf("ERROR! the content of file \"%s\" is incorrect\n", name);
            break;
        }
     }
     catch (exception e)
     {
         printf("ERROR! the size of file \"%s\" is too big\n", name);
     }

     if(fclose(myfile) == EOF)
     printf("ERROR! cannot close file \"%s\"\n", name);
     return;
}



int main(int argc, char *argv[])
{

    if(argc == 0)
    {
            printf("ERROR! no files names entered\n");
            return 0;
    }
    for(int i=1; i<argc;++i)
    {

            create_vector(argv[i]);
    }
    sort (myvector.begin(), myvector.end());

    FILE * myfile;
    myfile = fopen(argv[argc-1], "w");
    if(myfile == NULL)
    {
              printf("ERROR! cannot open file \"%s\" to write\n", argv[argc-1]);
              return 0;
    }

    for (it=myvector.begin(); it!=myvector.end(); ++it)
    if(fprintf(myfile, "%d\n", *it)<0)
    {
              printf("ERROR! cannot write in file \"%s\"\n", argv[argc-1]);
              break;
    }

    if(fclose(myfile) == EOF)
    printf("ERROR! cannot close the file \"%s\"\n", argv[argc-1]);
    return 0;
}
