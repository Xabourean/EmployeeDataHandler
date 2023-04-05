#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <fstream>
#include <regex>
#include <vector>
#include <sstream>
#include <map>

// Logan Sicking

/*
This program will allow the users to;
Read a file line by line to fill in employee data
Manually input in employee data
write employee data to a file

This is all done within a data structure named emp
*/

// Declare all the commands you are using
using std::vector;
using std::cout;
using std::cin;
using std::getline;
using std::endl;
using std::setw;
using std::string;
using std::unordered_map;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::regex;
using std::regex_match;
using std::regex_search;
using std::stringstream;
using std::stringbuf;
using std::smatch;
using namespace std;

// Create global variables for table formatting
const char separator = ' ';
const int numwidth = 10;
const int namewidth = 20;
const int dobwidth = 12;
const int telwidth = 13;
const int postwidth = 10;

string birth;
vector<string> seglist;
int done = 0;
smatch fix;
int flag = 0;

// Create a data structure for storing multiple values of the same variables
struct employee
{
    // num must be an int for organizing employees by number
    int num = 0;
    string name;
    string dob;
    string tel;
    string postcode;
    int checksum = 0;
};

string capitalise_words(string birth)
{
    // Iterate over each character in the string
    for (int i = 0; i < birth.length(); ++i)
    {
        // Convert the current character to uppercase
        birth[i] = toupper(birth[i]);
    }
    // Return the modified string
    return birth;
}

string lowercase_words(string birth)
{
    // Iterate over each character in the string
    for (int i = 0; i < birth.length(); ++i)
    {
        // Convert the current character to lowercase
        birth[i] = tolower(birth[i]);
    }
    // Return the modified string
    return birth;
}

vector<string> createvector(string birth)
{
    seglist.clear(); // Clear the contents of the vector before populating it
    stringstream conversion;
    conversion << birth; // Convert the birth string into a stringstream object
    string segment;

    while (getline(conversion, segment, ' ')) // Iterate over the stringstream object and split the words separated by a space
    {
        seglist.push_back(segment); // Add the split words to the vector
    }
    for (string& i : seglist) // Loop through the vector and print each element
    {
        cout << i << "  ";
    }
    cout << birth << endl; // Print the original birth string
    return seglist; // Return the populated vector
}

// Converts roman numerals to numbers
int romanToInteger(string s)
{
    // Define a map for roman numeral symbols and their integer values
    unordered_map<char, int> romanToIntMap = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000}
    };

    // Check for invalid inputs
    for (char c : s) {
        if (romanToIntMap.find(c) == romanToIntMap.end()) {
            // Invalid character found
            return 0;
        }
    }

    // Start the process of conversion
    int result = 0;
    for (int i = 0; i < s.length(); i++)
    {
        // Get the value of the current roman numeral symbol
        int currentValue = romanToIntMap[s[i]];

        // Check if the next symbol has a higher value
        if (i + 1 < s.length() && romanToIntMap[s[i + 1]] > currentValue)
        {
            // Subtract the current value from the next value
            result -= currentValue;
        }
        else
        {
            // Add the current value to the result
            result += currentValue;
        }
    }

    return result;
}

int wordvalue(std::string r)
{
    // Create a map of word to integer mappings
    static std::map<std::string, int> wordToIntMap = {
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9},
        {"ten", 10},
        {"eleven", 11},
        {"twelve", 12},
        {"thirteen", 13},
        {"fourteen", 14},
        {"fifteen", 15},
        {"sixteen", 16},
        {"seventeen", 17},
        {"eighteen", 18},
        {"nineteen", 19},
        {"twenty", 20},
        {"thirty", 30},
        {"forty", 40},
        {"fifty", 50},
        {"sixty", 60},
        {"seventy", 70},
        {"eighty", 80},
        {"ninety", 90},
        {"hundred", 100},
        {"thousand", 1000}
    };

    // Check if the given word is "negative" and return -1 if it is
    if (r == "negative")
    {
        return -1;
    }

    // Look up the integer value for the given word in the map
    auto it = wordToIntMap.find(r);
    if (it != wordToIntMap.end())
    {
        // Return the integer value if the word is found in the map
        return it->second;
    }
    else
    {
        // Return 0 if the word is not found in the map
        return 0;
    }
}

