#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100
#define NAME_LENGTH 50
#define PHONE_LENGTH 15
#define EMAIL_LENGTH 50

typedef struct {
    char name[NAME_LENGTH];
    char phone[PHONE_LENGTH];
    char email[EMAIL_LENGTH];
} Contact;

typedef struct {
    Contact contacts[MAX_CONTACTS];
    int count;
} AddressBook;

void loadContact(AddressBook* addressBook);
void createContact(AddressBook* addressBook);
void searchContact(AddressBook* addressBook);
void editContact(AddressBook* addressBook);
void deleteContact(AddressBook* addressBook);
void listContacts(AddressBook* addressBook);
void saveAndExit(AddressBook* addressBook);

#endif // CONTACT_H
