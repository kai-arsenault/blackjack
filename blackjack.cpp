// Must be compiled to support C++11
// Use the following
//    g++ -g -std=c++11 blackjack.cpp
#include <iostream>
#include <iterator>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

class Card{
	protected:
		string suit; // clubs, hearts, diamonds, spades
		int value;
		int altValue;
		string cardName;
	public:
		Card(string suit, string cardName){
			this->suit = suit;
			this->cardName = cardName;
			if(cardName == "J" || cardName == "Q" || cardName == "K"){
				value = 10;
				altValue = 10;
			}
			else if(cardName == "A"){
				value = 11;
				altValue = 1;
			}
			else{
				value = stoi(cardName);
				altValue = stoi(cardName);
			}
		}
		string getSuit(){return suit;}
		int getValue(){return value;}
		int getAltValue(){return altValue;}
		string getCardName(){return cardName;}
		string printCard(){return cardName + suit;}
		bool isSame(Card card){
			if(card.getSuit() == this->getSuit() && card.getCardName() == this->getCardName())
				return true;
			return false;
		}
		~Card(){}

};

// Pile cannot be instantiated
class Pile{
	protected:
		list<Card> cards;
	public:
		void addCard(Card& card){
			cards.push_back(card);
		}
		Card removeCard(Card card){
			for(list<Card>::iterator card_i = cards.begin(); card_i != cards.end(); ++card_i){
				if(card.isSame(*card_i)){
					Card toDelete = *card_i;
					cards.erase(card_i);
					return toDelete;
				}
			}
		}
		~Pile(){}
};

class Deck: public Pile{
	protected:
		const static int suitNum = 4, cardTypeNum = 13;
		const string heart = "\xe2\x99\xa5";//https://www.fileformat.info/info/unicode/char/2665/index.htm
		const string spade = "\xe2\x99\xa0";//https://www.fileformat.info/info/unicode/char/2660/index.htm
		const string diamond = "\xe2\x99\xa6";//https://www.fileformat.info/info/unicode/char/2666/index.htm
		const string club = "\xe2\x99\xa3";//https://www.fileformat.info/info/unicode/char/2663/index.htm
		string suits[suitNum] = {club, heart, diamond, spade};
		string cardNames[cardTypeNum] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
	public:
		// Constructor creates a complete card deck (no jokers)
		Deck(){
			for(int i = 0; i < suitNum; i++){
				for(int j = 0; i < cardTypeNum; j++){
					Card *card = new Card(suits[i], cardNames[j]);
					this->addCard(*card);
				}
			}
		}
		void shuffle(){
			cards.random_shuffle(cards.begin(), cards.end());
		}
		Card draw(){
			list<Card>::iterator toDelete = cards.begin();
			cards.erase(toDelete);
			return *toDelete;
		}
		// changed from void deal(list<User> &activePlayers){
		void deal(list<Pile>& activePlayers){
			int size = activePlayers.size();
			for(int i = 0; i < 2; i++){
				// old for(int j = 0; j < size; j++){
				for(list<Pile> activePlayers_i = activePlayers.begin(); activePlayers_i != activePlayers.end(); ++activePlayers_i){
					activePlayers_i.addCard(draw());
					// iterator iter1=Deck.begin();
					// Deck *temp = Deck.begin()
					// activePlayers[j].addCard(temp);
					// Deck.erase(iter1);
				}
			}
			return;
		}
		void deal(Pile& dealer){
			for(int i=0;i<2;i++){
				dealer.addCard(draw());
				// iterator iter1=Deck.begin();
				// Deck *temp = Deck.begin();
				// Dealer[i].addCard(temp);
				// Deck.erase(iter1);
			}
			return;
		}
		~Deck(){}
};

class Player: public Pile{
	public:
		int currentPoints(){
			int total = 0;
			int altTotal = 0;
			for(auto card = cards.begin(); card != cards.end(); ++card){
				total += card->first->total;
				altTotal += card->first->altTotal;
			}
			if(total <= 21)
				return total;
			else
				return altTotal;
		}
		bool operator <(const Player &player) const{
			return currentPoints()<player.currentPoints();
		}
		bool isBust(){this->currentPoints() > 21 ? return true : return false;}
		void hit(Card& card){this->addCard(card);}
};

