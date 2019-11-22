#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <list>

using namespace std;

class Game{
	protected:
		// Maps and all functions associated with maps unless otherwise cited https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
		map<string, Player> activePlayers;	// Map - key:name value:Player
		int rounds;
	public:
		int getRounds(){return rounds;}
		void newRound(){rounds++;}
		bool removePlayer(string name){activePlayers.erase(name);}
		bool addPlayer(Player player){activePlayers.insert({player.getName(), player});}
		void printPlayers(){
			// Found auto keyword in link cited for maps
			// Starts at first entry of map and prints entry until the iterator equals the last entry
			for(auto i = activePlayers.begin(); i != activePlayers.end(); ++i)
				cout << "Player: " << i->first << "  |  $" << i->second.getChips() << endl;
		}
};

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
		void print(){
			// TODO: print suit symbols
			cout << cardName << " " << suit << endl;
		}
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
		map <Card, string> cards;
	public:
		void addCard(Card& card){
			cards.insert({card, card.getCardName()});
		}
		Card removeCard(Card card){
			for(auto i = cards.begin(); i != cards.end(); ++i){
				if(card.isSame(i->first)){
					Card toDelete = i->first;
					cards.erase(toDelete);
					return toDelete;
				}
			}
		}
		~Pile(){}
};

class Deck: public Pile{
	protected:
		const static int suitNum = 4, cardTypeNum = 13;
		string suits[suitNum] = {"Clubs", "Hearts", "Diamonds", "Spades"};
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
		// TODO: add shuffle		
		void shuffle(){}
		~Deck(){}
};

class Player: public Pile{
	protected:
		string name;
		double chips;
	public:
		Player(string name){
			this->setName(name);
			chips = 500.00;
		}
		string getName(){return name;}
		double getChips(){return chips;}
		void setName(string name){this->name = name;}
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
		bool isBust(){this->currentPoints() > 21 ? return true : return false;}
		void hit(Card& card){this->addCard(card);}
	// TODO
};

class Dealer: public Player{
	// TODO:
};

class User: public Player{
	// TODO:
};

class Bot: public Player{
	// TODO:
};

int main(){
	// TODO:
	return 0;
}
