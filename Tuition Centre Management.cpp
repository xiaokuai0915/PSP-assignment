#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
// MODELS
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
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------

int main() {

}

void UserManagement() {
	User currentUser; //create a user based on the struct declared on models.h


	int choice;
	bool running = true; //set it to keep running until true become false
	int status;
	while (running) {
		std::cout << "\n--- Welcome System --\n";
		std::cout << "======================================\n1. Register\n2. Login\n0. Exit\n======================================\nPlease choose one option by typing the number\n";
		choice = intgerinputfilter("Enter your choice: ");//call the input filter function to get the input and check if it is valid
		if (choice == -1) {
			std::cout << "Invalid input! Please enter a valid number\n";
			continue;
		}
		if (choice == -2) {
			std::cout << "Input cannot be empty! Please enter a valid number\n";
			continue;
		}

		//switch here get result from the choice and entering it to case for different result
		switch (choice) {
		case 1:
			registerUser(); //take result from auth.cpp and continue
			break; //break means end this case and go back to the choice section
		case 2:
			status = login(currentUser); //take result from auth.cpp and continue
			if (status == 0) { //call login
				std::cout << "Login successful!\n";

				if (currentUser.role == 1) { //for admin role
					std::cout << "Redirecting to admin menu...\n";
					showAdminMenu(currentUser);
				}
				else if (currentUser.role == 0) { //for student role
					std::cout << "Redirecting to user menu...\n";
					loadUserCourses(currentUser); //load user profile from storage.cpp
					loadActionLogs(); //load action logs from storage.cpp
					showUserMenu(currentUser);
				}
			}
			if (status == 1) {
				std::cout << "Login cancelled.\n";
				break;
			}
			else {
				std::cout << "Login failed! Please try again.\n";
			}
			break;
		case 0:
			running = false; //change the running status to false, so it wont continue run
			std::cout << "\nStoping the program. Bye!";
			break;
		default:
			std::cout << "Invalid input! Please enter a valid number,\n";
			break;
		}
	}
}
//USER MANAGEMENT
void registerUser() {
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

	std::ifstream inFile("Data/user.txt"); //ifstream means read the file
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
//----------------------------------------------------------------------