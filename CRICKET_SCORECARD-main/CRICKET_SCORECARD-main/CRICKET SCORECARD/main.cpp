
#include <bits/stdc++.h>
//to include basic input out stream, math functions string functions
#include <windows.h>
#include <mysql.h> // to invoke Database connectivity
#include <sstream> //to pass string as queries in database


using namespace std;

/*
Classes Defined :
1.Match for Match Details : Connected with Match_Details Table in Database
2.Team_Batsman    :   for Generating Batting Details of the match
3.Team_Bowler   :    for Generating Bowling Details of the match
 */

 //Forward Declaration of classes
class Team_Batsman;
class Team_Bowler;

   //Forward Declaration of View functions
     void Batting_View(string Match_ID, string a);
     void Bowling_View(string Match_ID, string a);



class Match
{
    private:
        //Defining variables for all necessary match specific details
        string Competition;
        string Team1,Team2 ;
        int Team1_runs, Team1_wickets;
        int Team2_runs, Team2_wickets;
        string date;
        string venue;
        string Umpires, Third_Umpire,Match_refree;

        //making other two classes friends to enable private data sharing
        friend class Team_Batsman;
        friend class Team_Bowler;

    public:
        string Match_ID;
        /*defining Match_ID as public since it is passed as argument in table creation
        and match view functions */
        void setdata_Match(void); //Input Match Details
        void Match_Database_Entry(void); //Enter data in database
        friend void MatchList_View(void); //Extract and view list of matches from database
        friend void Match_View(void);//View a specific match selected via MATCH_ID
        friend void Delete_Match(void);//Delete a specific match related tables

};


//Match All Functions


/*Following function Inputs all the necessary match details taken from the user
and assigns to defined variables in the class*/
void Match :: setdata_Match(void)
{

//Initiating Data Input
    cout<<endl<<"----------NEW MATCH-----------"<<endl<<endl;
    cout<<"Enter Match ID : "; //ID unique to every match
    cin>>Match_ID;
    cout<<"Enter Competition : "; //mention Competition
    getline(cin, Competition);
    getline(cin, Competition);
    cout<<"Match Between ==> \nTeam 1 :";
   //taking team details date and match venue
    getline(cin, Team1);
    cout<<"Team 2: ";
    getline(cin, Team2);
    cout<<"DATE : ";
    getline(cin, date);
    cout<<"Match Venue : ";
    getline(cin, venue);
    //match refree and umpire inputs
    cout<<"Match Umpires : ";
    getline(cin, Umpires);

    cout<<"Third Umpire : ";
    getline(cin, Third_Umpire);

    cout<<"Match Refree : ";
    getline(cin, Match_refree);
    //Taking final score input
    cout<<endl<<"Enter "<<Team1<<" Runs / Wickets : ";
    cin>>Team1_runs>>Team1_wickets;
    cout<<"Enter "<<Team2<<" Runs / Wickets : ";
    cin>>Team2_runs>>Team2_wickets;


}

/*Following function stacks up all the match input data taken from the user
which has been stored in temporary assigned variables in the tables created
in the database for permanent storage purposes*/
void Match :: Match_Database_Entry(void){
    //Initiating Database connection
    MYSQL* conn;
    conn= mysql_init(0);
    conn=mysql_real_connect(conn, "192.168.29.53" ,"shailesh","1234", "cricket", 0, NULL, 0);
    int qstate=0 ;
    //Checking if connection is successful
        if(conn)
    {
        cout<<"DATABASE CONNECTED......."<<endl;
    }
    else{
        cout<<"DATABASE NOT CONNECTED........."<<endl;
    }
    stringstream ss;//Defining variable to pass query in the database for data insertion
    ss << " INSERT INTO Match_Details VALUES('" << Match_ID<< "','" << Competition << "','" << Team1 << "','" << Team2 << "','" << date << "','" << venue << "','" << Umpires << "','" << Third_Umpire << "','" << Match_refree << "','" << Team1_runs<<  "','" << Team1_wickets <<  "','" << Team2_runs <<  "','" << Team2_wickets <<  "')";
    string query = ss.str();
    const char* q =query.c_str();
    qstate = mysql_query(conn, q);
    //Checking if record successfully generated or not
    if(qstate ==0){
        cout<<"Record Inserted..."<<endl;
    }
    else{
        cout<<"Failed"<<endl;
    }

}


