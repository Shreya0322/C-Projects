/*Documentation:
Name: Shreya Shanbhag
Date: 12-12-2025
Description: 
Address Book project is designed to store and manage contact information.The main objectives of the project are:
1. To create new contacts and dtore them in a file.
2. To search for existing contacts and modify their details when required.
3. Delete specific contact.
4. To display the list of all saved contacts.
5. To save and retrive contacts from the file.
*/

#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "contact.h"

int main() {
    int choice;
    AddressBook abptr;   //structure variable for AddressBook
    initialize(&abptr); // Initialize the address book

    do {
        printf("\nAddress Book Menu:\n");   // Run loop to display the menu and enter the choice
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Save Contacts\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:  printf("Create Contact\n");
                    //Function call to create new contact
                     createContact(&abptr);
                     break;
            case 2:  printf("Search Contact\n");
                    //Function call to search for a contact
                     searchContact(&abptr);
                     break;
            case 3:  printf("Edit Contact\n");
                    //Function call to edit contact
                     editContact(&abptr);
                     break;
            case 4:  printf("Delete Contact\n");
                     //Function call to delete contact
                     deleteContact(&abptr);
                     break;
            case 5: printf("List contact\n");
                    //Function call to display contact list on the terminal
                     listContacts(&abptr);
                     break;
            case 6: printf("Saving contacts\n");
                    //Function call to save contact to file
                     saveContactsToFile(&abptr);
                     break;
            case 7: printf("Exit...\n");
                    //Exit program
                    exit(EXIT_SUCCESS); 
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice<7);  // run loop till choice is less than 7
    
       return 0;
}
