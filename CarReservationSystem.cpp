#include <windows.h> // For system("pause") and system("cls")
#include <iostream>  // For cin and cout
#include <cstring>   // For strcmp and strcpy
#include <unistd.h>  // For sleep
#include <cstdlib>   // For system functions
#include <cstdio>    // For sprintf

using namespace std;

// Global variables
const int MAX_CARS = 5;
char cars[MAX_CARS][50] = {
    "Honda Accord",
    "Nissan Altima",
    "Tesla Model 3",
    "BMW 3 Series",
    "Mercedes C-Class"
};

// Reservation numbers
char reservationNumbers[MAX_CARS][10]; // Stores unique reservation numbers (e.g., R001, R002, etc.)

// Stack structure to keep track of recent reservations
#define STACK_SIZE 5
typedef struct {
    char reservationNumber[10];
    char carName[50];
    int carIndex;
} Reservation;

Reservation reservationStack[STACK_SIZE];
int stackTop = -1; // Stack pointer

// Queue structure to manage a waiting list
#define QUEUE_SIZE 10
typedef struct {
    char username[50];
    int carIndex;
} WaitingUser;

WaitingUser waitingQueue[QUEUE_SIZE];
int queueFront = -1, queueRear = -1; // Queue pointers

int years[MAX_CARS] = {2022, 2021, 2023, 2022, 2023};
float price[MAX_CARS] = {10, 20, 30, 40, 50};

char reserveTime[MAX_CARS][50];
char username[50];
int carnum;
int availableCars = MAX_CARS;
char reservemorecar, feedback;
int choice;
bool reserved[MAX_CARS] = {false, false, false, false, false};
float price_of_car[MAX_CARS];
int period_of_car[MAX_CARS] = {0, 0, 0, 0, 0};
int carIndex;

// Function to push a reservation onto the stack
void pushReservation(char* reservationNumber, char* carName, int carIndex) {
    if (stackTop < STACK_SIZE - 1) {
        stackTop++;
        strcpy(reservationStack[stackTop].reservationNumber, reservationNumber);
        strcpy(reservationStack[stackTop].carName, carName);
        reservationStack[stackTop].carIndex = carIndex;
    } else {
        cout << "Stack is full. Cannot add more reservations." << endl;
        system("pause");
    }
}

Reservation popReservation() {
    Reservation emptyReservation = {"", "", -1};
    if (stackTop >= 0) {
        Reservation res = reservationStack[stackTop];
        stackTop--;
        return res;
    } else {
        cout << "Stack is empty. No reservations to pop." << endl;
        system("pause");
        return emptyReservation;
    }
}

// Function to display the recent reservations from the stack
void displayRecentReservations() {
    system("cls");
    printf("+-----------------------------+----------------+----------------+\n");
    printf("|          Car Name           | Reservation No. | Car Index      |\n");
    printf("+-----------------------------+----------------+----------------+\n");

    for (int i = stackTop; i >= 0; i--) {
        printf("| %-27s | %-15s | %-14d |\n", reservationStack[i].carName, reservationStack[i].reservationNumber, reservationStack[i].carIndex);
    }
    printf("+-----------------------------+----------------+----------------+\n");
    system("pause");
}

// Function to add a user to the waiting queue
void enqueueWaitingUser(char* username, int carIndex) {
    if (queueRear == QUEUE_SIZE - 1) {
        printf("Waiting list is full. Cannot add more users.\n");
        system("pause");
        return;
    } else {
        if (queueFront == -1) queueFront = 0;
        queueRear++;
        strcpy(waitingQueue[queueRear].username, username);
        waitingQueue[queueRear].carIndex = carIndex;
        printf("User %s added to the waiting list for car %d.\n", username, carIndex + 1);
    }
}