/*Following function is defined for data extraction for display purposes
It invokes database connection and display specific details to let the user know
all the matches which are there in database with their unique assigned MATCH_IDs*/
void MatchList_View(void){
    //Initiating Database connection
    MYSQL* conn; //defining connection variable
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn= mysql_init(0);
    conn=mysql_real_connect(conn, "192.168.29.53" ,"shailesh","1234", "cricket", 0, NULL, 0);
    if(conn){
            //Passing query to generate match lists from database
        int qstate = mysql_query(conn, "SELECT MATCH_ID , COMPETITION  FROM Match_Details  ");

        if(!qstate){
            res = mysql_store_result(conn);
        //specifying format of matches list
                 cout<<"-------------------------------------------------------------------------"<<endl;
                 cout<<"  Match ID    |                   LIST OF ALL MATCHES                     "<<endl;
                 cout<<"-------------------------------------------------------------------------"<<endl;

            while(row = mysql_fetch_row(res)){
              //specifying display of matches list
                     cout<<"    "<<setw(5)<<row[0]<<"     |         " <<"         "<<row[1]<<endl;  //Display All matches
                      cout<<"-------------------------------------------------------------------------"<<endl;
            }
        }
    }
}



/*Following function is defined for data extraction for display purposes
It invokes database connection followed by asking for MATCH_ID and
displays all the associated records and tables associated with match in
the specified manner*/
        void Match_View(void){
            MYSQL* conn; //defining connection variable
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn= mysql_init(0);
            conn=mysql_real_connect(conn, "192.168.29.53" ,"shailesh","1234", "cricket", 0, NULL, 0);
            if(conn){
                    /*Taking Match_ID as input to pass Match specific data generation query into
                    the database */
                string sqlQuery;
                cout<<"Enter Match_ID value : "<<endl;
                cin>>sqlQuery;

                string id; //creating variable to retain Match ID
                id=sqlQuery;
             //Query to select data from a specific table for display
            sqlQuery= "SELECT * FROM Match_Details WHERE  MATCH_ID =  "+ sqlQuery;
            int qstate =mysql_query(conn, sqlQuery.c_str());

        if(!qstate){
                res = mysql_store_result(conn);
                while(row = mysql_fetch_row(res)){

                        //Specifying display structure of match details display
                           cout<<"-------------------------------------------------------------------------"<<endl;
                           cout<<"|"<<"COMPETITION : "<<row[1]<<"        ||        "<<"DATE : "<<row[4]<<endl;
                           cout<<endl;
                           cout<<"|"<<"MATCH BETWEEN : "<<row[2]<<" V/S "<<row[3]<<"    "<<endl;
                          cout<<endl;
                            cout<<"|"<<"VENUE : "<<row[5]<<endl;
                         cout<<endl;
                            cout<<"|"<<"MATCH UMPIRES : "<<row[6]<<endl;
                             cout<<endl;
                            cout<<"|"<<"THIRD UMPIRE : "<<row[7]<<"  |  "<<"MATCH REFREE : "<<row[8]<<endl;
                            cout<<endl;
                            cout<<"|"<<row[2]<<" : "<<row[9]<<"/"<<row[10]<<"     |     "<<row[3]<<" : "<<row[11]<<"/"<<row[12]<<endl;


                             cout<<"------------------------------------------------------------------------"<<endl<<endl;

                        }

                }
            //calling function from batsmen and bowler classes and displaying
            //result extracted from database
          cout<<"\n\t---TEAM 1 BATTING PERFORMANCE ----"<<endl;
         Batting_View(id, "1");
         cout<<"\n\t---TEAM 1 BOWLING PERFORMANCE ----"<<endl;
         Bowling_View(id, "1");
         cout<<endl;
         cout<<"\n\t---TEAM 2 BATTING PERFORMANCE ----"<<endl;
         Batting_View(id, "2");
         cout<<"\n\t---TEAM 2 BOWLING PERFORMANCE ----"<<endl;
         Bowling_View(id, "2");

        }
    }



