/* This file contains the function definitions which performs the operations such as creating new contact, editing contact, searching, 
delete contact, display contact and initializing the address book*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

// function definition to list the contacts on the terminal
void listContacts(AddressBook *addressBook) 
{
    printf("Contact list (Name, phone no., Email id):\n");
    for(int i=0;i<addressBook->contactCount;i++)  // run loop till total number of contacts to display the contact list on the terminal
    {
        printf(" %s %s %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    
}

// function definition to initialize addressbook
void initialize(AddressBook *addressBook) 
{
    addressBook->contactCount=0;
    loadContactsFromFile(addressBook);      // function call to load contacts from file.

    if(addressBook->contactCount ==0)
    {
        populateAddressBook(addressBook);   // if file is empty populate with dummy contacts.
        saveContactsToFile(addressBook);    // save contacts to file.
    }
   
}

// Function definition yo save contacts to file
void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    
}

// Function definition to create new contact 
void createContact(AddressBook *addressBook)
{
    // Read name phone no and email of the new user
    printf("Enter the name: ");
    scanf(" %[^\n]",addressBook->contacts[addressBook->contactCount].name);
    printf("\nEnter phone no: ");
    scanf(" %[^\n]",addressBook->contacts[addressBook->contactCount].phone);
    printf("\nEnter email id: ");
    scanf(" %[^\n]",addressBook->contacts[addressBook->contactCount].email);
    addressBook->contactCount++; //increment contact count

}

// Function definition to search existing contact
void searchContact(AddressBook *addressBook) 
{
    char searchname[100];
    int flag=0;

    //Read the name to be searched from user
    printf("Enter the name to search: ");
    scanf(" %[^\n]",searchname);
    
    //Run loop from  0 till total contacts
    for(int i=0;i<addressBook->contactCount;i++)
    {
        // comapre user entered name with each name in the contact list
        if(strcmp(searchname, addressBook->contacts[i].name)==0)   
        {
            //if found display the details to the terminal ans stop
            printf("Contact Found! Here is the details of the contact:\n");
            printf("%s %s %s",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
            flag=1;
            break;
        }
    }
    //if the contact is not found display a message
    if(flag==0)
    {
        printf("No such contact found!\n");
    }
    

}

// Function definition to edit the contact
void editContact(AddressBook *addressBook)
{
    //Read the name to be edited from user
    char edit_name[50];
	printf("Enter the name of the contact to be edited: ");
    scanf(" %[^\n]",edit_name);
    int flag=0;
    for(int i=0;i<addressBook->contactCount;i++)
    {
        // comapre user entered name with each name in the contact list
        if(strcmp(edit_name,addressBook->contacts[i].name)==0)
        {
            int option;
            //run loop to display which is to edited(name/phone/email)
            do{
                printf("1. Edit name\n");
                printf("2.Edit phone no\n");
                printf("3.Edit email id\n");
                printf("Enter the option(1/2/3):");
                scanf("%d",&option);  
                //based on the option selected by the user edit the user details 
                switch(option){
                    case 1: char new_name[50];
                            printf("Please enter the new name: ");
                            scanf(" %[^\n]",new_name);
                            strcpy(addressBook->contacts[i].name,new_name);
                            break;

                    case 2: char new_phone[50];
                            printf("Please enter the new number: ");
                            scanf(" %[^\n]",new_phone);
                            strcpy(addressBook->contacts[i].phone,new_phone);
                            break;

                    case 3: char new_email[50];
                            printf("Please enter the new email: ");
                            scanf(" %[^\n]",new_email);
                            strcpy(addressBook->contacts[i].email,new_email);
                            break;

                    default:printf("Enter the correct option\n");
                            break;
                }
                printf("Edited contact details:\n");
                printf("%s %s %s",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                flag=1;
                break;
            }while(option<3);
            
        }
        

    }
    //if enter contact is not found display message
    if(flag==0)
    {
        printf("No such contact found!\n");
    }
    
}


// Function definition to delete the contact 
void deleteContact(AddressBook *addressBook)
{
    char del_name[50];
    int flag=0;

    //Read the name of the contact which is to be deleted
    printf("Enter the name to be deleted: ");
    scanf(" %[^\n]",del_name);
    for(int i=0;i<addressBook->contactCount;i++)
    {
         // comapre user entered name with each name in the contact list
        if(strcmp(del_name,addressBook->contacts[i].name)==0)
        {
            //if found replace that contact with the contact next to it till last contact
            flag=1;
            for(int j=i;j<addressBook->contactCount;j++)
            {
                strcpy(addressBook->contacts[j].name,addressBook->contacts[j+1].name);
                strcpy(addressBook->contacts[j].phone,addressBook->contacts[j+1].phone);
                strcpy(addressBook->contacts[j].email,addressBook->contacts[j+1].email);
                
            }
            addressBook->contactCount--;  //decrement contact count
        }
    }
    //if the name is not matched display message
    if(flag==0)
    {
        printf("No such contact found!\n");
    }

    saveContactsToFile(addressBook);  //save contacts to file

   
}
