#include <cstdio>
#include <vector>
using namespace std;

const char iName[] = "Plan9";

void printBoot(FILE *in, long off)
{
     fseek(in, off - 512, SEEK_SET);

     char buffer[512];
     int count = fread(buffer, 1, 512, in);
     if (count != 512 || ferror(in))
     {
        printf("Cannot read from %s!\n", iName);
        return;
     }

     char fName[50];
     sprintf(fName, "boot%09ld.txt", off);
     FILE *out = fopen(fName, "wb");

     if (out == NULL)
     {
        fprintf(stderr, "cannot open file %s\n", fName);
        return;
     }

     count = fwrite(buffer, 1, 512, out);

     if (count != 512 || ferror(out))
     {
        printf("Cannot write to %s!\n", fName);
     }

     if (fclose(out))
     {
        fprintf(stderr, "cannot close file %s \n", fName);
     }
}

class bufReader
{
      static const int bufSize = 1 << 16;
      char buffer[bufSize];
      FILE *in;
      int size;
      int pos;
      bool readNext() // можно ли прочитать след. элемент
      {
           pos = 0;
           size = fread(buffer, 1, bufSize, in);
           if (size == 0 || ferror(in))
           {
              return false;
           }
           return true;
      }

      public:
      bufReader(FILE *_in)
      {
           in = _in;
           size = 0;
           pos = 0;
      }
      bool getNext(char &c) // была ли ошибка
      {
           if (pos == size && feof(in))
           return true;
           if (pos == size)
           {
              if (!readNext())
              return true;
           }
           c = buffer[pos];
           ++pos;
           return false;
      }
      bool hasNext() // есть ли след. элемент
      {
           if (pos == size)
           {
              if (!readNext()) return false;
           }
           return pos < size;
      }
};

int main()
{
    FILE *in = fopen(iName, "rb");
    if (in == NULL)
    {
       fprintf(stderr, "cannot open file %s\n", iName);
       return 0;
    }


    bufReader reader(in);
    char prv;
    if (reader.getNext(prv))
    {
       printf("Cannot read from %s!\n", iName);
       return 0;
    }
    long pos = 1;
    vector<long> positions;
    while(reader.hasNext())
    {
       char cur;
       if (reader.getNext(cur))
       {
          printf("cannot read from %s!\n", iName);
          break;
       }
       ++pos;

       if (prv == char(0x55) && cur == char(0xAA) && pos >= 512)
       {
          positions.push_back(pos);
          printf("%ld\n", pos);
       }
       prv = cur;
    }
    for(vector<long>::iterator it = positions.begin(); it != positions.end(); ++it)
    {
       printBoot(in, *it);
    }
    if (fclose(in))
    {
       fprintf(stderr, "cannot close file %s \n", iName);
    }
    return 0;
}
