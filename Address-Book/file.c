#include <stdio.h>
#include "file.h"
#include "populate.h"

//Function definition to save contacts to a file
void saveContactsToFile(AddressBook *addressBook)
{
      FILE *fptr;  //file pointer
      fptr=fopen("contacts.txt","w");   // open the file in write mode
      if(fptr==NULL)   //if file pointer is null then file doesn't exist, terminate
      {
            printf("File not found\n");
            return;
      }
      fprintf(fptr,"Total contacts: %d\n",addressBook->contactCount); //if file exists write total contact count to file
      // run loop from 0 till total contacts
      for(int i=0;i<addressBook->contactCount;i++)
      {
            //write each contact to file
            fprintf(fptr," %s\n%s\n%s\n\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
      }
      //close file
      fclose(fptr);
}

//Function definition to load contacts from file
void loadContactsFromFile(AddressBook *addressBook) 
{
         
      FILE *fptr; //file pointer
      fptr=fopen("contacts.txt","r"); //open file in read mode. Now the file pointer is at the beginning
      if(fptr==NULL)  // if it is null the file is empty make count as 0 , terminate
      {
            printf("File not found\n");
            addressBook->contactCount=0;
            return;
      }
      // if not nullread total count
      if(fscanf(fptr,"Total contacts: %d\n",&addressBook->contactCount)!=1)  // if not read successfully make count as 0
      {
            addressBook->contactCount=0;
            fclose(fptr);
            return;
      }
      for(int i=0;i<addressBook->contactCount;i++)   //if read successfully read contacts one by one 
      {
            // if it reads the 3 data(name,phone,email) successfully store the count
            if(fscanf(fptr," %[^\n] %[^\n] %[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone, addressBook->contacts[i].email)!=3)
            {
                  addressBook->contactCount=i;
                  break;

            }
      }
      fclose(fptr);  // close file
      
    
}