/*Following function is defined for Match Deletion purposes
It asks the user for Match specific ID and Deletes the Match record from the
main table and also deletes all batsmen and bowler tables associated with
specified match from the database at once*/
void Delete_match(void){
    //Initiating Database connection
    MYSQL* conn;//Defining connection variable
    conn= mysql_init(0);
    conn=mysql_real_connect(conn, "192.168.29.53" ,"shailesh","1234", "cricket", 0, NULL, 0);

        if(conn){
        string sqlQuery;
        //asking for Match_Id
        cout<<"Enter Match_ID value : "<<endl;
        cin>>sqlQuery;
        string id;
        //retaining match_id to pass as query for deletion of associated tables
        id=sqlQuery;
        //Query to Delete Match record
        sqlQuery = "DELETE FROM Match_Details WHERE  MATCH_ID =  "+ sqlQuery;
         int qstate =mysql_query(conn, sqlQuery.c_str());
        stringstream ss;
        //Query to drop tables
        ss << " DROP TABLE "+id+"_team_1_batting,"+id+"_team_1_bowling,"+id+"_team_2_batting,"+id+"_team_2_bowling ;";
        string query = ss.str();
        const char* q =query.c_str();
        int rstate = mysql_query(conn, q);

        }
    }


    //--------------------------------------------------------------------------------------------------------------------

//Batsmen Information Class
    class Team_Batsman{
    private:
        //Defining information variables for Batsman
        string player;

        /*Rather than creating individual integer variables
        for runs, balls, 4s and 6s
        Creating an Array for taking all the numeric input */
        int p[4];
        //Defining Strike Rate as SR
        float SR;
        string status;

    public:
        void Create_Table_Batting(string Match_ID, string a);// Function to create table

        //Function to enter data from temporary assigned variables to database
        void Batting_Database_Entry(string Match_ID, string a, int num);
        //Function to display batting info
        friend void Batting_View(string Match_ID, string a);

    };

//Team_Batsman Functions   :


/*Following function is defined for Creating team specific batting performance
table via SQL query*/
    void Team_Batsman :: Create_Table_Batting(string Match_ID, string a)
{
    //Initiating Database Connection
    MYSQL* conn;
    conn= mysql_init(0);
    conn=mysql_real_connect(conn, "192.168.29.53" ,"shailesh","1234", "cricket", 0, NULL, 0);
    //Query for Table creation
    stringstream ss;
    ss << "CREATE TABLE "<< Match_ID<<"_Team_"<<a<<"_Batting  ("
    "Player varchar(70),"
    " Runs int , "
    "Balls int , "
    "4s int , "
    "6s int , "
    "SR float , "
    "Status varchar(70) ) ;";
    string query = ss.str();
    const char* q =query.c_str();
    int qstate = mysql_query(conn, q);
    //Checking if table created successfully
    if(qstate ==0){
        cout<<"Table Created..."<<endl;
    }
    else{
        cout<<"Failed!!"<<endl;
    }
}

