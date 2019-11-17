#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <list>

using namespace std;

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
			for(i = activePlayers.begin(); i != activePlayers.end(); ++i)
				cout << "Player: " << i->first << "  |  $" << i->second.getChips() << endl;
		}
};

class Card{
	protected:
		string suit; // clubs, hearts, diamonds, spades
		int value;
		int altValue;
		stirng cardName;
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
		getSuit(){return suit;}
		getValue(){return value;}
		getAltValue(){return altValue;}
		getCardName(){return cardName;}
		void print(){
			// TODO: print suit symbols
			cout << cardName << " " << suit << endl;
		}
		bool isSame(Card card){
			if(card.getSuit() == this.getSuit() && card.getCardName() == this.getCardName())
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
		void addCard(Card card){
			cards.insert({card, card.getCardName()});
		}
		Card removeCard(Card card){
			for(i = cards.begin(); i != cards.end(); ++i){
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
		suits["Clubs", "Hearts", "Diamonds", "Spades"];
		cardNames["2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"];
	public:
		// Constructor creates a complete card deck (no jokers)
		Deck(){
			for(int i = 0; i < suits.size(); i++){
				for(int j = 0; i < cardName.size(); j++){
					this->addCard(new Card(suits[i], cardName[j]);
				}
			}
		}
		// TODO: add shuffle		
		void shuffle(){}
		~Deck(){}
};

class Player: public Pile{
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
