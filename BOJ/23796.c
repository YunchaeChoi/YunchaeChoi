#include <stdio.h>

#define SWAP(a,b) {int temp=a;a=b;b=temp;}


void show_board(int board[8][8])
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            printf("%d ",board[i][j]);
        }
        printf("\n");
    }
}

void print_line()
{
    printf("--------------------------------------\n");
}

void newline()
{
    printf("\n");
}

int main()
{
    int board[8][8];

    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            scanf("%d",&board[i][j]);

      char key = (char)getchar();
      newline();

      printf("key: %c\n",key);

      show_board(board);
      print_line();

      switch(key)
      {
          case 'U':
              {
                  printf("case U\n");
                  for(int i=0;i<7;i++)
                  {
                      if(board[i][0]==0)
                      {
                          for(int j=i+1;j<8;j++)
                          {
                              if(board[j][0]==0)
                              {
                                  SWAP(board[i][0],board[j][0]);
                                  break;
                              }
                          }
                      }
                  }
                  break; // break case
              }
      }

      show_board(board);


}
