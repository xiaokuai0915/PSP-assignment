#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>

// MODELS
const int teacherCode = 1234;
const int Max_Logs = 100; // Maximum number of logs to store
std::string logActionName[Max_Logs];
int logMatrix2D[Max_Logs][3];
int logCounter = 0;
const int Max_Courses = 50; // Maximum number of courses to store
int allCourseCount = 20; // Total number of courses available

struct Course {
	int id = 0;
	std::string Name = "";
	double price = 0.0;
	double time = 0.0;
};

Course allCourse[Max_Courses] = {
	{ 1, "Mathematics", 100.0 },
	{ 2, "Science", 120.0 },
	{ 3, "English", 80.0 },
	{ 4, "History", 90.0 },
	{ 5, "Geography", 110.0 },
	{ 6, "Art", 70.0 },
	{ 7, "Music", 75.0 },
	{ 8, "Physical Education", 85.0 },
	{ 9, "Computer Science", 130.0 },
	{ 10, "Economics", 95.0 },
	{ 11, "Business Studies", 105.0 },
	{ 12, "Psychology", 115.0 },
	{ 13, "Sociology", 125.0 },
	{ 14, "Philosophy", 135.0 },
	{ 15, "Political Science", 145.0 },
	{ 16, "Environmental Science", 155.0 },
	{ 17, "Chemistry", 165.0 },
	{ 18, "Biology", 175.0 },
	{ 19, "Physics", 185.0 },
	{ 20, "Statistics", 195.0 }
};

struct User {
	std::string username;
	std::string password;
	Course mypackage[10];
	int packageCount = 0;
	int role = 0;
};

void readUserandCoursefile(User allusers[], int& allUsersCount) {
	std::ifstream userfile("user.txt");
	std::ifstream coursefile("user_courses.txt");
	User tempUser;
	allUsersCount = 0;
	//read user.txt file
	while (userfile >> tempUser.username >> tempUser.password >> tempUser.role) {
		if (tempUser.role == 0) {
			tempUser.packageCount = 0;//clear the remain record before starting the next command
			if (allUsersCount < 100) {
				allusers[allUsersCount++] = tempUser;
			}
		}
	}
	userfile.close();

	//read course file, since it has comma at the text file, we will use getline here instead of reading it directly
	std::string line;

	while (std::getline(coursefile, line)) {
		size_t pos1 = line.find(',');
		size_t pos2 = line.find(',', pos1 + 1);
		size_t pos3 = line.find(',', pos2 + 1);

		if (pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos) {
			std::string username = line.substr(0, pos1);
			std::string idStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
			std::string CourseStr = line.substr(pos2 + 1, pos3 - pos2 - 1);
			std::string priceStr = line.substr(pos3 + 1);

			Course c;
			c.id = std::stoi(idStr);
			c.Name = CourseStr;
			c.price = std::stod(priceStr);

			for (int i = 0; i < allUsersCount; ++i) {  //go back to allusers at line 18
				if (allusers[i].username == username) {    //check if this is the current user
					if (allusers[i].packageCount < 10) {
						allusers[i].mypackage[allusers[i].packageCount++] = c;  //store the course into the current user profile
					}
					break;
				}
			}
		}
	}
	coursefile.close();
}

bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

//-------------------------------------------------------------------------------------------------------------------------
// TOOLS

int intgerinputfilter(const std::string& prompt) { // to cout the prompt and get the input from user, then check if it is valid
	std::cin.clear();
	std::string input;
	std::cout << prompt;

	if (!std::getline(std::cin, input)) { // read whole line of input and check if it is valid, no matter it is just a space or empty, it will return -2, if the input is not valid(not integer), it will return -1
		return -1;
	}

	if (input.empty()) { //if input is empty, return -2 to indicate that the input is empty
		return -2;
	}

	size_t start_idx = 0;
	bool is_negative = false;

	if (input[0] == '-') {
		is_negative = true;
		start_idx = 1;
	}
	else if (input[0] == '+') {
		start_idx = 1;
	}
	if (start_idx == input.length()) { //if the input is just a sign, return -1 to indicate that the input is not valid
		return -1;
	}

	for (size_t i = start_idx; i < input.length(); ++i) {
		if (!is_digit(input[i])) {
			return -1; //return -1 to indicate that the input is not valid
		}
	}

	int result = 0;
	for (size_t i = start_idx; i < input.length(); ++i) {
		result = result * 10 + (input[i] - '0');
	}

	if (is_negative) {
		result = -result;
		return result;
	}
	return result;
}

std::string stringinputfilter(const std::string& prompt) {
	std::string input;
	while (true) {
		std::cin.clear();
		std::cout << prompt;
		if (!std::getline(std::cin, input)) {
			return ""; // Return empty string on input failure
		}

		if (input.empty()) {
			std::cout << "Input cannot be empty! Please try again.\n";
			continue;
		}

		if (input.find(' ') != std::string::npos) {
			std::cout << "Input cannot contain spaces! Please try again.\n";
			continue;
		}

		return input;

	}
}

bool exitConfirm() {
	while (true) {
		int choice = intgerinputfilter("Are you sure you want to exit? (1 for Yes, 0 for No): ");
		if (choice == -2) {
			std::cout << "Input cannot be empty! Please enter 1 for Yes or 0 for No.\n";
			continue;
		}
		if (choice == 1) {
			std::cout << "Thank you for using the program.\nSee you next time!\n\n";
			return true; // User confirmed exit
		}
		else if (choice == 0) {
			std::cout << "Program termination cancelled!\n";
			return false; // User canceled exit
		}
		else {
			std::cout << "Invalid input! Please enter 1 for Yes or 0 for No.\n";
			continue;
		}
	}
}

void clearScreen() {
	std::cout << "\n[->] Press enter to continue";
	std::cin.get();
	system("cls");
	std::cout << "Tuition Centre Management\n";
}

//-------------------------------------------------------------------------------------------------------------------------
// HEADINGS

// User Management Module
bool UserManagementModule(bool&, User&);
void login(User&, bool&, bool&);
void registerUser();
void addNewUser();
void updateUser(User&);
void deleteUser(User&);
void searchUser();
void displayUser();

// Student Package Module
void modifyUserPackage(User&, const Course[], int);
void addCoursetoPackage(User&, const Course[], int);
void myCourseSummary(User&, const Course[], int);
void removeCoursefromPackage(User&, const Course[], int);
void showAllCourses(const Course[], int);
void StudentPackageModule(User&, const Course[], int);

// Schedule Module
void ScheduleModule(User& currentUser, Course allCourse[], int& allcourseCount);
void displayMySchedule(User& currentUser, Course allCourse[], int allcourseCount);
void displayAllSchedules(const Course allCourse[], int allcourseCount);
void loadUserCourses(User& currentUser);
void addSchedule(Course allCourse[], int& allcourseCount);
void syncStudentPackage(User& currentUser, const Course allCourse[], int allCourseCount);
void studentDeleteSchedule(User& currentUser);
void studentSearchSchedule(User& currentUser);
void studentScheduleModule(User& currentUser, Course allCourse[], int allCourseCount);
void checkScheduleChanges(User& currentUser);
void saveScheduleChanges();
void loadScheduleChanges();
void studentUpdateSchedule(User& currentUser);

// ReportingModule
void ReportingModule();
void generateSummaryReport();
void generateDetailReport();
void calculateStatistic();
void sortRecord();
void displayAnalysis();

//CourseManagementModule
void CourseManagementModule();
void addnewcourse();
void viewallcourse();
void deletecourse();
void searchcourse();
void updatecourse();
void logout();