int finalword(vector<string> seglist)
{
    // Starting value
    int done = 0;
    // Start the process of conversion
    for (int i = 0; i < size(seglist); i++)
    {
        // Grab the individual words in the vector
        int currentsymbol = wordvalue(seglist[i]);

        // Check if there is a character following the current one
        if (i + 1 < size(seglist))
        {
            // Grab the value of the character following the previous one
            int nextsymbol = wordvalue(seglist[i + 1]);

            // If the character is 'C' or 'M', it means it multiplies the previous character
            if (nextsymbol == 100 || nextsymbol == 1000)
            {
                // Update the final value by adding the product of the two characters
                done = done + nextsymbol * currentsymbol;
                // Skip the character following the previous one
                i++;
            }
        }
        else
        {
            // If there are no more characters after the current one, simply add its value to the final value
            done = done + currentsymbol;
        }
    }
    return done;
}

// This function takes a string argument `birth` and returns an integer value after 
// processing the input according to some defined patterns.
int birthinfo(string birth)
{
    // Initializing variables
    int birth2 = 0;
    bool valid_input = false;

    regex regbirth("([0-9]*)");
    regex regbirthword("([a,A,e,E,o,O,u,U,s,S]*)");
    regex regbirthnumerals("([i,I,v,V,x,X,l,L,c,C,d,D,m,M]*)");

    // This loop continues until a valid input is entered by the user
    while (!valid_input)
    {
        // If the input string matches the pattern for a valid year format (only digits), 
        // then the year value is extracted and the input is considered valid.
        if (regex_match(birth, regbirth))
        {
            birth2 = stoi(birth); // convert string to integer
            valid_input = true; // valid input entered, stop the loop
        }
        // If the input string matches the pattern for a valid roman numeral format, 
        // then the roman numeral is converted to its equivalent decimal value, and the input is considered valid.
        else if (regex_match(birth, regbirthnumerals))
        {
            capitalise_words(birth); // capitalize the input string
            birth2 = romanToInteger(birth); // convert the roman numeral to its decimal value
            valid_input = true; // valid input entered, stop the loop
        }
        // If the input string matches the pattern for a valid word format, 
        // then the words in the input are converted to lowercase, 
        // stored in a vector, and then processed to calculate the final value of the year. 
        else if (regex_search(birth, regbirthword))
        {
            lowercase_words(birth); // convert the input string to lowercase
            createvector(birth); // create a vector of individual words from the input string
            birth2 = finalword(seglist); // calculate the final value of the year from the individual words
            valid_input = true; // valid input entered, stop the loop
        }
        // If the input is not valid (does not match any of the expected patterns), 
        // the user is prompted to enter the input again.
        else
        {
            cout << "Invalid input, please try again." << endl;
            cin >> birth; // take input again
        }
    }

    // Return the processed integer value representing the birth year
    return birth2;
}

// Create a function for printing each individual employee
void print_employee(const employee& emp)
{
    // Align text to the left and use previously made global variables for formatting
    cout << left << setw(numwidth) << setfill(separator) << emp.num;
    cout << left << setw(namewidth) << setfill(separator) << emp.name;
    cout << left << setw(dobwidth) << setfill(separator) << emp.dob;
    cout << left << setw(telwidth) << setfill(separator) << emp.tel;
    cout << left << setw(postwidth) << setfill(separator) << emp.postcode;
    cout << endl;
}

// Create a function for printing the table of employees
void print_employee_table(const unordered_map<int, employee>& employees)
{
    // Align text to the left and use previously made global variables for formatting
    cout << left << setw(numwidth) << setfill(separator) << "Number";
    cout << left << setw(namewidth) << setfill(separator) << "Name";
    cout << left << setw(dobwidth) << setfill(separator) << "DOB";
    cout << left << setw(telwidth) << setfill(separator) << "Telephone";
    cout << left << setw(postwidth) << setfill(separator) << "Postcode";
    cout << endl;
    // Print employees in a loop until all employees are entered
    for (const auto& emp : employees)
    {
        print_employee(emp.second);
    }
}

