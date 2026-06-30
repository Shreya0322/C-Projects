#ifndef FILE_H
#define FILE_H

#include "contact.h"

//Function declarations
void saveContactsToFile(AddressBook *addressBook);
void loadContactsFromFile(AddressBook *addressBook);
int getcount(char *file);

#endif