class Dealer: public Player{
	public:
		Dealer(){
			chips += 1000000;
		}
        bool play(int scoreToBeat){
			if(isBust())
				return false;
			else if(currentPoints()>=scoreToBeat)
				return true;
			else{
				this->hit();
				return play();
			}
        }
		Card showPublicHand(){
			list<Card>::iterator toShow = cards.begin();
			return toShow->printCard() + " ??\n";
		}
};

class User: public Player{
	private:
		string name;
		int chips = 500;
	public:
		User(string name){
			this->name = name
		}
		string getName(){return name;}
		double getChips(){return chips;}
		void setName(string name){this->name = name;}
		void getHand(){
			for(auto card = cards.begin(); card != cards.end(); ++card)
				cout << card->printCard();
			cout << endl;
		}
        bool play(string dealerHand){
			if (isBust())
				return false;
			else if (currentPoints() == 21)
				return true;
			else{
				while(true){
					cout << "Dealer: " << dealerHand << getName() << ": " << getHand() << "Current point count: " << this->currentPoints() << "\nHit? (y/n): ";
					char user;
					cin >> user;
					if(user == 'y' || user == 'Y'){
						this->hit();
						return play();
					}
					else if(user == 'n' || user == 'N'){
						return true;
					}
					else
						cout << "Error! Invalid input.\n"
				}
			}
        }
};

class Game{
	protected:
		// https://www.geeksforgeeks.org/list-cpp-stl/
		list<Player> activePlayers;
		Dealer dealer;
		int rounds;
		Deck deck;
	public:
		Game(int playerNumber){
			cout << "Welcome to blackjack\n";
			dealer = new Dealer();
			this->addPlayer(dealer); // Add dealer in first location
			playerNumber > 5 ? playerNumber = 5 : playerNumber = playerNumber; // Set maximum opponents to be 5
			playerNumber < 1 ? playerNumber = 1 : playerNumber = playerNumber; // Set minimum opponents to be 1
			string name;
			for(int i = 0; i < playerNumber; i++){
				cout << "Enter name: ";
				cin >> name;
				this->addPlayer(new User(name));
			}
			this->deck = new Deck();
		}
		int getRounds(){return rounds;}
		bool addPlayer(Player player){activePlayers.push_back(player);}
		void printPlayers(){ // Print all player names and chip count
			// https://stackoverflow.com/questions/22269435/how-to-iterate-through-a-list-of-objects-in-c/22269465
			for(list<User>::iterator user = activePlayers.begin(); user != activePlayers.end(); ++user)
				cout << "Player: " << user.getName() << "  |  $" << user.getChips() << endl;
		}
		void gameSummary(){
			cout << "Total rounds: " << getRounds() << endl;
			this->printPlayers();
		}
		void checkPlayers(){ // Check if any players have run out of chips
			for(list<User>::iterator user = activePlayers.begin(); user != activePlayers.end(); ++user){
				if(player.getChips() <= 0)
					activePlayers.erase(player);
			}
		}
		int getTopCurrentPoints(){
			for(list<Player>::iterator user = activePlayers.begin(); user != activePlayers.end(); ++user){
				if(user->currentPoints !> 21)
					return user->currentPoints;
			}
		}
		void newRound(){
			rounds++;
			this->checkPlayers();
			deck.shuffle();
			deck.deal(activePlayers);
			deck.deal(dealer);

			// All players play
			for(list<Player>::iterator user = activePlayers.begin(); user != activePlayers.end(); ++user){
				user->play(dealer.showPublicHand());
			}
			activePlayers = activePlayers.sort();
			dealer.play(getTopCurrentPoints());

			for(list<Player>::iterator player = activePlayers.begin(); player != activePlayers.end(); ++player){
				if(player->getName() != dealer->getName()){ // Do not compare dealer to itself
					if(player->isBust()){ // If player bust, they lose
						
					}
				}
			}
		}
};

int main(){
	Game game = new Game(2);
	char user;
	do{
		cout << "Enter P to play, S to check score, and Q to quit\n... ";
		cin >> user;
		switch(user){
			case "p":
			case "P":
				game.newRound();
				break;
			case "s":
			case "S":
				game.gameSummary();
				break;
			case "q":
			case "Q":
				break;
			default:
				cout << "Error! Invalid option.\n";
		}
	}(while user != "q" && user != "Q");
	return 0;
}