//-------------------------------------------------------------------------------------------------------------------------
//main function
int main() {
	std::cout << "Tuition Centre Management\n";

	User currentUser;
	bool loggedin = false;
	bool runningMain = true;
	while (runningMain) {
		if (!loggedin) {
			runningMain = UserManagementModule(loggedin, currentUser);
		}
		else {
			if (currentUser.role == 1) {
				std::cout //prompt to get menuChoice
					<< "\n==========\n"
					<< "Admin Menu\n"
					<< "==========\n"
					<< "Welcome, teacher " << currentUser.username
					<< "\n\nChoose one option by typing number:\n"
					<< "1. User Module\n"
					<< "2. Subject Module\n"
					<< "3. Schedule Module\n"
					<< "4. Reporting Module\n"
					<< "0. Logout\n"
					<< "=================================================\n"
					<< "\nEnter your choice (0-4): ";
				int menuChoice;
				if (!(std::cin >> menuChoice)) {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Invalid input! Please enter a number.\n";
					continue;
				}
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				if (menuChoice < 0) {
					std::cout << "Negative value input is not allowed!";
					continue;
				}

				switch (menuChoice) {
				case 1:
					std::cout << "\n[!] Opening User Module dashboard......\n";//load the user management module
					clearScreen();
					UserManagementModule(loggedin, currentUser);
					break;

				case 2:
					std::cout << "\n[!] Opening Subject Module dashboard......\n";//load the subject module
					clearScreen();
					CourseManagementModule();
					//subject module
					break;

				case 3:
					std::cout << "\n[!] Opening Admin Schedule Module dashboard......\n";//load the admin schedule modoule
					clearScreen();
					ScheduleModule(currentUser, allCourse, allCourseCount);
					break;

				case 4:
					std::cout << "\n[!] Opening Reporting Module dashboard......\n";//load the report module
					clearScreen();
					ReportingModule();
					break;

				case 0:
					loggedin = false;
					currentUser = User{};
					std::cout << "Logging out......\n";
					clearScreen();
					break;

				default:
					std::cout << "Invalid input! Please enter a valid number.\n\n";
					break;
				}
			}
			else {
				std::cout //prompt to get menuChoice
					<< "\n=========\n"
					<< "User Menu\n"
					<< "=========\n"
					<< "Welcome, student " << currentUser.username
					<< "\n\nChoose one option by typing number:\n"
					<< "1. Student Package Module\n"
					<< "2. Schedule Module\n"
					<< "0. Logout\n"
					<< "=================================================\n"
					<< "\nEnter your choice (0-2): ";
				int menuChoice;
				if (!(std::cin >> menuChoice)) {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Invalid input! Please enter a number.\n";
					continue;
				}
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				if (menuChoice < 0) {
					std::cout << "Negative value input is not allowed!";
					continue;
				}

				switch (menuChoice) {
				case 1:
					std::cout << "\n[!] Opening Student Package Module dashboard......\n";//load the student package module
					clearScreen();
					StudentPackageModule(currentUser, allCourse, allCourseCount);
					break;

				case 2:
					std::cout << "\n[!] Opening Student Schedule Module dashboard......\n";//load the student schedule
					clearScreen();
					loadUserCourses(currentUser);
					studentScheduleModule(currentUser, allCourse, allCourseCount);
					break;

				case 0:
					loggedin = false;
					currentUser = User{};
					std::cout << "Logging out......\n";
					clearScreen();
					break;

				default:
					std::cout << "Invalid input! Please enter a valid number.\n\n";
					break;
				}
			}
		}
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------
// STORAGE
void saveUserCourses(const User& currentUser) { //load current user profile
	const int MaxLines = 100; // Maximum number of lines to store
	std::string otherUserData[MaxLines]; // Array to store other users' data
	int linecount = 0;

	std::ifstream infile("user_courses.txt");
	std::string line;

	while (std::getline(infile, line)) {
		if (line.find(currentUser.username + ",") != 0) { //if the line does not start with currentUser.username, then store it into otherUserData
			if (linecount < MaxLines) {
				otherUserData[linecount++] = line;
			}
		}
	}
	infile.close();

	std::ofstream outfile("user_courses.txt");

	for (int i = 0; i < linecount; ++i) {
		outfile << otherUserData[i] << "\n"; //write all other users' data back to the file
	}

	for (int i = 0; i < currentUser.packageCount; ++i) {
		outfile << currentUser.username << ","
			<< currentUser.mypackage[i].id << ","
			<< currentUser.mypackage[i].Name << ","
			<< currentUser.mypackage[i].price << "\n"; //write current user's courses to the file
	}
	outfile.close();
}

void loadUserCourses(User& currentUser) {
	std::ifstream infile("user_courses.txt");//read the file
	std::string line;
	currentUser.packageCount = 0; //reset counter

	while (std::getline(infile, line)) {//same as on top, but this time we string more variable to 分类 those stuff out
		size_t pos1 = line.find(',');
		size_t pos2 = line.find(',', pos1 + 1);
		size_t pos3 = line.find(',', pos2 + 1);

		if (pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos) {
			std::string username = line.substr(0, pos1);
			std::string idStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
			std::string name = line.substr(pos2 + 1, pos3 - pos2 - 1);
			std::string priceStr = line.substr(pos3 + 1);

			if (username == currentUser.username) {//changing string that taken out from the file to the variable type that could be understand by the compiler
				if (currentUser.packageCount < 10) {
					Course c;
					c.id = std::stoi(idStr);// stoi is string to integer
					c.Name = name;
					c.price = std::stod(priceStr);//stod is string to double
					currentUser.mypackage[currentUser.packageCount] = c;
					currentUser.packageCount++;
				}
			}

		}
	}
	infile.close();//close the file to avoid error 
}

void loadActionLogs() {
	std::ifstream inFile("action_logs.txt"); //open action_logs.txt to read previous history
	if (!inFile.is_open()) {
		return; //if file doesnt exist yet (first time run), just return nothing
	}

	std::string username, name;
	int courseId, statusCode;

	while (inFile >> username >> name >> courseId >> statusCode) { //read data piece by piece from file loop
		if (logCounter >= Max_Logs) {
			break; // Stop loading if we reach the maximum number of logs (prevent overflow)
		}

		logActionName[logCounter] = name;                  //store action name into 1d array
		logMatrix2D[logCounter][0] = courseId;             //store course id into 2d array column 0
		logMatrix2D[logCounter][1] = statusCode;           //store status code into 2d array column 1
		logMatrix2D[logCounter][2] = logCounter + 1; // Use logCounter + 1 as a simple timestamp or unique identifier (column 2)
		logCounter++;                                      //increase counter for next row
	}
	inFile.close(); //close file to avoid leak
}

void recordUserAction(int courseId, const std::string& name, int statusCode, const std::string& username) {
	if (logCounter >= Max_Logs) {
		std::cout << "Log limit reached. Cannot record more actions.\n"; //protect array from out of bound
		return;
	}

	logActionName[logCounter] = name;                  //save action name to 1d array
	logMatrix2D[logCounter][0] = courseId;             //save course id to 2d array col 0
	logMatrix2D[logCounter][1] = statusCode;           //save status code to 2d array col 1
	logMatrix2D[logCounter][2] = logCounter + 1; // Use logCounter + 1 as a simple timestamp or unique identifier (col 2)
	logCounter++;                                      //move to next index

	std::ofstream outFile("action_logs.txt", std::ios::app); //open file with append mode to save permanently
	if (outFile.is_open()) {
		outFile << username << " " << name << " " << courseId << " " << statusCode << "\n"; //write log format into text
		outFile.close(); //close file
	}
	else {
		std::cout << "Error: Unable to open action_logs.txt for writing.\n";
	}
}

void searchBooking() {
	if (logCounter == 0) {
		std::cout << "No booking logs available.\n"; //check if array is empty
		return;
	}

	int searchId = intgerinputfilter("Enter Course ID to search: "); //ask user for course id they want to find
	if (searchId == -1) {
		std::cout << "Invalid input. Please try again!\n";
		return;
	}
	if (searchId == -2) {
		std::cout << "Input cannot be empty. Please enter a valid number.\n";
		return;
	}

	bool found = false; //flag to check if we matched anything
	std::cout << "\n--- Search Results for Course ID: " << searchId << " ---\n";

	for (int i = 0; i < logCounter; ++i) { //loop through 1d and 2d array one by one to search
		if (logMatrix2D[i][0] == searchId) { //if column 0 (course id) match user input
			std::cout << "Record No: " << logMatrix2D[i][2] //print out matching result details
				<< " | Action: " << logActionName[i]
				<< " | Course ID: " << logMatrix2D[i][0]
				<< " | Status: " << logMatrix2D[i][1] << "\n";
			found = true;
		}
	}
	if (!found) {
		std::cout << "No records found for Course ID: " << searchId << "\n"; //if loop finished but nothing match
	}
}
//-------------------------------------------------------------------------------------------------------------------------
// User Management Module

void registerUser(User& currentUser) {
	User newUser;
	bool checkname;
	bool registration = false;
	int teacherCode = 1234; //teacher code to register as teacher, can be changed to any number you want
	int codeT;
	//check if username already exists
	do {
		checkname = false;
		newUser.username = stringinputfilter("\nCreate username (Enter 0 to cancel registration) : "); //call the input filter function to get the input and check if it is valid

		if (newUser.username == "0") {
			std::cout << "\nRegistration cancelled.\n";
			clearScreen();
			return;
		}

		std::ifstream inFile("user.txt"); //ifstream means read the file
		std::string fileU, fileP;
		int fileR;

		while (inFile >> fileU >> fileP >> fileR) {
			std::cout << "[DEBUG] Comparing " << newUser.username << " with " << fileU << "\n"; //same reason as login, for debugging purpose
			if (newUser.username == fileU) {
				std::cout << "\n[!] Username already exist, please use another username.\n";
				checkname = true;
				break;
			}
		}
	} while (checkname);

	newUser.password = stringinputfilter("Create password: "); //call the input filter function to get the input and check if it is valid

	do {
		newUser.role = intgerinputfilter("Enter role (0 for Student, 1 for Teacher): "); //call the input filter function to get the input and check if it is valid

		if (newUser.role == -1) {
			std::cout << "\n[!] Invalid input. Please enter a valid number.\n";
		}

		else if (newUser.role == -2) {
			std::cout << "\n[!] Input cannot be empty. Please enter a valid number.\n";
		}

		else if (newUser.role == 0 || newUser.role == 1) {
			break;
		}
		else {
			std::cout << "\n[!] Invalid input. Please enter a valid number.\n";
		}
	} while (true); //code always run unless breaked

	if (newUser.role == 1) {
		do {
			codeT = intgerinputfilter("Enter Teacher code: "); //call the input filter function to get the input and check if it is valid

			if (codeT == -1) {
				std::cout << "\n[!] Invalid input. Please enter a valid number.\n";
			}
			else if (codeT == -2) {
				std::cout << "\n[!] Input cannot be empty. Please enter a valid number.\n";
			}
			else if (codeT == teacherCode) { //detect teacher code from models.h
				registration = true;
				break;
			}
			else {
				std::cout << "\n[!] Code invalid.\n";
				break;
			}
		} while (true);
	}

	else {
		registration = true;
	}

	if (registration) {
		//ofstream is create or edit the file, ios::app is to keep the previous thing that alr available inside the file so it wont get wipe out when running this row
		std::ofstream outFile("user.txt", std::ios::app);
		outFile << newUser.username << " " << newUser.password << " " << newUser.role << "\n"; // store username , password, role one by one
		outFile.close();//close the file to avoid error input into the file
		std::cout << "\nRegistered successfully! Please log in now!\n";
		clearScreen();
	}
	else {
		std::cout << "\nRegistration unsuccessful. Please try again.\n";
		clearScreen();
	}
}

void addNewUser() {
	std::ofstream outfile("user.txt", std::ios::app);
	std::string username, password;
	int role;

	bool checkname;
	do { //check if username exists or not
		checkname = false;
		username = stringinputfilter("\nCreate username (Enter 0 to cancel registration) : "); //call the input filter function to get the input and check if it is valid

		if (username == "0") {
			std::cout << "\nRegistration cancelled.\n";
			clearScreen();
			return;
		}

		std::ifstream file("user.txt");
		std::string fileU, fileP;
		int fileR;

		while (file >> fileU >> fileP >> fileR) {
			if (username == fileU) {
				std::cout << "\n[!] Username already exist, please use another username.\n";
				checkname = true;
				break;
			}
		}
		file.close();
	} while (checkname);
	password = stringinputfilter("Create password: ");
	do {
		role = intgerinputfilter("Enter role (Teacher=1, Student=0): ");
		if (role == -1) { //if -1 is returned, it means a !int value is entered
			std::cout << "\n[!] Invalid input. Please try again!\n";
		}
		else if (role == -2) { //if -2 is returned, it means an empty input was entered
			std::cout << "\n[!] Input cannot be empty. Please enter a valid number.\n";
		}
		else if (role == 0 || role == 1) { //role only accepts 0 and 1
			break;
		}
		else {
			std::cout << "\n[!] Invalid input. Please try again!\n";
		}
	} while (true);

	outfile << username << " " << password << " " << role << std::endl; //Enter data to file
	outfile.close();
	std::cout << "\nUser added successfully.\n";
	clearScreen();
}

void searchUser() {
	std::ifstream infile("user.txt");
	std::string username, password;
	int role;

	std::string searchUser;
	searchUser = stringinputfilter("\nEnter username to search (0 to cancel): ");
	if (searchUser == "0") {
		std::cout << "Searching cancelled\n";
		clearScreen();
		return; //return back to menu
	}

	bool found = false;
	while (infile >> username >> password >> role) {
		if (username == searchUser) { //== means found
			std::cout << "User found\n" << std::setfill(' ') << std::left
				<< "- --------- - " << "----------------------------- -" << std::endl
				<< "| Username: | " << std::setw(30) << username << "|" << std::endl
				<< "- --------- - " << "----------------------------- -" << std::endl
				<< "| Password: | " << std::setw(30) << password << "|" << std::endl
				<< "- --------- - " << "----------------------------- -" << std::endl
				<< "| Role:     | " << std::setw(30) << (role == 1 ? "Teacher" : "Student") << "|" << std::endl
				<< "- --------- - " << "----------------------------- -" << std::endl;
			found = true;
			break;
		}
	}

	if (!found) {
		std::cout << "User not found.\n";
	}
	infile.close();
	clearScreen();
}

void updateUser(User& currentUser) {
	std::ifstream infile("user.txt");
	std::string username, password;
	int role;

	const int MaxUsers = 100; // Maximum number of users to store
	User userList[MaxUsers];
	int listCount = 0;

	std::string searchUser = stringinputfilter("Enter username to update (Enter 0 to cancel): ");
	if (searchUser == currentUser.username) { //don't allow to edit logged in user
		std::cout << "Cannot update logged in user.\n";
		infile.close();
		clearScreen();
		return;
	}
	else if (searchUser == "0") {
		std::cout << "Updating cancelled.";
		infile.close();
		clearScreen();
		return;
	}
	bool found = false;
	while (infile >> username >> password >> role) {
		if (username == searchUser) { //edit user
			bool checkname;
			do { //check username
				checkname = false;
				username = stringinputfilter("Enter new username (Enter 0 to cancel) : ");

				if (username == "0") {
					std::cout << "Updating cancelled.\n";
					infile.close();
					clearScreen();
					return;
				}

				std::ifstream file("user.txt");
				std::string fileU, fileP;
				int fileR;
				while (file >> fileU >> fileP >> fileR) {
					if (username == fileU && username != searchUser) {
						std::cout << "Username already exist, please use another username.\n";
						checkname = true;
						break;
					}
				}
				file.close();
			} while (checkname);
			password = stringinputfilter("Enter new password: ");
			do {
				role = intgerinputfilter("Enter new role (Teacher=1, Student=0): ");
				if (role == -1) { //if -1 is returned, it means a !int value is entered
					std::cout << "Invalid input. Please try again!\n";
				}
				else if (role == -2) { //if -2 is returned, it means an empty input was entered
					std::cout << "Input cannot be empty. Please enter a valid number.\n";
				}
				else if (role == 0 || role == 1) { //role only accepts 0 and 1
					break;
				}
				else {
					std::cout << "Invalid input. Please try again!\n";
				}
			} while (true);

			found = true;
		}
		if (listCount < MaxUsers) { //push all data into userList
			userList[listCount].username = username;
			userList[listCount].password = password;
			userList[listCount].role = role;
			listCount++;
		}
	}

	infile.close();

	if (found) { //rewrites userList into user.txt if got changes
		std::ofstream outfile("user.txt");
		for (int i = 0; i < listCount; i++) {
			outfile << userList[i].username << " " << userList[i].password << " " << userList[i].role << "\n";
		}
		outfile.close();
		std::cout << "User updated.\n";
		clearScreen();
	}
	else {
		std::cout << "User not found.\n";
		clearScreen();
	}
}

void deleteUser(User& currentUser) {
	std::ifstream infile("user.txt");
	std::string username, password;
	int role;

	const int MaxUsers = 100;
	User userList[MaxUsers];
	int listCount = 0;

	std::string searchUser = stringinputfilter("Enter username to delete (Enter 0 to cancel) : ");

	if (searchUser == "0") {
		std::cout << "Deletion cancelled.\n";
		infile.close();
		clearScreen();
		return;
	}

	if (searchUser == currentUser.username) { //don't allow to delete logged in user
		std::cout << "Cannot delete logged in user.\n";
		infile.close();
		clearScreen();
		return;
	}

	bool found = false;
	while (infile >> username >> password >> role) {
		if (username != searchUser) { //push all data accept the user that'll be deleted
			if (listCount < MaxUsers) {
				userList[listCount].username = username;
				userList[listCount].password = password;
				userList[listCount].role = role;
				listCount++;
			}
		}
		else {
			found = true;
		}
	}

	infile.close();

	if (found) { //rewrites userList into user.txt if got changes
		std::ofstream outfile("user.txt");
		for (int i = 0; i < listCount; i++) {
			outfile << userList[i].username << " " << userList[i].password << " " << userList[i].role << "\n";
		}
		outfile.close();
		std::cout << "User deleted.\n";
		clearScreen();
	}
	else {
		std::cout << "User not found.\n";
		clearScreen();
	}
}

void displayUser() {
	std::ifstream infile("user.txt");
	std::string username, password;

	std::cout << std::setfill(' ');
	std::cout << "\nChoose one option by typing number (Enter 0 to cancel):\n" //ask for what to display
		<< "1. Display all user\n"
		<< "2. Display all teacher\n"
		<< "3. Display all student\n";
	int displayChoice = 0;
	do {
		displayChoice = intgerinputfilter("Enter your choice(1-3): ");
		if (displayChoice == -2) { //if -2 is returned, it means an empty input was entered
			std::cout << "Input cannot be empty. Please enter a valid number.\n";
		}
		else if (displayChoice == -1 || !(displayChoice >= 0 && displayChoice <= 3)) { //if -1 is returned, it means a !int value is entered, also checks if displayChoice is 0,1,2,3
			std::cout << "Invalid input. Please try again!\n";
		}
		else if (displayChoice == 0) { //return back to menu
			clearScreen();
			return;
		}
		else {
			break;
		}
	} while (true);

	const int MaxUsers = 100;
	User allUsers[MaxUsers];
	int userCount = 0;
	User temp;
	while (infile >> temp.username >> temp.password >> temp.role) { //repeats reading a line from user.txt and save into temp until it reaches the end
		if (userCount < MaxUsers) {
			allUsers[userCount++] = temp; //store temp into allUsers array and increase the counter
		}
	}

	std::cout << "\nSort by:\n"
		<< "1. Unsorted\n"
		<< "2. Ascending order\n"
		<< "3. Descending order\n";
	int sortChoice = 0; //ask for if need to sort according to what
	do {
		sortChoice = intgerinputfilter("Enter your choice(1-3): ");
		if (sortChoice == -2) { //if -2 is returned, it means an empty input was entered
			std::cout << "Input cannot be empty. Please enter a valid number.\n";
		}
		else if (sortChoice == -1 || !(sortChoice >= 1 && sortChoice <= 3)) { //if -1 is returned, it means a !int value is entered, also checks if sortChoice is 1,2,3
			std::cout << "Invalid input. Please try again!\n";
		}
		else {
			break;
		}
	} while (true);

	if (sortChoice == 2 || sortChoice == 3) {
		for (int i = 0; i < userCount - 1; i++) { //bubble sort
			for (int j = 0; j < userCount - i - 1; j++) {
				std::string lowerCaseName1 = allUsers[j].username; //get the username on the current index
				std::string lowerCaseName2 = allUsers[j + 1].username; //get the username on the next index
				for (char& c : lowerCaseName1) { //convert current into lowercase char by char
					c = tolower(c);
				}
				for (char& c : lowerCaseName2) { //convert next into lowercase char by char
					c = tolower(c);
				}
				if (sortChoice == 2) {
					if (lowerCaseName1 > lowerCaseName2) { //if current's ASCII value is bigger than next's, then swap place
						temp = allUsers[j];
						allUsers[j] = allUsers[j + 1];
						allUsers[j + 1] = temp;
					}
				}
				else if (sortChoice == 3) {
					if (lowerCaseName1 < lowerCaseName2) { //if current's ASCII value is smaller than next's, then swap place
						temp = allUsers[j];
						allUsers[j] = allUsers[j + 1];
						allUsers[j + 1] = temp;
					}
				}
			}
		}
	}
	system("cls");
	int totalUser = 0;
	std::cout << "\n- ------------------------------ - ------- -\n"
		<< "| Username                       | Role    |\n"
		<< "- ------------------------------ - ------- -\n";

	for (int i = 0; i < userCount; i++) {
		bool display = false;
		switch (displayChoice) {
		case 1:
			display = true;
			break;
		case 2:
			display = (allUsers[i].role == 1);
			break;
		case 3:
			display = (allUsers[i].role == 0);
			break;
		}
		if (display) {
			std::cout << "| " << std::left << std::setw(30) << allUsers[i].username
				<< " | " << (allUsers[i].role == 1 ? "Teacher" : "Student") << " |\n";
			totalUser++;
		}
	}
	std::cout << "- ------------------------------ - ------- -\n";
}

void login(User& currentUser, bool& loggedin, bool& runningUserM) {
	std::string inputU, inputP;

	do {
		inputU = stringinputfilter("\nUsername (Enter 0 to cancel login) : ");

		// Check for cancellation
		if (inputU == "0") {
			std::cout << "\nLogin cancelled.\n";
			clearScreen();
			return;
		}

		inputP = stringinputfilter("Password: ");

		std::ifstream inFile("user.txt");
		std::string fileU, fileP;
		int fileR;

		while (inFile >> fileU >> fileP >> fileR) {
			std::cout << "[DEBUG] Comparing " << inputU << " with " << fileU << "\n";

			if (fileU == inputU && fileP == inputP) {
				currentUser.username = inputU;
				currentUser.role = fileR;

				std::cout << "\nLogin successful!\n";
				loggedin = true;

				if (currentUser.role == 1) { // Teacher role
					std::cout << "Redirecting to admin menu...\n";
				}
				else if (currentUser.role == 0) { // Student role
					std::cout << "Redirecting to user menu...\n";
				}

				runningUserM = false;
				clearScreen();
				return;
			}
		}
		std::cout << "Login failed! Please try again.\n";
	} while (true);
}

bool UserManagementModule(bool& loggedin, User& currentUser) {
	int choice = 0;
	bool choiceExit = false;
	bool runningUserM = true; //set it to keep running until true become false
	if (!loggedin) {
		while (runningUserM) {
			std::cout << "\n--- User Management Module --\n"
				"======================================\n"
				"1. Register\n2. Login\n0. Exit\n"
				"======================================\n"
				"Please choose one option by typing the number\n";

			choice = intgerinputfilter("Enter your choice: ");//call the input filter function to get the input and check if it is valid

			//switch here get result from the choice and entering it to case for different result
			switch (choice) {
			case 1:
				registerUser(currentUser);
				break;

			case 2:
				login(currentUser, loggedin, runningUserM);
				break;

			case 0:
				std::cout << "\n---Exit comfirmation---\n";
				choiceExit = exitConfirm();
				if (choiceExit) {
					return false;
				}
				break;
			case -2: //-2 means empty input
				std::cout << "Input cannot be empty. Please enter a valid number.\n";
				break;
			default:
				std::cout << "Invalid input! Please enter a valid number,\n";
				break;
			}
		}
	}
	else {
		while (runningUserM) {
			std::cout << "\n=============================\n";
			std::cout << "| Admin Portal: User module |\n";
			std::cout << "=============================\n\n";

			std::cout << "Choose one option by typing number:\n" //prompt to get choice
				"1. Add new record\n"
				"2. Update record\n"
				"3. Delete record\n"
				"4. Search record\n"
				"5. Display records\n"
				"0. Back to Admin Menu\n";
			std::cout << std::setfill('=') << std::setw(50) << "" << '\n';

			choice = intgerinputfilter("Enter your choice(0-5): "); //input filter

			switch (choice) {
			case 1:
				addNewUser(); // to add a new user
				break;
			case 2:
				updateUser(currentUser); // to update an existing user
				break;
			case 3:
				deleteUser(currentUser); // to delete a user
				break;
			case 4:
				searchUser(); // search for user by name
				break;
			case 5:
				displayUser(); // display users
				break;
			case 0:
				runningUserM = false; // end loop
				std::cout << "Reverting back to Admin menu......\n"; clearScreen();
				break;
			case -2: //-2 means empty input
				std::cout << "Input cannot be empty. Please enter a valid number.\n";
				break;
			default:
				std::cout << "Invalid input! Please enter a valid number.\n\n";
				break;
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------------------------------------------
// Student Package Module
void StudentPackageModule(User& currentUser, const Course allCourse[], int allcourseCount) {
	loadUserCourses(currentUser); //load the current user profile from the text file to the vector
	loadActionLogs(); //load the previous action logs from the text file to the 1d and 2d array
	bool running = true; //loop control
	while (running) {
		std::cout << "\n---View Course or Manage Course Section---\n========================================\n1.View All Courses\n2.Add Course to Package\n3.Review my package\n4.Remove Course from Package\n5.Modify my package\n6.Search Booking Record\n0.Back to User Menu\n========================================\nPlease choose one option by typing the number\n";
		int subChoice = intgerinputfilter("Enter your choice: "); //call the input filter function to get the input and check if it is valid)
		switch (subChoice) {
		case -1:
			std::cout << "Invalid input. Please try again!\n";
			continue;

		case -2:
			std::cout << "Input cannot be empty. Please enter a valid number.\n";
			continue;

		case 1:
			showAllCourses(allCourse, allcourseCount);
			break;
		case 2:
			std::cout << "\n--- Add Course to Package ---\n";
			addCoursetoPackage(currentUser, allCourse, allcourseCount);
			break;
		case 3:
			myCourseSummary(currentUser, allCourse, allcourseCount);
			break;
		case 4:
			removeCoursefromPackage(currentUser, allCourse, allcourseCount);
			break;

		case 5:
			modifyUserPackage(currentUser, allCourse, allcourseCount);
			break;

		case 6:
			searchBooking();
			break;

		case 0:
			saveUserCourses(currentUser); //back up saving, save one more time to avoid accident
			running = false;    //end the loop by changing the bool variable on the top of this page
			std::cout << "Redirecting back to User Menu...\n";
			break;
		default:
			std::cout << "Invalid option, try again.\n";
			continue;
		}

	}
}

// 1. Add a course to the user's course package
void addCoursetoPackage(User& currentUser, const Course allCourse[], int allcourseCount) {
	if (currentUser.packageCount >= 10) {
		std::cout << "Error: Package is full (Max 10 courses). Cannot add more courses.\n";
		return;
	}

	int id = intgerinputfilter("Enter Course ID to add: "); // Ask user to input the course ID they want to add
	if (id == -2) {
		std::cout << "Input cannot be empty. Please enter a valid number.\n";
		return;
	}

	if (id == -1) {
		std::cout << "Invalid input. Please try again!\n";
		return;
	}


	bool alreadyExists = false; // Check if the course is already in the user's package
	for (const auto& c : currentUser.mypackage) {
		if (c.id == id) { alreadyExists = true; break; }
	}
	if (alreadyExists) {
		std::cout << "Error: Course (ID =" << id << ") already exists in your package\n";
	}
	else {
		bool found = false; // Flag to check if we can find this ID in all available courses
		for (int i = 0; i < allcourseCount; i++) {
			if (allCourse[i].id == id) { // Compare with all course IDs one by one
				currentUser.mypackage[currentUser.packageCount++] = allCourse[i]; // If found, push it back into the user's package vector
				std::cout << "\n------------------\n";
				std::cout << "Added " << allCourse[i].Name << " to package!\n";
				found = true;

				recordUserAction(id, "ADD", 1, currentUser.username); // Log the action into the 1D/2D arrays and txt file
				saveUserCourses(currentUser); // Save immediately to prevent data loss if the program closes halfway
				std::cout << "[DEBUG] Saved the data into the text file.\n";
				break;
			}
		}
		if (!found) {
			std::cout << "Course not found in the available courses.\n"; // Finished checking but ID doesn't exist
		}
	}
}

// 2. Display all courses chosen by the current user and calculate the total fee
void myCourseSummary(User& currentUser, const Course allCourse[], int allcourseCount) {
	if (currentUser.packageCount == 0) {
		std::cout << "\nYour package is currently empty.\n"; // If the package is empty, notify the user
		return;
	}

	std::cout << "\n--- My Package Summary ---\n";
	double total = 0;
	for (int i = 0; i < currentUser.packageCount; i++) { // Loop through every course in the user's package
		std::cout << "- " << currentUser.mypackage[i].Name << " ($" << std::fixed << std::setprecision(2) << currentUser.mypackage[i].price << ")\n";
		total += currentUser.mypackage[i].price; // Accumulate the total price
	}
	std::cout << "Total Fee: $" << std::fixed << std::setprecision(2) << total << "\n"; // Print final total fee
}

// 3. Remove a course from the user's package
void removeCoursefromPackage(User& currentUser, const Course allCourse[], int allcourseCount) {
	if (currentUser.packageCount == 0) {
		std::cout << "Your package is currently empty.\n";
	}

	std::cout << "\n--- Remove Course from Package ---\n";
	for (int i = 0; i < currentUser.packageCount; i++) { // First, list out what courses the user currently has
		std::cout << "(ID=" << currentUser.mypackage[i].id << ") - " << currentUser.mypackage[i].Name << " ($" << std::fixed << std::setprecision(2) << currentUser.mypackage[i].price << ")\n";
	}
	int id = intgerinputfilter("Enter Course ID to remove: "); // Ask for the course ID to remove
	if (id == -1) {
		std::cout << "Invalid input. Please try again!\n";
		return;
	}

	if (id == -2) {
		std::cout << "Input cannot be empty. Please enter a valid number.\n";
		return;
	}

	bool found = false;
	int removeIndex = -1;
	for (int i = 0; i < currentUser.packageCount; i++) {
		if (currentUser.mypackage[i].id == id) {
			found = true;
			removeIndex = i;
			break;
		}
	}

	if (found) {
		std::cout << "Removed " << currentUser.mypackage[removeIndex].Name << " from package.\n";
		for (int i = removeIndex; i < currentUser.packageCount - 1; i++) {
			currentUser.mypackage[i] = currentUser.mypackage[i + 1]; // Shift elements to the left to fill the gap
		}
		currentUser.packageCount--; // Decrease the count of courses in the package
		recordUserAction(id, "REMOVE", 2, currentUser.username); // Log the remove action
		saveUserCourses(currentUser); // Save changes to the text file
		std::cout << "[DEBUG] Saved the data into the text file.\n";
	}

	if (!found) {
		std::cout << "\nCourse ID entered is not found in your package.\n";
	}
}

// 4. Modify the user package (remove a course first, then add a new one)
void modifyUserPackage(User& currentUser, const Course allCourse[], int allcourseCount) {
	if (currentUser.packageCount == 0) {
		std::cout << "Your package is currently empty.\n";
	}

	std::cout << "\n--- Modify User Package ---\n";
	std::cout << "Current Courses in Package:\n";
	for (int i = 0; i < currentUser.packageCount; i++) {
		std::cout << currentUser.mypackage[i].id << ". " << currentUser.mypackage[i].Name << " - $" << std::fixed << std::setprecision(2) << currentUser.mypackage[i].price << "\n";
	}

	// Stage 1: Execute the "Remove" operation
	int removeIndex = -1;
	while (true) {
		int removeId = intgerinputfilter("Enter ID to remove (or 0 to cancel/exit): ");
		if (removeId == 0) {
			std::cout << "Exiting modify package.\n";
			return;
		}

		if (removeId == -1) {
			std::cout << "Invalid input. Please try again!\n";
			continue;
		}

		if (removeId == -2) {
			std::cout << "Input cannot be empty. Please enter a valid number.\n";
			continue;
		}

		bool found = false;
		for (int i = 0; i < currentUser.packageCount; i++) {
			if (currentUser.mypackage[i].id == removeId) {
				std::cout << "Removed " << currentUser.mypackage[i].Name << " from package.\n";
				removeIndex = i;
				found = true;
				break;
			}
		}

		if (found) {
			for (int i = removeIndex; i < currentUser.packageCount - 1; i++) {
				currentUser.mypackage[i] = currentUser.mypackage[i + 1]; // Shift elements to the left to fill the gap
			}
			currentUser.packageCount--;
			recordUserAction(removeId, "MODIFY_REMOVE", 2, currentUser.username); // Log the modify-remove action
			break; // Break out of this loop if successfully removed, then proceed to the add stage
		}

		if (!found) {
			std::cout << "Course ID not found in your package.\n";
			continue; // Loop back to let the user try again if ID wasn't found
		}
	}

	// Stage 2: Execute the "Add" operation
	while (true) {
		int id = intgerinputfilter("Enter Course ID to add (or 0 to cancel/exit): ");
		if (id == 0) {
			std::cout << "Exiting modify package.\n";
			return;
		}

		if (id == -1) {
			std::cout << "Invalid input. Please try again!\n";
			continue;
		}

		if (id == -2) {
			std::cout << "Input cannot be empty. Please enter a valid number.\n";
			continue;
		}

		bool alreadyExists = false;
		for (int i = 0; i < currentUser.packageCount; i++) {
			if (currentUser.mypackage[i].id == id) { alreadyExists = true; break; }
		}

		if (alreadyExists) {
			std::cout << "Error: Course (ID =" << id << ") already exists in your package\n";
			continue;
		}

		bool foundCourse = false;
		for (int i = 0; i < allcourseCount; ++i) {
			if (allCourse[i].id == id) {
				if (currentUser.packageCount < 10) {
					currentUser.mypackage[currentUser.packageCount++] = allCourse[i]; // Add the course to the user's package
					std::cout << "\n------------------\n";
					std::cout << "Added " << allCourse[i].Name << " to package!\n";
					recordUserAction(id, "MODIFY_ADD", 3, currentUser.username); // Log the modify-add action
					saveUserCourses(currentUser); // Save to the text file instantly
					std::cout << "[DEBUG] Saved the data into the text file.\n";
					foundCourse = true;
					break;
				}
				else {
					std::cout << "Error: Package is full (Max 10 courses). Cannot add more courses.\n";
				}
				break;
			}
			if (foundCourse) {
				break; // Break out of the entire modification loop once added successfully
			}

			if (!foundCourse) {
				std::cout << "Course not found in the available courses.\n";
				continue;
			}
		}
		break;
	}
}

//6 show all courses
void showAllCourses(const Course allCourse[], int allcourseCount) {
	std::cout << "\n--- All Available Courses ---\n";
	for (int i = 0; i < allcourseCount; ++i) {
		const auto& c = allCourse[i];
		std::cout << "ID= " << c.id << ". " << c.Name << " - $" << c.price << "\n"; //since all the variable type is auto sync so can direct output easily
	}
}

//-------------------------------------------------------------------------------------------------------------------------
// Schedule Module
const int Max_Schedules = 50;

struct Schedule {
	int courseId = 0;
	std::string teacherName = "";
	std::string day = "";
	std::string timeSlot = "";
	std::string room = "";
};

Schedule allSchedules[Max_Schedules] = {
	{ 1, "Lim",   "Monday",    "09:00 AM - 11:00 AM", "Room A1" },
	{ 2, "Lim",   "Monday",    "11:30 AM - 01:30 PM", "Room A2" },
	{ 3, "Tan",   "Tuesday",   "09:00 AM - 11:00 AM", "Room B1" },
	{ 4, "Wong",  "Tuesday",   "11:30 AM - 01:30 PM", "Room B2" },
	{ 5, "Wong",  "Wednesday", "09:00 AM - 11:00 AM", "Room C1" },
	{ 6, "Wong",  "Wednesday", "11:30 AM - 01:30 PM", "Room C2" },
	{ 7, "Lee",   "Thursday",  "09:00 AM - 11:00 AM", "Room D1" },
	{ 8, "Lee",   "Thursday",  "11:30 AM - 01:30 PM", "Room D2" },
	{ 9, "Kumar", "Friday",    "09:00 AM - 11:00 AM", "Room E1" },
	{ 10, "Kumar","Friday",    "11:30 AM - 01:30 PM", "Room E2" },
	{ 11, "Tan",  "Saturday",  "09:00 AM - 11:00 AM", "Room F1" },
	{ 12, "Lee",  "Saturday",  "11:30 AM - 01:30 PM", "Room F2" },
	{ 13, "Kumar","Sunday",    "09:00 AM - 11:00 AM", "Room G1" },
	{ 14, "Lim",  "Sunday",    "11:30 AM - 01:30 PM", "Room G2" },
	{ 15, "Tan",  "Monday",    "02:00 PM - 04:00 PM", "Room A3" },
	{ 16, "Wong", "Tuesday",   "02:00 PM - 04:00 PM", "Room B3" },
	{ 17, "Lee",  "Wednesday", "02:00 PM - 04:00 PM", "Room C3" },
	{ 18, "Kumar","Thursday",  "02:00 PM - 04:00 PM", "Room D3" },
	{ 19, "Lim",  "Friday",    "02:00 PM - 04:00 PM", "Room E3" },
	{ 20, "Wong", "Saturday",  "02:00 PM - 04:00 PM", "Room F3" }
};

int scheduleCount = 20;

struct ScheduleChange
{
	int courseId = 0;
	std::string oldDay = "";
	std::string oldTime = "";
	std::string oldRoom = "";
	std::string newDay = "";
	std::string newTime = "";
	std::string newRoom = "";
};

ScheduleChange scheduleChanges[Max_Schedules];
int scheduleChangeCount = 0;

std::string stringInputWithSpaces(const std::string& prompt) {
	std::string input;
	while (true) {
		std::cout << prompt;
		if (!std::getline(std::cin, input)) {
			return "";
		}
		if (input.empty()) {
			std::cout << "Input cannot be empty! Please try again.\n";
			continue;
		}
		return input;
	}
}

bool getCourseDetails(const Course allCourse[], int allcourseCount, int courseId, std::string& courseName, double& coursePrice) {
	for (int i = 0; i < allcourseCount; ++i) {
		if (allCourse[i].id == courseId) {
			courseName = allCourse[i].Name;
			coursePrice = allCourse[i].price;
			return true;
		}
	}
	return false;
}

// ==================== ADMIN SCHEDULE MODULE IMPLEMENTATION ====================
std::string getField(const std::string& line, int index) {
	int current = 0;
	size_t start = 0;
	size_t end = line.find(',');
	while (end != std::string::npos) {
		if (current == index) {
			return line.substr(start, end - start);
		}
		start = end + 1;
		end = line.find(',', start);
		current++;
	}
	if (current == index) {
		return line.substr(start);
	}
	return "";
}

void saveSchedulesToFile(const Course allCourse[], int allcourseCount) {
	std::ofstream cFile("courses.txt");
	if (cFile.is_open()) {
		for (int i = 0; i < allcourseCount; ++i) {
			cFile << allCourse[i].id << ","
				<< allCourse[i].Name << ","
				<< allCourse[i].price << "\n";
		}
		cFile.close();
	}
	std::ofstream sFile("schedules.txt");

	if (sFile.is_open()) {
		for (int i = 0; i < scheduleCount; ++i) {
			sFile << allSchedules[i].courseId << ","
				<< allSchedules[i].teacherName << ","
				<< allSchedules[i].day << ","
				<< allSchedules[i].timeSlot << ","
				<< allSchedules[i].room << "\n";
		}
		sFile.close();
	}
}

void loadSchedulesFromFile(Course allCourse[], int& allcourseCount) {
	std::ifstream sFile("schedules.txt");
	if (sFile.is_open()) {
		scheduleCount = 0;
		std::string line;

		while (scheduleCount < Max_Schedules && std::getline(sFile, line)) {
			if (line.empty())
				continue;

			std::string idStr = getField(line, 0);
			std::string teacherStr = getField(line, 1);
			std::string dayStr = getField(line, 2);
			std::string timeStr = getField(line, 3);
			std::string roomStr = getField(line, 4);

			if (!idStr.empty()) {
				allSchedules[scheduleCount].courseId = std::stoi(idStr);
				allSchedules[scheduleCount].teacherName = teacherStr;
				allSchedules[scheduleCount].day = dayStr;
				allSchedules[scheduleCount].timeSlot = timeStr;
				allSchedules[scheduleCount].room = roomStr;
				scheduleCount++;
			}
		}

		sFile.close();
	}
}

// 1. Admin Add Schedule
void addSchedule(Course allCourse[], int& allcourseCount) {

	if (scheduleCount >= Max_Schedules) {
		std::cout << "\n[!] Error: Schedule capacity is full!\n";
		return;
	}
	std::cout << "\n--- Add New Schedule ---\n";
	int id = intgerinputfilter("Enter Course ID (0 to cancel): ");
	if (id == 0) {
		std::cout << "[!] Operation cancelled.\n";
		return;
	}
	if (id < 1 || id > 50) {
		std::cout << "[!] Invalid Course ID!\n";
		return;
	}
	// Check Course exists in Subject Module
	int courseIdx = -1;
	for (int i = 0; i < allcourseCount; ++i) {
		if (allCourse[i].id == id) {
			courseIdx = i;
			break;
		}
	}
	if (courseIdx == -1) {
		std::cout << "[!] Course ID " << id << " not found in Subject Module.\n";
		return;
	}

	// Check whether this Course already has an active schedule
	// If it was deleted before, reuse that old schedule position
	int existingIndex = -1;

	for (int i = 0; i < scheduleCount; ++i) {
		if (allSchedules[i].courseId == id) {
			// Active schedule
			if (allSchedules[i].day != "-" && allSchedules[i].timeSlot != "-" && allSchedules[i].room != "-") {
				std::cout << "[!] Schedule for Course ID " << id << " already exists!\n";
				return;
			}

			// Deleted schedule found
			existingIndex = i;
			break;
		}
	}

	std::cout << "Course Name: "
		<< allCourse[courseIdx].Name
		<< "\n";

	// Enter schedule information
	std::string teacherName = stringInputWithSpaces("Enter Teacher Name: ");
	std::string day = stringInputWithSpaces("Enter Day/Date: ");
	std::string timeSlot = stringInputWithSpaces("Enter Time: ");
	std::string room = stringInputWithSpaces("Enter Location: ");

	// Reuse deleted schedule slot
	if (existingIndex != -1) {
		allSchedules[existingIndex].courseId = id;
		allSchedules[existingIndex].teacherName = teacherName;
		allSchedules[existingIndex].day = day;
		allSchedules[existingIndex].timeSlot = timeSlot;
		allSchedules[existingIndex].room = room;
	}
	else {
		// Add completely new schedule
		allSchedules[scheduleCount].courseId = id;
		allSchedules[scheduleCount].teacherName = teacherName;
		allSchedules[scheduleCount].day = day;
		allSchedules[scheduleCount].timeSlot = timeSlot;
		allSchedules[scheduleCount].room = room;
		scheduleCount++;
	}

	saveSchedulesToFile(allCourse, allcourseCount);

	std::cout << "\n[SUCCESS] Schedule added successfully!\n";
}

// 2. Admin Update Schedule
void updateSchedule(Course allCourse[], int& allcourseCount) {

	std::cout << "\n--- Update Schedule ---\n";

	int id = intgerinputfilter(
		"Enter Course ID to update (0 to cancel): "
	);

	if (id <= 0) {

		std::cout << "[!] Operation cancelled.\n";
		return;
	}

	int index = -1;

	for (int i = 0; i < scheduleCount; ++i) {

		if (allSchedules[i].courseId == id &&
			allSchedules[i].day != "-" &&
			allSchedules[i].timeSlot != "-" &&
			allSchedules[i].room != "-") {

			index = i;
			break;
		}
	}
	if (index == -1) {

		std::cout << "[!] Schedule not found for Course ID "
			<< id << ".\n";
		return;
	}

	std::string courseName = "Unknown";
	double price = 0.0;

	getCourseDetails(allCourse, allcourseCount, id, courseName, price);

	std::cout << "\nCourse ID   : " << id << "\n";
	std::cout << "Course Name : " << courseName << "\n";
	std::cout << "Current Teacher : " << allSchedules[index].teacherName << "\n";
	std::cout << "Current Day/Date : " << allSchedules[index].day << "\n";
	std::cout << "Current Time     : " << allSchedules[index].timeSlot << "\n";
	std::cout << "Current Location : " << allSchedules[index].room << "\n";
	std::cout << "\n--- Enter New Schedule Information ---\n";

	if (scheduleChangeCount < Max_Schedules)
	{
		scheduleChanges[scheduleChangeCount].courseId = allSchedules[index].courseId;

		scheduleChanges[scheduleChangeCount].oldDay = allSchedules[index].day;
		scheduleChanges[scheduleChangeCount].oldTime = allSchedules[index].timeSlot;
		scheduleChanges[scheduleChangeCount].oldRoom = allSchedules[index].room;
	}

	allSchedules[index].teacherName = stringInputWithSpaces("Enter new Teacher Name: ");
	allSchedules[index].day = stringInputWithSpaces("Enter new Day/Date: ");
	allSchedules[index].timeSlot = stringInputWithSpaces("Enter new Time: ");
	allSchedules[index].room = stringInputWithSpaces("Enter new Location: ");
	saveSchedulesToFile(allCourse, allcourseCount);

	// Keep only the latest 2 changes for the same Course ID
	int courseChangeCount = 0;

	for (int i = 0; i < scheduleChangeCount; ++i) {
		if (scheduleChanges[i].courseId == allSchedules[index].courseId) {
			courseChangeCount++;
		}
	}

	if (courseChangeCount >= 2) {
		int oldestIndex = -1;

		for (int i = 0; i < scheduleChangeCount; ++i) {
			if (scheduleChanges[i].courseId == allSchedules[index].courseId) {
				oldestIndex = i;
				break;
			}
		}

		if (oldestIndex != -1) {
			for (int i = oldestIndex; i < scheduleChangeCount - 1; ++i) {
				scheduleChanges[i] = scheduleChanges[i + 1];
			}

			scheduleChangeCount--;
		}
	}

	if (scheduleChangeCount < Max_Schedules) {
		scheduleChanges[scheduleChangeCount].newDay = allSchedules[index].day;
		scheduleChanges[scheduleChangeCount].newTime = allSchedules[index].timeSlot;
		scheduleChanges[scheduleChangeCount].newRoom = allSchedules[index].room;

		scheduleChangeCount++;
	}

	saveScheduleChanges();
	saveSchedulesToFile(allCourse, allcourseCount);

	std::cout << "\n[SUCCESS] Schedule updated successfully!\n";
}

// 3. Admin Delete Schedule
void deleteSchedule(Course allCourse[], int& allcourseCount) {

	std::cout << "\n--- Delete Schedule ---\n";

	int id = intgerinputfilter(
		"Enter Course ID to delete schedule (0 to cancel): "
	);

	if (id <= 0) {

		std::cout << "[!] Operation cancelled.\n";
		return;
	}

	int index = -1;

	for (int i = 0; i < scheduleCount; ++i) {

		if (allSchedules[i].courseId == id) {

			index = i;
			break;
		}
	}

	if (index == -1) {
		std::cout << "[!] Schedule not found for Course ID " << id << ".\n";
		return;
	}

	std::string courseName = "Unknown";
	double price = 0.0;
	getCourseDetails(allCourse, allcourseCount, id, courseName, price);

	std::cout << "\nCourse ID   : " << id << "\n";
	std::cout << "Course Name : " << courseName << "\n";
	std::cout << "Teacher     : " << allSchedules[index].teacherName << "\n";
	std::cout << "Day/Date    : " << allSchedules[index].day << "\n";
	std::cout << "Time        : " << allSchedules[index].timeSlot << "\n";
	std::cout << "Location    : " << allSchedules[index].room << "\n";

	// Record the old schedule before clearing it
	// Keep only the latest 2 changes for the same Course ID
	int courseChangeCount = 0;

	for (int i = 0; i < scheduleChangeCount; ++i) {
		if (scheduleChanges[i].courseId == allSchedules[index].courseId) {
			courseChangeCount++;
		}
	}

	if (courseChangeCount >= 2) {
		int oldestIndex = -1;

		for (int i = 0; i < scheduleChangeCount; ++i) {
			if (scheduleChanges[i].courseId == allSchedules[index].courseId) {
				oldestIndex = i;
				break;
			}
		}
		if (oldestIndex != -1) {
			for (int i = oldestIndex; i < scheduleChangeCount - 1; ++i) {
				scheduleChanges[i] = scheduleChanges[i + 1];
			}

			scheduleChangeCount--;
		}
	}

	if (scheduleChangeCount < Max_Schedules) {
		scheduleChanges[scheduleChangeCount].courseId = allSchedules[index].courseId;

		scheduleChanges[scheduleChangeCount].oldDay = allSchedules[index].day;
		scheduleChanges[scheduleChangeCount].oldTime = allSchedules[index].timeSlot;
		scheduleChanges[scheduleChangeCount].oldRoom = allSchedules[index].room;

		scheduleChanges[scheduleChangeCount].newDay = "-";
		scheduleChanges[scheduleChangeCount].newTime = "-";
		scheduleChanges[scheduleChangeCount].newRoom = "-";

		scheduleChangeCount++;
	}

	// Clear schedule information only
	allSchedules[index].teacherName = "-";
	allSchedules[index].day = "-";
	allSchedules[index].timeSlot = "-";
	allSchedules[index].room = "-";

	saveScheduleChanges();
	saveSchedulesToFile(allCourse, allcourseCount);

	std::cout << "\n[SUCCESS] Schedule information cleared.\n";
	std::cout << "[+] Course ID and Course Name are retained.\n";
}

// 4. Admin Search Schedule
void searchSchedule(const Course allCourse[], int allcourseCount) {
	std::cout << "\n--- Search Schedule ---\n";
	int id = intgerinputfilter("Enter Course ID to search (0 to cancel): ");
	if (id <= 0) {
		std::cout << "[!] Operation cancelled.\n";
		return;
	}

	bool found = false;
	for (int i = 0; i < scheduleCount; ++i) {
		if (allSchedules[i].courseId == id) {
			std::string courseName = "Unknown";
			double price = 0.0;
			getCourseDetails(allCourse, allcourseCount, id, courseName, price);

			std::cout << "\n=========================================================\n";
			std::cout << "                    SCHEDULE FOUND                      \n";
			std::cout << "=========================================================\n";
			std::cout << "Course ID   : " << allSchedules[i].courseId << "\n";
			std::cout << "Course Name : " << courseName << "\n";
			std::cout << "Teacher     : " << allSchedules[i].teacherName << "\n";
			std::cout << "Day/Date    : " << allSchedules[i].day << "\n";
			std::cout << "Time        : " << allSchedules[i].timeSlot << "\n";
			std::cout << "Location    : " << allSchedules[i].room << "\n";
			std::cout << "=========================================================\n";
			found = true;
			break;
		}
	}

	if (!found) {
		std::cout << "[!] No schedule found for Course ID " << id << ".\n";
	}
}

// 5. Admin Display All Schedules
void displayAllSchedules(const Course allCourse[], int allcourseCount) {

	std::cout << "\n====================================================================================================\n";
	std::cout << "                                       CLASS SCHEDULE                                       \n";
	std::cout << "====================================================================================================\n";

	std::cout << std::left
		<< std::setw(6) << "ID"
		<< std::setw(22) << "Course Name"
		<< std::setw(20) << "Teacher"
		<< std::setw(15) << "Day/Date"
		<< std::setw(25) << "Time"
		<< std::setw(15) << "Location"
		<< "\n";

	std::cout << "----------------------------------------------------------------------------------------------------\n";

	if (scheduleCount == 0) {

		std::cout << "No schedule entries found in the system.\n";
	}
	else {

		for (int i = 0; i < scheduleCount; ++i) {

			std::string courseName = "Unknown";
			double price = 0.0;

			getCourseDetails(
				allCourse,
				allcourseCount,
				allSchedules[i].courseId,
				courseName,
				price
			);

			std::cout << std::left
				<< std::setw(6) << allSchedules[i].courseId
				<< std::setw(22) << courseName
				<< std::setw(20) << allSchedules[i].teacherName
				<< std::setw(15) << allSchedules[i].day
				<< std::setw(25) << allSchedules[i].timeSlot
				<< std::setw(15) << allSchedules[i].room
				<< "\n";

		}
	}

	std::cout << "====================================================================================================\n";
}

//6. Admin Teaching Schedule
void displayTeacherSchedule(
	User& currentUser,
	const Course allCourse[],
	int allcourseCount) {

	std::cout << "\n====================================================================================\n";
	std::cout << "                              MY TEACHING SCHEDULE                                  \n";
	std::cout << "====================================================================================\n";

	std::cout << "Teacher: " << currentUser.username << "\n\n";
	std::cout << std::left
		<< std::setw(6) << "ID"
		<< std::setw(22) << "Course Name"
		<< std::setw(15) << "Day/Date"
		<< std::setw(25) << "Time"
		<< std::setw(15) << "Location"
		<< "\n";

	std::cout << "------------------------------------------------------------------------------------\n";

	bool found = false;

	for (int i = 0; i < scheduleCount; ++i) {
		if (allSchedules[i].teacherName ==
			currentUser.username) {
			std::string courseName = "Unknown";
			double price = 0.0;
			getCourseDetails(allCourse, allcourseCount, allSchedules[i].courseId, courseName, price);

			std::cout << std::left
				<< std::setw(6) << allSchedules[i].courseId
				<< std::setw(22) << courseName
				<< std::setw(15) << allSchedules[i].day
				<< std::setw(25) << allSchedules[i].timeSlot
				<< std::setw(15) << allSchedules[i].room << "\n";
			found = true;
		}
	}

	if (!found) {
		std::cout << "No teaching schedule found for " << currentUser.username << ".\n";
	}

	std::cout << "====================================================================================\n";
}

// ==================== STUDENT SCHEDULE MODULE IMPLEMENTATION ====================

void syncStudentPackage(User& currentUser, const Course allCourse[], int allCourseCount) {
	int validCount = 0;
	for (int i = 0; i < currentUser.packageCount; ++i) {
		int studentCourseId = currentUser.mypackage[i].id;
		int foundIdx = -1;

		for (int k = 0; k < allCourseCount; ++k) {
			if (allCourse[k].id == studentCourseId) {
				foundIdx = k;
				break;
			}
		}

		if (foundIdx != -1) {
			currentUser.mypackage[validCount] = allCourse[foundIdx];
			validCount++;
		}
	}
	currentUser.packageCount = validCount;
}

// 1. Student Check Schedule Changes
void saveScheduleChanges() {
	std::ofstream file("schedule_changes.txt");
	if (!file) {
		std::cout << "[!] Unable to save schedule changes.\n";
		return;
	}

	for (int i = 0; i < scheduleChangeCount; ++i) {
		file << scheduleChanges[i].courseId << "\n";
		file << scheduleChanges[i].oldDay << "\n";
		file << scheduleChanges[i].oldTime << "\n";
		file << scheduleChanges[i].oldRoom << "\n";
		file << scheduleChanges[i].newDay << "\n";
		file << scheduleChanges[i].newTime << "\n";
		file << scheduleChanges[i].newRoom << "\n";
	}

	file.close();
}

void loadScheduleChanges() {
	std::ifstream file("schedule_changes.txt");
	if (!file) {
		scheduleChangeCount = 0;
		return;
	}

	scheduleChangeCount = 0;

	while (scheduleChangeCount < Max_Schedules && file >> scheduleChanges[scheduleChangeCount].courseId) {
		file.ignore();

		std::getline(file, scheduleChanges[scheduleChangeCount].oldDay);
		std::getline(file, scheduleChanges[scheduleChangeCount].oldTime);
		std::getline(file, scheduleChanges[scheduleChangeCount].oldRoom);
		std::getline(file, scheduleChanges[scheduleChangeCount].newDay);
		std::getline(file, scheduleChanges[scheduleChangeCount].newTime);
		std::getline(file, scheduleChanges[scheduleChangeCount].newRoom);

		scheduleChangeCount++;
	}

	file.close();
}

void checkScheduleChanges(User& currentUser) {
	std::cout << "\n=========================================\n";
	std::cout << "         CHECK SCHEDULE CHANGES          \n";
	std::cout << "=========================================\n";

	if (currentUser.packageCount == 0) {
		std::cout << "[!] You do not have any courses in your package.\n";
		return;
	}

	bool foundChange = false;

	for (int i = 0; i < currentUser.packageCount; ++i) {
		int courseId = currentUser.mypackage[i].id;

		for (int j = 0; j < scheduleChangeCount; ++j) {
			if (scheduleChanges[j].courseId == courseId) {
				foundChange = true;

				std::cout << "\n-----------------------------------------\n";
				std::cout << "Course ID   : " << courseId << "\n";
				std::cout << "Course Name : " << currentUser.mypackage[i].Name << "\n";

				if (scheduleChanges[j].newDay == "-" && scheduleChanges[j].newTime == "-" && scheduleChanges[j].newRoom == "-") {
					std::cout << "\nSchedule Status : DELETED\n";
					std::cout << "\nPrevious Schedule:\n";
					std::cout << "Day         : " << scheduleChanges[j].oldDay << "\n";
					std::cout << "Time        : " << scheduleChanges[j].oldTime << "\n";
					std::cout << "Room        : " << scheduleChanges[j].oldRoom << "\n";
				}
				else {
					std::cout << "\nPrevious Schedule:\n";
					std::cout << "Day         : " << scheduleChanges[j].oldDay << "\n";
					std::cout << "Time        : " << scheduleChanges[j].oldTime << "\n";
					std::cout << "Room        : " << scheduleChanges[j].oldRoom << "\n";

					std::cout << "\nNew Schedule:\n";
					std::cout << "Day         : " << scheduleChanges[j].newDay << "\n";
					std::cout << "Time        : " << scheduleChanges[j].newTime << "\n";
					std::cout << "Room        : " << scheduleChanges[j].newRoom << "\n";
				}

				std::cout << "-----------------------------------------\n";
			}
		}
	}

	if (!foundChange) {
		std::cout << "\n[!] No schedule changes found for your courses.\n";
	}
	std::cout << "=========================================\n";
}

// 2. Student Update Schedule
void studentUpdateSchedule(User& currentUser) {
	std::cout << "\n=========================================\n";
	std::cout << "          UPDATE MY SCHEDULE             \n";
	std::cout << "=========================================\n";

	if (currentUser.packageCount == 0) {
		std::cout << "\n[!] You do not have any courses.\n";
		return;
	}
	std::cout << "\nYour Courses:\n";

	for (int i = 0; i < currentUser.packageCount; ++i) {
		std::cout << "Course ID   : " << currentUser.mypackage[i].id << "\n";
		std::cout << "Course Name : " << currentUser.mypackage[i].Name << "\n";
		std::cout << "-----------------------------------------\n";
	}

	int courseId = intgerinputfilter("Enter Course ID to update (0 to cancel): ");
	if (courseId == 0) {
		std::cout << "[!] Operation cancelled.\n";
		return;
	}
	if (courseId < 0) {
		std::cout << "[!] Invalid Course ID.\n";
		return;
	}
	int packageIndex = -1;
	for (int i = 0; i < currentUser.packageCount; ++i) {
		if (currentUser.mypackage[i].id == courseId) {
			packageIndex = i;
			break;
		}
	}
	if (packageIndex == -1) {
		std::cout << "\n[!] This course is not in your package.\n";
		return;
	}
	int scheduleIndex = -1;
	for (int i = 0; i < scheduleCount; ++i) {
		if (allSchedules[i].courseId == courseId && allSchedules[i].day != "-" && allSchedules[i].timeSlot != "-" && allSchedules[i].room != "-") {
			scheduleIndex = i;
			break;
		}
	}
	if (scheduleIndex == -1) {
		std::cout << "\n[!] No schedule found for this course.\n";
		return;
	}

	std::cout << "\n========== CURRENT SCHEDULE ==========\n";
	std::cout << "Course ID : " << courseId << "\n";
	std::cout << "Course    : " << currentUser.mypackage[packageIndex].Name << "\n";
	std::cout << "Teacher   : " << allSchedules[scheduleIndex].teacherName << "\n";
	std::cout << "Day/Date  : " << allSchedules[scheduleIndex].day << "\n";
	std::cout << "Time      : " << allSchedules[scheduleIndex].timeSlot << "\n";
	std::cout << "Location  : " << allSchedules[scheduleIndex].room << "\n";
	std::cout << "======================================\n";

	std::cout << "\n========== AVAILABLE SCHEDULES ==========\n";

	int availableCount = 0;
	int availableIndex[Max_Schedules];

	// Teacher of the current course
	std::string currentTeacher =
		allSchedules[scheduleIndex].teacherName;

	for (int i = 0; i < scheduleCount; ++i) {
		// Do not use the current schedule
		if (i == scheduleIndex) {
			continue;
		}
		// Skip deleted / empty schedules
		if (allSchedules[i].day == "-" || allSchedules[i].timeSlot == "-" || allSchedules[i].room == "-") {
			continue;
		}
		bool teacherConflict = false;
		bool studentConflict = false;

		// =========================================
		// 1. Check Teacher Conflict
		for (int j = 0; j < scheduleCount; ++j) {
			// Do not compare with the course being updated
			if (j == scheduleIndex) {
				continue;
			}
			if (allSchedules[j].teacherName == currentTeacher &&
				allSchedules[j].day == allSchedules[i].day &&
				allSchedules[j].timeSlot == allSchedules[i].timeSlot) {

				teacherConflict = true;
				break;
			}
		}

		if (teacherConflict) {
			continue;
		}
		if (teacherConflict) {
			continue;
		}

		// =========================================
		// 2. Check Student Conflict
		for (int j = 0; j < currentUser.packageCount; ++j) {
			int studentCourseId =
				currentUser.mypackage[j].id;

			// Ignore the course that student is updating
			if (studentCourseId == courseId) {
				continue;
			}
			for (int k = 0; k < scheduleCount; ++k) {
				if (allSchedules[k].courseId == studentCourseId) {
					if (allSchedules[k].day == allSchedules[i].day && allSchedules[k].timeSlot == allSchedules[i].timeSlot) {
						studentConflict = true;
						break;
					}
				}
			}
			if (studentConflict) {
				break;
			}
		}
		if (studentConflict) {
			continue;
		}

		// =========================================
		// Available
		availableIndex[availableCount] = i;

		std::cout << availableCount + 1 << ". " << allSchedules[i].day << " | " << allSchedules[i].timeSlot << " | " << allSchedules[i].room << "\n";
		availableCount++;
	}
	int choice = intgerinputfilter("Choose an available schedule (0 to cancel): ");
	if (choice == 0) {
		std::cout << "[!] Operation cancelled.\n";
		return;
	}
	if (choice < 1 || choice > availableCount) {
		std::cout << "[!] Invalid schedule choice.\n";
		return;
	}

	int selectedIndex = availableIndex[choice - 1];

	std::cout << "\n========== NEW SCHEDULE ==========\n";
	std::cout << "Course    : " << currentUser.mypackage[packageIndex].Name << "\n";
	std::cout << "Teacher   : " << allSchedules[scheduleIndex].teacherName << "\n";
	std::cout << "Day       : " << allSchedules[selectedIndex].day << "\n";
	std::cout << "Time      : " << allSchedules[selectedIndex].timeSlot << "\n";
	std::cout << "Location  : " << allSchedules[scheduleIndex].room << "\n";
	std::cout << "==================================\n";

	int confirm = intgerinputfilter("Are you sure you want to update? (1 = Yes, 0 = No): ");
	if (confirm == 0) {
		std::cout << "[!] Update cancelled.\n";
		return;
	}
	if (confirm != 1) {
		std::cout << "[!] Invalid choice.\n";
		return;
	}
	// Save the old schedule information
	std::string oldDay = allSchedules[scheduleIndex].day;
	std::string oldTime = allSchedules[scheduleIndex].timeSlot;
	std::string oldRoom = allSchedules[scheduleIndex].room;

	// Update student's course schedule
	// Same Teacher and Room
	allSchedules[scheduleIndex].day = allSchedules[selectedIndex].day;
	allSchedules[scheduleIndex].timeSlot = allSchedules[selectedIndex].timeSlot;

	saveSchedulesToFile(allCourse, allCourseCount);

	std::cout << "\n[SUCCESS] Your schedule has been updated successfully!\n";
	std::cout << "=========================================\n";
}

// 3. Student Delete Schedule
void studentDeleteSchedule(User& currentUser)
{
	if (currentUser.packageCount == 0)
	{
		std::cout << "\n[!] You do not have any courses in your package.\n";
		return;
	}

	std::cout << "\n========== DELETE MY SCHEDULE ==========\n";
	int id = intgerinputfilter("Enter Course ID to delete (0 to cancel): ");
	if (id == 0)
	{
		std::cout << "[!] Operation cancelled.\n";
		return;
	}
	if (id < 0)
	{
		std::cout << "[!] Invalid Course ID.\n";
		return;
	}
	int foundIndex = -1;

	// Find the course inside student's package
	for (int i = 0; i < currentUser.packageCount; ++i) {
		if (currentUser.mypackage[i].id == id) {
			foundIndex = i;
			break;
		}
	}

	// Course not found
	if (foundIndex == -1) {
		std::cout << "[!] This Course ID is not in your schedule.\n";
		return;
	}

	std::cout << "\nCourse found: " << currentUser.mypackage[foundIndex].Name << "\n";
	int confirm = intgerinputfilter("Are you sure you want to delete this schedule? (1 = Yes, 0 = No): ");

	if (confirm == 0) {
		std::cout << "[!] Delete cancelled.\n";
		return;
	}
	if (confirm != 1) {
		std::cout << "[!] Invalid choice.\n";
		return;
	}

	// Shift the remaining courses to the left
	for (int i = foundIndex; i < currentUser.packageCount - 1; ++i) {
		currentUser.mypackage[i] = currentUser.mypackage[i + 1];
	}

	currentUser.packageCount--;

	// Save the updated student package
	saveUserCourses(currentUser);

	std::cout << "\n[SUCCESS] Schedule deleted successfully!\n";
}

// 4.Student Search Schedule 
void studentSearchSchedule(User& currentUser) {
	std::cout << "\n--- Search My Schedule ---\n";
	if (currentUser.packageCount == 0) {
		std::cout << "[!] Your schedule is empty.\n";
		return;
	}

	int id = intgerinputfilter("Enter Course ID to search (0 to cancel): ");
	if (id <= 0) {
		std::cout << "[!] Operation cancelled.\n";
		return;
	}
	bool found = false;

	for (int i = 0; i < currentUser.packageCount; ++i) {
		if (currentUser.mypackage[i].id == id) {
			std::string day = "TBD", time = "TBD", room = "TBD", teacher = "TBD";

			for (int j = 0; j < scheduleCount; ++j) {
				if (allSchedules[j].courseId == id) {
					teacher = allSchedules[j].teacherName;
					day = allSchedules[j].day;
					time = allSchedules[j].timeSlot;
					room = allSchedules[j].room;
					break;
				}
			}

			std::cout << "\n==========================================================================================\n";
			std::cout << "                        SCHEDULE FOUND                        \n";
			std::cout << "==========================================================================================\n";
			std::cout << std::left << std::setw(6) << "ID"
				<< std::setw(24) << "Course Name"
				<< std::setw(18) << "Teacher"
				<< std::setw(12) << "Day"
				<< std::setw(22) << "Time Slot"
				<< std::setw(10) << "Room" << "\n";
			std::cout << "------------------------------------------------------------------------------------------\n";
			std::cout << std::left << std::setw(6) << currentUser.mypackage[i].id
				<< std::setw(24) << currentUser.mypackage[i].Name
				<< std::setw(18) << teacher
				<< std::setw(12) << day
				<< std::setw(22) << time
				<< std::setw(10) << room << "\n";
			std::cout << "==========================================================================================\n";
			found = true;
			break;
		}
	}

	if (!found) {
		std::cout << "[!] Course ID " << id << " is not in your schedule.\n";
	}
}

// 6.Student Display My Schedule 
void displayMySchedule(User& currentUser, Course allCourse[], int allCourseCount) {
	syncStudentPackage(currentUser, allCourse, allCourseCount);

	if (currentUser.packageCount == 0) { //for new student haven't add course
		std::cout << "\n=========================================================\n";
		std::cout << " [!] Your package is currently empty! No schedule available.\n";
		std::cout << "=========================================================\n";
		return;
	}

	std::cout << "\n========================================================================================\n";
	std::cout << "                                  MY CLASS SCHEDULE                             \n";
	std::cout << "========================================================================================\n";
	std::cout << std::left << std::setw(6) << "ID"
		<< std::setw(24) << "Course Name"
		<< std::setw(18) << "Teacher"
		<< std::setw(12) << "Day"
		<< std::setw(22) << "Time Slot"
		<< std::setw(10) << "Room" << "\n";
	std::cout << "----------------------------------------------------------------------------------------\n";

	for (int i = 0; i < currentUser.packageCount; ++i) {
		int currentId = currentUser.mypackage[i].id;
		std::string day = "TBD", time = "TBD", room = "TBD", teacher = "TBD";

		for (int k = 0; k < allCourseCount; ++k) {
			if (allCourse[k].id == currentId) {
				break;
			}
		}

		for (int j = 0; j < scheduleCount; ++j) {
			if (allSchedules[j].courseId == currentId) {
				teacher = allSchedules[j].teacherName;
				day = allSchedules[j].day;
				time = allSchedules[j].timeSlot;
				room = allSchedules[j].room;
				break;
			}
		}

		std::cout << std::left << std::setw(6) << currentId
			<< std::setw(24) << currentUser.mypackage[i].Name
			<< std::setw(18) << teacher
			<< std::setw(12) << day
			<< std::setw(22) << time
			<< std::setw(10) << room << "\n";
	}
	std::cout << "========================================================================================\n";
}




// Admin Schedule Dashboard
void ScheduleModule(User& currentUser, Course allCourse[], int& allcourseCount) {
	loadSchedulesFromFile(allCourse, allcourseCount);
	bool runningSchedule = true;

	while (runningSchedule) {
		std::cout << "\n=========================================\n";
		std::cout << "          SCHEDULE MODULE                \n";
		std::cout << "=========================================\n";
		std::cout << "1. Add Schedule\n";
		std::cout << "2. Update Schedule\n";
		std::cout << "3. Delete Schedule\n";
		std::cout << "4. Search Schedule\n";
		std::cout << "5. Display All Schedule\n";
		std::cout << "6. My Teaching Schedule\n";
		std::cout << "0. Back to Main Menu\n";
		std::cout << "=========================================\n";

		int choice =
			intgerinputfilter("Enter your choice (0-6): ");

		switch (choice) {
		case 1:
			addSchedule(allCourse, allcourseCount);
			break;
		case 2:
			updateSchedule(allCourse, allcourseCount);
			break;
		case 3:
			deleteSchedule(allCourse, allcourseCount);
			break;
		case 4:
			searchSchedule(allCourse, allcourseCount);
			break;
		case 5:
			displayAllSchedules(allCourse, allcourseCount);
			break;
		case 6:
			displayTeacherSchedule(currentUser, allCourse, allcourseCount);
			break;
		case 0:
			runningSchedule = false;
			std::cout << "Reverting back to main menu...\n";
			clearScreen();
			break;
		case -2:
			std::cout << "Input cannot be empty!\n";
			break;
		default:
			std::cout << "Invalid choice, please try again.\n";
			break;
		}
	}
}

// Student Schedule Dashboard 
void studentScheduleModule(User& currentUser, Course allCourse[], int allCourseCount) {
	loadSchedulesFromFile(allCourse, allCourseCount);
	loadScheduleChanges();
	syncStudentPackage(currentUser, allCourse, allCourseCount);

	int choice = -1;
	while (choice != 0) {
		std::cout << "\n=========================================\n";
		std::cout << "            STUDENT SCHEDULE        \n";
		std::cout << "=========================================\n";
		std::cout << "1. Check Schedule Changes\n";
		std::cout << "2. Update Schedule\n";
		std::cout << "3. Delete My Schedule\n";
		std::cout << "4. Search My Schedule\n";
		std::cout << "5. Display All Schedule\n";
		std::cout << "6. Display My Schedule\n";
		std::cout << "0. Back to User Menu\n";
		std::cout << "=========================================\n";

		choice = intgerinputfilter("Enter choice (0-6): ");

		switch (choice) { //switch to choose function
		case 1:
			checkScheduleChanges(currentUser);
			break;
		case 2:
			studentUpdateSchedule(currentUser);
			break;
		case 3:
			studentDeleteSchedule(currentUser);
			break;
		case 4:
			studentSearchSchedule(currentUser);
			break;
		case 5:
			displayAllSchedules(allCourse, allCourseCount);
			break;
		case 6:
			displayMySchedule(currentUser, allCourse, allCourseCount);
			break;
		case 0:
			std::cout << "Returning to Student Dashboard...\n"; clearScreen();
			break;
		default:
			std::cout << "[!] Invalid option. Try again.\n";
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------
//Reporting Module
void ReportingModule() {
	bool running = true;  //loop the menu
	int reportchoice;
	while (running) {

		std::cout << "\n================================\n";
		std::cout << "|Admin Portal: Reporting module|\n";
		std::cout << "================================\n\n";
		std::cout << "Choose one option by typing number:\n1. Generate Summary Report\n2. Generate Detailed Report\n3. Calculate Statistic\n4. Sort Record\n5. Display Analysis\n0. Back to Admin Menu\n";

		reportchoice = intgerinputfilter("Enter your choice(0-5): "); //integer filter to filter out any value that is not 0 to 5
		if (reportchoice == -1) {
			std::cout << "Invalid input! Please enter a valid number.";
			continue;
		}

		if (reportchoice == -2) {
			std::cout << "Input cannot be empty! Please enter a valid number.";
			continue;
		}
		//use switch to choose the function
		switch (reportchoice) {
		case 1:
			generateSummaryReport();
			break;
		case 2:
			generateDetailReport();
			break;
		case 3:
			calculateStatistic();
			break;
		case 4:
			sortRecord();
			break;
		case 5:
			displayAnalysis();
			break;
		case 0:
			running = false;  //end loop
			std::cout << "Reverting back to admin menu......\n\n";
			break;
		default:
			std::cout << "Invalid input. Please enter a valid number.\n";
		}

	}
}

//case 1: generate summary report
void generateSummaryReport() {
	const int MaxUsers = 100;   //declare the max users
	User allusers[MaxUsers];   //declare the allusers array
	int userCount = 0;   //initialize the user count
	readUserandCoursefile(allusers, userCount);   //load the users files from the header

	if (userCount == 0) {
		std::cout << "System ERROR. No user record found\n"; //if there is no text file detected, return error message
		return;
	}
	int totalenrollments = 0;    //total amount of course taken by student
	int zeroCourseStudent = 0;   //student who doesn't has any course taken

	//[i] is an array index that point to the current user's course that is store inside the text file
	for (int i = 0; i < userCount; ++i) {   //read the current user from the text file
		int studentCourseCount = allusers[i].packageCount;  //count the user course taken and save into the studentCourseCount
		totalenrollments += studentCourseCount;    //totalenrollments = totalenrollments + studentCourseCount, count the course that is added for this user and put into totalenrollments
		if (studentCourseCount == 0) {   //if compiler found out that this user has zero course, then the user will be record into here
			zeroCourseStudent++;
		}
	}

	//the menu for summary report and load out the results
	std::cout << '\n' << std::string(78, '=') << "\n";
	std::cout << std::string(30, ' ') << "Summary Report\n";
	std::cout << std::string(78, '=') << '\n';
	std::cout << "Total student registered: " << std::string(42, '.') << userCount << " students\n";
	std::cout << "Total course registered: " << std::string(43, '.') << totalenrollments << " courses\n";
	if (zeroCourseStudent > 0) {
		std::cout << "[WARNING] Currently there are " << zeroCourseStudent << " students that doesn't registered any courses!\n";
	}
	std::cout << std::string(78, '=') << "\n";
}

//case 2: generate detail report
void generateDetailReport() {
	const int MaxUsers = 100;
	User allusers[MaxUsers];
	int userCount = 0;
	readUserandCoursefile(allusers, userCount);

	if (userCount == 0) {
		std::cout << "System ERROR. No user record found.\n";
		return;
	}

	std::string searchUser;
	searchUser = stringinputfilter("Enter username to search: ");   //input filter
	bool founduser = false;

	for (int i = 0; i < userCount; ++i) { //used to search the user
		if (allusers[i].username == searchUser) {
			founduser = true;

			//detail report menu and load the results
			if (allusers[i].packageCount == 0) {   //if the course txt file is empty then give error message
				std::cout << "\nERROR. This student has not registered any course.\n";
			}
			else {
				std::cout << '\n' << std::string(78, '=') << "\n";
				std::cout << std::string(30, ' ') << "Detailed Report\n";
				std::cout << std::string(78, '=') << '\n';
				std::cout << "Name: " << allusers[i].username << '\n'; //display the username that had been searched out
				std::cout << std::string(78, '=') << '\n';
				std::cout << "[Class Enrolled]\n";
				std::cout << "Total course taken: " << allusers[i].packageCount << '\n'; // display how many course taken by the user
				std::cout << std::string(78, '-') << '\n';
				std::cout << std::setfill(' ');
				std::cout << std::left << std::setw(14) << "Course ID" << std::left << std::setw(46) << "Course Name" << std::right << std::setw(14) << "Price (RM)\n";
				std::cout << std::string(78, '-') << '\n';

				//[j] is an array index that is used to point the current user's course inside the course text file
				for (int j = 0; j < allusers[i].packageCount; ++j) { //a loop to generate the course id, course name and price one by one depends on how many course does the user have
					std::cout << " " << std::left << std::setw(13) << allusers[i].mypackage[j].id << std::left << std::setw(46) << allusers[i].mypackage[j].Name << std::right << std::setw(6) << "RM " << std::fixed << std::setprecision(2) << allusers[i].mypackage[j].price << '\n';
				}
				std::cout << std::string(78, '-') << '\n';
			}
		}
	}
	if (!founduser) {
		std::cout << "User not found!";
	}
}

//case 3: calculate statistic
void calculateStatistic() {
	const int MaxUsers = 100;
	User allusers[MaxUsers];
	int userCount = 0;
	readUserandCoursefile(allusers, userCount);

	if (userCount == 0) {
		std::cout << "System ERROR. No user record found.\n";
		return;
	}

	int totalenrollment = 0;
	int inactivestudent = 0;

	for (int i = 0; i < userCount; ++i) {
		int courseCount = allusers[i].packageCount; //used to count how many course 
		totalenrollment += courseCount; //if one course is added, store into the totalenrollement
		if (courseCount == 0) { //if there is no course inside the user
			inactivestudent++; //put the user into here
		}
	}
	//statis_cast here is used to show the remainder of the value after calculation.
	const double averageCourseperStudent = static_cast<double>(totalenrollment) / userCount; //calculate the average course taken by the student
	const double inactiveStudentRate = static_cast<double>(inactivestudent) / userCount * 100; //calculate the percentage of inactivestudent which is without course

	std::cout << '\n' << std::string(78, '=') << '\n';
	std::cout << std::string(30, ' ') << "Calculate Statistic\n";
	std::cout << std::string(78, '=') << '\n';
	std::cout << "Academic Metrics" << std::string(40, ' ') << "Current Data\n";
	std::cout << std::string(78, '-') << '\n';
	std::cout << "Total Students: " << std::string(40, ' ') << userCount << " students"; //display total student
	std::cout << "\nTotal Enrollments: " << std::string(37, ' ') << totalenrollment << " students"; //display how many course taken
	std::cout << "\nAverage Courses Taken per Student: " << std::string(21, ' ') << std::fixed << std::setprecision(1) << averageCourseperStudent << " per students";
	std::cout << "\nTotal Inactive Students: " << std::string(31, ' ') << inactivestudent << " students"; //number of inactive student
	std::cout << "\nTotal Inactive Students Rate: " << std::string(26, ' ') << std::fixed << std::setprecision(1) << inactiveStudentRate << " %\n"; //the rate of inactive student
	std::cout << std::string(78, '-') << '\n';
}

//case 4: sort students'name 
void sortRecord() {
	const int MaxUsers = 100;
	User allusers[MaxUsers];
	int userCount = 0;
	readUserandCoursefile(allusers, userCount);

	if (userCount == 0) {
		std::cout << "System ERROR. No user record found.\n";
		return;
	}

	//selection sort
	for (int i = 0; i < userCount - 1; i++) { //outer loop: Track the target position
		int minIndex = i; //assume the current position i is the smallest alphabet
		for (int j = i + 1; j < userCount; ++j) { //inner loop: find the real smallest alphabet
			if (allusers[j].username < allusers[minIndex].username) { //if the alphabet is smaller than the current alphabet, then swap the position
				minIndex = j;
			}
		}
		//this is where the alphabet being swap
		if (minIndex != i) { //this is a check if the minIndex is still equal to i, it means that the position is correct
			User temp = allusers[i]; //create temporarily user to store the data safely
			allusers[i] = allusers[minIndex]; //overwrite the current user, such as if C is at i, A is at minIndex, then A will become i 
			allusers[minIndex] = temp; //while C will become minIndex by putting back the user being saved inside the temp
		}
	}

	std::cout << '\n' << std::string(78, '=') << '\n';
	std::cout << std::string(30, ' ') << "Sort Menu By Username\n";
	std::cout << std::string(78, '=') << '\n';
	std::cout << "No.\tStudent\t\tCourse Taken\n";
	std::cout << std::string(78, '-') << '\n';

	for (int i = 0; i < userCount; i++) { //loop out the user name and courses one by one after sorted
		std::cout << " " << (i + 1) << "\t" << allusers[i].username << "\t\t" << allusers[i].packageCount << " courses" << '\n';
		std::cout << std::string(78, '-') << '\n';
	}
}

//case 5: display analysis
void displayAnalysis() {
	const int MaxUsers = 100;
	User allusers[MaxUsers];
	int userCount = 0;
	readUserandCoursefile(allusers, userCount);

	if (userCount == 0) {
		std::cout << "System ERROR. No user record found.\n";
		return;
	}

	std::cout << '\n' << std::string(80, '=') << '\n';
	std::cout << std::string(26, ' ') << "Tuition Centre Analysis\n";
	std::cout << std::string(80, '=') << '\n';
	std::cout << "Diagnostic Indicator 1: Zero Course Enrollment Attrtion Risk\n"; //display the first diagnostic for student who has zero course
	bool hasriskstudent = false;

	for (int i = 0; i < userCount; ++i) {
		if (allusers[i].packageCount == 0) { //loop the student who has no course registered
			std::cout << "[WARNING] Student " << allusers[i].username << " currently have 0 courses registered!\n";
			hasriskstudent = true;
		}
	}
	if (hasriskstudent) { //if there are students who doesn't have course taken
		std::cout << "\nSYSTEM RECOMMENDATION: Front-desk course consultants should call the\n";
		std::cout << "student's parents immediately to assist in completing course enrollment.\n";
	}
	else { //if every students have course taken
		std::cout << "Good! All students have successfully registered their courses, zero loss risk.\n";
	}
	std::cout << std::string(80, '-') << '\n';

	//display second diagnostic for good students
	std::cout << "Diagnostic Indicator 2: Academic Pressure for elite students\n";
	bool overloadstudent = false;

	for (int i = 0; i < userCount; ++i) {
		if (allusers[i].packageCount >= 10) { //if the current student has taken more or equal to 10 course, then display the warning message and overloadstudent become true 
			std::cout << "[WARNING] Student " << allusers[i].username << " has more or equal to 10 courses registered!\n";
			overloadstudent = true;
		}
	}
	if (overloadstudent) { //if there is overloadstudent
		std::cout << "\nSYSTEM RECOMMENDATION: Instructors of each subject should pay closer attention\n";
		std::cout << "to the follow up of the above student to prevent excessive academic pressure.\n";
	}
	else { //if there is no overloadstudent
		std::cout << "Status Normal : No student is currently enrolled in more than 10 courses; the academic workload is balanced.\n";
	}
	std::cout << std::string(80, '-') << '\n';
}
void CourseManagementModule() {
	bool running = true;


	while (running) {
		std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

		std::cout << "Welcome to admin menu\n";
		std::cout << "=======================================\n";
		std::cout << "1.Add new course\n" << "2.View all course\n" << "3.Delete a course \n" << "4.Search the course\n" << "5.update the course\n" << "6.logut\n " << "enter your choices\n";

		int choice = intgerinputfilter("");

		switch (choice) {
		case 1:
			addnewcourse();
			break;

		case 2:
			viewallcourse();
			break;

		case 3:
			deletecourse();
			break;

		case 4:
			searchcourse();
			break;

		case 5:
			updatecourse();
			break;

		case 6:
			running = false;

			break;
		default:
			std::cout << "Invalid choice,please try again.\n";
			break;






		}




	}
}
const int MAX_COURSES = 100;
int allCoursecount = 20;

void addnewcourse() {






	Course newcourse;
	bool idExists = false;
	int id;
	bool idvalid = false;
	do {
		id = intgerinputfilter("enter a course id:");
		idvalid = true;
		if (id == -1) {

			std::cout << "Invalid course id \n";
			idvalid = false;


		}
		else if (id == -2) {
			std::cout << "Course id can not be empty\n";
			idvalid = false;
		}
		else if (id < 0) {
			std::cout << "Course id can not be negative\n";

			idvalid = false;
		}

		newcourse.id = id;



		for (int i = 0;i < allCoursecount;i++) {
			if (allCourse[i].id == newcourse.id) {
				idExists = true;
				std::cout << "Course id " << newcourse.id << " is Exists.\n";
				idvalid = false;
			}
		}
	} while (!idvalid);
	std::string name = stringinputfilter("enter the course name:");

	if (name.empty()) {
		std::cout << "course name can not be empty\n";


	}
	newcourse.Name = name;

	bool pricevalid = false;
	int price;
	do {

		price = intgerinputfilter("Enter the course price:");
		pricevalid = true;
		if (price == -1) {
			std::cout << "Invalid price please enter again\n";
			pricevalid = false;
		}

		else if (price == -2) {
			std::cout << "Price can not be empty\n";
			pricevalid = false;
		}
		else if (price < 0) {

			std::cout << "Price can not be negatif\n";
			pricevalid = false;
		}


		if (pricevalid) {
			newcourse.price = price;
			std::cout << "Add course successful";
			break;
		}
	} while (!pricevalid);

	allCourse[allCoursecount++] = newcourse;
}





void viewallcourse() {
	std::cout << "================All course available:================\n";
	if (allCoursecount == 0) {
		std::cout << "Course list is emtpy\n";

	}
	else {
		std::cout << "id          " << "name         " << "                price(RM)        " << "      \n";
		for (int i = 0;i < allCoursecount;i++) {
			std::cout << std::left << std::setw(10) << allCourse[i].id;
			std::cout << std::left << std::setw(35) << allCourse[i].Name;
			std::cout << std::right << std::setw(6) << allCourse[i].price << "\n";



		}
	}
}
int deleteid;
void deletecourse() {

	bool found = false;
	bool idvalid = false;
	do {
		deleteid = intgerinputfilter("Please enter the course id to delete the course:");
		idvalid = true;
		if (deleteid == -1) {
			std::cout << "Invalid id,please try again\n";
			idvalid = false;
		}
		else if (deleteid == -2) {
			std::cout << "Input id can not be empty\n";

			idvalid = false;
		}
		else if (deleteid < 0) {
			std::cout << "Course id can not be negative\n";

			idvalid = false;
		}
	} while (!idvalid);
	if (allCoursecount == 0) {
		std::cout << "No course for delete.This list is emtpy\n";



	}

	for (int i = 0;i < allCoursecount;i++) {

		if (allCourse[i].id == deleteid) {
			found = true;
			std::cout << "found the course:" << allCourse[i].Name << "\n";
			std::cout << "course id:" << allCourse[i].id << "\n";
			bool confirmvalid = false;
			do {
				std::string confirm = stringinputfilter("\nAre you sure delete this course ? (Enter Y / N)\n");
				confirmvalid = true;

				if (confirm == "Y") {
					for (int j = i;j < allCoursecount - 1;j++) {
						allCourse[j] = allCourse[j + 1];

					}
					allCoursecount--;

					std::cout << "delete successful\n";
					break;
				}
				if (confirm == "N") {
					std::cout << "your cancel was deleted\n";
					break;
				}
				else {
					std::cout << "Invalid input,please try again";
					confirmvalid = false;
				}

			} while (!confirmvalid);

		}


	}
	if (!found) {
		std::cout << "Course id " << deleteid << " not found\n";

	}

}


void searchcourse() {

	int searchid;
	bool found = false;
	bool idvalid = true;
	do {
		idvalid = true;
		searchid = intgerinputfilter("Enter the course id to search course:");
		if (searchid == -1) {
			std::cout << "invalid id,please enter again.\n";
			idvalid = false;


		}

		else if (searchid == -2) {
			std::cout << "Input id can not be empty\n";
			idvalid = false;

		}
		else if (searchid < 0) {
			std::cout << "Course id can not be negative\n";
			idvalid = false;

		}
	} while (!idvalid);
	if (allCoursecount == 0) {


		std::cout << "Course list is empty.\n";

	}

	for (int i = 0;i < allCoursecount;i++) {
		if (searchid == allCourse[i].id) {
			found = true;
			std::cout << "course name: " << allCourse[i].Name << "\n";
			std::cout << "course price:RM " << allCourse[i].price << "\n";

			break;



		}


	}
	if (!found) {
		std::cout << "Course id " << searchid << " not found\n";

	}
}
void updatecourse() {



	int updateid;
	bool found = false;
	bool idvalid = false;
	do {
		updateid = intgerinputfilter("enter the course id to update information: ");
		idvalid = true;
		if (updateid == -1) {
			std::cout << "Invalid id,please try again\n";
			idvalid = false;


		}
		else if (updateid == -2) {
			std::cout << "Course id can not be empty\n";

			idvalid = false;
		}
		else if (updateid < 0) {
			std::cout << "Course id can not be negative\n";

			idvalid = false;
		}
	} while (!idvalid);

	if (allCoursecount == 0) {


		std::cout << "No course exists.\n";
		return;

	}

	for (int i = 0;i < allCoursecount;i++) {


		if (allCourse[i].id == updateid) {
			found = true;
			std::cout << "===============Course information now================\n";
			std::cout << "Course name:" << allCourse[i].Name << "\n";
			std::cout << "Course price:RM " << allCourse[i].price << "\n";


			int newid = 0;
			bool idvalid = false;
			do {
				bool found = false;

				newid = intgerinputfilter("Enter the new course id: ");
				allCourse[i].id = newid;
				idvalid = true;
				if (newid == -1) {
					std::cout << "Invalid id,please try again\n";
					idvalid = false;


				}
				else if (newid == -2) {
					std::cout << "Id can not be emtpy\n";

					idvalid = false;
				}
				else if (newid < 0) {
					std::cout << "id can not be negative \n";
					idvalid = false;
				}

				else {
					bool idExist = false;
					for (int k = 0;k < allCoursecount;k++) {
						if (k != i && allCourse[k].id == newid) {
							idExist = true;
							break;
						}
					}

					if (idExist) {
						std::cout << "Course id " << newid << " already exist\n";
						idvalid = false;


					}
					else {
						allCourse[i].id = newid;
						std::cout << "Update the course id to " << newid << '\n';


					}
				}
			} while (!idvalid);

			std::string newname = stringinputfilter("Enter the new name:");


			if (newname.empty()) {
				std::cout << "Course keep unchanged.\n";


			}
			else {
				allCourse[i].Name = newname;
				std::cout << "update the course name to " << newname << "\n";
			}
			bool pricevalid = true;
			do {
				double newprice = intgerinputfilter("Enter the newprice :");



				if (newprice == -1) {
					std::cout << "Invalid input,please try again\n";

					pricevalid = false;
				}
				else if (newprice == -2) {
					std::cout << "Price can not be empty\n";

					pricevalid = false;
				}
				else if (newprice < 0) {
					std::cout << "Price can not be negative\n";
					pricevalid = false;
				}
				else {
					allCourse[i].price = newprice;
					std::cout << "update the price to RM" << newprice << "\n";
					break;
				}
			} while (!pricevalid);



			std::cout << "==========New course information========= \n";
			std::cout << "Course id:" << allCourse[i].id << '\n';
			std::cout << "Course name:" << allCourse[i].Name << "\n";
			std::cout << "Course price :" << allCourse[i].price << "\n";
			std::cout << "Course update successful";
			break;




		}


	}
	if (!found) {
		std::cout << "Course id " << updateid << " not found \n";

	}
}


//-------------------------------------------------------------------------------------------------------------------------
