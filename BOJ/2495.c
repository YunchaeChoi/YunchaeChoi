#include <stdio.h>
#include <string.h>

int main()
{
    int answer;
    int cnt;
    char cur;
    char num[9];

    for(int test_case=0;test_case<3;test_case++)
    {
        answer=1;
        scanf("%s",num);
        cur=num[0];
        cnt=1;
        for(int i=1;i<8;i++)
        { 
            if(num[i]==cur)
                cnt++;
            else
            {
                cur=num[i];
                if(answer <= cnt)
                    answer=cnt;
                cnt=1;
            }
        }
        if(answer < cnt)
            answer = cnt;
        printf("%d\n",answer);
    }
}
