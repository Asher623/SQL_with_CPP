
#include <iostream>
#include <occi.h>
#include <iomanip>

using oracle::occi::Environment;
using oracle::occi::Connection;

using namespace oracle::occi;
using namespace std;
using namespace std;

Environment* env = nullptr;
Connection* conn = nullptr;
Statement* stmt = nullptr;
ResultSet* rs = nullptr;

string str;
string user = "";
string pass = "";
string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

struct Employee {
	int employeeNumber;
	string lastName;
	string firstName;
	string email;
	string phone;
	string extension;
	string reportsTo;
	string jobTitle;
	string city;

};
int findEmployee(Connection* conn, int employeeNumber, Employee* emp);

void deleteEmployee(Connection* conn, int employeeNumber) {

	stmt = conn->createStatement();
	stmt->setSQL("DELETE from employees WHERE EMPLOYEENUMBER = :1");
	stmt->setInt(1, employeeNumber);
	stmt->executeUpdate();
}
void updateEmployee(Connection* conn, int employeeNumber) {

	stmt = conn->createStatement();
	stmt->setSQL("UPDATE employees SET EXTENSION = :1 WHERE EMPLOYEENUMBER = :2");
	string ext;
	cout << "New extension: ";
	cin >> ext;
	stmt->setString(1, ext);
	stmt->setInt(2, employeeNumber);
	stmt->executeUpdate();
}

void insertEmployee(Connection* conn, struct Employee* emp) {
	stmt = conn->createStatement();
	stmt->setSQL("INSERT INTO employees values (:1,:2,:3,:4,:5,:6,:7,:8)");
	stmt->setInt(1, emp->employeeNumber);
	stmt->setString(2, emp->lastName);
	stmt->setString(3, emp->firstName);
	stmt->setString(4, emp->extension);
	stmt->setString(5, emp->email);
	stmt->setString(6, "1");
	stmt->setInt(7, 1002);
	stmt->setString(8, emp->jobTitle);
	stmt->executeUpdate();
	cout << "The new employee added successfully" << endl;


}

void displayEmployee(Connection* conn, struct Employee* emp) {
	cout << endl << "employeeNumber = " << emp->employeeNumber << endl;
	cout << "lastName = " << emp->lastName << endl;
	cout << "firstName = "<< emp->firstName << endl;
	cout << "email = " << emp->email << endl;
	cout << "phone = " << emp->phone << endl;
	cout << "extention = " << emp->extension << endl;
	cout << "reportsTo = " << emp->reportsTo << endl;
	cout << "jobTitle = " << emp->jobTitle << endl;
	cout << "city = " << emp->city << endl;
}
int findEmployee(Connection *conn, int employeeNumber, Employee* emp) {
	int check = 0;
	stmt = conn->createStatement();
	stmt->setSQL("SELECT E1.EMPLOYEENUMBER, E1.FIRSTNAME, E1.LASTNAME ,E1.EMAIL,Phone, E1.EXTENSION,E2.FIRSTNAME || ' ' || E2.LASTNAME, E1.jobtitle, city  FROM EMPLOYEES E1 JOIN OFFICES USING(officecode) JOIN EMPLOYEES E2 ON E1.REPORTSTO = E2.EMPLOYEENUMBER where E1.EMPLOYEENUMBER = :1");
	stmt->setInt(1, employeeNumber);
	rs = stmt->executeQuery();
	if (!rs->next())
	{

	}
	else
	{
		do {
			check = 1;
			emp->employeeNumber = rs->getInt(1);
			emp->lastName = rs->getString(2);
			emp->firstName = rs->getString(3);
			emp->email = rs->getString(4);
			emp->phone = rs->getString(5);
			emp->extension = rs->getString(6);
			emp->reportsTo = rs->getString(7);
			emp->jobTitle = rs->getString(8);
			emp->city = rs->getString(9);

		} while (rs->next());
	}
	return check;
}

