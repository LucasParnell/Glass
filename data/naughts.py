import os, random
import socket
from getkey import getkey, keys
from colorama import init
import threading
import time
import base64




init()

DEBUG_MODE = False #Shows what the AI is doing





class MySocket:
    def __init__(self, sock=None):
        if sock is None:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        else:
            self.sock = sock

    def connect(self, host, port):
        self.sock.connect((host, port))

    def mysend(self, msg, msglen):
        totalsent = 0
        while totalsent < msglen:
            sent = self.sock.send(msg[totalsent:])
            if sent == 0:
                raise RuntimeError("socket connection broken")
                totalsent = totalsent + sent

    def myreceive(self, msglen):
        chunks = []
        bytes_recd = 0
        while bytes_recd < msglen:
            chunk = self.sock.recv(min(msglen - bytes_recd, 2048))
            if chunk == b'':
                raise RuntimeError("socket connection broken")
            chunks.append(chunk)
            bytes_recd = bytes_recd + len(chunk)
        return b''.join(chunks)







clear = lambda: os.system('clear')  #Lambda function to clear screen (Linux)
#clear = lambda: os.system('cls')  #Lambda function to clear screen (Windows)


class bcolours:  #ANSI color codes for console output (wrapped using colorama for windows)
    BLUEH = "\u001b[44m"
    GREENH = "\u001b[42;1m"
    RESET = "\u001b[0m"
    CYAN = "\u001b[36;1m"
    


v_grid_arr_bk = [" "," "," ",      #Base grid array, contains Naughts and Crosses
                 " ", " ", " ",
                 " ", " "," "]



v_arr_x = 0  #Selection X coordinate
v_arr_y = 0  #Selection Y coordinate
v_set_grid = False #Boolean that is passed to the grid builder when space is pressed, places naught or cross.


plr_shape_lng = "" #Contains the long name for the players shape
plr_shape = ""    #Contains the Shape Character (X or 0)
cpu_shape = ""
winner = ""

if(random.randint(0,1) == 1): #There is a 50% chance of being either shape
  plr_shape_lng = "Crosses" #Set player shape to crosses
  plr_shape = "X"           # ^^^
  cpu_shape = "0"

else:
  plr_shape_lng = "Naughts"#Set player shape to Naughts
  plr_shape = "0"          #^^^
  cpu_shape = "X"


def buildGrid(arr_x, arr_y, set_grid): #Rebuild and print the grid

  grid_pos = v_arr_x + (v_arr_y * 3) #Variable to translate and store x and y coordinates as grid positions



  if(set_grid): #Check if the player wants to place a shape
    v_grid_arr_bk[grid_pos] = plr_shape #Set grid pos to player character in the base grid
  
  v_grid_arr = v_grid_arr_bk[:] #Copy the base grid to the function grid
                                #The function is what is displayed to the player

  v_grid_arr[grid_pos] = bcolours.BLUEH + v_grid_arr_bk[grid_pos] + bcolours.RESET #Display a blue square over the currently selected grid space

  #Print the grid to the screen:
  print( " " + v_grid_arr[0] + " | " + v_grid_arr[1] + " | " + v_grid_arr[2])
  print("---|---|---")
  print( " " + v_grid_arr[3] + " | " + v_grid_arr[4] + " | " + v_grid_arr[5]) 
  print("---|---|---")
  print( " " + v_grid_arr[6] + " | " + v_grid_arr[7] + " | " + v_grid_arr[8])

  return



def buildGridNA(): #buildGrid constructor without args for CPUs turn

  v_grid_arr = v_grid_arr_bk[:] #Copy the base grid to the function grid
                                #The function is what is displayed to the player

  #Print the grid to the screen:
  print( " " + v_grid_arr[0] + " | " + v_grid_arr[1] + " | " + v_grid_arr[2])
  print("---|---|---")
  print( " " + v_grid_arr[3] + " | " + v_grid_arr[4] + " | " + v_grid_arr[5]) 
  print("---|---|---")
  print( " " + v_grid_arr[6] + " | " + v_grid_arr[7] + " | " + v_grid_arr[8])

  return
  
def buildGridDEBUG(grid):
  v_grid_arr = grid[:]
  print()
  print( " " + v_grid_arr[0] + " | " + v_grid_arr[1] + " | " + v_grid_arr[2])
  print("---|---|---")
  print( " " + v_grid_arr[3] + " | " + v_grid_arr[4] + " | " + v_grid_arr[5]) 
  print("---|---|---")
  print( " " + v_grid_arr[6] + " | " + v_grid_arr[7] + " | " + v_grid_arr[8])
  return





