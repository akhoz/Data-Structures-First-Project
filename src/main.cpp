#include <Category.hh>
#include <Product.hh>
#include <Resource.hh>
#include <Staff.hh>
#include <Stage.hh>
#include <Workday.hh>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>
#include <string>

using std::string;
using std::tm;

// Create a pointer to the staff roster linked list, set it to null
Staff *staff_roster = nullptr;

// Create a pointer to the inventory linked list, set it to null
Category *inventory = nullptr;

bool exit_menu = false;

tm parse_hour(const string& hour) {
    tm time = {};
    strptime(hour.c_str(), "%H:%M", &time);
    return time;
}

//This is a template function, it can take any type of data, this is used avoid code duplication, such as having to write the same function for different structs or linked lists 
template <typename T>
void lineal_append_end(T*& linked_list, T*& element_to_append) {
	//This function appends an element to the end of a lineal linked list
	//It receives the linked list and the element to append 
	//Returns nothing

	if (!linked_list) {
		linked_list = element_to_append; //If the linked list is empty, the linked list is the element to append
		return; //Returns nothing
	}

	T* current = linked_list;
	while (current->next) {
		current = current->next;
	}
	current->next = element_to_append; //The next element of the linked list is the element to append
}

template <typename T>
void double_append_end(T*& linked_list, T*& element_to_append) {
	if (!element_to_append) { //If the element to append is null
		return;
	}

	if (!linked_list) { //If the linked list is empty
		linked_list = element_to_append; //Set the new element as the first node of the linked list
		return; //Return nothing
	}

	T* current = linked_list; //Create a pointer to the current node
	while (current->next) { //While the current node is not null
		current = current->next; //Move to the next node
	}
	current->next = element_to_append; //Set the next node to the new element
	element_to_append->prev = current; //Set the previous node of the new element to the current node
	if (!linked_list) {
		linked_list = element_to_append;
		return;
	}
}

template <typename T>
void double_circular_append_end(T*& linked_list, T*& element_to_append) {
	//This function appends an element to the end of a double circular linked list
	//Receives the linked list and the element to append
	//Returns nothing

	if (!linked_list) { //If the linked list is empty
		linked_list = element_to_append; //The linked list is the element to append
		linked_list->next = linked_list; //The next element of the linked list is itself
		linked_list->prev = linked_list; //The previous element of the linked list is itself
		return; //Returns nothing
	}

	T* current = linked_list;

    //While the next element of the current element is not the first element of the linked list
	while (current->next != linked_list) { 
       		current = current->next;
	}
	current->next = element_to_append; //The next element of the current element is the element to append
	element_to_append->prev = current; //The previous element of the element to append is the current element
	element_to_append->next = linked_list; //The next element of the element to append is the first element of the linked list
	linked_list->prev = element_to_append; //The previous element of the first element of the linked list is the element to append
}

template <typename T>
void print_linked_list(T*& linked_list) {
	//This is a function that prints the elements of a linked list
	//It receives a pointer to the linked list
	//Returns nothing

	T* current = linked_list; //Create a pointer to the current node
	while (current) { //While the current node is not null, print the data of the current node and move to the next node
        /* std::cout << *current << std::endl; */ // Error with this one
        std::cout << current << std::endl;
        current = current->next;
    }
}

void assign_boss(string new_staff_id) {
	//This is a function that assigns a boss to an employee
	//It receives the id of the employee
	//Returns nothing

	string boss_id;
	std::cout << "Boss id: ";
	std::cin >> boss_id;

	if (boss_id.empty()) //If the boss id is not empty, exit the function
		return;

	Staff* boss = nullptr; //Create a pointer that later will point to the boss of the employee
	Staff* current = staff_roster; //Create a pointer to the current node
	while (current) { //While the current node is not null, search for the boss of the employee
		if (current->id == boss_id) {
			boss = current; //Set the boss pointer to the current node
			break;
		}
		current = current->next;
	}

	current = staff_roster;
	while (current) { //While the current node is not null, search for the employee
		if (current->id == new_staff_id) {
			current->boss = boss; //Set the boss of the employee to the boss pointer
			return;
		}
		current = current->next;
	}
}

