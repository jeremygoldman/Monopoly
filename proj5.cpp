#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

//author: Jeremy Goldman

//class for spaces on the Monopoly board, contains all necessary properties
// i.e. color, name, price, location, current owner, houses on property, rent
class BoardSpace {
public:
	BoardSpace();
	BoardSpace(int newPrice, string newColor, string newName);
	void SetColor(string newColor);
	void SetName(string newName);
	void SetPrice(int newPrice);
	int GetPrice();
	string GetColor();
	string GetName();
	int GetOwner();
	void SetOwner(int playerIndex);
	void AddHouse();
	void ClearHouses();
	int GetHouses();
	int GetRent();
private:
	int location;
	string color;
	string name;
	int price;
	int owner;
	int houses;
	int rent;
};

BoardSpace::BoardSpace() {
	color = "";
	name = "";
	price = 0;
	owner = -1;
	houses = 0;
	rent = 0;
}

BoardSpace::BoardSpace(int newPrice, string newColor, string newName) {
	price = newPrice;
	color = newColor;
	name = newName;
	owner = -1;
	houses = 0;
	rent = static_cast<int>(newPrice * 0.1);
}
//returns rent of space
int BoardSpace::GetRent() {
	return rent;
}
//returns number of houses on space
int BoardSpace::GetHouses() {
	return houses;
}
//adds a house on space
void BoardSpace::AddHouse() {
	houses++;
}
//gets rid of all houses on space
void BoardSpace::ClearHouses() {
	houses = 0;
}
//sets color of space
void BoardSpace::SetColor(string newColor) {
	color = newColor;
}
//sets name of space
void BoardSpace::SetName(string newName) {
	name = newName;
}
//sets price of space
void BoardSpace::SetPrice(int newPrice) {
	price = newPrice;
}
//returns price of space
int BoardSpace::GetPrice() {
	return price;
}
//returns color of space
string BoardSpace::GetColor() {
	return color;
}
//returns name of space
string BoardSpace::GetName() {
	return name;
}
//returns index of owner of space
int BoardSpace::GetOwner() {
	return owner;
}
//sets owner of space
void BoardSpace::SetOwner(int playerIndex) {
	owner = playerIndex;
}

//class for players in game, contains all necessary properties
//  i.e. location, cash balance, indices of owned properties, 
//  property threshold, threshold for building a house, and 
//  whether the player is bankrupt or not
class Player {
public:
	Player();
	Player(int newCashBalance, int newLocation);
	void SetCashBalance(int newBalance);
	int GetCashBalance();
	int GetLoc();
	void SetLoc(int newLoc);
	double GetPropertyThreshold();
	void SetPropertyThreshold(double newPropThresh);
	int GetBuildThreshold();
	void SetBuildThreshold(int newBuildThresh);
	void AddOwnedPropertyIndex(int index);
	vector<int> GetOwnedPropertyIndices();
	bool IsBankrupt();
	void SetIsBankrupt(bool is);
private:
	int location;
	int cashBalance;
	vector<int> ownedPropertyIndices;
	double propertyThreshold;
	int buildThreshold;
	bool isBankrupt;
};

Player::Player() {
	cashBalance = 0;
	location = 0;
	isBankrupt = false;
}