/*Following function stacks up all the Batsmen input data taken from the user
which has been stored in temporary assigned variables in the tables created
in the database for permanent storage purposes
The function takes arguments MATCH_ID  to know for which match and team table
is being created and also an integer argument to know the number of players*/
void Team_Batsman :: Batting_Database_Entry(string Match_ID, string a, int num)
{
    //Initiating Database Connection
    MYSQL* conn;//Defining connection variable
    conn= mysql_init(0);
    conn=mysql_real_connect(conn, "192.168.29.53" ,"shailesh","1234", "cricket", 0, NULL, 0);

    cout<<"\n------TEAM"<<a<<" BATTING RECORD INPUT ------\n";
    //Taking input for various batsmen in loop
    for(int j=0;j<num;j++){
    fflush(stdin);
    cout<<"Enter Player Name : ";
    getline(cin, player);
    cout<<"Input \n  Runs   Balls   4s   6s  :"<<endl;
  for(int i=0;i<4;i++)
  {
      cout<<"\t";
      cin>>p[i];//taking numeric input into array
  }
  //Strike rate calculation for each batsman
  SR=((float) p[0]/(float) p[1])*100;

  fflush(stdin);
  cout<<"Enter Status : ";
  getline(cin, status);

    stringstream ss;
    //Query to transfer data from variables and array to database
    ss << " INSERT INTO "<< Match_ID<<"_Team_"<<a<<"_Batting VALUES('" << player<< "','" << p[0] << "','" << p[1] << "','" << p[2] << "','" << p[3] << "','" << SR<< "','" << status <<  "')";
    string query = ss.str();
    const char* q =query.c_str();
    int qstate = mysql_query(conn, q);
     //Checking if record successfully generated or not
    if(qstate ==0){
        cout<<"Record Inserted...\n"<<endl;
       }
    else{
        cout<<"Failed!!\n"<<endl;
       }
    }

}
/*Following function is defined to view data extracted from database*/
void Batting_View(string Match_ID, string a){
    //Initiating Database Connection
    MYSQL* conn; //Defining connection variable
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn= mysql_init(0);
    conn=mysql_real_connect(conn, "192.168.29.53" ,"shailesh","1234", "cricket", 0, NULL, 0);
    if(conn){
            string sqlQuery;
            //Query to extract data from tables
    sqlQuery ="SELECT *  FROM  "+ Match_ID+"_Team_"+a+"_Batting";
        int qstate = mysql_query(conn, sqlQuery.c_str() );

        if(!qstate){
            res = mysql_store_result(conn);
                 cout<<"-------------------------------------------------------------------------------------"<<endl;
                 cout<<"       Player    |     R(B)      |   4s  |   6s  |      SR     |    Status    "<<endl;
                 cout<<"-------------------------------------------------------------------------------------"<<endl;

            while(row = mysql_fetch_row(res)){
                    //Specifying format for displaying extracted data from database
                     cout<<setw(15)<<row[0]<<"  |  " <<setw(5)<<row[1]<<"("<<setw(3)<<row[2]<<") "<<"  |  "<<setw(3)<<row[3]<<"  |  "<<setw(3)<<row[4]<<"  |  "<<setw(9)<<row[5]<<"  |    "<<row[6]<<endl;  //Display Batsmen Performances
                     cout<<"-------------------------------------------------------------------------------------"<<endl;
            }
        }
    }
}


//---------------------------------------------------------------------------------------------------------------

    //Bowler Information Class
    class Team_Bowler{
        private:
        string bowler;
        int p[4];

     public:
     void Create_Table_Bowling(string Match_ID, string a);//Function to create table
     void Bowling_Database_Entry(string Match_ID, string a, int num);
     friend void Bowling_View(string Match_ID, string a);



    };
//Team_Bowler Functions   :

/*Following function is defined for Creating team specific bowling  performance
table via SQL query*/
    void Team_Bowler :: Create_Table_Bowling(string Match_ID, string a)
{
    //Initiating Database Connection
    MYSQL* conn;//Defining connection variable
    conn= mysql_init(0);
    conn=mysql_real_connect(conn, "192.168.29.53" ,"shailesh","1234", "cricket", 0, NULL, 0);

     stringstream ss;
    ss << "CREATE TABLE "<< Match_ID<<"_Team_"<<a<<"_Bowling ("
    "Bowler varchar(70),"
    " Overs int , "
    "Maidens int , "
    "Runs int , "
    "Wickets int "
    " ) ;";
    string query = ss.str();
    const char* q =query.c_str();
    int qstate = mysql_query(conn, q);
    //Checking if table created successfully
    if(qstate ==0){
        cout<<"Table Created..."<<endl;
    }
    else{
        cout<<"Failed!!"<<endl;
    }

}