void add_staff() {
	//This is a function that adds a new employee to the staff roster, it requests the information of the employee
	//Recieves nothing
	//Returns nothing

	//Here is where the information is requested
	Staff* new_staff = new Staff(); //Create a pointer to the new employee
	std::cout << "Please fill the following information: " << std::endl;
	std::cout << "Id: ";
	std::cin >> new_staff->id;
	std::cout << "Job title: ";
	std::cin >> new_staff->job_title;
	std::cout << "Salary bonuses: ";
	std::cin >> new_staff->salary_bonuses;
	std::cout << "Job performed: ";
	std::cin >> new_staff->job_performed;
	std::cout << "Employment sector: ";
	std::cin >> new_staff->employment_sector;

	double_append_end(staff_roster, new_staff); //Append the new employee to the staff roster

	if (new_staff->job_title != "boss") //If the employee is not the boss, assign a boss to the employee
		assign_boss(new_staff->id); //Assign the boss of the employee

	std::cout << "Employee added successfully" << std::endl;
}

void remove_staff() {
	//This is a function that removes an employee from the staff roster
	//Recieves nothing
	//Returns nothing

	string staff_id;
	std::cout << "Type the id of the employee you want to remove: ";
	std::cin >> staff_id;

	Staff* current = staff_roster; //Create a pointer to the current node
	while (current) { //While the current node is not null, search for the employee
		if (current->id == staff_id) { //If the employee is found, remove it from the staff roster
			if (current->prev) {
				current->prev->next = current->next; //The previous node of the current node points to the next node of the current node
			}
			if (current->next) {
				current->next->prev = current->prev; //The next node of the current node points to the previous node of the current node
			}
			if (current == staff_roster) {
				staff_roster = current->next; //If the current node is the first node, set the staff roster to the next node of the current node
			}
			delete current; //Delete the current node in order to free memory
			std::cout << "Employee removed successfully" << std::endl;
			return;
		}
		current = current->next;
	}
}
void modify_staff(string staff_id) {
	//This function modifies the information of an employee
	//Recieves the id of the employee
	//Returns nothing

	string property_to_change;
	std::cout << "Type the property you want to change: ";
	std::cin >> property_to_change;

	Staff* current = staff_roster; //Create a pointer to the current node
	Staff* staff_to_modify = nullptr; //Create a pointer to the employee to modify, set it to null

	while (current) {
		if (current->id == staff_id) { //If the employee is found, set the staff to modify pointer to the current node
			staff_to_modify = current; //Set the staff to modify pointer to the current node
			break;
		}
		current = current->next;
	}

	if (!staff_to_modify) { //If the employee is not found, exit the function
		std::cout << "No employee found with that id" << std::endl;
		return;
	}

	//Case comparison, this is to determine which property of the employee to change
	if (property_to_change == "id") { //If the property to change is the id, change the id of the employee
		std::cout << "New id: ";
		std::cin >> staff_to_modify->id;
	}
	else if (property_to_change == "job_title") { //If the property to change is the job title, change the job title of the employee
		std::cout << "New job title: ";
		std::cin >> staff_to_modify->job_title;
	}
	else if (property_to_change == "salary_bonuses") { //If the property to change is the salary bonuses, change the salary bonuses of the employee
		std::cout << "New salary bonuses: ";
		std::cin >> staff_to_modify->salary_bonuses;
	}
	else if (property_to_change == "job_performed") { //If the property to change is the job performed, change the job performed of the employee
		std::cout << "New job performed: ";
		std::cin >> staff_to_modify->job_performed;
	}
	else if (property_to_change == "employment_sector") { //If the property to change is the employment sector, change the employment sector of the employee
		std::cout << "New employment sector: ";
		std::cin >> staff_to_modify->employment_sector;
	}
	else if (property_to_change == "boss") { //If the property to change is the boss, change the boss of the employee
		assign_boss(staff_to_modify->id);
	}
	else { //If the property to change is not found, exit the function
		std::cout << "Property not found" << std::endl;
		return;
	}
}