// Function to remove a user from the waiting queue
WaitingUser dequeueWaitingUser() {
    WaitingUser emptyUser = {"", -1};
    if (queueFront == -1 || queueFront > queueRear) {
        printf("Waiting list is empty.\n");
        return emptyUser;
    } else {
        WaitingUser user = waitingQueue[queueFront];
        queueFront++;
        if (queueFront > queueRear) {
            queueFront = queueRear = -1; // Reset queue
        }
        return user;
    }
}

// Function to display the waiting list
void displayWaitingList() {
    system("cls");
    printf("+-----------------------------+----------------+\n");
    printf("|          Username           | Car Index      |\n");
    printf("+-----------------------------+----------------+\n");

    for (int i = queueFront; i <= queueRear; i++) {
        printf("| %-27s | %-14d |\n", waitingQueue[i].username, waitingQueue[i].carIndex);
    }
    printf("+-----------------------------+----------------+\n");
    system("pause");
}

// Custom time function
void custom_getTime(char* buffer) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(buffer, "%02d/%02d/%04d %02d:%02d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute); // Format: DD/MM/YYYY HH:MM
}

// Function prototypes
void Welcome();
void SignUp();
void Login();
void Menu();
void display_availableCars(bool showPrompt = true);
void add_carReservation();
void reserveCal();
void returnCar();
void displayInvoice(int reserved_car[], int num_car);
void Thankyou();
void editReservation();
void deleteReservation();
void displayReservation();
void searchReservation();

// Welcome screen
void Welcome() {
    cout << "=======================================================================================================================" << endl;
    cout << "=======================================================================================================================" << endl << endl;
    cout << "                                              _______________" << endl;
    cout << "                                             /               \\" << endl;
    cout << "                                            /                 \\" << endl;
    cout << "                                    _______/___________________\\______________" << endl;
    cout << "                                   |   ....................................   |" << endl;
    cout << "                                   |  :    Welcome To SwiftCar Reserve!    :  |" << endl;
    cout << "                                   |   ....................................   |" << endl;
    cout << "                                   |__________________________________________|" << endl;
    cout << "                                            O                     O           " << endl << endl;
    cout << " =======================================================================================================================" << endl;
    cout << "|                                               WELCOME ABROAD!                                                         |" << endl;
    cout << "|                                     It's Great to Have You as Part of Us ^_^                                          |" << endl;
    cout << " =======================================================================================================================" << endl << endl;
    cout << "                                                  System Aim:    " << endl;
    cout << "                                              -------------------" << endl;
    cout << "              SwiftCar Reserve aims to to provide a seamless, efficient and user-friendly car reservation system        " << endl;
    cout << "                     that simplifies booking processes and delivers a hassle-free experience for users. " << endl << endl;
    cout << "=======================================================================================================================" << endl << endl;

    system("pause");
    system("cls");
}

// Sign-up function
void SignUp() {
    char temp_username[50];
    int temp_password;

    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::: SIGN UP PAGE :::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl << endl;
    cout << "                                                      [Sign Up]" << endl;
    cout << "                                      ---------------------------------------------------" << endl;
    cout << "                                           Please set your username       : ";
    cin >> temp_username;

    // Check if username already exists
    FILE* check_file = fopen("reglog.txt", "r");
    char existing_username[50];
    int existing_password;
    bool username_exists = false;

    while (fscanf(check_file, "%s %d", existing_username, &existing_password) != EOF) {
        if (strcmp(existing_username, temp_username) == 0) {
            username_exists = true;
            break;
        }
    }
    fclose(check_file);

    if (username_exists) {
        cout << "\n                                      [Username already exists! Please try again.]" << endl << endl;
        system("pause");
        system("cls");
        SignUp(); // Recursively call SignUp() to prompt the user again
        return;
    }

    cout << "\n                                                    Mr/Ms. " << temp_username << endl;
    cout << "                                          Welcome to our Car Reservation System" << endl;

    int pss1, pss2;

    cout << "\n                                           Please set your password       : ";
    cin >> pss1;
    cout << "                                           Please set again your password : ";
    cin >> pss2;

    while (pss1 != pss2) {
        system("cls");
        cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
        cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl << endl;
        cout << "                                       [Error, both passwords are not the same !]" << endl << endl;
        cout << "                                                  [Please try again !]" << endl << endl;

        cout << "                                           Please set your password       : ";
        cin >> pss1;
        cout << "                                           Please set again your password : ";
        cin >> pss2;
    }

    // Save to file
    FILE* outFile = fopen("reglog.txt", "a");
    fprintf(outFile, "%s %d\n", temp_username, pss1);
    fclose(outFile);

    // Copy username to global variable
    strcpy(username, temp_username);

    cout << "\n                                                 [Sign Up Successfully !]" << endl << endl;
    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

    system("pause");
    system("cls");
}

