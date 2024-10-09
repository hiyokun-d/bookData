#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100

struct BookData
{
    int book_id;
    char book_title[50];
    char author_name[50];
    int Year_published;
};

FILE *fptr;

void addBook(struct BookData bookData[], int *numBooks)
{
    FILE *fptr = fopen("Library.txt", "a");
    if (*numBooks >= MAX_BOOKS)
    {
        printf("THE LIBRARY IS FULL TRY TO ERASE SOME BOOK THAT YOU DIDN'T ACTUALLY READ \n");
        return;
    }

    printf("Enter the book ID of yours: ");
    scanf("%d", &bookData[*numBooks].book_id);
    printf("your book id is %d \n", bookData[*numBooks].book_id);
    getchar();

    printf("Enter the book title: ");
    scanf("%49[^\n]", bookData[*numBooks].book_title);
    printf("your book id: %d \n your book title: %s \n", bookData[*numBooks].book_id, bookData[*numBooks].book_title);
    getchar();

    printf("Enter the author of the book: ");
    scanf("%49[^\n]", bookData[*numBooks].author_name);
    printf("your book id: %d \n your book title: %s \n author name is: %s \n", bookData[*numBooks].book_id, bookData[*numBooks].book_title, bookData[*numBooks].author_name);
    getchar();

    printf("Enter the year of the book published: ");
    scanf("%4d", &bookData[*numBooks].Year_published);
    printf("your book id: %d \n your book title: %s \n author name is: %s \n Year published: %4d \n", bookData[*numBooks].book_id, bookData[*numBooks].book_title, bookData[*numBooks].author_name, bookData[*numBooks].Year_published);
    getchar();

    fprintf(fptr, "Book id: %d \n Book title: %s \n Author name: %s \n Year published: %d \n", bookData[*numBooks].book_id, bookData[*numBooks].book_title, bookData[*numBooks].author_name, bookData[*numBooks].Year_published);
    (*numBooks)++;
    fclose(fptr);
}

void sortBooksById(struct BookData bookData[], int num_books)
{
    for (int i = 0; i < num_books - 1; i++)
    {
        for (int j = 0; j < num_books - i - 1; j++)
        {
            if (bookData[j].book_id > bookData[j + 1].book_id)
            {
                // Swap books[j] and books[j + 1]
                struct BookData temp = bookData[j];
                bookData[j] = bookData[j + 1];
                bookData[j + 1] = temp;
            }
        }
    }
}

void displayBook(struct BookData bookData[], int numBooks)
{
    if (numBooks == 0)
    {
        printf("There's no book in the library input some \n");
    }

    sortBooksById(bookData, numBooks);

    for (int i = 0; i < numBooks; i++)
    {
        printf("-----%s----\n", bookData[i].book_title);
        printf("book id: %d \n", bookData[i].book_id);
        printf("author: %s \n", bookData[i].author_name);
        printf("year published: %d \n", bookData[i].Year_published);
        printf("------------\n");
    }
    fclose(fptr);
}

int isIdInFile(FILE *file, int id) {
    int book_id;
    char buffer[256];

    rewind(file);
    while (fscanf(file, "Book id: %d\n", &book_id) == 1) {
        fgets(buffer, sizeof(buffer), file); 
        fgets(buffer, sizeof(buffer), file); 
        fgets(buffer, sizeof(buffer), file); 
        if (book_id == id) {
            return 1;
        }
    }
    return 0; 
}


void saveBooksAndExit(struct BookData bookData[], int numBooks) {
    FILE *fptr = fopen("Library.txt", "w");
    if (fptr == NULL) {
        perror("Error opening file for reading");
        return;
    }

    // Check for duplicates and adjust IDs
    for (int i = 0; i < numBooks; i++) {
        int original_id = bookData[i].book_id;
        while (isIdInFile(fptr, bookData[i].book_id)) {
            // Increment ID if duplicate is found
            bookData[i].book_id++;
        }
    }

    // Move to the end of the file
    fseek(fptr, 0, SEEK_END);

    // Save the updated book data
    for (int i = 0; i < numBooks; i++) {
        fprintf(fptr, "Book id: %d\nBook title: %s\nAuthor name: %s\nYear published: %d\n\n",
                bookData[i].book_id, bookData[i].book_title, bookData[i].author_name, bookData[i].Year_published);
    }

    fclose(fptr);
}

