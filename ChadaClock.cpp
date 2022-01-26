#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <ctime>
#include <thread>
#include <Windows.h>

using namespace std;

bool finished = false;           //Used to shutdown the program when user presses certain keys

void displayTime(int hrs, int mins, int sec, string clockType)            //Recieves integers that represent hours, minutes, and seconds. Uses them to make parts of the clock display
{

	std::vector<std::string> innerText;                       //innerText must be a vector to account for multiple lines of text

	string strHrs = to_string(hrs);
	string strMins = to_string(mins);                            //hours minutes and seconds are converted to string before being printed
	string strSec = to_string(sec);
	string newTime;
	string mornNight;                                     //mornNight stores the AM/PM value

	if (clockType == "standardClock") {
		if (hrs % 12 == 0) {                               // 12 O'clock is the first hour of a stardard clock, when 0 hours have passed 
			strHrs = "12";
		}
		else {
			strHrs = to_string(hrs % 12);                 //This makes strHrs 1-11, depending on how many hours have passed
		}
		if ((hrs / 12) % 2 == 0)                          //This checks to see if an even number of sets of 12 hours have passed
		{
			mornNight = "      A.M.";
		}
		else {
			mornNight = "      P.M.";
		}
		newTime = " " + strHrs + ":" + strMins + ":" + strSec + mornNight;
		
		innerText = { "      12-hour clock      ", newTime };
	}

	else if (clockType == "militaryClock") {
		strHrs = to_string(hrs % 24);                                 //24-hour clock starts at 0 and ends at 23
		newTime = " " + strHrs + ":" + strMins + ":" + strSec;
		innerText = { "      24-hour clock      ", newTime };
	}
	else if (clockType == "menu")
	{
		innerText = { "1 - Add One Hour            ", "2 - Add One Minute          ", "3 - Add One Second          ", "4 - Exit Program            " };
	}

	for (int i = 0; i < innerText.size(); i++)            //This displays each line of innerText, using i to iterate through the lines of text
	{
		cout << "*" << innerText[i] << "*" << endl;
	}	
}

void totalOutput(int added)                    //added is the sum of seconds passed and seconds added by the user. It is used here to calculate time and display the clock
{
	int hrs = added / 3600;
	int mins = (added % 3600) / 60;
	int sec = added % 60;
	string endCap = "***********************************************\n";

	cout << endCap;
	
	displayTime(hrs, mins, sec, "standardClock");
	cout << endCap;
	displayTime(hrs, mins, sec, "militaryClock");
	cout << endCap;
	displayTime(hrs, mins, sec, "menu");
	cout << endCap;	
}

void handleInput(int& seconds, string userInput)               //This alters seconds based on user input and prevents the program from waiting on input to proceed 
{
	if (userInput.empty())
	{
		return;                      //Used like "continue;". The loop will proceed
	}
	else if (userInput == "Exit" || userInput == "exit" || userInput == "4") {
		cout << "Goodbye!";
		finished = true;                         //Renders the condition of the loop false, ending the program
	}
	else if (userInput == "1")
	{
		seconds = seconds + 3600;                    //3600 is the number of seconds in an hour. In "totalOutput", this will be converted to "hrs++;"
	}
	else if (userInput == "2")
	{
		seconds = seconds + 60;                     //60 seconds in a minute. The effect is "mins++;" in totalOutput
	}
	else if (userInput == "3")
	{
		seconds = seconds + 1;
	}
	else if (userInput == "9")                       //An extra feature for testing. Adds 22 hours onto the clock
	{
		seconds = seconds + (3600 * 22);
	}
	else {
		cout << "Invalid Input!" << endl;
		
	}

}

int main()
{
	string userInput;
	int seconds = 0;
	while (!finished)
	{
		system("cls");                     //clears the screen at the beginning of each iteration, because this is effectively an animation
		totalOutput(seconds);              //seconds start at 0, but they are increased by 1 with each iteration of the loop. They are also affected by handleInput
		seconds++;
		
		thread t1([&]() {
			cin >> userInput;
			});                                //A seperate thread is created to handle input while main displays output. The thread must be detached so that the loop does not wait
		Sleep(1000);                             //The program waits for 1 second before handling input, then restarting the loop
		t1.detach();
		
		handleInput(seconds, userInput);
		userInput.clear();
	}
	return 0;
}