// Login function
void Login() {
    char temp_username[50];
    int input_password;
    bool login_success = false;

    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
    cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::: LOGIN PAGE ::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl << endl;
    cout << "                                                         [Login]" << endl;
    cout << "                                            ----------------------------------------------" << endl;

    cout << "                                              Please enter your username : ";
    cin >> temp_username;

    cout << endl << "                                              Please enter your password : ";
    cin >> input_password;

    FILE* inFile = fopen("reglog.txt", "r");
    if (inFile == NULL) {
        cout << "Error opening file. Please try again." << endl;
        system("pause");
        return;
    }

    char file_username[50];
    int file_password;

    while (fscanf(inFile, "%s %d", file_username, &file_password) != EOF) {
        if (strcmp(file_username, temp_username) == 0 && file_password == input_password) {
            login_success = true;
            strcpy(username, temp_username);
            break;
        }
    }
    fclose(inFile);

    while (!login_success) {
        system("cls");
        cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
        cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl << endl;
        cout << "                                                  [Wrong credentials!]" << endl;
        cout << "                                                 ----------------------" << endl;
        cout << "                                              Please enter your username : ";
        cin >> temp_username;
        cout << "                                              Please enter your password : ";
        cin >> input_password;

        inFile = fopen("reglog.txt", "r");
        if (inFile == NULL) {
            cout << "Error opening file. Please try again." << endl;
            system("pause");
            return;
        }

        while (fscanf(inFile, "%s %d", file_username, &file_password) != EOF) {
            if (strcmp(file_username, temp_username) == 0 && file_password == input_password) {
                login_success = true;
                strcpy(username, temp_username);
                break;
            }
        }
        fclose(inFile);
    }

    cout << endl << "                                                  [Login Successfully!]" << endl << endl;
    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

    system("pause");
    system("cls");
}

// Menu function
void Menu() {
    system("CLS");
    cout << ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
    cout << ":::::::::::::::::::::::::::::::::::::::::::::::::: MAIN MENU PAGE :::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl;

    cout << "1. Display available cars" << endl;
    cout << "2. Reserve a car" << endl;
    cout << "3. Return a car" << endl;
    cout << "4. Edit reservation" << endl;
    cout << "5. Delete reservation" << endl;
    cout << "6. Display reservation" << endl;
    cout << "7. Search reservation" << endl;
    cout << "8. Display recent reservations" << endl;
    cout << "9. Display waiting list" << endl;
    cout << "10. Exit" << endl;

    cout << "Enter your choice: ";
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 10) {
        printf("Invalid input. Please enter a number between 1 and 10: ");
        while (getchar() != '\n'); // Clear the input buffer
    }
}

// Display available cars
void display_availableCars(bool showPrompt) {
    system("CLS");
    printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");
    printf("|          Name               |   Year     | Price Per Day(RM)| Availability    | Reservation No. |\n");
    printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");

    for (int i = 0; i < MAX_CARS; i++) {
        if (!reserved[i]) {
            printf("| %-1d %-25s | %-10d | %-16.2f | Available       | %-15s |\n", i+1, cars[i], years[i], price[i], "N/A");
        }
    }

    printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");
    printf("Not Available Cars:\n");

    for (int i = 0; i < MAX_CARS; i++) {
        if (reserved[i]) {
            printf("| %-1d %-25s | %-10d | %-16.2f | Reserved        | %-15s |\n", i+1, cars[i], years[i], price[i], reservationNumbers[i]);
            printf("| Reservation Time: %-42s                                    |\n", reserveTime[i]);
            printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");
        }
    }

    if (showPrompt) {
        system("pause");
    }
}

