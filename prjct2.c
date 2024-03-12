#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Passenger {
    char name[50];
    int age;
};

struct Flight {
    int flightNumber;
    int totalSeats;
    int availableSeats;
    struct Passenger** passengers;
    struct Flight* left;
    struct Flight* right;
};

struct Passenger* createPassenger(const char* name, int age) {
    struct Passenger* newPassenger = (struct Passenger*)malloc(sizeof(struct Passenger));
    strcpy(newPassenger->name, name);
    newPassenger->age = age;
    return newPassenger;
}

struct Flight* createFlight(int flightNumber, int totalSeats) {
    struct Flight* newFlight = (struct Flight*)malloc(sizeof(struct Flight));
    newFlight->flightNumber = flightNumber;
    newFlight->totalSeats = totalSeats;
    newFlight->availableSeats = totalSeats;
    newFlight->passengers = (struct Passenger**)malloc(totalSeats * sizeof(struct Passenger*));
    for (int i = 0; i < totalSeats; i++) {
        newFlight->passengers[i] = NULL;
    }
    newFlight->left = NULL;
    newFlight->right = NULL;
    return newFlight;
}

struct Flight* insertFlight(struct Flight* root, int flightNumber, int totalSeats) {
    if (root == NULL) {
        return createFlight(flightNumber, totalSeats);
    }

    if (flightNumber < root->flightNumber) {
        root->left = insertFlight(root->left, flightNumber, totalSeats);
    } else if (flightNumber > root->flightNumber) {
        root->right = insertFlight(root->right, flightNumber, totalSeats);
    }

    return root;
}

struct Flight* searchFlight(struct Flight* root, int flightNumber) {
    if (root == NULL || root->flightNumber == flightNumber) {
        return root;
    }

    if (flightNumber < root->flightNumber) {
        return searchFlight(root->left, flightNumber);
    } else {
        return searchFlight(root->right, flightNumber);
    }
}

int checkAvailability(struct Flight* root, int flightNumber) {
    struct Flight* flight = searchFlight(root, flightNumber);
    if (flight == NULL) {
        return -1;
    }

    return flight->availableSeats;
}

bool bookTicket(struct Flight* root, int flightNumber, const char* name, int age) {
    struct Flight* flight = searchFlight(root, flightNumber);
    if (flight == NULL) {
        return false;
    }

    if (flight->availableSeats == 0) {
        return false;
    }

    int seatIndex;
    for (seatIndex = 0; seatIndex < flight->totalSeats; seatIndex++) {
        if (flight->passengers[seatIndex] == NULL) {
            flight->passengers[seatIndex] = createPassenger(name, age);
            break;
        }
    }

    flight->availableSeats--;
    return true;
}

void displayBookedTickets(struct Flight* root, int flightNumber) {
    struct Flight* flight = searchFlight(root, flightNumber);
    if (flight == NULL) {
        printf("Flight not found!\n");
        return;
    }

    printf("Booked Tickets for Flight Number %d:\n", flightNumber);
    for (int i = 0; i < flight->totalSeats; i++) {
        if (flight->passengers[i] != NULL) {
            printf("Seat %d: Passenger Name: %s, Age: %d\n", i + 1, flight->passengers[i]->name, flight->passengers[i]->age);
        }
    }
}

bool cancelBooking(struct Flight* root, int flightNumber, int seatIndex) {
    struct Flight* flight = searchFlight(root, flightNumber);
    if (flight == NULL) {
        return false;
    }

    if (seatIndex < 1 || seatIndex > flight->totalSeats || flight->passengers[seatIndex - 1] == NULL) {
        return false;
    }

    free(flight->passengers[seatIndex - 1]);
    flight->passengers[seatIndex - 1] = NULL;
    flight->availableSeats++;
    return true;
}

void displayPassengerDetails(struct Flight* root, int flightNumber, int seatIndex) {
    struct Flight* flight = searchFlight(root, flightNumber);
    if (flight == NULL) {
        printf("Flight not found!\n");
        return;
    }

    if (seatIndex < 1 || seatIndex > flight->totalSeats || flight->passengers[seatIndex - 1] == NULL) {
        printf("Invalid seat or seat not booked!\n");
        return;
    }

    printf("Passenger Details for Seat %d on Flight Number %d:\n", seatIndex, flightNumber);
    printf("Passenger Name: %s\n", flight->passengers[seatIndex - 1]->name);
    printf("Passenger Age: %d\n", flight->passengers[seatIndex - 1]->age);
}

void freeFlight(struct Flight* flight) {
    if (flight == NULL) {
        return;
    }

    for (int i = 0; i < flight->totalSeats; i++) {
        free(flight->passengers[i]);
    }

    free(flight->passengers);
    free(flight);
}

void displayFlights(struct Flight* root) {
    if (root == NULL) {
        return;
    }

    displayFlights(root->left);
    printf("Flight Number: %d, Total Seats: %d, Available Seats: %d\n", root->flightNumber, root->totalSeats, root->availableSeats);
    displayFlights(root->right);
}

int main() {
    struct Flight* root = NULL;

    // Statically define flight details
    root = insertFlight(root, 101, 50);
    root = insertFlight(root, 102, 60);
    root = insertFlight(root, 103, 40);
    root = insertFlight(root, 104, 70);
    root = insertFlight(root, 105, 55);

    int choice, flightNumber, totalSeats;
    char name[50];
    int age;
    int seatIndex;

    do {
        printf("\nFlight Reservation System\n");
        printf("1. Check Seat Availability\n");
        printf("2. Book Ticket\n");
        printf("3. Display Available Flights\n");
        printf("4. Display Booked Tickets for a Flight\n");
        printf("5. Display Passenger Details for a Seat\n");
        printf("6. Cancel Booking\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Flight Number: ");
                scanf("%d", &flightNumber);
                int availability = checkAvailability(root, flightNumber);
                if (availability == -1) {
                    printf("Flight not found!\n");
                } else {
                    printf("Available Seats: %d\n", availability);
                }
                break;
            case 2:
                printf("Enter Flight Number: ");
                scanf("%d", &flightNumber);
                printf("Enter Passenger Name: ");
                scanf("%s", name);
                printf("Enter Passenger Age: ");
                scanf("%d", &age);
                if (bookTicket(root, flightNumber, name, age)) {
                    printf("Ticket booked successfully!\n");
                } else {
                    printf("Booking failed. Either the flight is not found or no available seats.\n");
                }
                break;
            case 3:
                printf("Available Flights:\n");
                displayFlights(root);
                break;
            case 4:
                printf("Enter Flight Number: ");
                scanf("%d", &flightNumber);
                displayBookedTickets(root, flightNumber);
                break;
            case 5:
                printf("Enter Flight Number: ");
                scanf("%d", &flightNumber);
                printf("Enter Seat Index (1-%d): ", totalSeats);
                scanf("%d", &seatIndex);
                displayPassengerDetails(root, flightNumber, seatIndex);
                break;
            case 6:
                printf("Enter Flight Number: ");
                scanf("%d", &flightNumber);
                printf("Enter Seat Index (1-%d): ", totalSeats);
                scanf("%d", &seatIndex);
                if (cancelBooking(root, flightNumber, seatIndex)) {
                    printf("Booking canceled successfully!\n");
                } else {
                    printf("Cancellation failed. Either the flight is not found, invalid seat, or seat not booked.\n");
                }
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    // Free allocated memory before exiting
    freeFlight(root);

    return 0;
}