Staff* find_staff_by_id(const string& id) {
    Staff* current = staff_roster;
    while (current) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void print_workday(Staff*& staff) {
    Workday* current = staff->workdays;
    while (current) {
        std::cout << current << std::endl;
        current = current->next;
    }
}


int worked_hours(const string& start, const string& end) {
	tm parsed_start = parse_hour(start);
	tm parsed_end = parse_hour(end);

	time_t t_inicio = mktime(&parsed_start);
	time_t t_fin = mktime(&parsed_end);

	int seconds = difftime(t_fin, t_inicio);
	int hours = seconds / 3600;
	return hours;

}

void registerHours(Staff*& staff) {
	Workday* workday = new Workday();
	string weeknd;
	string holiday;
	std::cout << "Enter the start hour: \n";
	std::cin >> workday->start_time;
	std::cout << "Enter the end hour: \n";
	std::cin >> workday->end_time;
	std::cout << "Is it a weekend? (y/n): \n";
	std::cin >> weeknd;
	std::cout << "Is it a holiday? (y/n): \n";
	std::cin >> holiday;
	if (weeknd == "y") {
		workday->is_weekend = true;
	}
	else {
		workday->is_weekend = false;
	}
	if (holiday == "y") {
		workday->is_holiday = true;
	}
	else {
		workday->is_holiday = false;
	}
	workday->worked_hours = worked_hours(workday->start_time, workday->end_time);
	// Add the workday to the staff workdays list
	if (staff->workdays == nullptr) {
		staff->workdays = workday;
	}
	else {
		Workday* current = staff->workdays;
		while (current->next) {
			current = current->next;
		}
		current->next = workday;
	}
}

void set_salary(Staff*& person) {
	int user_worked_hours = 0;
	Workday* current = person->workdays;
	if (current == nullptr) {
		std::cout << "The user has not worked yet" << std::endl;
		return;
	}
	while (current) {
		if (current->is_weekend == true) {
			user_worked_hours += current->worked_hours * 2;
		}
		else if (current->is_holiday == true) {
			user_worked_hours += current->worked_hours * 3;
		}
		else {
			user_worked_hours += current->worked_hours;
		}
		current = current->next;
	}
	person->salary = user_worked_hours * 100;
    std::cout << "The salary of the user is: " << person->salary << std::endl;
}


void register_hours_menu(Staff*& staff) {
    std::cout << "Register hours menu" << std::endl;
    std::cout << "Type your id: ";
    string id;
    std::cin >> id;
    Staff* staff_member = find_staff_by_id(id);
    if (!staff_member) {
        std::cout << "Staff member not found" << std::endl;
        return;
    }
    registerHours(staff_member);
}

void consult_salary(Staff*& List) {
    std::cout << "Consult salary menu" << std::endl;
    std::cout << "Type your id: ";
    string id;
    std::cin >> id;
    Staff* staff_member = find_staff_by_id(id);
    if (!staff_member) {
        std::cout << "Staff member not found" << std::endl;
        return;
    }
    set_salary(staff_member);
}

void stock_inquiry(Category*& selected_category) {
	//This is a function that prints the products of a category
	//It receives a pointer to the category
	//Returns nothing

	Category* current = inventory; //Create a pointer to the current node
	while (current) {
		if (current->category_name == selected_category->category_name) { //If the current node is the selected category, print the products of the category
			print_linked_list(current->products);
			return; //Return nothing
		}
		current = current->next;
	}
}

void state_inquiry(string serial_number){
    Category* current = inventory;
    while(current){
        if(current->products->serial_number == serial_number){
            break;
        }
        Stage* current_stage = current->products->current_stage;
        while (current_stage->completed) {
            current_stage = current_stage->next;
        }
        std::cout << "The product is in the " << current_stage->stage_name << " stage" << std::endl; //Print the current stage of the product
    }
}
//------> Evaluate if this functions will stay or we will replace the template conversor <------

void append_category(Category*& category_to_append) {
    //This is a function that appends a category to the inventory
    //It receives a pointer to the category to append
    //Returns nothing

    if (!inventory) { //If the inventory is empty, the inventory is the category to append
        inventory = category_to_append;
        return; //Return nothing
    }

    Category* current = inventory; //Create a pointer to the current node
    while (current->next) { //While the current node is not null, move to the next node
        current = current->next;
    }
    current->next = category_to_append; //The next node of the current node is the category to append
}

void append_product_to_category(Category*& category, Product*& product) {
    //This is a function that appends a product to a category
    //It receives a pointer to the category and a pointer to the product
    //Returns nothing

    if (!category->products) { //If the products of the category is empty, the products of the category is the product
        category->products = product;
        return; //Return nothing
    }

    Product* current = category->products; //Create a pointer to the current node
    while (current->next) { //While the current node is not null, move to the next node
        current = current->next;
    }
    current->next = product; //The next node of the current node is the product
}

/* void append_re */

//------> Evaluate if this functions will stay or we will replace the template conversor <------
void complete_category(const string name){
    // This function can be factored into several smaller functions 
    Category *new_category = new Category();
    Product *new_product = new Product();
    Stage *new_stage = new Stage();
    Resource *new_resource = new Resource();
    bool completed = false;

    std::cout << "Type the product you want to register: " << std::endl;
    std::cin >> new_product->name;
    std::cout << "Type the serial number of the product: " << std::endl;
    std::cin >> new_product->serial_number;
    std::cout << "Type the name of the stage: " << std::endl;
    std::cin >> new_stage->stage_name;
    std::cout << "Type if the stage is completed (true/false): " << std::endl;
    std::cin >> completed;
    std::cout << "Type the name of the resource needed to build the product" << std::endl;
    std::cin >> new_resource->name;
    // Add the Resource to the stage
    double_append_end(new_stage->resources, new_resource);
    // Add the stage to the product
    new_product->current_stage = new_stage;
    // Add the product to the category
    append_product_to_category(new_category, new_product);
    // Add the category to the inventory
    append_category(new_category);


}

void menu(Staff*& List){
    int option;
    std::cout << "Welcome to the Staff menu" << std::endl;
    std::cout << "1. Register user" << std::endl;
    std::cout << "2. Register Category" << std::endl;
    std::cout << "3. Register Product" << std::endl;
    std::cout << "4. Register user worked hours" << std::endl;
    std::cout << "5. Consult salary" << std::endl;
    std::cout << "6. Prints" << std::endl;
    std::cout << "10. Exit" << std::endl;
    std::cin >> option;

    switch (option) {
        case 1:
            add_staff();
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            register_hours_menu(List);
            /* system("clear"); */
            break;
        case 5:
            consult_salary(List);
            break;
        case 6:
            print_linked_list(List);
            /* system("clear"); */
            break;
        case 10:
            exit_menu = true;
            break;
        default:
            std::cout << "Invalid option" << std::endl;
            break;
    }

}

int main(){
	Staff* first_staff = new Staff("1", "boss", "experience", "managment", "administration"); //Create the first employee, the boss
	Staff* second_staff = new Staff("2", "employee", "experience", "production", "production"); //Create the second employee, the employee
	Staff* third_staff = new Staff("3", "boss", "experience", "managment", "administration"); //Create the first employee, the boss
	Staff* fourth_staff = new Staff("4", "intern", "experience", "managment", "maintenance"); //Create the first employee, the boss
	Staff* fifth_staff = new Staff("5", "boss", "experience", "managment", "production"); //Create the first employee, the boss
    double_append_end(staff_roster,first_staff);
    double_append_end(staff_roster,second_staff);
    double_append_end(staff_roster,third_staff);
    double_append_end(staff_roster,fourth_staff);
    double_append_end(staff_roster,fifth_staff);
    while (!exit_menu) {
        menu(staff_roster);
    }
    
    return 0;
}