// Add car reservation
// Function to add a car reservation
void add_carReservation(int *check) {

    do {
        label1:
        system("cls");
        display_availableCars(false); // Pass false to hide the prompt


        printf("Reserving a car. Please enter the car number (1-5)\n");
        printf("(Enter 'q' or '0' to return to the main menu)\n");
        printf("Please enter your choice: ");

        char input[10];
        scanf("%s", input);

        if (strcmp(input, "q") == 0 || strcmp(input, "0") == 0) {
            *check = 0;
            return; // Return to the main menu
        }

        // Convert input to integer and validate
        char *endptr;
        carnum = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\0' || carnum < 1 || carnum > MAX_CARS) {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            system("pause");
            goto label1;
            continue;
        }

        carIndex = carnum - 1;

        if (carIndex >= 0 && carIndex < MAX_CARS && !reserved[carIndex]) {
            custom_getTime(reserveTime[carIndex]);
            system("cls");

            printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");
            printf("|          Name               |   Year     | Price Per Day(RM)| Reservation     | Reservation No. |\n");
            printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");
            printf("| %-27s | %-10d | %-16.2f | Reserved        | %-15s |\n", cars[carIndex], years[carIndex], price[carIndex], reservationNumbers[carIndex]);
            printf("| Reservation Time: %-41s |                 |                 |\n", reserveTime[carIndex]);
            printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");
            reserved[carIndex] = true;
            availableCars--;

            sprintf(reservationNumbers[carIndex], "R%03d", carIndex + 1);
            printf("| Reservation Number: %-7s |                                                                   |\n", reservationNumbers[carIndex]);
            printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");

            pushReservation(reservationNumbers[carIndex], cars[carIndex], carIndex);

            while (1) {
                printf("Confirm Your Choice (y/n): ");
                char confirm[10];
                scanf("%s", confirm);
                if (strcmp(confirm, "y") == 0 || strcmp(confirm, "Y") == 0) {
                    reservemorecar = 'y';
                    break;
                } else if (strcmp(confirm, "n") == 0 || strcmp(confirm, "N") == 0) {
                    reservemorecar = 'n';
                    reserved[carIndex] = false;
                    availableCars++;
                    break;
                } else {
                    printf("Invalid input. Please enter 'y' or 'n'.\n");
                }
            }

            if (reservemorecar == 'q' || reservemorecar == '0') {
                return; // Return to the main menu
            }

        } else {
            printf("\n----------------------------------------\n");
            printf("The car had been reserved.");
            printf("Do you want to be in the waiting list?\n");


            while(1)
            {
                printf("Confirm Your Choice (y/n): ");
                char confirm[10];
                scanf("%s", confirm);
                if(strcmp(confirm, "y")==0) {

                    printf("\nUploading.....\n\n");
                    sleep(1);
                    enqueueWaitingUser(username, carIndex);
                    printf(" \n");
                    sleep(1);
                    break;

                } else if (strcmp(confirm, "n")==0) {
                    goto label1;
                } else {
                    printf("Invalid input. Please enter 'y' or 'n'.\n");
            }


            }

            //printf("Press 'q' or '0' to return to the main menu, or any other key to select again: ");
            //char confirm[10];
            //scanf("%s", confirm);
            //reservemorecar = confirm[0];

            //if (reservemorecar == 'q' || reservemorecar == '0') {
              //  *check = 0;
                //return; // Return to the main menu
            //}
        }
    } while (reservemorecar == 'n' || reservemorecar == 'N');
}

