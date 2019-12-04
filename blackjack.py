#! /bin/python
import random

# Use for all class basics https://www.geeksforgeeks.org/inheritance-in-python/
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
    def getCard(self):
        return self.cardName + self.suit
    def isSame(self, card):
        if card.getSuit() == self.suit and card.getCardName() == self.cardName:
            return True
        return False

class Pile:
    def __init__(self):
        self.cards = []
    def addCard(self, card):
        self.cards.append(card)
    def removeCard(self, cardToDelete):
        for i in range(len(cards)):
            if self.cards[i].isSame(cardToDelete):
                # https://www.geeksforgeeks.org/python-list-pop/
                return self.cards.pop(i)
        return None
                
class Deck (Pile):
    suits = ['♠', '♦', '♥', '♣']
    #Dictionary of all card values. Key - Name, Value - Points
    values = {'2' : 2, '3' : 3, '4' : 4, '5' : 5, '6' : 6, '7' : 7, '8' : 8, '9' : 9, '10' : 10, 'J' : 10, 'Q' : 10, 'K' : 10, 'A' : 11}
    def __init__(self):
        # https://cardsoverflow.com/questions/5166473/inheritance-and-init-method-in-python
        Pile.__init__(self)
        for suit in self.suits:
            for value in self.values:
                self.addCard(Card(suit, value, self.values[value]))
    def shuffle(self):
        # https://www.geeksforgeeks.org/python-ways-to-shuffle-a-list/
        random.shuffle(self.cards)
    def draw(self):
        return self.cards.pop(0)
    def deal(self,players, dealer):
        for i in range(2):
            for player in players:
                player.addCard(self.draw())
            dealer.addCard(self.draw())

class Player (Pile):
    deck = None
    def __init__(self, deck):
        Pile.__init__(self)
        self.deck = deck
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
    def displayCards(self):
        hand = ''
        for card in self.cards:
            hand += card.getCard() + ' '
        return hand
    def isBust(self):
        if self.getPoints() > 21:
            return True
        return False
    def hit(self):
        card = self.deck.draw()
        self.addCard(card)
        return card
    def putCardsBack(self):
        while len(self.cards) > 0:
            self.deck.addCard(self.cards.pop())

class Dealer (Player):
    def __init__(self, deck):
        Player.__init__(self, deck)
    def getCurrentScore(self):
        return self.currentScore
    def play(self, topScore):
        if(self.isBust()):
            print('Bust!\n')
            self.currentScore = self.getPoints()
            print('Dealer: ' + self.displayCards() + ' Dealer total: ' + str(self.getPoints()) + '\nDealer Bust!\n')
            return self.currentScore
        elif(self.getPoints() >= topScore):
            self.currentScore = self.getPoints()
            print('Dealer: ' + self.displayCards() + ' Dealer total: ' + str(self.getPoints()) + '\n')
            return self.currentScore
        else:
            self.hit()
            self.play(topScore)
    def displayCards_protected(self):
        hand = ''
        for i in range(len(self.cards) - 1):
            hand += '?? '
        # https://www.geeksforgeeks.org/python-how-to-get-the-last-element-of-list/
        hand += self.cards[-1].getCard()
        return hand

class User (Player):
    chips = 500
    def __init__(self, deck, name):
        Player.__init__(self, deck)
        self.chips = 500
        self.name = name
    def getChips(self):
        return self.chips
    def getName(self):
        return self.name
    def changeName(self, name):
        self.name = name
    def getCurrentScore(self):
        return self.currentScore
    def lostBet(self):
        self.chips -= self.bet
        print(self.getName() + ' lost ' + str(self.bet) + ' chips. New total: ' + str(self.chips))
    def wonBet(self):
        self.chips += self.bet
        print(self.getName() + ' won ' + str(self.bet) + ' chips. New total: ' + str(self.chips))
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
                choice = input('Hit (y/n)? ')
                if choice == 'y' or choice == 'Y':
                    cardAdded = self.hit()
                    print('Picked up:', cardAdded.getCard())
                    return self.play()
                elif choice == 'n' or choice == 'N':
                    self.currentScore = self.getPoints()
                    return self.getPoints()
                else:
                    print('Error! Invalid input.')

class Game:
    users = []
    def __init__(self):
        print('Welcome to Blackjack.')
        self.rounds = 0
        self.deck = Deck()
        self.dealer = Dealer(self.deck)
        self.getUserNum()
        for i in range(int(self.userNum)):
            name = input('Enter name.\n... ')
            self.users.append(User(self.deck, name))
    def getUserNum(self):
        self.userNum = input('Enter number of players (1-6 players allowed).\n... ')
        while int(self.userNum) < 1 or int(self.userNum) > 6:
            self.userNum = input('Error! Number of players must be between 1 and 6.\n... ')
    def getRounds(self):
        return self.rounds
    def getScoreBoard(self):
        scoreBoard = '\nLeaderboard:\n'
        sortedUsers = sorted(self.users, key=lambda user: user.getChips())
        for i in range(len(sortedUsers)):
            scoreBoard += '#' + str(i + 1) + ' ' + sortedUsers[i].getName() + ' || $' + str(sortedUsers[i].getChips()) + '\n'
        return scoreBoard
    def getGameSumary(self):
        gameSummary = '\n----------\nEND\n----------\nTotal rounds: ' + str(self.rounds)
        gameSummary += self.getScoreBoard()
        return gameSummary
    def checkUsers(self):
        usersToDelete = []
        for i in range(len(self.users)):
            if self.users[i].getChips() <= 0:
                usersToDelete.append(i)
        for userIndex in usersToDelete:
            self.users.pop(userIndex)
    def newRound(self):
        # Start of round sequence
        self.rounds += 1
        self.checkUsers()
        self.deck.shuffle()
        self.deck.deal(self.users, self.dealer)
        # All users play
        for user in self.users:
            print('\n----------\n' + user.getName() + "'s turn.")
            user.placeBet()
            print('\nDealer:', self.dealer.displayCards_protected())
            user.play()
        # Get user top score
        topScore = 0
        for user in self.users:
            if user.getCurrentScore() <= 21 and user.getCurrentScore() > topScore:
                topScore = user.getCurrentScore()
        # Dealer play based on top score
        self.dealer.play(topScore)
        # Settle bets
        for user in self.users:
            # If dealer wins
            if user.isBust() or (user.getCurrentScore() < self.dealer.getCurrentScore() and not self.dealer.isBust()):
                user.lostBet()
            else:
                user.wonBet()
        # End of round sequence
        print(self.getScoreBoard())
        for user in self.users:
            user.putCardsBack()
        self.dealer.putCardsBack()

def main():
    game = Game()
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