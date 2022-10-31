######################
# Created By Akanate #
######################


import os
import time
class NoughtsAndCrosses:
    def __init__(self, board):
        self.board = [
            '1','2','3',
            '4','5','6',
            '7','8','9'
        ]
        for i in range(len(board)):
          if board[i] == '0': continue
          self.board[i] = board[i] == '1' and 'X' or 'O'
        self.turn = False
        self.score = None
        self.ai = 'O'   
        self.human = 'X'
        self.depthMin = 0
        self.depthMax = 0
        self.execution = None
    def run(self):
        return self.AI()
    def getBoard(self):
        return self.board


    def AI(self):
        #AI TRIES TO MAKE BEST MOVE POSSIBLE
        bestScore = float('-inf')
        self.depthMax = 0
        self.depthMin = 0
        bestMove = 0
        start_time = time.time()
        for i in range(len(self.board)):

            if self.board[i].isalpha():
                continue
            else:
                self.depthMax += 1
                self.board[i] = self.ai
                score = self.minimax(self.depthMax,False)
                if score > bestScore: bestScore = score; bestMove = i
                self.board[i] = str(i+1)
                self.depthMax+=1

        end_time = time.time()
        return bestMove
        self.board[bestMove] = self.ai
        self.execution = f'Calculation Time: {abs(start_time-end_time)}'
        self.turn = False
        self.drawGrid()

        
         
    def minimax(self,depth,returnValue):
        #MINIMAX ALGORITHM TO FIND OUT BEST MOVE TO PERFORM
        result = self.checkWinner()
        if result != None:
            return result
        if returnValue:
            bestScore = float('-inf')
            for i in range(len(self.board)):
                #CHECKS IF SPOT IS AVAILABLE ON THE BOARD AND THEN PERFORMS MINIMAX
                if self.board[i].isalpha():
                    continue
                else:
                    self.depthMax += 1
                    self.board[i] = self.ai
                    score = self.minimax(self.depthMax,False)
                    bestScore = max(score,bestScore)
                    self.board[i] = str(i+1)
            return bestScore
        else:
            bestScore = float('inf')
            for i in range(len(self.board)):
                #CHECKS IF A SPOT IS AVAILABLE ON THE BOARD AND THEN PERFORMS MINIMAX
                if self.board[i].isalpha():
                    continue
                else:
                    self.depthMin += 1
                    self.board[i] = self.human
                    score = self.minimax(self.depthMin,True)
                    bestScore = min(score,bestScore)
                    self.board[i] = str(i+1)
            return bestScore


    def checkWinner(self):
        #CHECKS IF X OR O HAS WON BY GOING ACROSS THE BOARD.
        if(self.board[0] == self.ai and self.board[1] == self.ai and self.board[2] == self.ai or self.board[3] == self.ai and self.board[4] == self.ai and self.board[5] == self.ai or self.board[6] == self.ai and self.board[7] == self.ai and self.board[8] == self.ai):
            return 1

        if(self.board[0] == self.human and self.board[1] == self.human and self.board[2] == self.human or self.board[3] == self.human and self.board[4] == self.human and self.board[5] == self.human or self.board[6] == self.human and self.board[7] == self.human and self.board[8] == self.human):
            return -1

        #CHECKS IF X OR O HAS WON GOING UP OR DOWN THE BOARD.
        if(self.board[0] == self.ai and self.board[3] == self.ai and self.board[6] == self.ai or self.board[1] == self.ai and self.board[4] == self.ai and self.board[7] == self.ai or self.board[2] == self.ai and self.board[5] == self.ai and self.board[8] == self.ai):
            return 1

        if(self.board[0] == self.human and self.board[3] == self.human and self.board[6] == self.human or self.board[1] == self.human and self.board[4] == self.human and self.board[7] == self.human or self.board[2] == self.human and self.board[5] == self.human and self.board[8] == self.human):
            return -1

        #CHECKS IF X OR O HAS WON VIA GOING DIAGNOL ON THE BOARD.
        if(self.board[0]  == self.ai and self.board[4]  == self.ai and self.board[8] == self.ai or self.board[2]  == self.ai and self.board[4]  == self.ai and self.board[6] == self.ai):
            return 1

        if(self.board[0] == self.human and self.board[4] == self.human and self.board[8] == self.human or self.board[2] == self.human and self.board[4] == self.human and self.board[6] == self.human):
            return -1
        #CHECKS FOR A DRAW BY SEEING IF THERE ARE ANY NUMBERS REMAINING
        e=0
        for i in self.board:
            if i.isdigit():
                continue
            else:
                e+=1
                if e==len(self.board):
                    return 0
        return None

def base3(n):
  sign = '-' if n<0 else ''
  n = abs(n)
  if n < 3:
     return str(n)
  s = ''
  while n != 0:
     s = str(n%3) + s
     n = n//3
  return sign+s

def run1():

  boards = []
  for i in range(3**9):
    
  
    boards.append('%09d' % int(base3(i)))
  
  def filt(board):
    if (board.count('0') == 0): return False
    if (abs(board.count('1') - board.count('2')) > 1): return False
    return True
  
  
  boards = list(filter(filt,  boards))
  
  for i in range(len(boards)):
    if boards[i] == None: continue
    for m in range(len(boards)):
      if (boards[m] == None): continue
      if (boards[i] == None): continue
      if boards[i].replace("1", "3").replace("2", "1").replace("3", "2") == boards[m]:
        boards[m] = None
  
  boards = list(filter(lambda x: x != None, boards))

  with open("outmid.txt", "w") as file:
    for i in boards:  
      file.write(i + '\n')
  #NoughtsAndCrosses()

def run2():
  win = [
    [0, 1, 2],
    [3, 4, 5],
    [6, 7, 8],
    [0, 3, 6],
    [1, 4, 7],
    [2, 5, 8],
    [0, 4, 8],
    [2, 4, 6],
  ]
  boards = []
  with open("outmid.txt", "r") as file:
    boards = file.read().split("\n")
  boards = boards[:-1]

  def filter2(board):
    for i in win:
      a = int(board[i[0]]) + int(board[i[1]]) + int(board[i[2]])
      if (a == 1 * 3 or a == 2 * 3):
        return False
    return True
  boards = tuple(filter(filter2, boards))
  with open("outmid2.txt", "w") as file:
    for i in boards:  
      file.write(i + '\n')


def run3():
  with open("outmid2.txt", "r") as file:
    boards = file.read().split("\n")
  boards = boards[:-1]
  for i in range(len(boards)):
    boards[i] += ", " + str(NoughtsAndCrosses(boards[i]).run())
    print(boards[i])#
  with open("outmid2.txt", "w") as file:
    for i in boards:  
      file.write(i + '\n')
run3()
print("done")