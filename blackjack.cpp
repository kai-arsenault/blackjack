#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <list>

using namespace std;

const char heart[] = "\xe2\x99\xa5";//https://www.fileformat.info/info/unicode/char/2665/index.htm
const char spade[] = "\xe2\x99\xa0";//https://www.fileformat.info/info/unicode/char/2660/index.htm
const char diamond[] = "\xe2\x99\xa6";//https://www.fileformat.info/info/unicode/char/2666/index.htm
const char club[] = "\xe2\x99\xa3";//https://www.fileformat.info/info/unicode/char/2663/index.htm

class Game{
	protected:
		// Maps and all functions associated with maps unless otherwise cited https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
		map<string, Player> activePlayers;	// Map - key:Player value:cash
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
		char suit; // clubs, hearts, diamonds, spades
		int value;
		int altValue;
		string cardName;
	public:
		Card(char suit, string cardName){
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
		char getSuit(){return suit;}
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
		char suits[suitNum] = {clubs, hearts, diamonds, spades};
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
		void shuffle(){
			iterator iter1=Deck.begin();
			iterator iter2=Deck.end();
			Deck.random_shuffle(iter1, iter2);
		}
		void deal(list<Player> &activePlayers){
			int size = activePlayers.size();
			for(int i=0;i<2;i++){
				for(int j=0;j<size;j++){
					iterator iter1=Deck.begin();
					Deck *temp = Deck.begin()
					activePlayers[j].addCard(temp);
					Deck.erase(iter1);
				}
			}
			return;
		}
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
