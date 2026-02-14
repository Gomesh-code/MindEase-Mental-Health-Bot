/*
 Bank Customer Queue Management System (Linked List Queue)
 Purpose: Simple dynamic queue for bank customers 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Customer {
    int ticket;                 // unique ticket number
    char name[64];              // customer name
    char service[32];           // service type (e.g., "Cash", "Account", "Loan")
    struct Customer *next;      // pointer to next customer (queue link)
} Customer;

Customer *front = NULL;
Customer *rear  = NULL;
int next_ticket = 1;           // increments for each customer

// Check if queue is empty
int isEmpty() {
    return front == NULL;
}

// Enqueue a new customer at the rear
void enqueue(const char *name, const char *service) {
    Customer *newNode = (Customer *)malloc(sizeof(Customer));
    if (!newNode) {
        printf("Memory allocation failed. Cannot enqueue.\n");
        return;
    }
    newNode->ticket = next_ticket++;
    strncpy(newNode->name, name, sizeof(newNode->name)-1);
    newNode->name[sizeof(newNode->name)-1] = '\0';
    strncpy(newNode->service, service, sizeof(newNode->service)-1);
    newNode->service[sizeof(newNode->service)-1] = '\0';
    newNode->next = NULL;

    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }

    printf("Customer enqueued. Ticket no: %d | Name: %s | Service: %s\n",
           newNode->ticket, newNode->name, newNode->service);
}

// Dequeue the customer at the front (serve customer)
void dequeue() {
    if (isEmpty()) {
        printf("Queue is empty. No customer to serve.\n");
        return;
    }
    Customer *temp = front;
    printf("Serving Ticket no: %d | Name: %s | Service: %s\n",
           temp->ticket, temp->name, temp->service);

    front = front->next;
    if (front == NULL) // queue became empty
        rear = NULL;

    free(temp);
}

// Peek front customer without removing
void peek() {
    if (isEmpty()) {
        printf("Queue is empty.\n");
        return;
    }
    printf("Next to be served -> Ticket no: %d | Name: %s | Service: %s\n",
           front->ticket, front->name, front->service);
}

// Display entire queue
void displayQueue() {
    if (isEmpty()) {
        printf("Queue is empty.\n");
        return;
    }
    printf("Current Queue (front --> rear):\n");
    Customer *ptr = front;
    while (ptr) {
        printf("Ticket: %3d | Name: %-20s | Service: %-10s\n",
               ptr->ticket, ptr->name, ptr->service);
        ptr = ptr->next;
    }
}

// Clear entire queue and free memory
void clearQueue() {
    while (!isEmpty()) {
        dequeue();
    }
}

// Utility to read a line of input (trim newline)
void readLine(char *buffer, int size) {
    // read up to newline (including spaces)
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    // remove trailing newline
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
}

int main() {
    int choice;
    char name[64];
    char service[32];

    printf("=== Bank Customer Queue Management System ===\n");

    while (1) {
        printf("\nMenu:\n");
        printf("1. Enqueue customer\n");
        printf("2. Dequeue (Serve) customer\n");
        printf("3. Peek front customer\n");
        printf("4. Display queue\n");
        printf("5. Clear queue\n");
        printf("6. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            // handle non-int input
            printf("Invalid input. Exiting.\n");
            break;
        }
        // clear newline left by scanf
        getchar();

        switch (choice) {
            case 1:
                printf("Enter customer name: ");
                readLine(name, sizeof(name));
                if (strlen(name) == 0) {
                    printf("Name cannot be empty. Enqueue cancelled.\n");
                    break;
                }
                printf("Enter service type (Cash/Account/Loan/Other): ");
                readLine(service, sizeof(service));
                if (strlen(service) == 0) strncpy(service, "General", sizeof(service));
                enqueue(name, service);
                break;

            case 2:
                dequeue();
                break;

            case 3:
                peek();
                break;

            case 4:
                displayQueue();
                break;

            case 5:
                clearQueue();
                printf("Queue cleared.\n");
                break;

            case 6:
                clearQueue();
                printf("Exiting. Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    // safety cleanup
    clearQueue();
    return 0;
}