Player::Player(int newCashBalance, int newLocation) {
	cashBalance = newCashBalance;
	location = newLocation;
	isBankrupt = false;
}
//returns property threshold of player
double Player::GetPropertyThreshold() {
	return propertyThreshold;
}
//sets property threshold of player
void Player::SetPropertyThreshold(double newPropThresh) {
	propertyThreshold = newPropThresh;
}
//returns threshold to build house
int Player::GetBuildThreshold() {
	return buildThreshold;
}
//sets threshold to build house
void Player::SetBuildThreshold(int newBuildThresh) {
	buildThreshold = newBuildThresh;
}
//sets location on board of player
void Player::SetLoc(int newLoc) {
	location = newLoc;
}
//returns location on board of player
int Player::GetLoc() {
	return location;
}
//sets cash balance
void Player::SetCashBalance(int newBalance) {
	cashBalance = newBalance;
}
//returns cash balance of player
int Player::GetCashBalance() {
	return cashBalance;
}
//adds index of a property to vector of properties owned by player
void Player::AddOwnedPropertyIndex(int index) {
	ownedPropertyIndices.push_back(index);
}
//returns vector of indices of properties owned by player
vector<int> Player::GetOwnedPropertyIndices() {
	return ownedPropertyIndices;
}
//sets if player is bankrupt or not
void Player::SetIsBankrupt(bool is) {
	isBankrupt = is;
}
//returns if player is bankrupt or not
bool Player::IsBankrupt() {
	return isBankrupt;
}
//checks if there's a monopoly on a certain color or not (for calculating rent)
bool CheckIfMonopoly(vector<BoardSpace> spaces, string color, int index) {
	bool monopoly = true;
	for (int j = 0; j<spaces.size(); j++) {
		if (spaces.at(j).GetColor() == color) {
			if (spaces.at(j).GetOwner() != index) {
				monopoly = false;
			}
		}
	}
	return monopoly;
}
//determines what course of action to take for all players depending on 
//  the properties of the space on which they land.
int MakeMove(vector<Player>& players, 
	vector<BoardSpace>& boardSpaces, vector<int> cards, 
	vector<int>& playersRemoved, int& currentSpace) {
	
	int moves = 0;
	for (int i = 0; i<players.size(); i++) {
		moves = cards.at(currentSpace);
		currentSpace++;
		if (currentSpace == cards.size()) {
			currentSpace = 0;
		}
		if (!players.at(i).IsBankrupt()) {
			players.at(i).SetLoc(players.at(i).GetLoc()+moves);
			if (players.at(i).GetLoc() == boardSpaces.size()) {
				//if space player lands on is GO
				cout << "Player " << i << " moves " << moves 
				<< " step(s) to GO and receives $2000." << endl;
				players.at(i).SetCashBalance(players.at(i).GetCashBalance()+2000);
			} else {
				//if space player lands on is not GO
				if (players.at(i).GetLoc() > boardSpaces.size()-1) {
					//if player goes over amount of total # spaces on board, wraps around to beginning
					players.at(i).SetLoc(players.at(i).GetLoc()-boardSpaces.size());
				}
				if (boardSpaces.at(players.at(i).GetLoc()).GetName() == "Parking") {
					//if space player lands on is parking
					cout << "Player " << i << " moves " << moves 
					<< " step(s) to Parking and stays." << endl;
				} else {
					//if space player lands on is not parking or GO
					int currentSpaceOwner = boardSpaces.at(players.at(i).GetLoc()).GetOwner();
					if (currentSpaceOwner == -1) {
						//if no one owns property
						if (players.at(i).GetCashBalance() >= players.at(i).GetPropertyThreshold() 
							* boardSpaces.at(players.at(i).GetLoc()).GetPrice()) {
							//if player can afford to buy property
							players.at(i).AddOwnedPropertyIndex(players.at(i).GetLoc());
							players.at(i).SetCashBalance(players.at(i).GetCashBalance()-
								boardSpaces.at(players.at(i).GetLoc()).GetPrice());
							cout << "Player " << i << " moves " << moves << " step(s) to " 
							<< boardSpaces.at(players.at(i).GetLoc()).GetName();
							cout << " and purchases " 
							<< boardSpaces.at(players.at(i).GetLoc()).GetName() << "." << endl;
							boardSpaces.at(players.at(i).GetLoc()).SetOwner(i);

						} else {
							//if player can't afford to buy property
							cout << "Player " << i << " moves " << moves << " step(s) to " 
							<< boardSpaces.at(players.at(i).GetLoc()).GetName();
							cout << " and stays." << endl;
						}
					} else if (currentSpaceOwner == i) {
						//player owns space
						if (players.at(i).GetCashBalance() > 1000 && 
							boardSpaces.at(players.at(i).GetLoc()).GetHouses() < 2) {
							//if can buy house & there are less than 2 houses already built
							cout << "Player " << i << " moves " << moves << " step(s) to " 
						<< boardSpaces.at(players.at(i).GetLoc()).GetName();
							cout << " and builds house number " 
							<< boardSpaces.at(players.at(i).GetLoc()).GetHouses()+1 << "." << endl;
							players.at(i).SetCashBalance(players.at(i).GetCashBalance()-1000);
							boardSpaces.at(players.at(i).GetLoc()).AddHouse();
						} else {
							//if can't buy house or there are 2 houses already built
							cout << "Player " << i << " moves " << moves << " step(s) to " 
							<< boardSpaces.at(players.at(i).GetLoc()).GetName();
							cout << " and stays." << endl;
						}
					} else {
						//house is owned by another player
						//calculate rent
						int rent = boardSpaces.at(players.at(i).GetLoc()).GetRent();
						for (int j = 0; j<boardSpaces.at(players.at(i).GetLoc()).GetHouses(); j++) {
							rent *= 2;
						}
						//check if monopoly
						if (CheckIfMonopoly(boardSpaces, boardSpaces.at(players.at(i).GetLoc())
							.GetColor(), currentSpaceOwner)) {
							rent *= 2;
						}
						if (players.at(i).GetCashBalance() >= rent) {
							//can pay rent
							cout << "Player " << i << " moves " << moves << " step(s) to " 
							<< boardSpaces.at(players.at(i).GetLoc()).GetName();
							cout << " and pays $" << rent << " rent to Player " 
							<< currentSpaceOwner << "." << endl;
							players.at(i).SetCashBalance(players.at(i).GetCashBalance()-rent);
							players.at(currentSpaceOwner).SetCashBalance(
								players.at(currentSpaceOwner).GetCashBalance()+rent);
						} else {
							//cant pay rent
							cout << "Player " << i << " moves " << moves << " step(s) to " 
							<< boardSpaces.at(players.at(i).GetLoc()).GetName();
							cout << " and bankrupt, transfers property to Player " 
							<< currentSpaceOwner << "." << endl;
							vector<int> propertiesToBeTransferred = 
							players.at(i).GetOwnedPropertyIndices();
							int cashToBeTransferred = players.at(i).GetCashBalance();
							//bankrupt current player
							players.at(i).SetCashBalance(0);
							//transfer money to player that owns space
							players.at(currentSpaceOwner).SetCashBalance(
								players.at(currentSpaceOwner).GetCashBalance()+cashToBeTransferred);
							//transfer properties to player that owns space
							for (int k = 0; k<propertiesToBeTransferred.size(); k++) {
								players.at(currentSpaceOwner).AddOwnedPropertyIndex(
									propertiesToBeTransferred.at(k));
							}
							playersRemoved.push_back(i);
							players.at(i).SetIsBankrupt(true);
						}
					}
				}	
			}
		}
	}
	return currentSpace;
}
//outputs info about player at end of the game 
void OutputInfo(Player tempPlayer) {
	if (tempPlayer.IsBankrupt()) {
		cout << "Bankrupt and out of game." << endl;
	} else {
		cout << "Cash Balance: $" << tempPlayer.GetCashBalance() << endl;
		if (tempPlayer.GetOwnedPropertyIndices().size() > 0) {
			cout << "Number of Purchased Properties: " 
			<< tempPlayer.GetOwnedPropertyIndices().size() << endl;
		} else {
			cout << "No purchased property." << endl;
		}
		//cout << "Property Threshold: " << tempPlayer.GetPropertyThreshold() << endl;
		//cout << "House threshold: " << tempPlayer.GetBuildThreshold() << endl;
	}
}

