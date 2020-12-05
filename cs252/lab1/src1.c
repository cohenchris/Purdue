#include <stdio.h>

int main()
{
	int num1, num2, i, j;
	int flag, count;

	count=0;
	
	scanf("%d", &num1);
	scanf("%d", &num2);

	if ( num2 < 0 || num1 < 0 )
	{
		printf("Error...Both the numbers should be positive\n");
		return 1;
	}

	if ( num2 < num1 )
	{
		printf("Error...lower limit is larger than upper limit\n");
		return 1;
	}

	if ( num2-num1 > 5000 )
	{
		printf("The diff between the lower and the upper limit ");
		printf("should not be larger than 5000\n");
		return 1;
	}
	if ( num1 == 0 || num1 == 1 )
		num1=2;

	for ( i=num1; i<num2; i++)
	{
		flag=0;
		for ( j = 2; j <= i/2; j++)
		{
			if ( (i%j) == 0 )
			{
				flag=1;
				break;
			}
		}
		if ( flag == 0 )
		{
			if ( count == 0 )
				printf("The prime numbers between %d and %d are: ", num1,num2);
			count++;
			printf("%d ", i);
		}
	}
	if ( count == 0 )
		printf("There isn't any prime number between the two\n");
	else
		printf("\n");
        return 0;    
}