void displayAllEmployees(Connection * conn) {

	stmt = conn->createStatement("SELECT E1.EMPLOYEENUMBER, E1.FIRSTNAME || ' ' || E1.LASTNAME ,E1.EMAIL,Phone, E1.EXTENSION,E2.FIRSTNAME || ' ' || E2.LASTNAME FROM EMPLOYEES E1 JOIN OFFICES USING(officecode) JOIN EMPLOYEES E2 ON E1.REPORTSTO = E2.EMPLOYEENUMBER");
	rs = stmt->executeQuery();
	std::cout << std::left << std::setw(10) << "No" << std::left << std::setw(20) << "Employee Name" << std::left << std::setw(40)
		<< "Email" << std::left << std::setw(20) << "Phone" << std::left << std::setw(10) << "Ext" << "Manager" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
	if (!rs->next())
	{
		cout << "There is no employees' information to be displayed" << endl;
	}
	else
	{
		do {



			std::cout << std::left << std::setw(10) << rs->getInt(1) << std::left << std::setw(20) << rs->getString(2) << std::left << std::setw(40)
				<< rs->getString(3) << std::left << std::setw(20) << rs->getString(4) << std::left << std::setw(10) << rs->getString(5) << rs->getString(6) << endl;
		} while (rs->next());
	}


}
void menu() {

	try {
		env = Environment::createEnvironment(Environment::DEFAULT);
		conn = env->createConnection(user, pass, constr);

		char num;

		Employee* emp = new Employee();
		while (num != 6) {
			cout << endl << "********************* HR Menu *********************" << endl;
			cout << "1)Find Employee" << endl;
			cout << "2)Employees Report" << endl;
			cout << "3)Add Employee" << endl;
			cout << "4)Update Employee" << endl;
			cout << "5)Remove Employee" << endl;
			cout << "6)Exit" << endl;
			cout << "Enter the number:";
			cin >> num;
			if (num == '1') {
				int employeeNumber = 0;
				cout << "Enter employee number:";
				cin >> employeeNumber;

				if (findEmployee(conn, employeeNumber, emp) == 1) {
					displayEmployee(conn, emp);
				}
				else {
					cout << "Employee " << employeeNumber << " does not exist";
				}


			}
			else if (num == '2') {
				displayAllEmployees(conn);
			}
			else if (num == '3') {
				cout << "Enter new Employee Data" << endl;
				cout << "Employee Number: ";
				int empNum;
				cin >> empNum;
				if (findEmployee(conn, empNum, emp) == 0) {
					emp->employeeNumber = empNum;
					cout << "Last Name: ";
					cin >> emp->lastName;
					cout << "First Name: ";
					cin >> emp->firstName;
					cout << "Email: ";
					cin >> emp->email;
					cout << "extension: ";
					cin >> emp->extension;
					cout << "Job Title: ";
					cin >> emp->jobTitle;
					insertEmployee(conn, emp);
					cin.clear();
					cin.ignore(1000, '\n');
				}
				else {
					cout << "An employee with the same employee number exists." << endl;
				}

			}
			else if (num == '4') {
				cout << "Employee Number: ";
				cin >> emp->employeeNumber;
				if (findEmployee(conn, emp->employeeNumber, emp) == 1) {
					updateEmployee(conn, emp->employeeNumber);
				}
				else {

				}

			}
			else if (num == '5') {
				cout << "Employee Number: ";
				cin >> emp->employeeNumber;
				if (findEmployee(conn, emp->employeeNumber, emp) == 1) {
					deleteEmployee(conn, emp->employeeNumber);
					cout << "The employee is deleted." << endl;
				}
				else {
					cout << "The employee does not exist." << endl;
				}
			}
			else if (num == '6') {
				break;
			}
			else {
				cout << "Wrong number, try again: ";
				do {
					cin >> num;
					if (num != '1' || num != '2' || num != '3' || num != '4' || num != '5' || num != '6') {
						cout << "Wrong number, try again: ";
						cin.clear();
					}
				} while (num != '1' || num != '2' || num != '3' || num != '4' || num != '5' || num != '6');
			}
		}
	}
	catch (SQLException& sqlExcp) {
		cout << "error";
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}

int main(void) {

	menu();
};