int main() {
	ifstream locationFS;
	ifstream cardFS;
	ifstream infoFS;
	string spaceFileName = "";
	cin >> spaceFileName;
	locationFS.open(spaceFileName.c_str());

	string cardFileName = "";
	cin >> cardFileName;
	cardFS.open(cardFileName.c_str());
	int startingCashBalance = 0;

	vector<Player> players;
	string infoFileName = "";
	cin >> infoFileName;
	infoFS.open(infoFileName.c_str());
	istringstream infoSS;

	int numRounds = 0;
	cin >> numRounds;
	//checks if files are open and usable
	if (cardFS.is_open() && locationFS.is_open() && infoFS.is_open()) {
		
		cardFS >> startingCashBalance;
		//create vector of board spaces
		vector<BoardSpace> boardSpaces;
		string tempName = "";
		string tempColor = "";
		int tempPrice = 0;
		bool improperInputs = false;
		while (locationFS.good()) {
			locationFS >> tempName;
			locationFS >> tempPrice;
			locationFS >> tempColor;
			//checks if price board space are all positive
			if (tempPrice >= 0) {
				//checks if board spaces all have correct colors
				if (tempColor == "RED" || tempColor == "BLUE" || tempColor == "GREEN" 
					|| tempColor == "YELLOW" || tempColor == "BLACK" || tempColor == "NONE") {
					
					BoardSpace tempSpace(tempPrice, tempColor, tempName);
					boardSpaces.push_back(tempSpace);
				} else {
					improperInputs = true;
				}
			} else {
				improperInputs = true;
			}
		}
		locationFS.close();
		boardSpaces.resize(boardSpaces.size()-1);

		//creates vector of players in game w/ info
		while (infoFS.good()) {
			string currentLine = "";
			getline(infoFS, currentLine);
			infoSS.clear();
			infoSS.str(currentLine);
			double tempPropThresh = 0;
			int tempBuildThresh = 0;
			infoSS >> tempPropThresh;
			infoSS >> tempBuildThresh;
			Player tempPlayer(startingCashBalance, 0);
			tempPlayer.SetPropertyThreshold(tempPropThresh);
			tempPlayer.SetBuildThreshold(tempBuildThresh);
			if (tempPropThresh != 0 && tempBuildThresh != 0) {
				players.push_back(tempPlayer);
			}
		}
		infoFS.close();

		//creates vector of moves for players to take
		vector<int> cards;
		int currMove = 0;
		while (!cardFS.fail()) {
			cardFS >> currMove;
			cards.push_back(currMove);
		}
		cards.resize(cards.size()-1);
		

		vector<int> playersRemoved;
		//if there are no errors with inputs
		if (!improperInputs) {
			int roundCounter = 0;
			cout << "***MONOPOLY GAME STARTS***" << endl;
			int currentSpace = 0;
			int numBankrupt = 0;
			//check to see that there is more than 1 player left in game
			bool oneLeft = false;
			while (roundCounter<numRounds && !oneLeft) {
				
				cout << "Round: " << roundCounter+1 << endl;

				currentSpace = MakeMove(players, boardSpaces, cards, 
					playersRemoved, currentSpace);
				roundCounter++;

				//count how many of the players are bankrupt
				numBankrupt = 0;
				for (int i = 0; i<players.size(); i++) {
					if (players.at(i).IsBankrupt()) {
						numBankrupt++;
					}
				}
				//if only one player left in game, end game after round is over.
				if (numBankrupt+1 == players.size()) {
					oneLeft = true;
				}
			}
			//end of game summary
			cout << "***SIMULATION RESULTS***" << endl;
			
			//find winner
			int indexOfWinner = 0;
			int maxWealth = 0;
			int tempWealth = 0;
			vector<int> tempOwnedPropertyIndices;

			for (int j = 0; j<players.size(); j++) {
				tempWealth += players.at(j).GetCashBalance();
				tempOwnedPropertyIndices = players.at(j).GetOwnedPropertyIndices();
				for (int k = 0; k<tempOwnedPropertyIndices.size(); k++) {
					tempWealth += boardSpaces.at(tempOwnedPropertyIndices.at(k)).GetPrice();
				}
				if (tempWealth > maxWealth && !players.at(j).IsBankrupt()) {
					maxWealth = tempWealth;
					indexOfWinner = j;
				}
				tempWealth = 0;
			}

			//output winner
			cout << "Player " << indexOfWinner << " wins the game with total asset value of $" 
			<< maxWealth << "." << endl;


			//output all players info
			cout << "***GAME SUMMARY***" << endl;
			for (int i = 0; i<players.size(); i++) {
				cout << "Player " << i << ":" << endl;
				OutputInfo(players.at(i));
			}
		//if there are errors with inputs
		} else {
			cout << "Improper inputs." << endl;
		}
	}

	return 0;
}




