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
			continue;
		}

		if (newUser.role == -2) {
			std::cout << "Input cannot be empty. Please enter a valid number.\n";
			continue;
		}

		if (newUser.role == 0 || newUser.role == 1) {
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

int login(User& currentUser) { //0 if success, 1 if cancel, 2 if fail
	std::string inputU, inputP;
	inputU = stringinputfilter("Username (Enter 0 to cancel login) : ");

	if (inputU == "0") {
		return 1;
	}

	inputP = stringinputfilter("Password: ");

	std::ifstream inFile("user.txt"); //ifstream means read the file
	std::string fileU, fileP;
	int fileR;

	//if input username = username in txt file then send true as output, if no then false, password also
	while (inFile >> fileU >> fileP >> fileR) {
		std::cout << "[DEBUG] Comparing " << inputU << " with " << fileU << "\n"; //temporary for me to debug, i just leave it here until when we need to delete it :D

		if (fileU == inputU && fileP == inputP) {
			currentUser.username = inputU; //if entered username and password is both found from the text file and it is correct
			currentUser.role = fileR;  // assign username and role into the user structure that create on main file
			return 0;
		}

	}
	return 2;
}

bool UserManagementModule(bool& loggedin, User& currentUser) {
	int choice = 0;
	int status = 0;
	if (!loggedin) {
		bool runningUserManagement = true; //set it to keep running until true become false
		while (runningUserManagement) {
			std::cout
				<< "\n--- User Management Module --\n"
				<< "======================================\n"
				<< "1. Register\n2. Login\n0. Exit\n"
				<< "======================================\n"
				<< "Please choose one option by typing the number\n";

			choice = intgerinputfilter("Enter your choice: ");//call the input filter function to get the input and check if it is valid
			if (choice == -1) {
				std::cout << "Invalid input! Please enter a valid number\n";
				continue;
			}
			else if (choice == -2) {
				std::cout << "Input cannot be empty! Please enter a valid number\n";
				continue;
			}

			//switch here get result from the choice and entering it to case for different result
			switch (choice) {
			case 1:
				registerUser(currentUser); //take result from auth.cpp and continue
				break; //break means end this case and go back to the choice section
			case 2:
				status = login(currentUser); //take result from auth.cpp and continue
				if (status == 0) { //call login
					std::cout << "Login successful!\n";
					loggedin = true;
					return true;

					if (currentUser.role == 1) { //for admin role
						std::cout << "Redirecting to admin menu...\n";
					}
					else if (currentUser.role == 0) { //for student role
						std::cout << "Redirecting to user menu...\n";
					}
					runningUserManagement = false;
					return true;
				}
				else if (status == 1) {
					std::cout << "Login cancelled.\n";
				}
				else {
					std::cout << "Login failed! Please try again.\n";
				}
				break;
			case 0:
				std::cout << "\nStoping the program. Bye!";
				return false;
				break;
			default:
				std::cout << "Invalid input! Please enter a valid number,\n";
				break;
			}
		}
	}
	else {
		bool runningUserManagement = true; //set it to keep running until true become false
		while (runningUserManagement) {
			std::cout << "\n=============================\n";
			std::cout << "| Admin Portal: User module |\n";
			std::cout << "=============================\n\n";

			std::cout << "Choose one option by typing number:\n"
				"1. Add new record\n"
				"2. Update record\n"
				"3. Delete record\n"
				"4. Search record\n"
				"5. Display records\n"
				"0. Back to Admin Menu\n";
			std::cout << std::setfill('=') << std::setw(50) << "" << '\n';

			choice = intgerinputfilter("Enter your choice(0-5): ");

			switch (choice) {
			case 1:
				//addNewUser();
				break;
			case 2:
				//updateUser(currentUser);
				break;
			case 3:
				//deleteUser(currentUser);
				break;
			case 4:
				//searchUser();
				break;
			case 5:
				//displayUser();
				break;
			case 0:
				runningUserManagement = false;
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

		if (subChoice == -1) {
			std::cout << "Invalid input. Please try again!\n";
			continue;
		}

		if (subChoice == -2) {
			std::cout << "Input cannot be empty. Please enter a valid number.\n";
			continue;
		}

		if (subChoice == 1) {
			for (const auto& c : allCourse) { /* c is temporary variable, autolet it auto detect the type of variable of c for allCourse that store in vector
												 const to avoid is telling the compiler to read the file only(no edit to the file)    & direct pass by reference ( no need to copy anymore so faster)*/
				std::cout << '\n';
				std::cout << "ID= " << c.id << ". " << c.Name << " - $" << c.price << "\n"; //since all the variable type is auto sync so can direct output easily
			}
		}
		else if (subChoice == 2) {
			std::cout << "\n--- Add Course to Package ---\n";
			addCoursetoPackage(currentUser, allCourse);
		}

		else if (subChoice == 3) {
			myCourseSummary(currentUser, allCourse);
		}

		else if (subChoice == 4) {
			removeCoursefromPackage(currentUser, allCourse);
		}

		else if (subChoice == 5) {
			modifyUserPackage(currentUser, allCourse);
		}

		else if (subChoice == 6) {
			searchBooking();
		}

		else if (subChoice == 0) {
			saveUserCourses(currentUser); //back up saving, save one more time to avoid accident
			running = false;    //end the loop by changing the bool variable on the top of this page
			std::cout << "Redirecting back to User Menu...\n";
			break;
		}
		else {
			std::cout << "Invalid option, try again.\n";
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
//-------------------------------------------------------------------------------------------------------------------------

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
				std::cout
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
				int subchoice;
				if (!(std::cin >> subchoice)) {
					std::cin.clear();
					std::cin.ignore(100, '\n');
					std::cout << "Invalid input! Please enter a number.\n";
					continue;
				}
				std::cin.ignore(100, '\n');
				switch (subchoice) {
				case 1:
					std::cout << "\n[!] Opening User Module dashboard......\n";//load the user module
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
				std::cout
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
				int subchoice;
				if (!(std::cin >> subchoice)) {
					std::cin.clear();
					std::cin.ignore(100, '\n');
					std::cout << "Invalid input! Please enter a number.\n";
					continue;
				}
				std::cin.ignore(100, '\n');

				switch (subchoice) {
				case 1:
					std::cout << "\n[!] Opening Student Package Module dashboard......\n";
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