#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student 
{
    int rollno;
    char name[50];
    float percentage;
    struct Student* next;
} Student;

// Function prototypes
void stu_add(Student** head);
void stu_del(Student** head);
void stu_show(Student* head);
void stu_mod(Student* head);
void stu_save(Student* head, const char* filename);
void stu_load(Student** head, const char* filename);
void stu_sort(Student** head, int byPercentage);
void stu_rev(Student** head);
void delete_all_records(Student** head);

int main() 
{
    Student* head = NULL;
    stu_load(&head, "student.dat");  // Load records from file

    char choice;
    while (1) 
    {   //////////list format
        printf("***** STUDENT RECORD MENU *****\n");
        printf("a/A: Add new record\n");
        printf("d/D: Delete a record\n");
        printf("s/S: Show the list\n");
        printf("m/M: Modify a record\n");
        printf("v/V: Save\n");
        printf("c/E: Exit\n");
        printf("t/T: Sort the list\n");
        printf("l/L: Delete all records\n");
        printf("r/R: Reverse the list\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice) 
        {
            case 'a': case 'A': stu_add(&head); break;
            case 'd': case 'D': stu_del(&head); break;
            case 's': case 'S': stu_show(head); break;
            case 'm': case 'M': stu_mod(head); break;
            case 'v': case 'V': stu_save(head, "student.dat"); break;
            case 'c': case 'e':
                printf("Save and exit (s/S) or exit without saving (e/E)? ");
                scanf(" %c", &choice);
                if (choice == 's' || choice == 'S') 
                stu_save(head, "student.txt");
                else if(choice == 'e' || choice =='E')
                delete_all_records(&head);
                exit(0);
            case 't': case 'T': stu_sort(&head, 0); break;
            case 'l': case 'L': delete_all_records(&head); break;
            case 'r': case 'R': stu_rev(&head); break;
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

////////add new data
void stu_add(Student** head) 
{
    Student* new = (Student*)malloc(sizeof(Student));
    if (!new) 
    {
        printf("Memory allocation failed!\n");
        return;
    }

    int rollno = 1;
    Student* temp = *head;
    while (temp)                    ////////till the last element 
    {
        if (temp->rollno == rollno) 
        {
            rollno++;
            temp = *head;
        } 
        else 
        {
            temp = temp->next;
        }
    }
    new->rollno = rollno;

    printf("Enter student name: ");
    scanf("%s", new->name);
    printf("Enter percentage: ");
    scanf("%f", &new->percentage);

    new->next = *head;
    *head = new;
    printf("Student added with rollno %d\n", rollno);
}

void stu_show(Student* head) 
{
    printf("Roll No\tName\tPercentage\n");
    printf("-----------------------------------------\n");
    while (head) 
    {
        printf("%d\t%s\t%.2f\n", head->rollno, head->name, head->percentage);
        head = head->next;
    }
}

void stu_save(Student* head, const char* filename) 
{
    FILE* fp = fopen(filename, "wb");
    if (!fp) 
    {
        printf("Error opening file for saving!\n");
        return;
    }
    while (head) 
    {
        fwrite(head, sizeof(Student), 1, fp);
        head = head->next;
    }
    fclose(fp);
    printf("Records saved to %s.\n", filename);
}

void stu_load(Student** head, const char* filename) 
{
    FILE* fp = fopen(filename, "rb");
    if (!fp) return;

    Student temp;
    while (fread(&temp, sizeof(Student), 1, fp)) 
    {
        Student* new = (Student*)malloc(sizeof(Student));
        if (!new) 
        {
            printf("Memory allocation failed!\n");
            return;
        }
        *new = temp;
        new->next = *head;
        *head = new;
    }
    fclose(fp);
}

void stu_mod(Student* head) 
{
    if (!head) 
    {
        printf("No records available to modify.\n");
        return;
    }

    int rollno;
    printf("Enter roll number to modify: ");
    scanf("%d", &rollno);

    Student* temp = head;
    while (temp && temp->rollno != rollno) 
    {
        temp = temp->next;
    }

    if (temp) 
    {
        printf("Enter new name: ");
        scanf("%s", temp->name);
        printf("Enter new percentage: ");
        scanf("%f", &temp->percentage);
        printf("Record updated successfully.\n");
    } 
    else 
    {
        printf("Roll number not found.\n");
    }
}

void stu_sort(Student** head, int byPercentage) 
{
    if (!*head) 
    {
        printf("No records to sort.\n");
        return;
    }

    for (Student* i = *head; i && i->next; i = i->next) 
    {
        for (Student* j = i->next; j; j = j->next) 
        {
            if ((byPercentage && i->percentage > j->percentage) || 
                (!byPercentage && strcmp(i->name, j->name) > 0)) 
                {
                // Swap records
                int temp_roll = i->rollno;
                char temp_name[50];
                float temp_percentage = i->percentage;

                strcpy(temp_name, i->name);
                i->rollno = j->rollno;
                strcpy(i->name, j->name);
                i->percentage = j->percentage;

                j->rollno = temp_roll;
                strcpy(j->name, temp_name);
                j->percentage = temp_percentage;
            }
        }
    }

    printf("Records sorted %s.\n", byPercentage ? "by percentage" : "by name");
}

void stu_rev(Student** head) 
{
    Student* prev = NULL;
    Student* current = *head;
    Student* next = NULL;

    while (current) 
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
    printf("List reversed.\n");
}

void delete_all_records(Student** head) 
{
    Student* temp;

    while (*head) 
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }

    printf("All records have been deleted.\n");
}

void stu_del(Student** head) 
{
    int rollno;
    printf("Enter roll number to delete: ");
    scanf("%d", &rollno);

    Student* temp = *head;
    Student* prev = NULL;

    while (temp && temp->rollno != rollno) 
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp) 
    {
        if (prev) 
        {
            prev->next = temp->next;
        } 
        else 
        {
            *head = temp->next;
        }
        free(temp);
        printf("Record deleted.\n");
    } 
    else 
    {
        printf("Roll number not found.\n");
    }
}