def checkifwon(grid):

  
  for i in range(0, 3): #Check columns and rows


    #Check columns    
    if(grid[i] == grid[i + 3]) & (grid[i + 3] == grid[i + 6]): 
      if(grid[i] != " ") & (grid[i + 3] != " ") & (grid[i + 6] != " "):
        if(grid[i] == plr_shape): #Check one of the squares in the column / row to see who one
          winner = "Player"
        else:
          winner = "CPU"

        #Highlight the win
        grid[i] = bcolours.GREENH + grid[i] + bcolours.RESET
        grid[i+3] = bcolours.GREENH + grid[i+3] + bcolours.RESET
        grid[i+6] = bcolours.GREENH + grid[i+6] + bcolours.RESET
        clear()
        buildGridNA()
        
        return winner

    #Check rows

    if(i == 0):  
      ypos = 0 #Convert i into rows 
    elif(i==1):
      ypos = 3  #^^^
    elif(i==2):
      ypos = 6  #^^^

    if(grid[ypos] == grid[ypos + 1]) & (grid[ypos + 1] == grid[ypos + 2]):
      if(grid[ypos] != " ") & (grid[ypos + 1] != " ") & (grid[ypos + 2] != " "):


       if(grid[ypos] == plr_shape):
         winner = "Player"
       else:
         winner = "CPU"      
       
       grid[ypos] = bcolours.GREENH + grid[ypos] + bcolours.RESET
       grid[ypos + 1] = bcolours.GREENH + grid[ypos+1] + bcolours.RESET
       grid[ypos + 2] = bcolours.GREENH + grid[ypos+2] + bcolours.RESET
       clear()
       buildGridNA()
         
       return winner
        

 
    

  #Check diagonal top right to bottom left
  if(grid[2] == grid[4]) & (grid[4] == grid[6]):
    if(grid[2] != " ") & (grid[4] != " ") & (grid[6] != " "):
      if(grid[4] == plr_shape):
        winner = "Player"
      else:
        winner = "CPU"

      grid[2] = bcolours.GREENH + grid[2] + bcolours.RESET
      grid[4] = bcolours.GREENH + grid[4] + bcolours.RESET
      grid[6] = bcolours.GREENH + grid[6] + bcolours.RESET
      clear()
      buildGridNA()


      return winner

  #Check diagonal top left to bottom right
  if(grid[0] == grid[4]) & (grid[4] == grid[8]):
    if(grid[0] != " ") & (grid[4] != " ") & (grid[8] != " "):
      if(grid[4] == plr_shape):
        winner = "Player"
      else:
        winner = "CPU"

      grid[0] = bcolours.GREENH + grid[0] + bcolours.RESET
      grid[4] = bcolours.GREENH + grid[4] + bcolours.RESET
      grid[8] = bcolours.GREENH + grid[8] + bcolours.RESET
      clear()
      buildGridNA()


      return winner

  

  return ""

possibleMoves = [0,0,0,   #Array to store possible moves
                 0,0,0,
                 0,0,0]

def checkpossible(): #This builds an array of all squares that are vacant and also checks if a stalemate has been reached

  #Check possible moves
  for i in range(0,len(v_grid_arr_bk)):
    if(v_grid_arr_bk[i] != " "):
      possibleMoves[i] = 0
    else:
      possibleMoves[i] = 1 

#Check for a stalemate by counting the remaining moves using the possibleMoves array
  remainingmoves = 0

  for i in range(0, len(possibleMoves)):
    if(possibleMoves[i] == 1):
      remainingmoves += 1


  if(remainingmoves == 0):
    print(bcolours.CYAN + "Stalemate." + bcolours.RESET)
    quit()

  return possibleMoves #Return the array

def cpu_AILogic():
  possibleMoves = checkpossible() #updates the possibleMoves array

  AIgrid = v_grid_arr_bk[:] #Create a fake board that the CPU can test on

  for i in range(0,9): #Test every grid square
    if(possibleMoves[i]): #Check if it is possible to place a shape at i
      AIgrid[i] = cpu_shape #if it is, test i on the fake grid
      if(checkifwon(AIgrid) == "CPU"): #If a winning square is found, use it on the real board
        move_square = i

        if(DEBUG_MODE):
          print()
          print("DEBUG: Potential CPU win")
          buildGridDEBUG(AIgrid)
          getkey()

        return move_square
      
      else:
          AIgrid = v_grid_arr_bk[:]
          move_square = random.randint(0,len(possibleMoves)-1)
          while not (possibleMoves[move_square]):
           move_square = random.randint(0,len(possibleMoves)-1)



  for i in range(0,9): #This tries to intercept the player
    AIgrid = v_grid_arr_bk[:]
    if(possibleMoves[i]):
        AIgrid[i] = plr_shape
        if(checkifwon(AIgrid) == "Player"):
          move_square = i

          if(DEBUG_MODE):
            print()
            print("DEBUG: Potential player win")
            buildGridDEBUG(AIgrid)
            getkey()

          return move_square
        
        else:
          AIgrid = v_grid_arr_bk[:]
          move_square = random.randint(0,len(possibleMoves)-1)
          while not (possibleMoves[move_square]):
           move_square = random.randint(0,len(possibleMoves)-1)

  return move_square
  