// Reserve calculation
void reserveCal() {
    system("cls");
    float day;
    printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");
    printf("|          Name               |   Year     | Price Per Day(RM)| Reservation     | Reservation No. |\n");
    printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");
    printf("| %-27s | %-10d | %-16.2f | Reserved        | %-15s |\n", cars[carIndex], years[carIndex], price[carIndex], reservationNumbers[carIndex]);
    printf("| Reservation Time: %-41s |                 |                 |\n", reserveTime[carIndex]);
    printf("+-----------------------------+------------+------------------+-----------------+-----------------+\n");

    while (true) {
        printf("How many days you want to reserve: ");
        char input[10];
        scanf("%s", input);

    // Convert input to float and validate
    char *endptr;
    day = strtof(input, &endptr);
    if (endptr == input || *endptr != '\0' || day <= 0) {
        printf("Invalid input. Please enter a positive number of days.\n");
        continue;
    }
    break;
    }

    period_of_car[carIndex] = day;
    price_of_car[carIndex] = day * price[carIndex];

    int reservedCars[MAX_CARS];
    int numReserved = 0;
    for (int i = 0; i < MAX_CARS; ++i) {
        if (reserved[i]) {
            reservedCars[numReserved++] = i;
        }
    }

    displayInvoice(reservedCars, numReserved);

}

// Return car function
void returnCar() {
    char input[10]; // Buffer to store the reservation number
    bool found = false;
    int carIndex = -1;

    while (true) {
        label2:
        system("cls");
        printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
        printf("|          Name                |   Year     | Price Per Day(RM)| Reservation     | Reservation No. |\n");
        printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");

        bool carsReserved = false;
        for (int i = 0; i < MAX_CARS; ++i) {
            if (reserved[i]) {
                carsReserved = true;
                printf("| %-27s  | %-10d | %-16.2f | Reserved        | %-15s |\n", cars[i], years[i], price[i], reservationNumbers[i]);
                printf("| Reservation Period(Day): %-3d | Price Per Day(RM): %-6.2f     | Total Price(RM): %-6.2f           |\n", period_of_car[i], price[i], price_of_car[i]);
                printf("| Reservation Time: %-42s |                                   |\n", reserveTime[i]);
                printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
            }
        }

        if (!carsReserved) {
            printf("No cars reserved.\n");
            printf("Press Enter to return to the main menu...\n");
            system("pause");
            return;
        }

        // Prompt for reservation number
        printf("(Enter 'q' or '0' to return to the main menu)\n");
        printf("Enter the reservation number to return the car (e.g., R001): ");
        scanf("%s", input);

        // Check if the user wants to return to the main menu
        if (strcmp(input, "q") == 0 || strcmp(input, "0") == 0) {
            return; // Return to the main menu
        }

        // Find the car index for the given reservation number
        found = false;
        for (int i = 0; i < MAX_CARS; ++i) {
            if (reserved[i] && strcmp(reservationNumbers[i], input) == 0) {
                found = true;
                carIndex = i;
                break;
            }
        }

        if (!found) {
            printf("Invalid reservation number or reservation not found.\n");
            system("pause");
            continue; // Prompt again
        }

        // Confirm return
        while(1) {
            char confirm[10];
            printf("Are you sure you want to return the car with reservation number %s? (y/n): ", reservationNumbers[carIndex]);
            scanf("%s", confirm);

            if (strcmp(confirm, "y") == 0 || strcmp(confirm, "Y") == 0) {
                // Mark the car as available
                reserved[carIndex] = false;
                availableCars++;

            // Clear reservation details
                strcpy(reservationNumbers[carIndex], "");
                strcpy(reserveTime[carIndex], "");
                period_of_car[carIndex] = 0;
                price_of_car[carIndex] = 0;

                printf("\nUploading.....\n\n");
                sleep(1);
                printf("Car returned successfully.\n");
                system("pause");
                break;
                return; // Return to the main menu
            } else if (strcmp(confirm, "n") == 0 || strcmp(confirm, "N") == 0) {
                printf("Return canceled.\n");
                system("pause");
                goto label2;
                //return; // Return to the main menu
            } else {
                printf("Invalid input. Please enter 'y' or 'n'.\n");

            }
        }

    }
}

