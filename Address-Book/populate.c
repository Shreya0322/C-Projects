#include "contact.h"
#include "file.h"
#include<stdio.h>
// Dummy contact data
static Contact dummyContacts[] = {
    {"John Doe", "1234567890", "john@example.com"},
    {"Alice Smith", "0987654321", "alice@example.com"},
    {"Bob Johnson", "1112223333", "bob@company.com"},
    {"Carol White", "4445556666", "carol@company.com"},
    {"David Brown", "7778889999", "david@example.com"},
    {"Eve Davis", "6665554444", "eve@example.com"},
    {"Frank Miller", "3334445555", "frank@example.com"},
    {"Grace Wilson", "2223334444", "grace@example.com"},
    {"Hannah Clark", "5556667777", "hannah@example.com"},
    {"Ian Lewis", "8889990000", "ian@example.com"}
};

// Function definition to load dummy contact to the array
void populateAddressBook(AddressBook* addressBook)
{
   addressBook->contactCount=0;  //initialize count to 0

    for(int i=0;i<10;i++)  // as there are 10 dummy contacts run loop from 0 till 9
    {
         addressBook->contacts[i]=dummyContacts[i];    // add dummy contacts in contacts array one by one
         addressBook->contactCount++;   //increment contact count
    }
      
}