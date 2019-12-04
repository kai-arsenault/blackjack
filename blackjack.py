#! /bin/python
import random

# Use for all class basics and inheritance syntax in python https://www.geeksforgeeks.org/inheritance-in-python/

# Author: Kai Arsenault and Daniel Arpide
class Card:
    def __init__(self, suit, cardName, value):
        self.suit = suit
        self.cardName = cardName
        self.value = value
        self.altValue = value
        if value == 11:
            self.altValue = 1
    def getSuit(self):
        return self.suit
    def getValue(self):
        return self.value
    def getAltValue(self):
        return self.altValue
    def getCardName(self):
        return self.cardName
    # Display card: card name followed by suit
    def getCard(self):
        return self.cardName + self.suit
    # Check if card is the same with the card being passed
    def isSame(self, card):
        if card.getSuit() == self.suit and card.getCardName() == self.cardName:
            return True
        return False

# Author: Kai Arsenault
# Pile is not made to be instantiated but is a base class for anything that uses a list of cards
class Pile:
    # Each Pile class has a list of cards
    def __init__(self):
        self.cards = []
    def addCard(self, card):
        self.cards.append(card)
    # Find if this Pile has the card passed, if it does delete it and return the deleted card
    def removeCard(self, cardToDelete):
        for i in range(len(cards)):
            if self.cards[i].isSame(cardToDelete):
                # https://www.geeksforgeeks.org/python-list-pop/
                return self.cards.pop(i)
        return None

# Author: Daniel Arpide
# Derived from Pile, is a stack of cards shared by all players, is instantiated as a full deck of cards
class Deck (Pile):
    #suits = ['♠', '♦', '♥', '♣']
    club = "\xe2\x99\xa3"
    spade = "\xe2\x99\xa0"
    heart = "\xe2\x99\xa5"
    diamond = "\xe2\x99\xa6"
    suits = [club, spade, heart, diamond]
    #Dictionary of all card values. Key - Name, Value - Points
    values = {'2' : 2, '3' : 3, '4' : 4, '5' : 5, '6' : 6, '7' : 7, '8' : 8, '9' : 9, '10' : 10, 'J' : 10, 'Q' : 10, 'K' : 10, 'A' : 11}
    # Constructor creates a full 52 card deck
    def __init__(self):
        # https://cardsoverflow.com/questions/5166473/inheritance-and-init-method-in-python
        Pile.__init__(self)
        for suit in self.suits:
            for value in self.values:
                self.addCard(Card(suit, value, self.values[value]))
    def shuffle(self):
        # https://www.geeksforgeeks.org/python-ways-to-shuffle-a-list/
        random.shuffle(self.cards)
    # Draw a card from the top of the deck
    def draw(self):
        return self.cards.pop(0)
    # Deal 2 cards to each player in the list of players passed and to the dealer
    def deal(self,players, dealer):
        for i in range(2):
            for player in players:
                player.addCard(self.draw())
            dealer.addCard(self.draw())

# Author: Kai Arsenault
# Player class derived from Pile, not meant to be instantiated, used for User and Dealer
class Player (Pile):
    deck = None
    # Any player that is created must be "tied" to a deck. Each player has a deck it uses
    def __init__(self, deck):
        Pile.__init__(self)
        self.deck = deck
    # Find players total points, if points are over 21 check the alternate points (any Ace as 1 instead of 11)
    def getPoints(self):
        value = 0
        altValue = 0
        for card in self.cards:
            value += card.getValue()
            altValue += card.getAltValue()
        if value > 21:
            return altValue
        else:
            return value
    # Return each card in player's hand/all players cards
    def displayCards(self):
        hand = ''
        for card in self.cards:
            hand += card.getCard() + ' '
        return hand
    # Checks if players points are over 21, returns true or false accordingly
    def isBust(self):
        if self.getPoints() > 21:
            return True
        return False
    # Add card to players hand from deck
    def hit(self):
        card = self.deck.draw()
        self.addCard(card)
        return card
    # Empties user's hand putting the cards back into deck
    def putCardsBack(self):
        while len(self.cards) > 0:
            self.deck.addCard(self.cards.pop())

#Author: Kai Arsenault and Noah Damergi (sort of)
class Dealer (Player):
    # Dealer must have a deck
    def __init__(self, deck):
        Player.__init__(self, deck)
    # Get dealer's current score (how many points they have in their hand)
    def getCurrentScore(self):
        return self.currentScore
    # Dealer plays based off the score passed. They will keep hitting until the same or higher than the score past or until bust
    def play(self, topScore):
        # Checks if dealer has busted
        if(self.isBust()):
            print('Bust!\n')
            self.currentScore = self.getPoints()
            print('Dealer: ' + self.displayCards() + ' Dealer total: ' + str(self.getPoints()) + '\nDealer Bust!\n')
            return self.currentScore
        # Checks if dealer has beat or is the same as the top score
        elif(self.getPoints() >= topScore):
            self.currentScore = self.getPoints()
            print('Dealer: ' + self.displayCards() + ' Dealer total: ' + str(self.getPoints()) + '\n')
            return self.currentScore
        # If dealer is not bust and is under top score will hit and reruns function
        else:
            self.hit()
            self.play(topScore)
    # Return dealers hand, only displaying the second card
    def displayCards_protected(self):
        hand = ''
        for i in range(len(self.cards) - 1):
            hand += '?? '
        # https://www.geeksforgeeks.org/python-how-to-get-the-last-element-of-list/
        hand += self.cards[-1].getCard()
        return hand

