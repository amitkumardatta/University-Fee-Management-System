#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- STRUCTURE ---------- */
struct Student
{
    char id[20];
    char name[50];
    char dept[20];
    float total_fee;
    float paid;
};

/* ---------- GLOBAL ---------- */
struct Student students[100];
int count = 0;
int isAdmin = 0;

/* ---------- GET FEE BY DEPARTMENT ---------- */
float getFee(char dept[])
{
    if (strcmp(dept, "CSE") == 0) return 75000;
    else if (strcmp(dept, "EEE") == 0) return 80000;
    else if (strcmp(dept, "Civil") == 0) return 70000;
    else if (strcmp(dept, "BBA") == 0) return 65000;
    else if (strcmp(dept, "Architecture") == 0) return 90000;
    else return -1;
}

/* ---------- SAVE ---------- */
void saveFile()
{
    FILE *fp = fopen("students.txt", "w");
    if (!fp) return;

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%s %s %s %.2f %.2f\n",
                students[i].id,
                students[i].name,
                students[i].dept,
                students[i].total_fee,
                students[i].paid);
    }
    fclose(fp);
}

/* ---------- LOAD ---------- */
void loadFile()
{
    FILE *fp = fopen("students.txt", "r");
    if (!fp) return;

    while (count < 100 &&
            fscanf(fp, "%s %s %s %f %f",
                   students[count].id,
                   students[count].name,
                   students[count].dept,
                   &students[count].total_fee,
                   &students[count].paid) != EOF)
    {
        count++;
    }
    fclose(fp);
}

/* ---------- FIND ---------- */
struct Student* findStudent(char id[])
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(students[i].id, id) == 0)
        {
            return &students[i];
        }
    }
    return NULL;
}

// Clears the terminal screen

void clear_screen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Waits for the user to press ENTER before continuing */

void pause_screen(void)
{
    printf("\n  Press ENTER to continue...");
    while (getchar() != '\n');
    getchar();
}

/* ---------- ADD STUDENT ---------- */
void addStudent()
{
    if (count >= 100)
    {
        printf("Limit reached!\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%s", students[count].id);
    getchar();

    printf("Enter Name: ");
    gets(students[count].name);

    printf("Enter Department (CSE/EEE/Civil/BBA/Architecture): ");
    scanf("%s", students[count].dept);

    float fee = getFee(students[count].dept);

    if (fee == -1)
    {
        printf("Invalid department!\n");
        pause_screen();
        return;
    }

    students[count].total_fee = fee;
    students[count].paid = 0;

    count++;
    saveFile();

    printf("Student added! Tuition fee for %s is %.2f\n",
           students[count - 1].dept,
           students[count - 1].total_fee);
    pause_screen();
}

/* ---------- PAYMENT ---------- */
void payFee()
{
    char id[20];
    printf("Enter ID: ");
    scanf("%s", id);

    struct Student *s = findStudent(id);

    if (!s)
    {
        printf("Not found!\n");
        pause_screen();
        return;
    }

    float amount;
    printf("Enter amount: ");
    scanf("%f", &amount);

    s->paid += amount;

    saveFile();
    printf("Payment successful!\n");
    pause_screen();
}

/* ---------- VIEW ---------- */
void viewStudent()
{
    char id[20];
    printf("Enter ID: ");
    scanf("%s", id);

    struct Student *s = findStudent(id);

    if (!s)
    {
        printf("Not found!\n");
        pause_screen();
        return;
    }

    printf("\nID: %s\n", s->id);
    printf("Name: %s\n", s->name);
    printf("Department: %s\n", s->dept);
    printf("Total Fee: %.2f\n", s->total_fee);
    printf("Paid: %.2f\n", s->paid);
    printf("Due: %.2f\n", s->total_fee - s->paid);
    pause_screen();
}

/* ---------- ADMIN LOGIN ---------- */
void adminLogin()
{
    char pass[50];
    printf("Enter Admin Password: ");
    scanf("%s", pass);

    if (strcmp(pass, "NSU@123") == 0)
    {
        isAdmin = 1;
        printf("Admin login successful!\n");
    }
    else
    {
        printf("Wrong password!\n");
    }
    pause_screen();
}

/* ---------- DELETE ---------- */
void deleteStudent()
{
    if (!isAdmin)
    {
        printf("Admin only!\n");
        pause_screen();
        return;
    }

    char id[20];
    printf("Enter ID: ");
    scanf("%s", id);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(students[i].id, id) == 0)
        {

            for (int j = i; j < count - 1; j++)
            {
                students[j] = students[j + 1];
            }

            count--;
            saveFile();

            printf("Deleted successfully!\n");
            pause_screen();
            return;
        }
    }

    printf("Not found!\n");
    pause_screen();
}

/* ---------- SCHOLARSHIP ---------- */
void giveScholarship()
{
    if (!isAdmin)
    {
        printf("Admin only!\n");
        pause_screen();
        return;

    }

    char id[20];
    printf("Enter ID: ");
    scanf("%s", id);

    struct Student *s = findStudent(id);

    if (!s)
    {
        printf("Not found!\n");
        pause_screen();
        return;
    }

    float amount;
    printf("Enter scholarship amount: ");
    scanf("%f", &amount);

    if (amount > s->total_fee)
    {
        printf("Invalid amount!\n");
        pause_screen();
        return;
    }

    s->total_fee -= amount;

    saveFile();
    printf("Scholarship applied!\n");
    pause_screen();
}

