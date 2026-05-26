# University Fee Management (C)

Simple console-based fee management system written in C. It stores student fee records in a text file and can generate fee receipts as HTML/PDF files.

## Features

- Add students with department-based tuition fee
- Record payments and view balance due
- Admin-only actions: scholarship and delete
- Receipt generation (HTML and PDF)

## Project Structure

- `Fee.c` - main program source
- `students.txt` - data file (auto-created/updated)
- `receipts/` - generated receipts

## Build and Run

From the project root:

```sh
clang Fee.c -o Fee
./Fee
```

## Usage

- Use the menu to add students, pay fees, and view records.
- Admin login is required for scholarships and deletion.

Default admin password (from source): `NSU@123`

## Receipt Generation Notes

- The program writes an HTML receipt to `receipts/receipt_<ID>.html`.
- It then tries to convert the HTML to PDF using wkhtmltopdf.
- The current command targets a Windows install path:
  `C:\Program Files\wkhtmltopdf\bin\wkhtmltopdf.exe`

If you are on macOS or Linux, update the command in `generatePDF()` to point to your local wkhtmltopdf binary or remove the PDF step and use the HTML file directly.

## Data Format

Each line in `students.txt` is:

```
<ID> <Name> <Department> <TotalFee> <Paid>
```

Notes:
- `Name` is stored as a single token (no spaces).
- Departments supported: CSE, EEE, Civil, BBA, Architecture.
