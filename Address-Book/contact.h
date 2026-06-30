#ifndef CONTACT_H
#define CONTACT_H   

#define MAX_CONTACTS 100   //maximum contacts that can be strored

// structure Contact - user defined datatype which contains name, phone and email as its members.
typedef struct {
    char name[50]; 
    char phone[20];
    char email[50];
} Contact;

// structure AddressBook - (nested structure)  contains array of structure Contact with the limit of 100 and contact count as its member
typedef struct {   
    Contact contacts[MAX_CONTACTS];
    int contactCount;
} AddressBook;


// Function declarations
void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *AddressBook);

#endif