/* ---------- PDF GENERATION ---------- */
void generatePDF()
{
    char id[20];
    printf("Enter ID: ");
    scanf("%s", id);

    struct Student *s = findStudent(id);

    if (!s)
    {
        printf("Not found!\n");
        pause_screen();
        return;
    }

    /* Create receipts folder if it does not exist */
#ifdef _WIN32
    system("if not exist receipts mkdir receipts");
#else
    system("mkdir -p receipts");
#endif

    /* Build per-student file paths so old receipts are never overwritten */
    char htmlPath[60];
    char pdfPath[60];
    char cmd[300];

    snprintf(htmlPath, sizeof(htmlPath), "receipts/receipt_%s.html", s->id);
    snprintf(pdfPath,  sizeof(pdfPath),  "receipts/receipt_%s.pdf",  s->id);

    float due = s->total_fee - s->paid;

    FILE *fp = fopen(htmlPath, "w");
    if (!fp)
    {
        printf("Could not create receipt file!\n");
        pause_screen();
        return;
    }

    fprintf(fp,
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head><meta charset='UTF-8'>\n"
            "<style>\n"
            "@page { size: A4; margin: 20mm 15mm; }\n"
            "body { font-family: Arial, sans-serif; font-size: 13px; color: #222; }\n"
            ".header { text-align: center; border-bottom: 3px solid #003366; padding-bottom: 12px; margin-bottom: 24px; }\n"
            ".header h1 { font-size: 50px; color: #003366; margin: 0; }\n"
            ".header p  { font-size: 11px; color: #555; margin: 3px 0 0; }\n"
            ".title { text-align: center; font-size: 15px; font-weight: bold; letter-spacing: 1px; margin-bottom: 20px; text-transform: uppercase; }\n"
            ".info-table { width: 100%%; border-collapse: collapse; margin-bottom: 24px; }\n"
            ".info-table td { padding: 6px 10px; font-size: 13px; }\n"
            ".info-table td:first-child { color: #555; width: 160px; }\n"
            ".info-table td:last-child  { font-weight: bold; }\n"
            ".fee-table { width: 100%%; border-collapse: collapse; margin-bottom: 24px; }\n"
            ".fee-table th { background: #003366; color: #fff; padding: 9px 12px; text-align: left; font-size: 12px; }\n"
            ".fee-table th.r { text-align: right; }\n"
            ".fee-table td { padding: 9px 12px; border-bottom: 1px solid #ddd; }\n"
            ".fee-table td.r { text-align: right; }\n"
            ".fee-table tr.due td { font-weight: bold; background: #f5f5f5; border-top: 2px solid #003366; }\n"
            ".sig { display: flex; justify-content: space-between; margin-top: 50px; }\n"
            ".sig-box { text-align: center; width: 160px; }\n"
            ".sig-box .line { border-top: 1px solid #333; margin-bottom: 5px; }\n"
            ".sig-box .label { font-size: 11px; color: #555; }\n"
            ".footer { text-align: center; font-size: 10px; color: #888; border-top: 1px solid #ddd; margin-top: 40px; padding-top: 10px; }\n"
            "</style>\n"
            "</head><body>\n"

            "<div class='header'>\n"
            "  <h1>North South University</h1>\n"
            "  <p>Plot 15, Block B, Bashundhara, Dhaka-1229 | https://www.northsouth.edu</p>\n"
            "</div>\n"

            "<div class='title'>Fee Payment Receipt</div>\n"

            "<table class='info-table'>\n"
            "  <tr><td>Student ID</td><td>: %s</td></tr>\n"
            "  <tr><td>Name</td><td>: %s</td></tr>\n"
            "  <tr><td>Department</td><td>: %s</td></tr>\n"
            "</table>\n"

            "<table class='fee-table'>\n"
            "  <tr><th>Description</th><th class='r'>Amount (BDT)</th></tr>\n"
            "  <tr><td>Total Tuition Fee</td><td class='r'>%.2f</td></tr>\n"
            "  <tr><td>Amount Paid</td><td class='r'>%.2f</td></tr>\n"
            "  <tr class='due'><td>Balance Due</td><td class='r'>%.2f</td></tr>\n"
            "</table>\n"

            "<div class='footer'>This is a computer-generated receipt. No signature required.</div>\n"

            "</body></html>\n",

            s->id, s->name, s->dept,
            s->total_fee, s->paid, due
           );

    fclose(fp);

    /* The outer quotes wrap the whole command — required when the exe path has spaces */
    snprintf(cmd, sizeof(cmd),
             "\"\"C:\\Program Files\\wkhtmltopdf\\bin\\wkhtmltopdf.exe\""
             " --page-size A4 \"%s\" \"%s\"\"",
             htmlPath, pdfPath);
    system(cmd);

    printf("Receipt saved: %s\n", pdfPath);
    pause_screen();
}

/* ---------- MAIN ---------- */
int main()
{
    char choice;

    loadFile();

    while (1)
    {
        clear_screen();
        printf("\n--- MENU ---\n");
        printf("1. Add Student\n");
        printf("2. Pay Fee\n");
        printf("3. View Student\n");
        printf("4. Generate PDF\n");
        printf("5. Scholarship (ADMIN)\n");
        printf("6. Delete Student (ADMIN)\n");
        printf("A. Admin Login\n");
        printf("0. Exit\n");

        printf("Choice: ");
        scanf(" %c", &choice);

        if (choice == '1') addStudent();
        else if (choice == '2') payFee();
        else if (choice == '3') viewStudent();
        else if (choice == '4') generatePDF();
        else if (choice == '5') giveScholarship();
        else if (choice == '6') deleteStudent();
        else if (choice == 'A' || choice == 'a') adminLogin();
        else if (choice == '0') break;
        else
        {
            printf("Invalid choice!\n");
            pause_screen();
        }
    }

    return 0;
}
