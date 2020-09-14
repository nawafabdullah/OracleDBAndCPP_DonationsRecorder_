/* Nawaf Alharbi 
   Assignement 3
   Part one 
   To: Mrs. Liu Huizhu 
*/

#include <iostream>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;
using namespace oracle::occi;

string Client_ID;
int Donation_Amount;


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

// function to welcome the user and get the client ID
void CLIENT_ID () {
  cout <<" ===============================Welcome to the donation system=============================== "<<endl;
  cout << " please enter your ID " << endl;
  cin >> Client_ID;
  
  }


// function to handele and process the donation amount  
int DONATION_AMOUNT () {
    cout << " please enter the donation amount " << endl;
    cin >> Donation_Amount;
    if (cin.fail() || Donation_Amount < 0 ){
		cout << "please enter a positive integer for the amount of donation" <<endl ;
		cin.clear();
		cin.ignore (256, '\n');
		DONATION_AMOUNT();
	}
	return 1;
}



int main(){
  // declaration of variables that will be used throughout the program 
    string userName;
    string password;
  // a string that contains the querry to get the maximum DID
    string querryStr = "select max (did) from donations";
    int Termenator = 1011;
    int i;
    int DID;

	
    cout << "Your user name: ";
    getline(cin, userName);

    cout << "Your password: ";
    password = readPassword();
    const string connectString = "sunfire.csci.viu.ca";
    // create enviroment and connection to database 
    Environment *env = Environment::createEnvironment();
    Connection *conn = env->createConnection
                          (userName, password, connectString);
    // rule to keep the program running 
    while ( i!= Termenator) {
	
    // calling functions at the begininng of the program 
    CLIENT_ID ();
    i=DONATION_AMOUNT(); 

    // rule to terminate the program
    if (Client_ID == "0" && Donation_Amount == 0) {
      cout << "Thank you for using the donation system " << endl << "GOODBYE" <<endl; 
      env->terminateConnection(conn);
      Environment::terminateEnvironment(env);
      i = Termenator;
      }

    // if amount entry is valid 
    if (i == 1 ){
      try {
      
        // create statment 
        Statement *stmt = conn->createStatement();            

        // create result set that containg the maximum DID in the table 
        ResultSet *rs = stmt->executeQuery(querryStr);

        // get the maximum DID and store it in the variable DID        
        rs->next();   
        DID= rs->getInt(1);

        // increament the DID by one in order to avoid duplications in the DID
        DID++;

        cout << DID <<endl;
                
        // create statment that will insert the donation to the database 
        Statement *ustmt = conn->createStatement("insert into Donations values (:1, :2, sysdate, :3)");
        
        // setting varibles :1 and :2 to the previously obtained information from the user 
	ustmt->setInt(1, DID);
	ustmt->setString(2, Client_ID);
	ustmt->setInt(3, Donation_Amount);

        // success indication 
        int rows = ustmt->executeUpdate();
        cout << "Inserted " << rows << " rows" << endl;
        
        // terminate the statments, connection, and the enviroment 
	stmt->closeResultSet(rs);
        conn->terminateStatement(stmt);
	conn->terminateStatement(ustmt);
        } catch (SQLException & e) {
            cout << e.what();
            
        }      
    }
}













	
  
}
