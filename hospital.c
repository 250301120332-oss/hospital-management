/* hospital_system.c
   Simple Hospital Management System (console, file-backed)
   Compile: gcc -o hospital_system hospital_system.c
   Run: ./hospital_system
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATAFILE "hospital.dat"

typedef struct {
    int id;
    char name[100];
    int age;
    char gender[10];
    char address[150];
    char phone[20];
    char disease[100];
    char doctor[100];
    int daysAdmitted;
    float bill;
} Patient;

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void pause_and_clear() {
    printf("\nPress Enter to continue...");
    getchar();
    // Optionally clear screen (works on many terminals)
    // system("clear"); // unix
    // system("cls");   // windows
}

int get_next_id() {
    FILE *fp = fopen(DATAFILE, "rb");
    if (!fp) return 1;
    Patient p;
    int maxId = 0;
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.id > maxId) maxId = p.id;
    }
    fclose(fp);
    return maxId + 1;
}

void input_string(const char *prompt, char *buf, int size) {
    printf("%s: ", prompt);
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    // remove newline
    size_t ln = strlen(buf);
    if (ln > 0 && buf[ln-1] == '\n') buf[ln-1] = '\0';
}

void addPatient() {
    Patient p;
    p.id = get_next_id();
    printf("\n--- Add New Patient ---\n");
    printf("Assigned Patient ID: %d\n", p.id);
    input_string("Full name", p.name, sizeof(p.name));
    printf("Age: ");
    if (scanf("%d", &p.age) != 1) { clear_stdin(); p.age = 0; }
    clear_stdin();
    input_string("Gender", p.gender, sizeof(p.gender));
    input_string("Address", p.address, sizeof(p.address));
    input_string("Phone", p.phone, sizeof(p.phone));
    input_string("Disease/Complaint", p.disease, sizeof(p.disease));
    input_string("Doctor assigned", p.doctor, sizeof(p.doctor));
    printf("Days admitted (integer): ");
    if (scanf("%d", &p.daysAdmitted) != 1) { clear_stdin(); p.daysAdmitted = 0; }
    clear_stdin();
    // Simple billing rule: base + day rate (customize as needed)
    float baseFee = 500.0f;
    float dayRate = 1500.0f;
    p.bill = baseFee + (p.daysAdmitted * dayRate);

    FILE *fp = fopen(DATAFILE, "ab");
    if (!fp) {
        perror("Unable to open data file for writing");
        return;
    }
    fwrite(&p, sizeof(Patient), 1, fp);
    fclose(fp);
    printf("\nPatient added successfully (ID: %d). Calculated bill: %.2f\n", p.id, p.bill);
    pause_and_clear();
}

void viewAllPatients() {
    FILE *fp = fopen(DATAFILE, "rb");
    if (!fp) {
        printf("\nNo records found.\n");
        pause_and_clear();
        return;
    }
    Patient p;
    printf("\n--- All Patients ---\n");
    printf("%-5s %-20s %-4s %-6s %-10s %-8s %-10s %-12s %-4s %-8s\n",
           "ID", "Name", "Age", "Gender", "Phone", "Days", "Disease", "Doctor", "Days", "Bill");
    rewind(fp);
    int count = 0;
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        printf("%-5d %-20s %-4d %-6s %-10s %-8d %-10s %-12s %-4d %-8.2f\n",
               p.id, p.name, p.age, p.gender, p.phone, p.daysAdmitted, p.disease, p.doctor, p.daysAdmitted, p.bill);
        count++;
    }
    if (count == 0) printf("No patient records.\n");
    fclose(fp);
    pause_and_clear();
}

void searchPatient() {
    printf("\n--- Search Patient By ID ---\n");
    int id;
    printf("Enter patient ID: ");
    if (scanf("%d", &id) != 1) { clear_stdin(); printf("Invalid ID.\n"); pause_and_clear(); return; }
    clear_stdin();

    FILE *fp = fopen(DATAFILE, "rb");
    if (!fp) { printf("No records found.\n"); pause_and_clear(); return; }
    Patient p;
    int found = 0;
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.id == id) {
            found = 1;
            printf("\nID: %d\nName: %s\nAge: %d\nGender: %s\nAddress: %s\nPhone: %s\nDisease: %s\nDoctor: %s\nDays Admitted: %d\nBill: %.2f\n",
                   p.id, p.name, p.age, p.gender, p.address, p.phone, p.disease, p.doctor, p.daysAdmitted, p.bill);
            break;
        }
    }
    if (!found) printf("Patient with ID %d not found.\n", id);
    fclose(fp);
    pause_and_clear();
}

void updatePatient() {
    printf("\n--- Update Patient Record ---\n");
    int id;
    printf("Enter patient ID to update: ");
    if (scanf("%d", &id) != 1) { clear_stdin(); printf("Invalid ID.\n"); pause_and_clear(); return; }
    clear_stdin();

    FILE *fp = fopen(DATAFILE, "rb+");
    if (!fp) { printf("No records found.\n"); pause_and_clear(); return; }

    Patient p;
    int found = 0;
    long pos;
    while ((pos = ftell(fp)) , fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.id == id) {
            found = 1;
            printf("\nCurrent data:\nName: %s\nAge: %d\nGender: %s\nAddress: %s\nPhone: %s\nDisease: %s\nDoctor: %s\nDays Admitted: %d\nBill: %.2f\n",
                   p.name, p.age, p.gender, p.address, p.phone, p.disease, p.doctor, p.daysAdmitted, p.bill);

            printf("\nEnter new values (leave blank to keep current):\n");
            char buf[200];

            printf("Full name [%s]: ", p.name);
            if (fgets(buf, sizeof(buf), stdin)) {
                if (buf[0] != '\n') {
                    buf[strcspn(buf, "\n")] = 0;
                    strncpy(p.name, buf, sizeof(p.name));
                }
            }

            printf("Age [%d]: ", p.age);
            if (fgets(buf, sizeof(buf), stdin)) {
                if (buf[0] != '\n') p.age = atoi(buf);
            }

            printf("Gender [%s]: ", p.gender);
            if (fgets(buf, sizeof(buf), stdin)) {
                if (buf[0] != '\n') {
                    buf[strcspn(buf, "\n")] = 0;
                    strncpy(p.gender, buf, sizeof(p.gender));
                }
            }

            printf("Address [%s]: ", p.address);
            if (fgets(buf, sizeof(buf), stdin)) {
                if (buf[0] != '\n') {
                    buf[strcspn(buf, "\n")] = 0;
                    strncpy(p.address, buf, sizeof(p.address));
                }
            }

            printf("Phone [%s]: ", p.phone);
            if (fgets(buf, sizeof(buf), stdin)) {
                if (buf[0] != '\n') {
                    buf[strcspn(buf, "\n")] = 0;
                    strncpy(p.phone, buf, sizeof(p.phone));
                }
            }

            printf("Disease [%s]: ", p.disease);
            if (fgets(buf, sizeof(buf), stdin)) {
                if (buf[0] != '\n') {
                    buf[strcspn(buf, "\n")] = 0;
                    strncpy(p.disease, buf, sizeof(p.disease));
                }
            }

            printf("Doctor [%s]: ", p.doctor);
            if (fgets(buf, sizeof(buf), stdin)) {
                if (buf[0] != '\n') {
                    buf[strcspn(buf, "\n")] = 0;
                    strncpy(p.doctor, buf, sizeof(p.doctor));
                }
            }

            printf("Days Admitted [%d]: ", p.daysAdmitted);
            if (fgets(buf, sizeof(buf), stdin)) {
                if (buf[0] != '\n') p.daysAdmitted = atoi(buf);
            }

            // Recompute bill (using same simple rule)
            float baseFee = 500.0f;
            float dayRate = 1500.0f;
            p.bill = baseFee + (p.daysAdmitted * dayRate);

            // Move file pointer back to overwrite record
            fseek(fp, pos, SEEK_SET);
            fwrite(&p, sizeof(Patient), 1, fp);
            fflush(fp);
            printf("\nRecord updated. New bill: %.2f\n", p.bill);
            break;
        }
    }
    if (!found) printf("Patient with ID %d not found.\n", id);
    fclose(fp);
    pause_and_clear();
}

void deletePatient() {
    printf("\n--- Delete Patient Record ---\n");
    int id;
    printf("Enter patient ID to delete: ");
    if (scanf("%d", &id) != 1) { clear_stdin(); printf("Invalid ID.\n"); pause_and_clear(); return; }
    clear_stdin();

    FILE *fp = fopen(DATAFILE, "rb");
    if (!fp) { printf("No records found.\n"); pause_and_clear(); return; }
    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) { perror("Unable to open temporary file"); fclose(fp); return; }

    Patient p;
    int found = 0;
    while (fread(&p, sizeof(Patient), 1, fp) == 1) {
        if (p.id == id) {
            found = 1;
            continue; // skip writing this record
        }
        fwrite(&p, sizeof(Patient), 1, temp);
    }
    fclose(fp);
    fclose(temp);

    if (found) {
        remove(DATAFILE);
        rename("temp.dat", DATAFILE);
        printf("Patient with ID %d deleted successfully.\n", id);
    } else {
        remove("temp.dat");
        printf("Patient with ID %d not found.\n", id);
    }
    pause_and_clear();
}

void printMenu() {
    printf("\n===== HOSPITAL MANAGEMENT SYSTEM =====\n");
    printf("1. Add new patient\n");
    printf("2. View all patients\n");
    printf("3. Search patient by ID\n");
    printf("4. Update patient record\n");
    printf("5. Delete patient record\n");
    printf("6. Exit\n");
    printf("Enter choice: ");
}

int main() {
    int choice;
    while (1) {
        printMenu();
        if (scanf("%d", &choice) != 1) { clear_stdin(); choice = 0; }
        clear_stdin();
        switch (choice) {
            case 1: addPatient(); break;
            case 2: viewAllPatients(); break;
            case 3: searchPatient(); break;
            case 4: updatePatient(); break;
            case 5: deletePatient(); break;
            case 6:
                printf("Exiting. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                pause_and_clear();
        }
    }
    return 0;
}