/*Following function stacks up all the Bowler input data taken from the user
which has been stored in temporary assigned variables in the tables created
in the database for permanent storage purposes
The function takes arguments MATCH_ID  to know for which match and team table
is being created and also an integer argument to know the number of players*/
void Team_Bowler :: Bowling_Database_Entry(string Match_ID, string a, int num)
{
    //Initiating Database Connection
    MYSQL* conn; //Defining connection variable
    conn= mysql_init(0);
    conn=mysql_real_connect(conn, "192.168.29.53" ,"shailesh","1234", "cricket", 0, NULL, 0);

    cout<<"\n------TEAM_"<<a<<" BOWLING RECORD INPUT ------\n";
    //Taking bowler inputs in loop
    for(int j=0;j<num;j++){
    fflush(stdin);
    cout<<"Enter Bowler Name : ";
    getline(cin, bowler);
    cout<<"Input \n  Overs   Maidens   Runs  Wickets  :"<<endl;
  for(int i=0;i<4;i++)
  {
      cout<<"\t";
      cin>>p[i]; //taking Numeric Input
  }

    stringstream ss;//variable to write query
     //Query to transfer data from variables and array to database
    ss << " INSERT INTO "<< Match_ID<<"_Team_"<<a<<"_Bowling VALUES('" << bowler<< "','" << p[0] << "','" << p[1] << "','" << p[2] << "','" << p[3] <<  "')";
    string query = ss.str();
    const char* q =query.c_str();
    int qstate = mysql_query(conn, q);
    if(qstate ==0){
        cout<<"Record Inserted...\n"<<endl;
    }
    else{
        cout<<"Failed!!\n"<<endl;
}
    }

}
/*Following function is defined to view data extracted from database*/
void Bowling_View(string Match_ID, string a){
    //Initiating Database Connection
    MYSQL* conn; //Defining connection variable
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn= mysql_init(0);
    conn=mysql_real_connect(conn, "192.168.29.53" ,"shailesh","1234", "cricket", 0, NULL, 0);
    if(conn){
            string sqlQuery;
        //Query to extract data from table
    sqlQuery ="SELECT *  FROM  "+ Match_ID+"_Team_"+a+"_Bowling";
        int qstate = mysql_query(conn, sqlQuery.c_str() );

        if(!qstate){
            res = mysql_store_result(conn);
                 cout<<"-------------------------------------------------------"<<endl;
                 cout<<"       Bowler    |    O   |   M  |   R   |    W   "<<endl;
                 cout<<"-------------------------------------------------------"<<endl;

            while(row = mysql_fetch_row(res)){
                     //Specifying format for displaying extracted data from database
                     cout<<setw(15)<<row[0]<<"  |  " <<setw(5)<<row[1]<<" |"<<setw(4)<<row[2]<<"  |  "<<setw(3)<<row[3]<<"  |  "<<setw(3)<<row[4]<<endl;  //Display Bowler Performances
                     cout<<"-------------------------------------------------------"<<endl;
            }
      }
    }
}

//---------------------------------------------------------------------------------------------------------------------

//Main_Function
int main()
{
    //Setting display text colour
    system("Color 0B");
    fflush(stdin);
    int choice;
    cout << endl
         << endl
         << "--------------------WELCOME TO CRICKET SCORE MANAGEMENT SYSTEM----------------------" << endl;
    while (1)
    {
        cout << endl
             << "CHOOSE OPERATION : " << endl;
        cout << "1.Match Details Input\n2.View Match\n3.Delete Match \nEnter choice : ";
        cin >> choice;

        if (choice == 1)
        {

            //Match Details Input
            Match TD;
            //New Match
            TD.setdata_Match();

            //Match Details Database Entry (New Record)
            TD.Match_Database_Entry();
            //Creating objects for Batting and Bowling teams
            Team_Batsman A, B;
            Team_Bowler C, D;

           //Input number of batsmen and bowlers in each team
            int batsmen, bowlers;
            cout << endl
                 << "--------INPUT INFO----" << endl;
            cout << "Enter No. of Batsmen in Each Team : ";
            cin >> batsmen;
            cout << "Enter No. of Bowler in Each Team : ";
            cin >> bowlers;

            /*Team specific Batting and Bowling table creation and
            calling neccesary functions for input
            Also passing MATCH_ID , team no. and no of player
            as arguments*/
            A.Create_Table_Batting(TD.Match_ID, "1");
            A.Batting_Database_Entry(TD.Match_ID, "1", batsmen);
            C.Create_Table_Bowling(TD.Match_ID, "1");
            C.Bowling_Database_Entry(TD.Match_ID, "1", bowlers);

            B.Create_Table_Batting(TD.Match_ID, "2");
            B.Batting_Database_Entry(TD.Match_ID, "2", batsmen);
            D.Create_Table_Bowling(TD.Match_ID, "2");
            D.Bowling_Database_Entry(TD.Match_ID, "2", bowlers);
        }

        else if (choice == 2)
        {
            //View Matches List
            MatchList_View();
            //View Specific match
            Match_View();
        }
        else if (choice == 3)
        {
            //Delete Match Record and associated tables
            Delete_match();
        }
        else
        {
            cout << "Invalid choice !!!";
        }
    }
}
