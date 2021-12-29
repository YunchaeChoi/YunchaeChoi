#include <stdio.h>

int main()
{
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        for(int blank=0;blank<i;blank++)
            printf(" ");
        for(int star=n-i;star>0;star--)
            printf("*");
        printf("\n");
    }
}