// Display invoice
void displayInvoice(int reserved_car[], int num_car) {
    if (num_car == 0) {
        printf("No cars reserved.\n");
    } else {
        system("cls");
        printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
        printf("|          Name                |   Year     | Price Per Day(RM)| Reservation     | Reservation No. |\n");
        printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");

        float total = 0;

        for (int i = 0; i < num_car; ++i) {
            int carIndex = reserved_car[i];

            printf("| %-27s  | %-10d | %-16.2f | Reserved        | %-15s |\n", cars[carIndex], years[carIndex], price[carIndex], reservationNumbers[carIndex]);
            printf("| Reservation Period(Day): %-3d | Price Per Day(RM): %-6.2f     | Total Price(RM): %-6.2f           |\n", period_of_car[carIndex], price[carIndex], price_of_car[carIndex]);
            printf("| Reservation Time: %-42s |                                   |\n", reserveTime[carIndex]);
            printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");

            total += price_of_car[carIndex];
        }

        printf("| Total Amount (RM): %-6.2f                                                                        |\n", total);
        printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
        system("pause");
    }
}

// Thank you message
void Thankyou() {
    system("cls"); // Clear the screen to create a new "page"
    cout << "=======================================================================================================================" << endl;
    cout << "                                                 [Logout Successfully!]" << endl;
    cout << "=======================================================================================================================" << endl << endl;
    cout << "                                      Mr/Ms. " << username << ", Thank you for using our system !" << endl << endl;

    cout << "                                    ================ SYSTEM DEVELOPERS =================" << endl;
    cout << "                                   |                                                    |" << endl;
    cout << "                                  |           TEO U XI          (1211105793)             |" << endl;
    cout << "                                  |           LOO WEI MENG      (1211105911)             |" << endl;
    cout << "                                  |           KONG LEE JIE      (1211105824)             |" << endl;
    cout << "                                  |           DANIEL TANG SI QI (1211105974)             |" << endl;
    cout << "                                   |                                                    |" << endl;
    cout << "                                    ====================================================" << endl << endl;
}