// Create a function for manual input of employee data in the employee data set
employee read_employee_user()
{
    // Create a new employee object
    employee emp;

    // Prompt the user to enter an employee number and store it in the employee object
    cout << "Enter employee number: ";
    cin >> emp.num;

    // Define a regular expression pattern to match the employee name input format
    regex regnam("^[A-Za-z\p{L}]([a-z,',-]*)");

    // Prompt the user to enter an employee name and store it in the employee object
    cout << "Enter employee name: ";
    getline(cin >> ws, emp.name);

    // Validate that the employee name matches the expected format using regular expressions
    while (!(regex_match(emp.name, regnam)))
    {
        // If the input doesn't match the format, prompt the user to re-enter the employee name and try again
        cout << "Enter employee name: ";
        getline(cin >> ws, emp.name);
    }

    // Define a string variable to store the final format of the date of birth
    string finalbirthformat = "";

    // Define a regex pattern to check if the entered date of birth matches the required format
    regex regformat("([0-9]{4}([/]{1}([0-9]{1,2}([/]{1}([0-9]{1,2})))))");

    // Define a flag to control the loop
    int loopflag = 0;

    // Define an integer variable to store the birth year
    int birth2 = 0;

    // Loop until a valid date of birth is entered
    while (loopflag != 1)
    {
        int year, month, day;

        // Get year of birth from user and convert to integer using the birthinfo function
        cout << "Enter year of birth (use numbers, roman numerals, or words): ";
        getline(cin >> ws, birth);
        birth2 = birthinfo(birth);
        year = birth2;

        // Add the year to the final format string and add a slash
        finalbirthformat += to_string(birth2);
        finalbirthformat += "/";

        // Get month of birth from user and convert to integer using the birthinfo function
        cout << "Enter month of birth (use numbers, roman numerals, or words): ";
        getline(cin >> ws, birth);
        birth2 = birthinfo(birth);
        month = birth2;

        // Add the month to the final format string and add a slash
        finalbirthformat += to_string(birth2);
        finalbirthformat += "/";

        // Get day of birth from user and convert to integer using the birthinfo function
        cout << "Enter day of birth (use numbers, roman numerals, or words): ";
        getline(cin >> ws, birth);
        birth2 = birthinfo(birth);
        day = birth2;

        // Add the day to the final format string
        finalbirthformat += to_string(birth2);

        // Decrement flag variable to keep track of number of inputs remaining
        flag = flag - 1;

        // Check if the entered date is realistic by validating the month and day based on the year
        bool realistic = true;
        if (month < 1 || month > 12) {
            cout << "Invalid month entered. Please enter a number between 1 and 12." << endl;
            realistic = false;
        }
        else if (day < 1 || day > 31) {
            cout << "Invalid day entered. Please enter a number between 1 and 31." << endl;
            realistic = false;
        }
        else if (month == 4 || month == 6 || month == 9 || month == 11) {
            if (day > 30) {
                cout << "Invalid day entered. Please enter a number between 1 and 30." << endl;
                realistic = false;
            }
        }
        else if (month == 2) {
            if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
                if (day > 29) {
                    cout << "Invalid day entered. February has at most 29 days in a leap year." << endl;
                    realistic = false;
                }
            }
            else {
                if (day > 28) {
                    cout << "Invalid day entered. February has at most 28 days in a non-leap year." << endl;
                    realistic = false;
                }
            }
        }

        // If the entered date is realistic, print the final format
        if (realistic) {
            cout << finalbirthformat << endl;
            loopflag = loopflag + 1;
        }
        else {
            finalbirthformat = "";
        }
    }
    emp.dob = finalbirthformat;

    // Define a regex pattern to match a telephone number that starts with 0 and has 10 digits
    regex regtel("^0([0-9]{10})");

    // Prompt the user to input a telephone number
    cout << "Enter tel: ";

    // Read the input from the user and store it in the employee's 'tel' field
    getline(cin >> ws, emp.tel);

    // Continuously prompt the user to enter a telephone number until the entered number matches the defined regex pattern
    while (!(regex_match(emp.tel, regtel)))
    {
        cout << "Enter tel: ";
        getline(cin >> ws, emp.tel);
    }

    // Define a regular expression for postcodes
    regex regpost("^[a-zA-Z]{1,2}([0-9]{1,2}([ ]{1}([0-9]{1}[a-zA-Z]{1,2})))");

    // Prompt the user to enter a postcode and store it in emp.postcode
    cout << "Enter postcode: ";
    getline(cin >> ws, emp.postcode);

    // Use a loop to continue prompting the user until a valid postcode is entered
    while (!(regex_match(emp.postcode, regpost)))
    {
        cout << "Enter postcode: ";
        getline(cin >> ws, emp.postcode);
    }

    return emp;
}

// This function capitalizes the employee's name using inline assembly code
void capitalise_name(employee& emp)
{
    // Get a pointer to the start of the string and its length
    auto data = emp.name.c_str();
    auto len = emp.name.length();

    __asm
    {
        // Set up the loop counter and pointer to the end of the string
        mov ecx, len
        mov ebx, data

        start_loop :
        // Load the current character into the AL register
        mov al, byte ptr[ebx + ecx - 1]

            // Check if it is lowercase 'a' or lower
            cmp al, 'a'
            jl end_loop

            // Check if it is uppercase 'z' or higher
            cmp al, 'z'
            jg end_loop

            // Subtract 32 from the ASCII value to convert the character to uppercase
            sub al, 32

            // Store the capitalized character back into the string
            mov byte ptr[ebx + ecx - 1], al

            end_loop :
        // Decrement the loop counter and check if we're done
        loop start_loop
    }
}

