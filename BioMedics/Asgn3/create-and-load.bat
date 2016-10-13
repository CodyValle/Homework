@ECHO off
:: 
:: Uses the MySQL monitor to run the SQL scripts that create
:: and populate the tables in the  sample database.

"C:\Program Files\MySQL\MySQL Server 5.7\bin\mysql" -u root -p < "C:\Users\cvalle\Desktop\create-and-load.sql"

ECHO.
ECHO If no error message is shown, the databases named PatientDatabase was created successfully.
ECHO.

:: Display 'press any key to continue' message
PAUSE