def cpu_takeMove(): #This is the function that handles the AI logic

  move_square = cpu_AILogic()
  

  v_grid_arr_bk[move_square] = cpu_shape
  possibleMoves[move_square] = 0 #Update possible moves

  #-----------------------------------------------


  clear()


  buildGridNA() #Update the grid without passing any arguments
                #This is done because v_grid_arr_bk is manually updated

  print(bcolours.CYAN + "It is the CPUs turn" + bcolours.RESET) 

  winner = checkifwon(v_grid_arr_bk) #Check if the CPU has won
  if(winner!=""):
    print(winner + " wins.")
    quit()
  

  return

 #LEFT = b'K'
  #RIGHT = b'M'
  #UP = b'H'
  #DOWN = b'P'
  #SPACE = b' '
  #ENTER = b'\r'
  

  



def controls_menu():
  clear()
  print("Press the arrow keys to select a grid square")
  print("Press Space to place a mark on the grid")
  print("On the enemies turn press space to proceed")
  print()
  print("Press any key to return.")
  getkey()
  return



def main_menu():
  
  menu_txt = [" Singleplayer game"," Controls", " Host LAN Game", " Join LAN Game"]
  menu_txt_bk = menu_txt[:]


  sel = 0

  while (True):


      

    

    clear()
    menu_txt[sel] = "*" + menu_txt[sel]
    
    print(menu_txt[0])
    print(menu_txt[1])
    print(menu_txt[2])
    print(menu_txt[3])

    menu_txt = menu_txt_bk[:]
    a = getkey()

    if(a == keys.DOWN) & (sel < 3):
      sel += 1

    elif(a == keys.UP) & (sel > 0):
      sel-=1

    elif(a == keys.ENTER) & (sel == 0):
      return

    elif(a == keys.ENTER) & (sel == 1):
      controls_menu()

    elif (a == keys.ENTER) & (sel == 2):
      multiplayer.lan_host()
  return


  

class multiplayer:

  def lan_host():
    clear()

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    ip = s.getsockname()[0]
    s.close()

    ip_split = ip.split(".")
    code = base64.b64encode(bytes(ip_split[3], 'utf-8')).decode('utf-8')

    
    print("The game code is: '" + code + "'")

    waiting = True
    players_bk = 0
    players = 0
    while(waiting):
      if(players > players_bk):
        players_bk = players
        clear()
        print("The game code is: '" + code + "'")
        print("Players connected: " + str(players))
      
      


    getkey()
  






main_menu()
while (True): #Main game loop


  clear() #Clear screen at start of loop
  buildGrid(v_arr_x, v_arr_y, v_set_grid) #Build the grid using the given coordinates and whether or not space has been pressed

  v_set_grid = False #Make sure set_grid is not True


  print(bcolours.CYAN + "You are " + plr_shape_lng + bcolours.RESET) #Print the players shape

  print(bcolours.CYAN + "It is your turn" + bcolours.RESET)

  key = getkey() #Get key input
  
  if (key==keys.LEFT) & (v_arr_x > 0):  #If the left arrow key is pressed
      v_arr_x -= 1
  elif (key==keys.RIGHT) & (v_arr_x < 2): #If the right arrow key is pressed
      v_arr_x += 1
  elif (key==keys.DOWN) & (v_arr_y < 2): #If the down arrow key is pressed
      v_arr_y += 1

  elif (key==keys.UP) & (v_arr_y > 0): #If the up arrow key is pressed
      v_arr_y -= 1

  elif(key==keys.SPACE): #if space is pressed make set_grid True and end the playerturn

    clear()
    v_set_grid = True                                  #Initially v_set_grid is set to true

    if not (checkpossible()[v_arr_x + (v_arr_y * 3)]): # If this is not possible however
      v_set_grid = False                               # it will be set back to False and
      continue                                         # the program will continue
      



    buildGrid(v_arr_x, v_arr_y, v_set_grid) #Build the grid


    winner = checkifwon(v_grid_arr_bk) #Check if player has won
    if(winner!=""):
      print(winner + " wins.")
      quit()

    #CPU takes move

    cpu_takeMove() #CPU takes its move
    getkey() #pause


      
  








  
