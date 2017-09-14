//
// Name : Matthew Kwiecien
// Date : 09/11/2017
// Course : CISP1010-70
// Instructor : Dr. Brown
// Program : Week 3 - Rock Paper Scissors
// Description:
// Rock Paper Scissors game
//

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <unistd.h>
#include <stdexcept>
#include <iostream>

using namespace std;

// Get Linux user name
string username = getlogin();

// AI Player tracking data
int lastPlayedByCPU;
int lastPlayedByPlayer;
int beforeLastPlayByPlayer;
int playerWinsInARow;
bool openingTurn;


// Declaring so it will be available
void mainMenu(char* selected = NULL,string errorMessage = "");

// Convert INT to STRING
string toStr(int i){
	ostringstream out;
	out << i;
	return out.str();
}

// Transform a string to lowercasse
string strtolower(string str){
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

// Returns TTY color escapes
string ttyTextColor(string c, int b=0){
	string out = "";
	string bold = toStr(b);
	string color = strtolower(c);
	if(color=="black"			|| color=="30"){ out = "\033["+bold+";30m";
	}else if(color=="red"		|| color=="31"){ out = "\033["+bold+";31m";
	}else if(color=="green"		|| color=="32"){ out = "\033["+bold+";32m";
	}else if(color=="yellow"	|| color=="33"){ out = "\033["+bold+";33m";
	}else if(color=="blue"		|| color=="34"){ out = "\033["+bold+";34m";
	}else if(color=="magenta"	|| color=="35"){ out = "\033["+bold+";35m";
	}else if(color=="cyan"		|| color=="36"){ out = "\033["+bold+";36m";
	}else if(color=="white"		|| color=="37"){ out = "\033["+bold+";37m";
	}else if(color=="reset"){ out = "\033[0m"; }
	return out;
}

// Returns TTY background color escapes
string ttyTextBackgroundColor(string c, int b=0){
	string out = "";
	string bold = toStr(b);
	string color = strtolower(c);
	if(color=="black"			|| color=="40"){ out = "\033["+bold+";40m";
	}else if(color=="red"		|| color=="41"){ out = "\033["+bold+";41m";
	}else if(color=="green"		|| color=="42"){ out = "\033["+bold+";42m";
	}else if(color=="yellow"	|| color=="43"){ out = "\033["+bold+";43m";
	}else if(color=="blue"		|| color=="44"){ out = "\033["+bold+";44m";
	}else if(color=="magenta"	|| color=="45"){ out = "\033["+bold+";45m";
	}else if(color=="cyan"		|| color=="46"){ out = "\033["+bold+";46m";
	}else if(color=="white"		|| color=="47"){ out = "\033["+bold+";47m";
	}else if(color=="reset"){ out = "\033[0m"; }
	return out;
}

// Put global AI-related variables back to default
void resetAIPlayerTracking(){
	lastPlayedByCPU=-1;
	lastPlayedByPlayer=-1;
	beforeLastPlayByPlayer=-1;
	playerWinsInARow=0;
	openingTurn=1;
}

// Translate from tool name to number
int rpssl_nameToNumber(string name){
	int number=-1;
	if(name=="rock"		|| name=="r") number=0;
	if(name=="paper"	|| name=="p") number=2;
	if(name=="scissors"	|| name=="s") number=4;
	if(name=="spock"	|| name=="v") number=1;
	if(name=="lizard"	|| name=="l") number=3;
	return number;
}

// Translate from tool number to name
string rpssl_numberToName(int number){
	string name;
	if(number==0) name="Rock";
	if(number==2) name="Paper";
	if(number==4) name="Scissors";
	if(number==1) name="Spock";
	if(number==3) name="Lizard";
	return name;
}

// Translate from tool number to name + verb
string rpssl_numberToAction(int number){
	string name;
	if(number==0) name="Rock crushes";
	if(number==2) name="Paper covers";
	if(number==4) name="Scissors cut";
	if(number==1) name="Spock smashes";
	if(number==3) name="Lizard eats";
	return name;
}

// RPS AI
// Easy mode, bro. Pure chance.
int ai_rps(){
	return ((rand() % 3) * 2);
}

// RPS HARD AI
// AI that attempts to take into account player psychology
// Based on a study in China by Zhijian Wang at Zhejiang University
// as reported here: https://www.psychologytoday.com/blog/the-blame-game/201504/the-surprising-psychology-rock-paper-scissors
int ai2_rps(){
	int hand=-1;
	int random = ((rand() % 3) * 2);
	if(openingTurn==1){ // If it is the opening turn
		if((rand() % 3) == 0){ // Give 2/3 chance of player throwing rock
			if((rand() % 3) == 0){ // Give 2/3 chance of player throwing scissors
				hand = rpssl_nameToNumber("rock");
			}else{ // Pure chance
				hand = random;
			}
		}else{
			hand = rpssl_nameToNumber("paper");
		}
	}else{ // If it is NOT the opening turn
		// Give 5% chance of a random throw
		if((rand() % 20) == 0){
			hand = random;
		}else{
			if(playerWinsInARow>0){ // If the player won last turn
				if(beforeLastPlayByPlayer==-1){ // If this is the second turn
					if(lastPlayedByPlayer==rpssl_nameToNumber("rock")) hand=rpssl_nameToNumber("paper");
					if(lastPlayedByPlayer==rpssl_nameToNumber("paper")) hand=rpssl_nameToNumber("scissors");
					if(lastPlayedByPlayer==rpssl_nameToNumber("scissors")) hand=rpssl_nameToNumber("rock");
				}else{ // If after the second turn
					if(beforeLastPlayByPlayer==lastPlayedByPlayer){ // If they've played the same thing twice
						if(lastPlayedByPlayer==rpssl_nameToNumber("rock")) hand=rpssl_nameToNumber("scissors");
						if(lastPlayedByPlayer==rpssl_nameToNumber("paper")) hand=rpssl_nameToNumber("rock");
						if(lastPlayedByPlayer==rpssl_nameToNumber("scissors")) hand=rpssl_nameToNumber("paper");
					}else{
						if(playerWinsInARow<=2){ // If they've NOT played the same twice AND are WINNING (assume they will repeat)
							if(lastPlayedByPlayer==rpssl_nameToNumber("rock")) hand=rpssl_nameToNumber("paper");
							if(lastPlayedByPlayer==rpssl_nameToNumber("paper")) hand=rpssl_nameToNumber("scissors");
							if(lastPlayedByPlayer==rpssl_nameToNumber("scissors")) hand=rpssl_nameToNumber("rock");
						}else if(playerWinsInARow>2){ // If they've NOT played the same twice AND are STILL WINNING (assume they will throw confident (s) or aggressive (r))
							if(lastPlayedByPlayer==rpssl_nameToNumber("rock")) hand=rpssl_nameToNumber("scissors");
							if(lastPlayedByPlayer==rpssl_nameToNumber("paper")) hand=rpssl_nameToNumber("scissors");
							if(lastPlayedByPlayer==rpssl_nameToNumber("scissors")) hand=rpssl_nameToNumber("rock");
						}else{ // This shouldn't happen
							hand = random;
						}
					}
				}
			}else{ // If the player did NOT win the last turn
				if(beforeLastPlayByPlayer==lastPlayedByPlayer){  // If they've HAVE played the same twice...
				}else{  // If they've NOT played the same twice...
					if(playerWinsInARow<0){ // If they're on a losing streak (assume they won't throw paper)
						if(lastPlayedByPlayer==rpssl_nameToNumber("rock")) hand=rpssl_nameToNumber("rock");
						if(lastPlayedByPlayer==rpssl_nameToNumber("paper")) hand=rpssl_nameToNumber("rock");
						if(lastPlayedByPlayer==rpssl_nameToNumber("scissors")) hand=rpssl_nameToNumber("paper");
					}else{ // If they've only lost once (assume they will cycle)
						if(lastPlayedByPlayer==rpssl_nameToNumber("rock")) hand=rpssl_nameToNumber("scissors");
						if(lastPlayedByPlayer==rpssl_nameToNumber("paper")) hand=rpssl_nameToNumber("rock");
						if(lastPlayedByPlayer==rpssl_nameToNumber("scissors")) hand=rpssl_nameToNumber("paper");
					}
				}
			}
		}
	}

	// Error catch
	if(hand==-1) hand=random;

	return hand;
}

// RPSSL AI
// Easy mode again. Pure chance.
int ai_rpssl(){
	return (rand() % 5);
}

// RPS game logic
int game_rps(int wins,int plays, int hardMode=0){
	system("clear");

	// Header text
	cout << ttyTextColor("white") << "Session SCORE: " << (wins-(plays-wins));
	cout << ttyTextColor("blue",1) << "   (WINS:";
	cout << ttyTextColor("white") << wins;
	cout << ttyTextColor("blue",1) << ", ROUNDS: ";
	cout << ttyTextColor("white") << plays;
	cout << ttyTextColor("blue",1) << ")\n";
	cout << ttyTextColor("green") << "*** Rock-Paper-Scissors ***\n\n";
	cout << ttyTextColor("blue",1) << "The computer has selected a tool.\n";
	
	// Initialize
	int player=-1;
	string tool;

	// Get player choice
	while(player==-1){
		if(player!=-1){
			cout << ttyTextColor("red") << "Sorry, that is not a valid tool! Select again.\n";
			cout << ttyTextColor("blue",1);
		}
		cout << "What tool will you use (";
		cout << ttyTextColor("red") << "r";
		cout << ttyTextColor("blue",1) << ", ";
		cout << ttyTextColor("green") << "p";
		cout << ttyTextColor("blue",1) << ", ";
		cout << ttyTextColor("yellow") << "s";
		cout << ttyTextColor("blue",1) << "): ";
		cout << ttyTextColor("white");
		getline(cin,tool);
		player = rpssl_nameToNumber(strtolower(tool));
		cout << ttyTextColor("blue",1);
	}
	
	// Call ai based on mode (chance / hard)
	int cpu;
	if(hardMode==1){
		cpu = ai2_rps();
	}else{
		cpu = ai_rps();
	}

	// Math to determine winner
	int result = ((player - cpu + 5) % 5);

	// cout << result << endl;
	if(result>=3){
		// CPU wins!
		cout << rpssl_numberToAction(cpu) << " " << rpssl_numberToName(player) << ".";
		cout << " You " << ttyTextColor("red",1) << "lose" << ttyTextColor("blue",1) << "!\n\n";
	}else{
		if(result==0){
			// Tie!
			cout << "You both chose " << rpssl_numberToName(player) << "!";
			cout << " " << ttyTextColor("yellow",1) << "Tie" << ttyTextColor("blue",1) << ".\n\n";
		}else{
			// Player wins!
			wins++;
			cout << rpssl_numberToAction(player) << " " << rpssl_numberToName(cpu) << ".";
			cout << " You " << ttyTextColor("green",1) << "win" << ttyTextColor("blue",1) << "!\n\n";
	
			// AI Tracking for player win
			lastPlayedByCPU=cpu;
			beforeLastPlayByPlayer=lastPlayedByPlayer;
			lastPlayedByPlayer=player;
			playerWinsInARow++;
			openingTurn=0;
	
			return 1;
		}

		// AI Tracking for player non-win
		lastPlayedByCPU=cpu;
		beforeLastPlayByPlayer=lastPlayedByPlayer;
		lastPlayedByPlayer=player;
		if(playerWinsInARow<=0){
			playerWinsInARow--;
		}else{
			playerWinsInARow=0;
		}
		openingTurn=0;
	}

	return 0;
}

// RPSSL game logic
int game_rpssl(int wins,int plays){
	system("clear");

	// Header text
	cout << ttyTextColor("white") << "Session SCORE: " << (wins-(plays-wins));
	cout << ttyTextColor("blue",1) << "   (WINS:";
	cout << ttyTextColor("white") << wins;
	cout << ttyTextColor("blue",1) << ", ROUNDS: ";
	cout << ttyTextColor("white") << plays;
	cout << ttyTextColor("blue",1) << ")\n";
	cout << ttyTextColor("green") << "*** Rock-Paper-Scissors-Spock-Lizard ***\n\n";
	cout << ttyTextColor("blue",1) << "The computer has selected a tool.\n";

	// Initialize
	int player=-1;
	string tool;

	// Get player choice
	while(player==-1){
		if(player!=-1	){
			cout << ttyTextColor("red") << "Sorry, that is not a valid tool! Select again.\n";
			cout << ttyTextColor("blue",1);
		}
		cout << "What tool will you use?\n(";
		cout << ttyTextColor("red") << "rock (r)";
		cout << ttyTextColor("blue",1) << ", ";
		cout << ttyTextColor("green") << "paper (p)";
		cout << ttyTextColor("blue",1) << ", ";
		cout << ttyTextColor("yellow") << "scissors (s)";
		cout << ttyTextColor("blue",1) << ", ";
		cout << ttyTextColor("blue",1) << "Spock (v)";
		cout << ttyTextColor("blue",1) << ", ";
		cout << ttyTextColor("cyan") << "lizard (l)";
		cout << ttyTextColor("blue",1) << ")\n? ";
		cout << ttyTextColor("white");
		getline(cin,tool);
		player = rpssl_nameToNumber(strtolower(tool));
		cout << ttyTextColor("blue",1);
	}
	
	// Call cpu ai
	int cpu = ai_rpssl();

	// Math to determine winner
	int result = ((player - cpu + 5) % 5);

	if(result>=3){
		// CPU wins!
		cout << rpssl_numberToAction(cpu) << " " << rpssl_numberToName(player) << ".";
		cout << " You " << ttyTextColor("red",1) << "lose" << ttyTextColor("blue",1) << "!\n\n";
	}else{
		if(result==0){
			// Tie!
			cout << "You both chose " << rpssl_numberToName(player) << "!";
			cout << " " << ttyTextColor("yellow",1) << "Tie" << ttyTextColor("blue",1) << ".\n\n";
		}else{
			// Player wins!
			wins++;
			cout << rpssl_numberToAction(player) << " " << rpssl_numberToName(cpu) << ".";
			cout << " You " << ttyTextColor("green",1) << "win" << ttyTextColor("blue",1) << "!\n\n";
	
			// AI Tracking for player win
			lastPlayedByCPU=cpu;
			beforeLastPlayByPlayer=lastPlayedByPlayer;
			lastPlayedByPlayer=player;
			playerWinsInARow++;
			openingTurn=0;
			
			return 1;
		}
		
		// AI Tracking for player non-win
		lastPlayedByCPU=cpu;
		beforeLastPlayByPlayer=lastPlayedByPlayer;
		lastPlayedByPlayer=player;
		if(playerWinsInARow<=0){
			playerWinsInARow--; // If you don't win you LOSE! :)
		}else{
			playerWinsInARow=0;
		}
		openingTurn=0;
	}

	return 0;
}

// Call game with session score tracking
void playGame(int gameNumber){
	if(gameNumber == 0){
		// Rock Paper Scissors

		// Initialize
		int wins = 0;
		int plays = 0;
		string again = "";
		do{
			// Play until user exits
			wins += game_rps(wins,plays);
			plays++;
			
			do{
				cout << "Play again? (Y/n): ";
				getline(cin,again);
				// again = cin.get();
				again = strtolower(again);
			}
			while(again!="y" && again!="n" && again!="");
		}
		while(again!="n");
	}
	if(gameNumber == 1){
		// Rock Paper Scissors HARD

		// Initialize
		int wins = 0;
		int plays = 0;
		string again = "";
		do{
			// Play until user exits
			wins += game_rps(wins,plays,1);
			plays++;
			
			do{
				cout << "Play again? (Y/n): ";
				getline(cin,again);
				// again = cin.get();
				again = strtolower(again);
			}
			while(again!="y" && again!="n" && again!="");
		}
		while(again!="n");
	}
	if(gameNumber == 2){
		// Rock Paper Scissors Spock Lizard

		// Initialize
		int wins = 0;
		int plays = 0;
		string again;
		do{
			// Play until user exits
			plays++;
			wins += game_rpssl(wins,plays);
			
			do{
				cout << "Play again? (Y/n): ";
				getline(cin,again);
				again = strtolower(again);
			}
			while(again!="y" && again!="n" && again!="");
		}
		while(again!="n");
	}

	mainMenu();

	return;
}

// Show main menu
void mainMenu(char* selected,string errorMessage){
	system("clear");
	string game;
	resetAIPlayerTracking();
	if(selected!=NULL){
		// Skip straight to selected game
		game = strtolower(string(selected));
	}else{
		// Show menu
		cout << ttyTextColor("blue",1);
		cout << "Greetings " << username << ".\nShall we play a game?";
		cout << endl;
		cout << "\n1] Rock Paper Scissors";
		cout << "\n2] Rock Paper Scissors [Hard Mode]";
		cout << "\n3] Rock Paper Scissors Spock Lizard";
		cout << "\nQ] Quit";
		cout << endl;
		if(!errorMessage.empty())
			cout << endl << ttyTextColor("red") << errorMessage << ttyTextColor("blue",1) << endl;
		cout << "\n? ";
		getline(cin,game);
		game = strtolower(game);
	}


	if(game=="1"){
		// rock paper scissors (pure chance mode)
		playGame(0);
	}else if(game=="2" || game=="rps"){
		// rock paper scissors (ai mode)
		playGame(1);
	}else if(game=="3" || game=="rpssl" || game=="rpsls"){
		cout << "RPSLS selected" <<endl;
		playGame(2);
	}else{
		// Exit game
		if(game=="q" || game=="quit" || game=="exit") return;
		else mainMenu(NULL,"Invalid game selection. Try again.");
	}
	return;
}

int main(int argc,char* argv[]){
	if(argc>1){
		// Skip main menu if provided the correct command line argument
		mainMenu(argv[1]);
	}else{
		// Show main menu
		mainMenu();
	}
	
	system("clear");
	cout << "Thanks for playing, " << username << "!" << ttyTextColor("reset") << endl;
	return 0;
}
