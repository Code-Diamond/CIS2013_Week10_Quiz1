#include <iostream>
#include <locale>
#include <string>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <cmath>

using namespace std;

//Functions
int rng(int);
void generateOTP();
bool doesFileExist(char[256]);

//Global Variables

//User's encryption/decryption decision string
string d;
//Secret phrase
char e[1000];
//Encrypted Message
char f[1000];


//Locale object
locale l;
//Character array
char c[27] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' '};
//Create input and output file streams
ofstream ofs;
ifstream ifs;

char loopController = 'Y';

//Main
int main()
{
    while(loopController=='Y' || loopController=='y')
    {
        //Prompt user to either load or generate a OTP

        char openFileDecision;
        char fileName[256];

        cout << "Would you like to open a pre-made one time pad? (y/n)", cin >> openFileDecision;

        if(openFileDecision == 'y' || openFileDecision == 'Y')
        {
            cout << "\nEnter the file name on the following line..\n--Please ensure you have the extension as well.\n\nFile name: ", cin >> fileName;
            //Check if the file exists
            if(doesFileExist(fileName))
            {
                ifs.open(fileName);
                //Error typically happens from read or write privileges
                if(ifs.fail())
                {
                    cout << "Unable to open the one time pad.";
                    exit(1);
                }
            }
            else
            {
                cout << "\nThe file you entered does not exist. \nEnsure you have the correct file name before running again.\nExiting!\n" << endl;
                exit(1);
            }

        }
        else
        {
            cout << "\nGenerating the one time pad . . ." << endl;
            generateOTP();

             ifs.open("otp.dat");
            //Error typically happens from read or write privileges
            if(ifs.fail())
            {
                cout << "Unable to open the one time pad.";
                exit(1);
            }

        }

        //Prompt user if they want to encrypt or decrypt

        //Use time as a seed for the rng
        srand(time(NULL));


        cout << "Would you like to \"Encrypt\" or \"Decrypt\"?: ", cin >> d, cout << endl;


        //Loop through the d string and turn it into uppercase
        for (int i = 0; i < d.length(); i++)
        {
            d[i] = toupper(d[i],l);
        }

        //Trick to clear cin's error flags
        cin.seekg(0,ios::end);
        cin.clear();

        // ofs.close();

        //If the user types in encrypt
        if(d == "ENCRYPT")
        {
            cout << "Type in what you want to encrypt on the following line. . .\n";

            //Receive input
            cin.getline(e, 1000);
            //Get length
            int length = strlen(e);

            //Glean rotation key from the OTP
            char otpKey;
            int rotationKey[length];
            cout << "-------OTPKEY------" << endl;
            for(int i = 0; i < length; i++)
            {
                ifs >> otpKey;
                for(int j = 0; j < 27; j++)
                {
                    if(otpKey == c[j])
                    {
                        cout << i << " - " << otpKey << " : " << j << endl;
                        rotationKey[i] = j;
                    }
                }
            }

            cout << "-------START------" << endl;

            //Glean current key
            int currentKey[length];
            for(int i = 0; i < length; i++)
            {
                for(int j = 0; j < 27; j++)
                {
                    if(e[i] == c[j])
                    {
                        cout << i << " - " << e[i] << " : " << j << endl;
                        currentKey[i] = j;
                    }
                }
            }

            cout << "------OUTPUT-------" << endl;

            //Create final encrypted message
            for(int i = 0; i < length; i++)
            {
                int newKey = currentKey[i] + rotationKey[i];
                newKey = newKey % 27;
                cout << i << " - " << c[newKey] << endl;
                f[i] = c[newKey];
            }

            cout << "\nYour encrypted message...\n";

            //Print encrypted message
            for(int i = 0; i < length; i++)
            {
                cout << f[i];
            }
            cout << endl;

        }


        //In process of editing decryption algorithm
        //If the user types in decrypt
        if(d == "DECRYPT")
        {
            int length = strlen(f);

            if(length == 0)
            {
                cout << "Enter in a message to decrypt:" << endl;
                cin.getline(f, 1000);
                length = strlen(f);
            }

            int currentKey[length];
            //Determine current key
            for(int i = 0; i < length; i++)
            {
                for(int j = 0; j < 27; j++)
                {
                    if(f[i] == c[j])
                    {
                        currentKey[i] = j;
                    }
                }
            }
            //Glean rotation key from the OTP
            char otpKey;
            int rotationKey[length];
            for(int i = 0; i < length; i++)
            {
                ifs >> otpKey;
                for(int j = 0; j < 27; j++)
                {
                    if(otpKey == c[j])
                    {
                        cout << i << " - " << otpKey << " : " << j << endl;
                        rotationKey[i] = j;
                    }
                }
            }
            //Decrypt the message
            char decryptedMessage[length];
            for(int i = 0; i < length; i++)
            {
                int newKey = currentKey[i] - rotationKey[i];

                if(newKey < 0)
                {
                    newKey = 27 - abs(newKey);
                }

                decryptedMessage[i] = c[newKey];
            }

            //Show decrypted Message
            for(int i = 0; i < length; i++)
            {
                cout << decryptedMessage[i];
            }
            cout << endl;
        }
        //Prompt user if they want to continue
        cout << "Would you like to continue? (y/n): ", cin >> loopController, cout << endl;

        //Close the file streams as to not encounter error when looping back through
        ifs.close();
    }

    return 0;
}



//Function that generates a random number
int rng(int max)
{
    int randomN = 1 + rand() % max;
    return randomN;
}

//Function generates a otp
void generateOTP()
{
    ofs.open("otp.dat");
    //Error typically happens from read or write privileges
    if(ofs.fail())
    {
        cout << "Unable to open the one time pad.";
        exit(1);
    }
    //Generates the One Time Pad file
    //Ten lines
    for(int i = 0; i < 10; i++)
    {
        //Fifty Characters
        for(int j = 0; j < 50; j++)
        {
            //Write a random character to the file
            ofs << c[rng(27)-1];
        }
        ofs << endl;
    }

    ofs.close();
}

//Function checks if the file exists
bool doesFileExist(char fileName[256])
{
    ifstream infile(fileName);
    return infile.good();
}
