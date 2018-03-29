#include <iostream>

using namespace std;

int main()
{
	char input[1000];

	//Set every element to escape sequence first
	for(int i = 0; i < 1000; i++)
	{
	    input[i] = '\n';
	}

	cout << "Type in a line to be substituted:" << endl;
	cin.getline(input, 1000);

    //The following yields the array size + 1, so start at 0 then subtract afterwards
    int size = 0;
    while(input[size] != '\n')
    {
        size++;
    }

    //Get the real size by subtracting 1
    size--;

    char realInput[size];
    for(int i =0; i < size; i++)
    {
        realInput[i] = input[i];
    }

    int counter = 0;
	for(int i = 0; i < size; i++)
	{

		if(realInput[i] == 'b')
		{
			counter++;
			if(counter%2 == 0)
			{
				cout << "STAND";
			
			}
			else
			{
				cout << "SIT";
			
			}
		}
		else
		{
			cout << realInput[i];
		}
	}
	return 0;
}