// Check for errors
int generate_checksum(employee& emp)
{
    // Get a pointer to the string containing the employee name and its length
    auto data = emp.name.c_str();
    auto len = emp.name.length();
    // Initialize a counter to keep track of the number of valid characters
    int count = 0;
    __asm
    {
        // Load the length of the string into the ECX register and the address of the first character into the EBX register
        mov ecx, len
        mov ebx, data
        start_loop :
        // Load the character at the current position into the AL register
        mov al, byte ptr[ebx + ecx - 1]
            // Compare the character to the ASCII value for 'A' (65)
            cmp al, 'A'
            jl end_loop
            // Compare the character to the ASCII value for 'z' (122)
            cmp al, 'z'
            jg end_loop
            // Compare the character to the ASCII value for 'Z' (90)
            cmp al, 'Z'
            jl increment
            // Compare the character to the ASCII value for 'a' (97)
            cmp al, 'a'
            jg increment
            // If the character is not within the valid range, jump to the end of the loop
            jmp end_loop
            increment :
        // If the character is valid, increment the counter
        mov eax, count
            inc eax
            mov count, eax
            end_loop :
        // Decrement the loop counter (ECX) and jump back to the start of the loop if it is not zero
        loop start_loop
    }
    // Return the final count of valid characters
    return count;
}

// This function is for reading a file for employee data
void read_employees_file(unordered_map<int, employee>& employees)
{
    // Create a variable for the file
    string filename;
    cout << "Enter filename: ";
    cin >> filename;
    // Use file to create a stream
    ifstream file(filename);
    // Loop until the file is completely read
    while (file.peek() != EOF)
    {
        // Add file data to employee data structure
        employee emp;
        string buf;
        getline(file, buf);
        emp.num = atoi(buf.c_str());
        // To take an integer value from the file you cannot use getline() as it only takes strings
        file >> emp.num;
        getline(file, emp.name);
        getline(file, emp.dob);
        getline(file, emp.tel);
        getline(file, emp.postcode);
        capitalise_name(emp);
        generate_checksum(emp);
        employees[emp.num] = emp;
    }
    // close the file
    file.close();
}

// This function is for creating a file of the employee data structure
void write_employees_file(const unordered_map<int, employee>& employees)
{
    // Name the file
    string filename;
    cout << "Enter filename: ";
    cin >> filename;
    // Offload the data as a stream
    ofstream file(filename);
    // Loop for the amount of employees
    for (const auto& emp : employees)
    {
        file << emp.second.num << endl;
        file << emp.second.name << endl;
        file << emp.second.dob << endl;
        file << emp.second.tel << endl;
        file << emp.second.postcode << endl;
    }
    // close the file
    file.close();
}

// This function is for changing the navigation variable in the main function
int menu()
{
    // Default navigation variable
    int choice = -1;
    // Create the loop for changing the navigation variable
    while (choice < 0 || choice > 4)
    {
        cout << "Main menu" << endl;
        cout << "1. Add employee" << endl;
        cout << "2. Print employees" << endl;
        cout << "3. Read employees from file" << endl;
        cout << "4. Output employees to file" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        // Create a fail condition if navigation variable is outside expected parameters
        if (choice < 0 || choice > 4)
        {
            cout << "Error. Enter number between 0 and 4" << endl;
        }
    }
    // Return new navigation variable
    return choice;
}

// Main function start
int main(int argc, char** argv)
{
    // Create data structure
    unordered_map < int, employee > employees;
    // Create a navigation variable, default it to a choice outside its normal parameters
    int choice = -1;
    // Create a loop for navigating the code
    while (choice != 0)
    {
        // Go to the menu function
        choice = menu();
        // Test the value of the navigation variable
        switch (choice)
        {
            // Create end condition
        case 0:
        {
            cout << "Goodbye!" << endl;
            break;
        }
        // Create manual input option
        case 1:
        {
            // Run read_employee_user function within the employee data set
            employee emp = read_employee_user();
            // Run capitalise_name function
            capitalise_name(emp);
            // Run generate_checksum function
            generate_checksum(emp);
            // Update data set
            employees[emp.num] = emp;
            cout << "the checksum is :" << generate_checksum(emp) << endl;
            break;
        }
        // Create a print option
        case 2:
        {
            print_employee_table(employees);
            break;
        }
        // Create an automatic entry option
        case 3:
        {
            read_employees_file(employees);
            break;
        }
        // Create a file creation option
        case 4:
        {
            write_employees_file(employees);
            break;
        }
        }
    }
}