#Author: Kai Arsenault
class User (Player):
    # User must have a deck
    def __init__(self, deck, name):
        Player.__init__(self, deck)
        self.chips = 500
        self.name = name
    # Get chip total for user
    def getChips(self):
        return self.chips
    def getName(self):
        return self.name
    def changeName(self, name):
        self.name = name
    # Get card total of current hand
    def getCurrentScore(self):
        return self.currentScore
    # User loses the amount they bet
    def lostBet(self):
        self.chips -= self.bet
        print(self.getName() + ' lost ' + str(self.bet) + ' chips. New total: ' + str(self.chips))
    # User wins the amount they bet
    def wonBet(self):
        self.chips += self.bet
        print(self.getName() + ' won ' + str(self.bet) + ' chips. New total: ' + str(self.chips))
    # User can place a bet, must be greater than 0 (must bet something) and must be less than chip total
    def placeBet(self):
        while True:
            print('Current chips:', self.chips)
            self.bet = input('Enter bet amount (only dollar amount, cannot bet cents)\n... ')
            self.bet = int(self.bet)
            if self.bet <= 0:
                print('Error! Bet must be greater than 0.')
            elif self.bet > self.chips:
                print('Error! Cannot bet more than you have.')
            else:
                break
    # Users plays until bust or has 21 (perfect score). Each round user has choice of whether to hit
    def play(self):
        if self.isBust():
            print('\nBust!')
            self.currentScore = self.getPoints()
            return self.getPoints()
        elif self.getPoints() == 21:
            print('\nGot 21, highest possible score.\n' + self.name + ':', self.displayCards(), 'Current total:', self.getPoints())
            self.currentScore = self.getPoints()
            return self.getPoints()
        else:
            print('\n' + self.name + ':', self.displayCards(), 'Current total:', self.getPoints())
            while True:
                # Gives option to hit
                choice = input('Hit (y/n)? ')
                if choice == 'y' or choice == 'Y':
                    cardAdded = self.hit()
                    # Shows card that was picked up
                    print('Picked up:', cardAdded.getCard())
                    return self.play()
                elif choice == 'n' or choice == 'N':
                    self.currentScore = self.getPoints()
                    return self.getPoints()
                else:
                    print('Error! Invalid input.')

# Author: Kai Arsenault
class Game:
    # List of all users that are playing
    users = []
    # Sets rounds to 0, creates a deck, dealer, and users based on how many people are playing
    def __init__(self):
        print('Welcome to Blackjack.')
        self.rounds = 0
        self.deck = Deck()
        self.dealer = Dealer(self.deck)
        self.getUserNum()
        for i in range(int(self.userNum)):
            name = input('Enter name.\n... ')
            self.users.append(User(self.deck, name))
    # Check to see how many users are playing, must be between 1 and 6
    def getUserNum(self):
        self.userNum = input('Enter number of players (1-6 players allowed).\n... ')
        while int(self.userNum) < 1 or int(self.userNum) > 6:
            self.userNum = input('Error! Number of players must be between 1 and 6.\n... ')
    # Find how many rounds have been played
    def getRounds(self):
        return self.rounds
    # Get leaderboard, displays all players (still in the game) in order by number of chips
    def getScoreBoard(self):
        scoreBoard = '\nLeaderboard:\n'
        sortedUsers = sorted(self.users, key=lambda user: user.getChips())
        for i in range(len(sortedUsers)):
            scoreBoard += '#' + str(i + 1) + ' ' + sortedUsers[i].getName() + ' || $' + str(sortedUsers[i].getChips()) + '\n'
        return scoreBoard
    # Displays leaderboard with total rounds printed. Used at the end of the game
    def getGameSumary(self):
        gameSummary = '\n----------\nEND\n----------\nTotal rounds: ' + str(self.rounds)
        gameSummary += self.getScoreBoard()
        return gameSummary
    # Checks all users to see if they still have chips, if they are out, they are removed from game
    def checkUsers(self):
        usersToDelete = []
        for i in range(len(self.users)):
            if self.users[i].getChips() <= 0:
                usersToDelete.append(i)
        for userIndex in usersToDelete:
            self.users.pop(userIndex)
    # Simulates a round of blackjack
    def newRound(self):
        # Start of round sequence: add round, remove users out of chips, shuffle the deck, and deal cards
        self.rounds += 1
        self.checkUsers()
        self.deck.shuffle()
        self.deck.deal(self.users, self.dealer)
        # All users play: all users play, display dealers protected hand (just second card)
        for user in self.users:
            print('\n----------\n' + user.getName() + "'s turn.")
            user.placeBet()
            print('\nDealer:', self.dealer.displayCards_protected())
            user.play()
        # Get user top score: find the top score among the users that is not bust
        topScore = 0
        for user in self.users:
            if user.getCurrentScore() <= 21 and user.getCurrentScore() > topScore:
                topScore = user.getCurrentScore()
        # Dealer plays: plays based on top score
        self.dealer.play(topScore)
        # Settle bets: all users that have won or lost recieve or lose chips repectively
        for user in self.users:
            # If dealer wins
            if user.isBust() or (user.getCurrentScore() < self.dealer.getCurrentScore() and not self.dealer.isBust()):
                user.lostBet()
            else:
                user.wonBet()
        # End of round sequence: print updated score board with all players chip totals, all players put cards back
        print(self.getScoreBoard())
        for user in self.users:
            user.putCardsBack()
        self.dealer.putCardsBack()

def main():
    game = Game()
    # Player has the option to quit or to play again, will repeat until players quit
    while True:
        choice = input('\nOptions:\n  1 - Play hand\n  2 - Quit game\n... ')
        choice = int(choice)
        if choice == 1:
            game.newRound()
        elif choice == 2:
            print(game.getGameSumary())
            break
        else:
            print('Error! Invalid input.')

if __name__ == '__main__':
    main()