// Edit reservation
void editReservation() {
    system("cls");
    printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
    printf("|          Name                |   Year     | Price Per Day(RM)| Reservation     | Reservation No. |\n");
    printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");

    bool carsReserved = false;
    for (int i = 0; i < MAX_CARS; ++i) {
        if (reserved[i]) {
            carsReserved = true;
            printf("| %-27s  | %-10d | %-16.2f | Reserved        | %-15s |\n", cars[i], years[i], price[i], reservationNumbers[i]);
            printf("| Reservation Period(Day): %-3d | Price Per Day(RM): %-6.2f     | Total Price(RM): %-6.2f           |\n", period_of_car[i], price[i], price_of_car[i]);
            printf("| Reservation Time: %-42s |                                   |\n", reserveTime[i]);
            printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
        }
    }

    if (!carsReserved) {
        printf("No cars reserved.\n");
        system("pause");
        return;
    }

    char input[10];
    int reservationIndex = -1;

    while (true) {
        printf("(Enter 'q' or '0' to return to the main menu)\n");
        printf("Enter the reservation number to edit (e.g., R001): ");
        scanf("%s", input);

        // Check if the user wants to return to the main menu
        if (strcmp(input, "q") == 0 || strcmp(input, "0") == 0) {
            return; // Return to the main menu
        }

        // Find the reservation number in the array
        bool found = false;
        for (int i = 0; i < MAX_CARS; ++i) {
            if (reserved[i] && strcmp(reservationNumbers[i], input) == 0) {
                found = true;
                reservationIndex = i;
                break;
            }
        }

        if (!found) {
            printf("Invalid reservation number or reservation not found. Please try again.\n");
            //system("pause");
            continue; // Prompt again
        }

        // Display current reservation details
        printf("\nCurrent reservation details:\n");
        printf("Car: %s\n", cars[reservationIndex]);
        printf("Reservation Number: %s\n", reservationNumbers[reservationIndex]);
        printf("Reservation Period (Days): %d\n", period_of_car[reservationIndex]);
        printf("Total Price (RM): %.2f\n", price_of_car[reservationIndex]);

        while(1){
            // Prompt for new reservation period
            printf("\nEnter new reservation period (days): ");
            char new_period_str[10];
            scanf("%s", new_period_str);

            // Check if the user wants to return to the main menu
            if (strcmp(new_period_str, "q") == 0 || strcmp(new_period_str, "0") == 0) {
                return; // Return to the main menu
            }

            // Validate and convert the new period
            char *endptr;
            int new_period = strtol(new_period_str, &endptr, 10);
            if (endptr == new_period_str || *endptr != '\0' || new_period <= 0) {
                printf("Invalid input. Please enter a positive number of days.\n");
                system("pause");
                continue; // Prompt again
            }

            // Update reservation details
            period_of_car[reservationIndex] = new_period;
            price_of_car[reservationIndex] = new_period * price[reservationIndex];

            printf("\nReservation Updated Successfully...\n");
            printf("New Reservation Period: %d days\n", period_of_car[reservationIndex]);
            printf("New Total Price: RM %.2f\n", price_of_car[reservationIndex]);
            break;

        }

        system("pause");
        return; // Return to the main menu after editing
    }
}

// Delete reservation
void deleteReservation() {
    system("cls");
    printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
    printf("|          Name                |   Year     | Price Per Day(RM)| Reservation     | Reservation No. |\n");
    printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");

    bool carsReserved = false;
    for (int i = 0; i < MAX_CARS; ++i) {
        if (reserved[i]) {
            carsReserved = true;
            printf("| %-27s  | %-10d | %-16.2f | Reserved        | %-15s |\n", cars[i], years[i], price[i], reservationNumbers[i]);
            printf("| Reservation Period(Day): %-3d | Price Per Day(RM): %-6.2f     | Total Price(RM): %-6.2f           |\n", period_of_car[i], price[i], price_of_car[i]);
            printf("| Reservation Time: %-42s |                                   |\n", reserveTime[i]);
            printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
        }
    }

    if (!carsReserved) {
        printf("No cars reserved.\n");
        system("pause");
        return;
    }

    int reservationIndex;
    printf("(Enter 'q' or '0' to return to the main menu)\n");
    printf("Enter the reservation number to delete (e.g., R001): ");

    char input[10];
    scanf("%s", input);

    // Check if the user wants to return to the main menu
    if (strcmp(input, "q") == 0 || strcmp(input, "0") == 0) {
        return; // Return to the main menu
    }

    bool found = false;
    for (int i = 0; i < MAX_CARS; ++i) {
        if (reserved[i] && strcmp(reservationNumbers[i], input) == 0) {
            found = true;
            reservationIndex = i;
            break;
        }
    }

    if (!found) {
        printf("Invalid reservation number or reservation not found.\n");
        system("pause");
        return;
    }

    reserved[reservationIndex] = false;
    availableCars++;
    printf("Reservation deleted successfully.\n");
    system("pause");
}