void fileReader(struct BookData bookData[], int *numBooks)
{
    FILE *fptr = fopen("Library.txt", "r+");
    if (fptr == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct BookData tempBook;

    *numBooks = 0;
    while (fscanf(fptr, "Book id: %d\n", &tempBook.book_id) == 1)
    {
        fscanf(fptr, "Book title: %[^\n]\n", tempBook.book_title);
        fscanf(fptr, "Author name: %[^\n]\n", tempBook.author_name);
        fscanf(fptr, "Year published: %d\n", &tempBook.Year_published);

        if (*numBooks < MAX_BOOKS)
        {
            bookData[*numBooks] = tempBook;
            (*numBooks)++;
        }
        else
        {
            printf("The book data array is full.\n");
            break;
        }
    }
}

void updateBooks(struct BookData bookData[], int *numBooks)
{
    FILE *fptr = fopen("Library.txt", "r+");
    if (fptr == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Temporary storage for reading the file
    int idToUpdate;
    int found = 0;

    // Prompt user for the book ID to update
    printf("Enter the book ID you want to update: ");
    scanf("%d", &idToUpdate);

    // Find the book with the given ID and update it
    for (int i = 0; i < *numBooks; i++)
    {
        if (bookData[i].book_id == idToUpdate)
        {
            found = 1;
            // Prompt user for new details
            printf("Updating book with ID %d\n", idToUpdate);
            printf("Enter new book title: ");
            scanf(" %[^\n]", bookData[i].book_title); 
            printf("Enter new author name: ");
            scanf(" %[^\n]", bookData[i].author_name);
            printf("Enter new year published: ");
            scanf("%d", &bookData[i].Year_published);

            fseek(fptr, 0, SEEK_SET);

            for (int j = 0; j < *numBooks; j++)
            {
                fprintf(fptr, "Book id: %d\nBook title: %s\nAuthor name: %s\nYear published: %d\n\n",
                        bookData[j].book_id, bookData[j].book_title, bookData[j].author_name, bookData[j].Year_published);
            }

            printf("Book updated successfully.\n");
            break;
        }
    }

    if (!found)
    {
        printf("No book found with ID %d.\n", idToUpdate);
    }

    fclose(fptr);
}


void eraseBook(struct BookData bookData[], int *numBooks)
{
        FILE *fptr = fopen("Library.txt", "r+");
    if (fptr == NULL) {
        perror("Error opening file");
        return;
    }

    // Temporary storage for reading the file
    struct BookData tempBook;
    int idToErase;
    int found = 0;

    // Prompt user for the book ID to erase
    printf("Enter the book ID you want to delete: ");
    scanf("%d", &idToErase);

    // Read the existing file data
    struct BookData tempBooks[MAX_BOOKS];
    int tempNumBooks = 0;

    while (fscanf(fptr, "Book id: %d\n", &tempBook.book_id) == 1) {
        fscanf(fptr, "Book title: %[^\n]\n", tempBook.book_title);
        fscanf(fptr, "Author name: %[^\n]\n", tempBook.author_name);
        fscanf(fptr, "Year published: %d\n", &tempBook.Year_published);

        // Check if the book ID matches the ID to be erased
        if (tempBook.book_id != idToErase) {
            // Store the book in the temporary array if not to be erased
            if (tempNumBooks < MAX_BOOKS) {
                tempBooks[tempNumBooks++] = tempBook;
            }
        } else {
            found = 1; // Mark as found
        }
    }

    // If book not found
    if (!found) {
        printf("No book found with ID %d.\n", idToErase);
        fclose(fptr);
        return;
    }

    // Reopen the file in write mode to overwrite it
    freopen("Library.txt", "w", fptr);
    if (fptr == NULL) {
        perror("Error opening file for writing");
        return;
    }

    // Write back the books except the one deleted
    for (int i = 0; i < tempNumBooks; i++) {
        fprintf(fptr, "Book id: %d\nBook title: %s\nAuthor name: %s\nYear published: %d\n\n",
                tempBooks[i].book_id, tempBooks[i].book_title, tempBooks[i].author_name, tempBooks[i].Year_published);
    }

    // Update bookData array and numBooks
    int newNumBooks = 0;
    for (int i = 0; i < tempNumBooks; i++) {
        if (tempBooks[i].book_id != idToErase) {
            bookData[newNumBooks++] = tempBooks[i];
        }
    }
    *numBooks = newNumBooks;

    printf("Book erased successfully.\n");

    fclose(fptr);
}

int main()
{
    int choose = 0;
    int numBooks = 0;
    struct BookData bookData[MAX_BOOKS];

    fileReader(bookData, &numBooks);

    while (choose != 5)
    {
        printf("1. input Book record \n 2. DIsplay book record \n 3. Update book record \n 4. Erase book record \n 5. Save and Exit \n");
        scanf("%d", &choose);
        getchar();

        switch (choose)
        {
        case 1:
            addBook(bookData, &numBooks);
            break;
        case 2:
            displayBook(bookData, numBooks);
            break;
        case 3:
            updateBooks(bookData, &numBooks);
            break;
        case 4:
            eraseBook(bookData, &numBooks);
            break;
        case 5:
            saveBooksAndExit(bookData, numBooks);
        default:
            break;
        }
    }
    return 0;
}