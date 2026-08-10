#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

// MODELS
int mainchoice, teacherCode = 1234;
const int Max_Logs = 100; // Maximum number of logs to store
std::string logActionName[Max_Logs];
int logMatrix2D[Max_Logs][3];
int logCounter = 0;

struct Course {
	int id = 0;
	std::string Name = "";
	double price = 0.0;
};
std::vector<Course> allCourse = { //create a course vector to store courses
		{1,"Mathematics", 100.5},
		{2,"Problem solving and programming", 200.0},
		{3,"Science", 350.5}
};

struct User {
	std::string username;
	std::string password;
	std::vector<Course> mypackage;
	int role = 0;
};

inline void readUserandCoursefile(std::vector<User>& allusers) { //inline is a function that when multiple files use this header file, it will not copy the whole real function into 
	std::ifstream userfile("user.txt");          // its own cpp file, instead, it act like a virtual header file that only one function exist in the header file so that the compiler won't crash due to multiple same file in different cpp file
	std::ifstream coursefile("user_courses.txt");   //意思是强制系统就算有很多cpp在用report.h，最后只需要融合成一个function，不可以重复复制一样的function,不然我需要写很多次一样的function
	User tempUser;
	//read user.txt file
	while (userfile >> tempUser.username >> tempUser.password >> tempUser.role) {
		if (tempUser.role == 0) {
			tempUser.mypackage.clear();//clear the remain record before starting the next command
			allusers.push_back(tempUser);
		}
	}
	userfile.close();

	//read course file, since it has comma at the text file, we will use getline here instead of reading it directly
	std::string line;

	while (std::getline(coursefile, line)) {
		std::stringstream ss(line);
		std::string username, idStr, CourseStr, priceStr;

		if (std::getline(ss, username, ',') &&
			std::getline(ss, idStr, ',') &&
			std::getline(ss, CourseStr, ',') &&
			std::getline(ss, priceStr, ',')) {

			//make a folder to store the value and equalize the models.h
			Course c;
			c.id = std::stoi(idStr);   //convert string to int
			c.Name = CourseStr;
			c.price = std::stod(priceStr);  //convert string to double

			for (auto& user : allusers) {  //go back to allusers at line 18
				if (user.username == username) {    //check if this is the current user
					user.mypackage.push_back(c); //if this is the current user, put the course that he had selected and put into this currrent user
					break;
				}
			}
		}
	}
	coursefile.close();
}

//-------------------------------------------------------------------------------------------------------------------------
// TOOLS

int intgerinputfilter(const std::string& prompt) { // to cout the prompt and get the input from user, then check if it is valid
	std::cin.clear();
	std::string input;
	while (true) {
		std::cout << prompt;

		if (!std::getline(std::cin, input)) { // read whole line of input and check if it is valid, no matter it is just a space or empty, it will return -2, if the input is not valid(not integer), it will return -1
			return -1;
		}

		if (input.empty()) { //if input is empty, return -2 to indicate that the input is empty
			return -2;
		}

		std::stringstream ss(input); //use stringstream to convert the input string to integer
		int val;
		char extra;

		if (ss >> val && !(ss >> extra)) { //fetch integer from the stringstream and check if there is any extra character after the integer, if yes, it will return -1 to indicate that the input is not valid
			return val;
		}
		else {
			return -1; //return -1 to indicate that the input is not valid
		}
	}
}