// Display reservations
void displayReservation() {
    system("cls");
    printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
    printf("|          Name                |   Year     | Price Per Day(RM)| Reservation     | Reservation No. |\n");
    printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");

    // Collect all reserved car indices
    int reservationIndices[MAX_CARS];
    int count = 0;
    for (int i = 0; i < MAX_CARS; ++i) {
        if (reserved[i]) {
            reservationIndices[count++] = i;
        }
    }

    // Bubble Sort to sort reservation numbers in ascending order
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (strcmp(reservationNumbers[reservationIndices[j]], reservationNumbers[reservationIndices[j + 1]]) > 0) {
                int temp = reservationIndices[j];
                reservationIndices[j] = reservationIndices[j + 1];
                reservationIndices[j + 1] = temp;
            }
        }
    }

    // Display sorted reservations
    for (int i = 0; i < count; ++i) {
        int index = reservationIndices[i];

        printf("| %-27s  | %-10d | %-16.2f | Reserved        | %-15s |\n", cars[index], years[index], price[index], reservationNumbers[index]);

        printf("| Reservation Period(Day): %-3d | Price Per Day(RM): %-6.2f     | Total Price(RM): %-6.2f           |\n", period_of_car[index], price[index], price_of_car[index]);
        printf("| Reservation Time: %-42s |                                   |\n", reserveTime[index]);

        printf("+------------------------------+------------+------------------+-----------------+----------------+\n");
    }

    system("pause");
}

// Search reservation
void searchReservation() {
    system("cls");
    printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
    printf("|          Name                |   Year     | Price Per Day(RM)| Reservation     | Reservation No. |\n");
    printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");

    char input[10];
    printf("Enter the reservation number to search (e.g., R001): ");
    scanf("%s", input);
    system("cls");

    printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
    printf("|          Name                |   Year     | Price Per Day(RM)| Reservation     | Reservation No. |\n");
    printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");

    bool found = false;
    for (int i = 0; i < MAX_CARS; ++i) {
        if (reserved[i] && strcmp(reservationNumbers[i], input) == 0) {
            found = true;
            printf("| %-27s  | %-10d | %-16.2f | Reserved        | %-15s |\n", cars[i], years[i], price[i], reservationNumbers[i]);
            printf("| Reservation Period(Day): %-3d | Price Per Day(RM): %-6.2f     | Total Price(RM): %-6.2f           |\n", period_of_car[i], price[i], price_of_car[i]);
            printf("| Reservation Time: %-42s |                                   |\n", reserveTime[i]);

            printf("+------------------------------+------------+------------------+-----------------+-----------------+\n");
            break;
        }
    }

    if (!found) {
        printf("Reservation not found.\n");
    }

    system("pause");
}

// Main function
int main() {
    Welcome();

    char feedback;
    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
    cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl << endl;
    cout << "                                Do you want to Sign Up (S/s) (For new user) or Login (L/l)" << endl;
    cout << "                                ----------------------------------------------------------" << endl;
    cout << "                                                 Enter your selection : ";
    cin >> feedback;

    while (feedback != 'S' && feedback != 's' && feedback != 'L' && feedback != 'l') {
        system("cls");
        cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
        cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl << endl;
        cout << "                                               [Error, please try again !]" << endl << endl;
        cout << "                                 Do you want to Sign Up (S/s) (For new user) or Login (L/l)" << endl;
        cout << "                                 ----------------------------------------------------------" << endl;
        cout << "                                                 Enter your selection : ";
        cin >> feedback;
    }

    system("cls");

    if (feedback == 'S' || feedback == 's') {
        SignUp();
    } else if (feedback == 'L' || feedback == 'l') {
        Login();
    }

    while (true) {
        int check = 1;
        Menu();
        switch (choice) {
            case 1:
                system("cls");
                display_availableCars(true);
                break;
            case 2:
                system("cls");
                add_carReservation(&check);
                if (check == 0) {
                } else {
                    reserveCal();
                }
                break;
            case 3:
                system("cls");
                returnCar();
                break;
            case 4:
                system("cls");
                editReservation();
                break;
            case 5:
                system("cls");
                deleteReservation();
                break;
            case 6:
                system("cls");
                displayReservation();
                break;
            case 7:
                system("cls");
                searchReservation();
                break;
            case 8:
                system("cls");
                displayRecentReservations();
                break;
            case 9:
                system("cls");
                displayWaitingList();
                break;
            case 10:
                Thankyou();
                return 0;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    }
}
