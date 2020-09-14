/* Nawaf Alharbi 
   Assignement 3
   Part one 
   To: Mrs. Liu Huizhu 
*/

#include <iostream>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
using namespace std;
using namespace oracle::occi;

// read database password from user input
// without showing the password on the screen
string readPassword()
{
    struct termios settings;
    tcgetattr( STDIN_FILENO, &settings );
    settings.c_lflag =  (settings.c_lflag & ~(ECHO));
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );

    string password = "";
    getline(cin, password);

    settings.c_lflag = (settings.c_lflag |   ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );
    return password;
}

string sanitize(string s)
{
    string ns = "";
    for(int i = 0; i < s.length(); i++) {
        if (s[i] == '\'' || s[i] == '\\')
            ns = ns + '\\';
        ns = ns + s[i];
    }
    return ns;
}



int main(int argc, char* argv[])
{
	if (argc !=2) { 
		cout << "the program expects a command line argument besides the program name" <<endl;
	} else {
	        // declaration of varibles that will be used throughout the program
		string userName;
		string password;
		ofstream TaxFile;
		string Arg = argv[1];
		string FileName = "TAX-" + Arg + "-2017.txt" ;  
		// address of the Oracle server
		const string connectString = "sunfire.csci.viu.ca";
		//opens a new textfile and gives it the name declared above 
		TaxFile.open(FileName.c_str());
		//error check if the file does not open 
		if (TaxFile.fail()) {
			cout << " could not create the file " <<endl; 		
		}
		

		cout << "Your user name: ";
		getline(cin, userName);

		cout << "Your password: ";
		password = readPassword();
		cout << endl;
		try {
         	    // establish database connection
		        Environment *env = Environment::createEnvironment();
		        Connection *conn = env->createConnection
		                      (userName, password, connectString);

		    // a string containg the querry that will be used to get the information needed
			string queryStr = "";
			queryStr = queryStr + " select C.cid, C.name, C.address, D.donationDate, D.amount " 
			         	    + " from clients C, donations D " 
		                            + " WHERE D.cid = C.cid AND d.donationDate between '30-DEC-16' AND '01-Jan-18' AND C.cid =  " + argv[1]
								;

		    //	a string to contain thequery to get the total amount of donations 
			string queryStr_ = "";
                        queryStr_ = queryStr_ + "select SUM(D.amount) as TotalDonation "
	                          + " from clients C, donations D "
                                  + " WHERE D.cid = C.cid AND d.donationDate between '30-DEC-16' AND '01-Jan-18' AND C.cid =  " + argv[1]
                                  ;

			
			// create statment objects
		        Statement *stmt = conn->createStatement();
			Statement *stmt_ = conn->createStatement();

			
			// using the statement objects to execute a query
      			ResultSet *rs = stmt->executeQuery(queryStr);
		        ResultSet *rs_ = stmt_->executeQuery(queryStr_);

			
			// pulling the total donation amount from the result set 
                        rs_->next();
			string Total_Donation = rs_->getString(1);
			

			// closing the result set and the statment 
			stmt_->closeResultSet(rs_);
                        conn->terminateStatement(stmt_);
		
			//pulling info from result set - process results
		        rs->next();
		        string ClientID = rs->getString(1);
		        string Name = rs->getString(2);
		        string Address = rs->getString(3);
		        string DonationDate = rs->getString(4);
		        string Amount = rs->getString(5);
			//printing recipt with information obtained in previous step into the text file  
			TaxFile << "TAX RECEIPT (2017)"<<
			endl << "Client ID: "<< ClientID <<
			endl << "Client Name: "<< Name <<
			endl << "Client Address: "<< Address <<
			endl << "Total donations in 2017: $" << Total_Donation << 
			endl << "2017 Donation Details: "<<
			endl << "Date (DD-MMM-YY) Time (HH-MM-SS)     "<< "amount "<<
			endl << DonationDate <<"         "<< "$" << Amount <<
			endl;	   
			
			// process results
			while (rs->next()) {
				DonationDate = rs->getString(4);
				Amount = rs->getString(5);
				TaxFile << DonationDate <<"         "<< "$" << Amount << 
				endl;
				}
				
				// terminating the result set and the statment 
				stmt->closeResultSet(rs);
				conn->terminateStatement(stmt);
				
				// trrmenating the connection and the enviroment 
				env->terminateConnection(conn);
				Environment::terminateEnvironment(env);
			} catch (SQLException & e) {
				cout << e.what();
				return 0;
			}

			return 0;
		}
		}
