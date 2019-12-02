// TODO: read the following
// Use the following
//    g++ -g -std=c++11 blackjack.cpp
//
// TODO: add a remove for all news
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
		void addCard(Card card){
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
			random_shuffle(cards.begin(), cards.end());
		}
		Card draw(){
			list<Card>::iterator toDelete = cards.begin();
			cards.erase(toDelete);
			return *toDelete;
		}
		void deal(list<Pile>& activePlayers){
			int size = activePlayers.size();
			for(int i = 0; i < 2; i++){
				for(list<Pile>::iterator activePlayers_i = activePlayers.begin(); activePlayers_i != activePlayers.end(); ++activePlayers_i){
					activePlayers_i->addCard(draw());
				}
			}
			return;
		}
		void deal(Pile& dealer){
			for(int i=0;i<2;i++){
				dealer.addCard(draw());
			}
			return;
		}
		~Deck(){}
};

class Player: public Pile{
	private:
		static Deck deck;
		int currentPoints;
	public:
		int getCurrentPoints() const{return currentPoints;}
		int calculateCurrentPoints(){
			int value = 0;
			int altValue = 0;
			for(auto card = cards.begin(); card != cards.end(); ++card){
				value += card->getValue();
				altValue += card->getAltValue();
			}
			if(value <= 21){
				currentPoints = value;
				return value;
			}
			else{
				currentPoints = altValue;
				return altValue;
			}
		}
		inline bool operator <(const Player &player) const{
			return this->getCurrentPoints() < player.getCurrentPoints();
		}
		bool isBust(){
			if(this->calculateCurrentPoints() > 21)
				return true;
			else
				return false;
		}
		void hit(){
			this->addCard(deck.draw());
			this->calculateCurrentPoints();
		}
};

class Dealer: public Player{
	public:
        bool play(int scoreToBeat){
			if(isBust())
				return false;
			else if(calculateCurrentPoints()>=scoreToBeat)
				return true;
			else{
				this->hit();
				return this->play(scoreToBeat);
			}
        }
		string showPublicHand(){
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
			this->name = name;
		}
		string getName(){return name;}
		double getChips(){return chips;}
		void setName(string name){this->name = name;}
		string getHand(){
			string hand = "";
			for(auto card = cards.begin(); card != cards.end(); ++card)
				hand += card->printCard() + " ";
			hand += "\n";
			return hand;
		}
        bool play(string dealerHand){
			if (isBust())
				return false;
			else if (calculateCurrentPoints() == 21)
				return true;
			else{
				while(true){
					string moveMessage = string("Dealer: " + dealerHand + " " + this->getName() + ": " + this->getHand() + "\nCurrent point count: " + string(this->calculateCurrentPoints()) + "\nHit? (y/n): ");
					cout << moveMessage;
					char user;
					cin >> user;
					if(user == 'y' || user == 'Y'){
						this->hit();
						return this->play(dealerHand);
					}
					else if(user == 'n' || user == 'N'){
						return true;
					}
					else
						cout << "Error! Invalid input.\n";
				}
			}
        }
};

class Game{
	protected:
		// https://www.geeksforgeeks.org/list-cpp-stl/
		list<Player> activePlayers;
		Dealer *dealer;
		int rounds;
		Deck *deck;
	public:
		Game(int playerNumber){
			Deck *deck = new Deck();
			this->deck = deck;
			cout << "Welcome to blackjack\n";
			Dealer *dealer = new Dealer();
			this->dealer = dealer;
			this->addPlayer(this->dealer); // Add dealer in first location
			playerNumber > 5 ? playerNumber = 5 : playerNumber = playerNumber; // Set maximum opponents to be 5
			playerNumber < 1 ? playerNumber = 1 : playerNumber = playerNumber; // Set minimum opponents to be 1
			string name;
			for(int i = 0; i < playerNumber; i++){
				cout << "Enter name: ";
				cin >> name;
				User *user = new User(name);
				this->addPlayer(user);
			}
		}
		int getRounds(){return rounds;}
		bool addPlayer(Player &player){activePlayers.push_back(player);}
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
		int getTopcalculateCurrentPoints(){
			for(list<Player>::iterator user = activePlayers.begin(); user != activePlayers.end(); ++user){
				if(user->calculateCurrentPoints !> 21)
					return user->calculateCurrentPoints;
			}
		}
		void newRound(){
			rounds++;
			this->checkPlayers();
			deck.shuffle();
			deck.deal(activePlayers);
			deck.deal(dealer);

			// All players play
			for(list<User>::iterator user = activePlayers.begin(); user != activePlayers.end(); ++user){
				user->play(dealer.showPublicHand());
			}
			activePlayers = activePlayers.sort();
			dealer.play(getTopcalculateCurrentPoints());

			// for(list<User>::iterator user = activePlayers.begin(); user != activePlayers.end(); ++user){
			// 		if(user->isBust()){ // If user bust, they lose
						
			// 		}
			}
		}
		~Game(){

		}
};

int main(){
	Game *game = new Game(2);
	int user;
	do{
		cout << "Enter 1 to play, 2 to check score, and 0 to quit\n... ";
		cin >> user;
		switch(user){
			case 1:
				game->newRound();
				break;
			case 2:
				game->gameSummary();
				break;
			case 0:
				break;
			default:
				cout << "Error! Invalid option.\n";
		}
	}while (user != 0);
	return 0;
}
