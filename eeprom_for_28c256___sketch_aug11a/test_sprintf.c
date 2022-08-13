#include <stdio.h>
 
int main(void)
{
    int num;
    int len;
    char mystr[10]; 
 
    printf("Please enter a number: ");
    scanf("%d", &num);
 
    //converting num to string
    //sprintf(mystr, "%d", num);  
    len = sprintf(mystr, "%d", num);   
 
    printf("\n Num to string: %s", mystr);
    printf("\n Length of converted string: %d ", len);
 
    return 0;
}
