#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"

void loadContact(AddressBook* addressBook) {
    addressBook->count = 0;
    FILE* file = fopen("contacts.csv", "r");
    if (file != NULL) 
    {
        // Read the count of contacts
        fscanf(file, "%d\n", &addressBook->count);
        for (int i = 0; i < addressBook->count; i++) {
            fscanf(file, " %[^\n],%[^\n],%[^\n]", 
                   addressBook->contacts[i].name, 
                   addressBook->contacts[i].phone, 
                   addressBook->contacts[i].email);
        }
        fclose(file);
    }
}

int isValidName(const char* name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0; // Invalid character found
        }
    }
    return 1; // Valid name
}

int isValidPhone(const char* phone) {
    if (strlen(phone) != 10) {
        return 0; // Phone must be 10 digits
    }
    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i])) {
            return 0; // Invalid character found
        }
    }
    return 1; // Valid phone
}

int isValidEmail(const char* email) {
    const char* at = strchr(email, '@');
    const char* dot = strrchr(email, '.');
    return (at != NULL && dot != NULL && at < dot && strcmp(dot, ".com") == 0);
}

void createContact(AddressBook* addressBook) {
    if (addressBook->count >= MAX_CONTACTS) {
        printf("Address book is full!\n");
        return;
    }
    
    Contact newContact;

    // Get the name
    printf("Enter name: ");
    scanf(" %[^\n]", newContact.name); // Read until newline
    if (strlen(newContact.name) == 0) {
        printf("Name cannot be empty.\n");
        return;
    }
    
    // Validate name
    if (!isValidName(newContact.name)) {
        printf("Invalid name. Please use only alphabetic characters.\n");
        return;
    }

    // Get the phone number
    printf("Enter phone (10 digits): ");
    scanf(" %[^\n]", newContact.phone);
    
    // Validate phone number
    if (!isValidPhone(newContact.phone)) {
        printf("Invalid phone number. Please enter exactly 10 digits.\n");
        return;
    }

    // Get the email
    printf("Enter email: ");
    scanf(" %[^\n]", newContact.email);
    
    // Validate email
    if (!isValidEmail(newContact.email)) {
        printf("Invalid email. It should contain '@' and end with '.com'.\n");
        return;
    }

    // Add the contact
    addressBook->contacts[addressBook->count] = newContact;
    addressBook->count++;
    printf("Contact added!\n");
}


void searchContact(AddressBook* addressBook) {
    char name[NAME_LENGTH];
    printf("Enter name to search: ");
    scanf(" %[^\n]", name);
    for (int i = 0; i < addressBook->count; i++) {
        if (strcmp(addressBook->contacts[i].name, name) == 0) {
            printf("Contact found: %s, %s, %s\n", 
                   addressBook->contacts[i].name, 
                   addressBook->contacts[i].phone, 
                   addressBook->contacts[i].email);
            return;
        }
    }
    printf("Contact not found.\n");
}

void editContact(AddressBook* addressBook) {
    char name[NAME_LENGTH];
    printf("Enter name to edit: ");
    scanf(" %[^\n]", name);
    for (int i = 0; i < addressBook->count; i++) {
        if (strcmp(addressBook->contacts[i].name, name) == 0) {
            printf("Editing contact: %s\n", name);
            printf("Enter new phone (10 digits): ");
            scanf(" %[^\n]", addressBook->contacts[i].phone);
            if (!isValidPhone(addressBook->contacts[i].phone)) {
                printf("Invalid phone number. It should be 10 digits.\n");
                return;
            }

            printf("Enter new email: ");
            scanf(" %[^\n]", addressBook->contacts[i].email);
            if (!isValidEmail(addressBook->contacts[i].email)) {
                printf("Invalid email. It should contain '@' and end with '.com'.\n");
                return;
            }
            printf("Contact updated!\n");
            return;
        }
    }
    printf("Contact not found.\n");
}

void deleteContact(AddressBook* addressBook) {
    char name[NAME_LENGTH];
    printf("Enter name to delete: ");
    scanf(" %[^\n]", name);
    for (int i = 0; i < addressBook->count; i++) {
        if (strcmp(addressBook->contacts[i].name, name) == 0) {
            addressBook->contacts[i] = addressBook->contacts[addressBook->count - 1];
            addressBook->count--;
            printf("Contact deleted!\n");
            return;
        }
    }
    printf("Contact not found.\n");
}

void listContacts(AddressBook* addressBook) {
    if (addressBook->count == 0) {
        printf("No contacts available.\n");
        return;
    }
    for (int i = 0; i < addressBook->count; i++) {
        printf("Contact %d: %s, %s, %s\n", 
               i + 1, 
               addressBook->contacts[i].name, 
               addressBook->contacts[i].phone, 
               addressBook->contacts[i].email);
    }
}

void saveAndExit(AddressBook* addressBook) {
    FILE* file = fopen("contacts.csv", "w");
    if (file == NULL) {
        printf("Error saving contacts.\n");
        return;
    }
    fprintf(file, "%d\n", addressBook->count);
    for (int i = 0; i < addressBook->count; i++) {
        fprintf(file, "%s,%s,%s\n", 
                addressBook->contacts[i].name, 
                addressBook->contacts[i].phone, 
                addressBook->contacts[i].email);
    }
    fclose(file);
    printf("Contacts saved. Exiting...\n");
}