std::string stringinputfilter(const std::string& prompt) {
	std::string input;
	while (true) {
		std::cin.clear();
		std::cout << prompt;
		if (!std::getline(std::cin, input)) {
			return ""; // Return empty string on input failure
		}

		if (input == "0") {
			return "0"; // Return 0 if the user inputs "0"
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
//-------------------------------------------------------------------------------------------------------------------------
// HEADINGS
void modifyUserPackage(User& currentUser, const std::vector<Course>& allCourse);
void addCoursetoPackage(User& currentUser, const std::vector<Course>& allCourse);
void myCourseSummary(User& currentUser, const std::vector<Course>& allCourse);
void removeCoursefromPackage(User& currentUser, const std::vector<Course>& allCourse);
void showAllCourses(const std::vector<Course>& allCourse);
void StudentPackageModule(User& currentUser, const std::vector<Course>& allCourse);
bool UserManagementModule(bool& loggedin, User& currentUser);
void ReportingModule();
void showReportModule();
void generateSummaryReport();
void generateDetailReport();
void calculateStatistic();
void sortRecord();
void displayAnalysis();

//-------------------------------------------------------------------------------------------------------------------------
//main function
int main() {
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
					<< "Welcome, Admin.\n\n"
					<< "Choose one option by typing number:\n"
					<< "1. User Module\n"
					<< "2. Subject Module\n"
					<< "3. Schedule Module\n"
					<< "4. Reporting Module\n"
					<< "0. Back to Main Menu\n"
					<< "=================================================\n"
					<< "\nEnter your choice (0-5): ";
				int menuChoice;
				if (!(std::cin >> menuChoice)) {
					std::cin.clear();
					std::cin.ignore(100, '\n');
					std::cout << "Invalid input! Please enter a number.\n";
					continue;
				}
				std::cin.ignore(100, '\n');
				switch (menuChoice) {
				case 1:
					std::cout << "\n[!] Opening User Module dashboard......\n";//load the user management module
					UserManagementModule(loggedin, currentUser);
					break;
				case 2:
					std::cout << "\n[!] Opening Subject Module dashboard......\n";//load the subject module
					//subject module
					break;
				case 3:
					std::cout << "\n[!] Opening Schedule Module dashboard......\n";//load the schedule module
					//schedule module
					break;
				case 4:
					std::cout << "\n[!] Opening Reporting Module dashboard......\n";//load the report module
					ReportingModule();
					break;
				case 0:
					loggedin = false;
					currentUser = User{};
					std::cout << "Logging out......\n\n";
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
					<< "0. Back to Main Menu\n"
					<< "=================================================\n"
					<< "\nEnter your choice (0-2): ";
				int menuChoice;
				if (!(std::cin >> menuChoice)) {
					std::cin.clear();
					std::cin.ignore(100, '\n');
					std::cout << "Invalid input! Please enter a number.\n";
					continue;
				}
				std::cin.ignore(100, '\n');

				switch (menuChoice) {
				case 1:
					std::cout << "\n[!] Opening Student Package Module dashboard......\n";//load the student package module
					StudentPackageModule(currentUser, allCourse);
					break;
				case 0:
					loggedin = false;
					currentUser = User{};
					std::cout << "Logging out......\n\n";
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
	std::vector<std::string> otherUserData;  //using vector to temporary save other user profile（and skip the current user one first)
	std::ifstream infile("user_courses.txt"); //open user_courses.txt file
	std::string line;

	while (std::getline(infile, line)) {  //loop , read all the things inside the file one by one and store into variable line
		if (line.find(currentUser.username + ",") != 0) { //if the line doesnt have current user name
			otherUserData.push_back(line);                //store this line (as i mention on row 13) to the vector 
		}
	}
	infile.close();//close the file to avoid error 

	std::ofstream outfile("user_courses.txt");//write the file

	for (const auto& l : otherUserData) {//write all the things into the file one by one that alr store in the vector
		outfile << l << "\n";// l is temporary variable that create on row 22, this is write all the other user profile things back to the file
	}                        //i m not using ios::app here cuz i face multiplying bug when change user account when using that so i change it to this
	for (const auto& c : currentUser.mypackage) {//store current user data into the file
		outfile << currentUser.username << "," << c.id << "," << c.Name << "," << c.price << "\n";
	}
	outfile.close();//close the file to avoid error 
}

void loadUserCourses(User& currentUser) {
	std::ifstream infile("user_courses.txt");//read the file
	std::string line;
	currentUser.mypackage.clear();//clean the vector before read

	while (std::getline(infile, line)) {//same as on top, but this time we string more variable to 分类 those stuff out
		std::stringstream ss(line);//auto process those variable into the format given(and we feed line into the when ss is created instantly everyloop)
		std::string username, idStr, name, priceStr;

		if (std::getline(ss, username, ',') &&//when reach , then take the thing before , and put it into variable to 分类
			std::getline(ss, idStr, ',') &&//&& means if the stuff is correct and can be read then continue to next row
			std::getline(ss, name, ',') &&
			std::getline(ss, priceStr, ',')) {

			if (username == currentUser.username) {//changing string that taken out from the file to the variable type that could be understand by the compiler
				Course c;
				c.id = std::stoi(idStr);// stoi is string to integer
				c.Name = name;
				c.price = std::stod(priceStr);//stod is string to double
				currentUser.mypackage.push_back(c);//
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
		newUser.username = stringinputfilter("Create username (Enter 0 to cancel registration) : "); //call the input filter function to get the input and check if it is valid

		if (newUser.username == "0") {
			std::cout << "Registration cancelled.\n";
			return;
		}

		std::ifstream inFile("user.txt"); //ifstream means read the file
		std::string fileU, fileP;
		int fileR;

		while (inFile >> fileU >> fileP >> fileR) {
			std::cout << "[DEBUG] Comparing " << newUser.username << " with " << fileU << "\n"; //same reason as login, for debugging purpose
			if (newUser.username == fileU) {
				std::cout << "Username already exist, please use another username.\n";
				checkname = true;
				break;
			}
		}
	} while (checkname);

	newUser.password = stringinputfilter("Create password: "); //call the input filter function to get the input and check if it is valid

	do {
		newUser.role = intgerinputfilter("Enter role (0 for Student, 1 for Teacher): "); //call the input filter function to get the input and check if it is valid

		if (newUser.role == -1) {
			std::cout << "Invalid input. Please try again.\n";
		}

		else if (newUser.role == -2) {
			std::cout << "Input cannot be empty. Please enter a valid number.\n";
		}

		else if (newUser.role == 0 || newUser.role == 1) {
			break;
		}
		else {
			std::cout << "Invalid input.\n";
		}
	} while (true); //code always run unless breaked

	if (newUser.role == 1) {
		codeT = intgerinputfilter("Enter Teacher code: "); //call the input filter function to get the input and check if it is valid

		if (codeT == -1) {
			std::cout << "Invalid input. Please try again.\n";
			return;
		}

		if (codeT == -2) {
			std::cout << "Input cannot be empty. Please enter a valid number.\n";
			return;
		}

		if (codeT == teacherCode) { //detect teacher code from models.h
			registration = true;
		}
		else {
			std::cout << "Code invalid.\n";
			return;
		}
	}


	else {
		registration = true;
	}

	if (registration) {
		//ofstream is create or edit the file, ios::app is to keep the previous thing that alr available inside the file so it wont get wipe out when running this row
		std::ofstream outFile("user.txt", std::ios::app);
		outFile << newUser.username << " " << newUser.password << " " << newUser.role << "\n"; // store username , password, role one by one
		outFile.close();//close the file to avoid error input into the file
		std::cout << "Registered successfully! Please log in now!\n";
	}
	else {
		std::cout << "Registration unsuccessful. Please try again.\n";
	}
}

void addNewUser() {
	std::ofstream outfile("user.txt", std::ios::app);
	std::string username, password;
	int role;

	bool checkname;
	do { //check if username exists or not
		checkname = false;
		username = stringinputfilter("Create username (Enter 0 to cancel registration) : "); //call the input filter function to get the input and check if it is valid

		if (username == "0") {
			std::cout << "Registration cancelled.\n";
			return;
		}

		std::ifstream file("user.txt");
		std::string fileU, fileP;
		int fileR;

		while (file >> fileU >> fileP >> fileR) {
			if (username == fileU) {
				std::cout << "Username already exist, please use another username.\n";
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

	outfile << username << " " << password << " " << role << std::endl; //Enter data to file
	outfile.close();
	std::cout << "User added successfully.\n";
}

void searchUser() {
	std::ifstream infile("user.txt");
	std::string username, password;
	int role;

	std::string searchUser;
	searchUser = stringinputfilter("Enter username to search: ");

	bool found = false;
	while (infile >> username >> password >> role) {
		if (username == searchUser) { //== means found
			std::cout << "User found\n";
			std::cout << "Username: " << username << std::endl;
			std::cout << "Password: " << password << std::endl;
			std::cout << "Role: " << (role == 1 ? "Teacher" : "Student") << std::endl;
			found = true;
			break;
		}
	}

	if (!found) {
		std::cout << "User not found.\n";
	}
	infile.close();
}

void updateUser(User& currentUser) {
	std::ifstream infile("user.txt");
	std::string username, password;
	int role;
	std::vector<User> userList;

	std::string searchUser = stringinputfilter("Enter username to update: ");
	if (searchUser == currentUser.username) { //don't allow to edit logged in user
		std::cout << "Cannot update logged in user.\n";
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
					std::cout << "Registration cancelled.\n";
					return;
				}

				std::ifstream file("Data/user.txt");
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
		User u; //push all data into userList
		u.username = username;
		u.password = password;
		u.role = role;
		userList.push_back(u);
	}

	infile.close();

	if (found) { //rewrites userList into user.txt if got changes
		std::ofstream outfile("user.txt");
		for (User& u : userList) {
			outfile << u.username << " " << u.password << " " << u.role << "\n";
		}
		outfile.close();
		std::cout << "User updated.\n";
	}
	else {
		std::cout << "User not found.\n";
	}
}

void deleteUser(User& currentUser) {
	std::ifstream infile("user.txt");
	std::string username, password;
	int role;
	std::vector<User> userList;

	std::string searchUser = stringinputfilter("Enter username to delete (Enter 0 to cancel) : ");

	if (searchUser == "0") {
		std::cout << "Deletion cancelled.\n";
		return;
	}

	if (searchUser == currentUser.username) { //don't allow to delete logged in user
		std::cout << "Cannot delete logged in user.\n";
		return;
	}

	bool found = false;
	while (infile >> username >> password >> role) {
		if (username != searchUser) { //push all data accept the user that'll be deleted
			User u;
			u.username = username;
			u.password = password;
			u.role = role;
			userList.push_back(u);;
		}
		else {
			found = true;
		}
	}

	infile.close();

	if (found) { //rewrites userList into user.txt if got changes
		std::ofstream outfile("user.txt");
		for (User& u : userList) {
			outfile << u.username << " " << u.password << " " << u.role << "\n";
		}
		outfile.close();
		std::cout << "User deleted.\n";
	}
	else {
		std::cout << "User not found.\n";
	}
}

void displayUser() {
	std::ifstream infile("user.txt");
	std::string username, password; int role;

	std::cout << std::setfill(' ');
	std::cout << "\nChoose one option by typing number:\n"
		<< "1. Display all user\n"
		<< "2. Display all teacher\n"
		<< "3. Display all student\n";
	int displayChoice = 0;
	do {
		displayChoice = intgerinputfilter("Enter your choice(1-3): ");
		if (displayChoice == -2) { //if -2 is returned, it means an empty input was entered
			std::cout << "Input cannot be empty. Please enter a valid number.\n";
		}
		else if (displayChoice == -1 || !(displayChoice >= 1 && displayChoice <= 3)) { //if -1 is returned, it means a !int value is entered, also checks if displayChoice is 1,2,3
			std::cout << "Invalid input. Please try again!\n";
		}
		else {
			break;
		}
	} while (true);

	std::vector<User> allUsers;
	User temp;
	while (infile >> temp.username >> temp.password >> temp.role) { //repeats reading a line from user.txt and save into temp until it reaches the end
		allUsers.push_back(temp); //push data from temp into vector allUsers
	}
	for (int i = 0; i < allUsers.size() - 1; i++) { //bubble sort
		for (int j = 0; j < allUsers.size() - i - 1; j++) {
			std::string lowerCaseName1 = allUsers[j].username; //get the username on the current index
			std::string lowerCaseName2 = allUsers[j + 1].username; //get the username on the next index
			for (char& c : lowerCaseName1) { //convert current into lowercase char by char
				c = tolower(c);
			}
			for (char& c : lowerCaseName2) { //convert next into lowercase char by char
				c = tolower(c);
			}
			if (lowerCaseName1 > lowerCaseName2) { //comparison for string compares their ASCII values, if current's ASCII value is bigger than next's, then swap place
				temp = allUsers[j];
				allUsers[j] = allUsers[j + 1];
				allUsers[j + 1] = temp;
			}
		}
	}

	int totalUser = 0;
	std::cout << "\n- -------------------- - ------- -\n"
		<< "| Username             | Role    |\n"
		<< "- -------------------- - ------- -\n";

	switch (displayChoice) {
	case 1:
		for (const User& user : allUsers) { //display all user records
			std::cout << "| "
				<< std::left << std::setw(20) << user.username
				<< " | "
				<< (user.role == 1 ? "Teacher" : "Student")
				<< " |\n";
			totalUser++;
		}
		std::cout << "- -------------------- - ------- -\n"
			<< "                    Total Users: " << totalUser << std::endl;
		break;

	case 2:
		for (const User& user : allUsers) { //display teacher records
			if (user.role == 1) {
				std::cout << "| "
					<< std::left << std::setw(20) << user.username
					<< " | Teacher |\n";
				totalUser++;
			}
		}
		std::cout << "- -------------------- - ------- -\n"
			<< "                  Total teacher: " << totalUser << std::endl;
		break;

	case 3:
		for (const User& user : allUsers) { //display student records
			if (user.role == 0) {
				std::cout << "| "
					<< std::left << std::setw(20) << user.username
					<< " | Student |\n";
				totalUser++;
			}
		}
		std::cout << "- -------------------- - ------- -\n"
			<< "                  Total student: " << totalUser << std::endl;
		break;
	}
}

int login(User& currentUser, bool& loggedin, bool& runningUserManagement) {
	std::string inputU, inputP;
	inputU = stringinputfilter("Username (Enter 0 to cancel login) : ");

	// Check for cancellation
	if (inputU == "0") {
		std::cout << "Login cancelled.\n";
		return 1;
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

			std::cout << "Login successful!\n";
			loggedin = true;

			if (currentUser.role == 1) { // Teacher role
				std::cout << "Redirecting to admin menu...\n";
			}
			else if (currentUser.role == 0) { // Student role
				std::cout << "Redirecting to user menu...\n";
			}

			runningUserManagement = false;
			return 0;
		}
	}
	std::cout << "Login failed! Please try again.\n";
	return 2;
}

bool UserManagementModule(bool& loggedin, User& currentUser) {
	int choice = 0;
	bool runningUserManagement = true; //set it to keep running until true become false
	if (!loggedin) {
		while (runningUserManagement) {
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
				login(currentUser, loggedin, runningUserManagement);
				break;
			case 0:
				std::cout << "\nStopping the program. Bye!";
				return false;
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
		while (runningUserManagement) {
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
				runningUserManagement = false; // end loop
				std::cout << "Reverting back to Admin menu......\n\n";
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
void StudentPackageModule(User& currentUser, const std::vector<Course>& allCourse) {
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
			showAllCourses(allCourse);
			break;
		case 2:
			std::cout << "\n--- Add Course to Package ---\n";
			addCoursetoPackage(currentUser, allCourse);
			break;
		case 3:
			myCourseSummary(currentUser, allCourse);
			break;
		case 4:
			removeCoursefromPackage(currentUser, allCourse);
			break;

		case 5:
			modifyUserPackage(currentUser, allCourse);
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
void addCoursetoPackage(User& currentUser, const std::vector<Course>& allCourse) {
	int id = intgerinputfilter("Enter Course ID to add: "); // Ask user to input the course ID they want to add
	if (id == -1) {
		std::cout << "Invalid input. Please try again!\n";
		return;
	}

	if (id == -2) {
		std::cout << "Input cannot be empty. Please enter a valid number.\n";
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
		for (const auto& c : allCourse) {
			if (c.id == id) { // Compare with all course IDs one by one
				currentUser.mypackage.push_back(c); // If found, push it back into the user's package vector
				std::cout << "\n------------------\n";
				std::cout << "Added " << c.Name << " to package!\n";
				found = true;

				recordUserAction(id, "ADD", 1, currentUser.username); // Log the action into the 1D/2D arrays and txt file
				saveUserCourses(currentUser); // Save immediately to prevent data loss if the program closes halfway
				std::cout << "[DEBUG] Saved the data into the text file.\n";
			}
		}
		if (!found) {
			std::cout << "Course not found in the available courses.\n"; // Finished checking but ID doesn't exist
		}
	}
}

// 2. Display all courses chosen by the current user and calculate the total fee
void myCourseSummary(User& currentUser, const std::vector<Course>& allCourse) {
	if (currentUser.mypackage.empty()) {
		std::cout << "Your package is currently empty.\n"; // If the package is empty, notify the user
	}

	std::cout << "\n--- My Package Summary ---\n";
	double total = 0;
	for (const auto& c : currentUser.mypackage) { // Loop through every course in the user's package
		std::cout << "- " << c.Name << " ($" << std::fixed << std::setprecision(2) << c.price << ")\n";
		total += c.price; // Accumulate the total price
	}
	std::cout << "Total Fee: $" << std::fixed << std::setprecision(2) << total << "\n"; // Print final total fee
}

// 3. Remove a course from the user's package
void removeCoursefromPackage(User& currentUser, const std::vector<Course>& allCourse) {
	if (currentUser.mypackage.empty()) {
		std::cout << "Your package is currently empty.\n";
	}

	std::cout << "\n--- Remove Course from Package ---\n";
	for (const auto& c : currentUser.mypackage) { // First, list out what courses the user currently has
		std::cout << "(ID=" << c.id << ") - " << c.Name << " ($" << std::fixed << std::setprecision(2) << c.price << ")\n";
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
	for (auto it = currentUser.mypackage.begin(); it != currentUser.mypackage.end(); ++it) {
		if (it->id == id) { // Found the matching ID
			std::cout << "\nRemoved " << it->Name << " from package!\n";
			currentUser.mypackage.erase(it); // Erase it from the vector using an iterator

			recordUserAction(id, "REMOVE", 2, currentUser.username); // Log the remove action
			found = true;
			break;
		}
	}
	if (!found) {
		std::cout << "\nCourse ID entered is not found in your package.\n";
	}
}

// 4. Modify the user package (remove a course first, then add a new one)
void modifyUserPackage(User& currentUser, const std::vector<Course>& allCourse) {
	if (currentUser.mypackage.empty()) {
		std::cout << "Your package is currently empty.\n";
	}

	std::cout << "\n--- Modify User Package ---\n";
	std::cout << "Current Courses in Package:\n";
	for (const auto& c : currentUser.mypackage) {
		std::cout << c.id << ". " << c.Name << " - $" << std::fixed << std::setprecision(2) << c.price << "\n";
	}

	// Stage 1: Execute the "Remove" operation
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
		for (auto it = currentUser.mypackage.begin(); it != currentUser.mypackage.end(); ++it) {
			if (it->id == removeId) {
				std::cout << "Removed " << it->Name << " from package.\n";
				currentUser.mypackage.erase(it); // Successfully erased from package

				recordUserAction(removeId, "MODIFY_REMOVE", 3, currentUser.username); // Log the modify-remove action
				found = true;
				break;
			}
		}

		if (found) {
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
		for (const auto& c : currentUser.mypackage) {
			if (c.id == id) { alreadyExists = true; break; }
		}

		if (alreadyExists) {
			std::cout << "Error: Course (ID =" << id << ") already exists in your package\n";
			continue;
		}

		bool foundCourse = false;
		for (const auto& c : allCourse) {
			if (c.id == id) {
				currentUser.mypackage.push_back(c); // Push the new course in
				std::cout << "\n------------------\n";
				std::cout << "Added " << c.Name << " to package!\n";

				recordUserAction(id, "MODIFY_ADD", 3, currentUser.username); // Log the modify-add action
				saveUserCourses(currentUser); // Save to the text file instantly
				std::cout << "[DEBUG] Saved the data into the text file.\n";
				foundCourse = true;
				break;
			}
		}
		if (foundCourse) {
			break; // Break out of the entire modification loop once added successfully
		}

		if (!foundCourse) {
			std::cout << "Course not found in the available courses.\n";
			continue;
		}
	}
}

//6 show all courses
void showAllCourses(const std::vector<Course>& allCourse) {
	std::cout << "\n--- All Available Courses ---\n";
	for (const auto& c : allCourse) { /* c is temporary variable, autolet it auto detect the type of variable of c for allCourse that store in vector
												 const to avoid is telling the compiler to read the file only(no edit to the file)    & direct pass by reference ( no need to copy anymore so faster)*/
		std::cout << '\n';
		std::cout << "ID= " << c.id << ". " << c.Name << " - $" << c.price << "\n"; //since all the variable type is auto sync so can direct output easily
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
		std::cout << "Choose an option Choose one option by typing number:\n1. Generate Summary Report\n2. Generate Detailed Report\n3. Calculate Statistic\n4. Sort Record\n5. Display Analysis\n0. Back to Admin Menu\n";

		reportchoice = intgerinputfilter("Enter your choice(0-5): ");
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
			break;
		}

	}
}

//case 1: generate summary report
void generateSummaryReport() {
	std::vector<User> allusers;
	readUserandCoursefile(allusers);   //load the users files from the header、

	size_t totalusers = allusers.size();   //size() tells compiler to go and count how many users they are in the allusers
	if (totalusers == 0) {
		std::cout << "System ERROR. No user record found\n";
		return;
	}
	size_t totalenrollments = 0;    //total amount of course taken by student
	int zeroCourseStudent = 0;   //student who doesn't has any course taken

	for (const auto user : allusers) {   //read the current user from the text file 
		size_t studentCourseCount = user.mypackage.size();  //count the user course taken and save into the studentCourseCount
		totalenrollments += studentCourseCount;    //totalenrollments = totalenrollments + studentCourseCount
		if (studentCourseCount == 0) {   //if compiler found out that this user has zero course, then the user will be save into here
			zeroCourseStudent++;
		}
	}

	//summary report menu
	//I'm not using setw and setfill due to the weird mechanic from it when I output my menu, so I will just use std::string to act as setw and setfill
	std::cout << '\n' << std::string(78, '=') << "\n";
	std::cout << std::string(30, ' ') << "Summary Report\n";
	std::cout << std::string(78, '=') << '\n';
	std::cout << "Total student registered: " << std::string(42, '.') << totalusers << " students\n";
	std::cout << "Total course registered: " << std::string(43, '.') << totalenrollments << " courses\n";
	if (zeroCourseStudent > 0) {
		std::cout << "[WARNING] Currently there are " << zeroCourseStudent << " students that doesn't registered any courses!\n";
	}
	std::cout << std::string(78, '=') << "\n";
}

//case 2: generate detail report
void generateDetailReport() {
	std::vector<User> allusers;   //declare the allusers
	readUserandCoursefile(allusers);    //read file from header

	if (allusers.size() == 0) {
		std::cout << "System ERROR. No user record found.\n";  //error message if there is no txt file
		return;
	}

	std::string searchUser;
	searchUser = stringinputfilter("Enter username to search: ");   //input filter
	bool founduser = false;

	for (const auto& user : allusers) {
		if (user.username == searchUser) {
			founduser = true;

			//detail report menu
			if (user.mypackage.empty()) {   //if the course txt file is empty then give error message
				std::cout << "\nERROR. This student has not registered any course.\n";  //when they is no course recorded in this user, return an error message
			}
			else {
				//detail report menu
				std::cout << '\n' << std::string(78, '=') << "\n";
				std::cout << std::string(30, ' ') << "Detailed Report\n";
				std::cout << std::string(78, '=') << '\n';
				std::cout << "Name: " << user.username << '\n';
				std::cout << std::string(78, '=') << '\n';
				std::cout << "[Class Enrolled]\n";
				std::cout << "Total course taken: " << user.mypackage.size() << '\n';
				std::cout << std::string(78, '-') << '\n';
				std::cout << std::setfill(' ');
				std::cout << std::left << std::setw(14) << "Course ID" << std::left << std::setw(46) << "Course Name" << std::right << std::setw(14) << "Price (RM)\n";
				std::cout << std::string(78, '-') << '\n';

				//a loop for the compiler to load the specific user's course one by one from the course.txt file
				for (size_t j = 0; j < user.mypackage.size(); ++j) {
					std::cout << " " << std::left << std::setw(13) << user.mypackage[j].id << std::left << std::setw(46) << user.mypackage[j].Name << std::right << std::setw(6) << "RM " << std::fixed << std::setprecision(2) << user.mypackage[j].price << '\n';
				}
				std::cout << std::string(78, '-') << '\n';
			}
		}
	}
	if (!founduser) {
		std::cout << "User not found!";  //if the user not found, return error message and end loop
		founduser = false;
	}
}

//case 3 
void calculateStatistic() {
	std::vector<User> allusers;
	readUserandCoursefile(allusers);

	size_t totalusers = allusers.size();   //size() tells compiler to go and count how many users they are in the allusers
	if (totalusers == 0) {
		std::cout << "System ERROR. No user record found.\n";
		return;
	}

	size_t totalenrollment = 0;
	int inactivestudent = 0;

	for (const auto user : allusers) {   //take the data from the txt file and put in here to calculate
		size_t courseCount = user.mypackage.size();
		totalenrollment += courseCount;   //totalenrollment = totalenrollment + coursecount, when coursecount = 1, then total is 1, if another user have 2, then is 1+2=3 for total enrollment
		if (courseCount == 0) {
			inactivestudent++;  //if the user didnt register any course, it will direct to here
		}
	}
	double averageCourseperStudent = static_cast<double>(totalenrollment) / totalusers;   //static_cast is used to get even accurate 小数点
	double inactiveStudentRate = static_cast<double>(inactivestudent) / totalusers * 100;  //make it into percentage

	//calculate statistic menu
	std::cout << '\n' << std::string(78, '=') << '\n';
	std::cout << std::string(30, ' ') << "Calculate Statistic\n";
	std::cout << std::string(78, '=') << '\n';
	std::cout << "Academic Metrics" << std::string(40, ' ') << "Current Data\n";
	std::cout << std::string(78, '-') << '\n';
	std::cout << "Total Students: " << std::string(40, ' ') << totalusers << " students";
	std::cout << "\nTotal Enrollments: " << std::string(37, ' ') << totalenrollment << " students";
	std::cout << "\nAverage Courses Taken per Student: " << std::string(21, ' ') << std::fixed << std::setprecision(1) << averageCourseperStudent << " per students";
	std::cout << "\nTotal Inactive Students: " << std::string(31, ' ') << inactivestudent << " students";
	std::cout << "\nTotal Inactive Students Rate: " << std::string(26, ' ') << std::fixed << std::setprecision(1) << inactiveStudentRate << " %\n";
	std::cout << std::string(78, '-') << '\n';
}

//case 4: sort students'name 
void sortRecord() {
	std::vector<User> allusers;
	readUserandCoursefile(allusers);  //load the files from the header

	//check if the txt is create
	if (allusers.size() == 0) {
		std::cout << "System ERROR. No user record found.\n";
		return;
	}

	//selection sort
	size_t n = allusers.size();
	//outer loop: to decide who should be the first, second, third... etc
	for (size_t i = 0; i < n - 1; i++) {
		size_t min_idx = i; //it means that this is the first person who start from alphabet A, then it will put in the first place
		//inner loop: continue to find other name that is more forward than the previous i 
		for (size_t j = i + 1; j < n; j++) {
			if (allusers[j].username < allusers[min_idx].username) {
				min_idx = j;    //record the person who has closer alphabet
			}
		}
		if (min_idx != i) {
			std::swap(allusers[i], allusers[min_idx]); //if the smaller number is not i, then swap place
		}
	}

	//the sort menu
	std::cout << '\n' << std::string(78, '=') << '\n';
	std::cout << std::string(30, ' ') << "Sort Menu By Username\n";
	std::cout << std::string(78, '=') << '\n';
	std::cout << "No.\tStudent\t\tCourse Taken\n";
	std::cout << std::string(78, '-') << '\n';

	for (size_t i = 0; i < n; i++) {  //compiler will always start from 0,1,2... This command can force the compiler to start from 1,2,3
		std::cout << " " << (i + 1) << "\t" << allusers[i].username << "\t\t" << allusers[i].mypackage.size() << " courses" << '\n';
		std::cout << std::string(78, '-') << '\n';
	}
}

//case 5: display analysis
void displayAnalysis() {
	std::vector<User> allusers;
	readUserandCoursefile(allusers);

	size_t totalusers = allusers.size();   //size() tells compiler to go and count how many users they are in the allusers
	if (totalusers == 0) {
		std::cout << "System ERROR. No user record found.\n";
		return;
	}

	//menu
	std::cout << '\n' << std::string(80, '=') << '\n';
	std::cout << std::string(26, ' ') << "Tuition Centre Analysis\n";
	std::cout << std::string(80, '=') << '\n';
	std::cout << "Diagnostic Indicator 1: Zero Course Enrollment Attrtion Risk\n";
	bool hasriskstudent = false;

	for (const auto& user : allusers) {
		if (user.mypackage.empty()) {   //if course txt is empty for this user, output the warning message
			std::cout << "[WARNING] Student " << user.username << " currently have 0 courses registered!\n";
			hasriskstudent = true;
		}
	}
	if (hasriskstudent) {  //give suggestion for the current user if its true
		std::cout << "\nSYSTEM RECOMMENDATION: Front-desk course consultants should call the\n";
		std::cout << "student's parents immediately to assist in completing course enrollment.\n";
	}
	else { //if its false
		std::cout << "Good! All students have successfully registered their courses, zero loss risk.\n";
	}
	std::cout << std::string(80, '-') << '\n';

	std::cout << "Diagnostic Indicator 2: Academic Pressure for elite students\n";
	bool overloadstudent = false;

	for (const auto& user : allusers) {
		if (user.mypackage.size() >= 3) {  //if the course registered by the current user is more than equal to 3, give warning message
			std::cout << "[WARNING] Student " << overloadstudent << " has more or equal to 3 courses registered!\n";
			overloadstudent = true;
		}
	}
	if (overloadstudent) {  //if its true, give suggestions
		std::cout << "\nSYSTEM RECOMMENDATION: Instructors of each subject should pay closer attention\n";
		std::cout << "to the follow up of the above student to prevent excessive academic pressure.\n";
	}
	else {  //if its false
		std::cout << "Status Normal : No student is currently enrolled in more than three courses; the academic workload is balanced.\n";
	}
	std::cout << std::string(80, '-') << '\n';
}
//-------------------------------------------------------------------------------------------------------------------------