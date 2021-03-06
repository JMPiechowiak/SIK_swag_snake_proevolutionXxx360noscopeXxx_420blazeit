import pygame
import socket
import sys

CAPTION = "Snake!"

# TO DO
# x wysylamie statusu "READY" po kliknieciu spacji, teraz dzieje sie to automatycznie
# x wyswietlanie komunikatow ("oczekiwanie na drugiego gracza", "wcisnij spacje by rozpoczac" ,"wygrana", "przegrana", "remis")
# -------- to na dole juz jak sie komus bedzie chcialo :P ------------
# x polaczenie i zbieranie pakietow w osobnym watku
#
#

class Control(object):
    def __init__(self, socket, data, x, y): #x, y resolution parameters
        #set variables
        self.screen = pygame.display.get_surface()
        self.done = False
        self.socket = socket
        self.keys = pygame.key.get_pressed()
        self.clock = pygame.time.Clock()
        #load from rec data
        self.segment_width = x/int(data[1])
        self.segment_height = y/int(data[2])
        self.state = data[0]#0-4
        self.direction = data[5]#0-3
        self.my_segments = [[int(data[3]),int(data[4])]]
        self.apple = [int(data[8]),int(data[9])]
        self.enemy_segments = [[int(data[6]),int(data[7])]]
        self.resolution = [x, y]
        self.ready = False

    def event_loop(self):
        for event in pygame.event.get():
            self.keys = pygame.key.get_pressed()
            if event.type == pygame.QUIT  or self.keys[pygame.K_ESCAPE]:
                self.done = True
            if self.keys[pygame.K_UP] and self.direction != '1':
                self.direction = '0'
            if self.keys[pygame.K_DOWN] and self.direction != '0':
                self.direction = '1'
            if self.keys[pygame.K_RIGHT] and self.direction != '3':
                self.direction = '2'
            if self.keys[pygame.K_LEFT] and self.direction != '2':
                self.direction = '3'

    def inputText(self, str, x, y, r, g, b):
        myfont = pygame.font.SysFont("monospace", 32)
        label = myfont.render(str, 1, (r, g, b))
        self.screen.blit(label, (x/2 - label.get_rect().width/2 , y/2 - label.get_rect().height/2))
        pygame.display.flip()

    def waitAnyKey(self):
        while True:
            for event in pygame.event.get():
                if event.type == pygame.KEYDOWN:
                    return

    def update(self):
        data = get_response(self.socket)
        self.state = data[0]

        if self.state == "2":
		    #READY?
            self.screen.fill((0, 0, 0))
            self.inputText("Press SPACE to start", self.resolution[0], self.resolution[1], 255, 255, 255)
            while True:
                for event in pygame.event.get():
                    if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                        self.socket.send("2")
                        self.ready = True
                        return

        elif self.state == "3":
            #GAME IS RUNING!
            self.snakes_update(data)
            self.socket.send(self.direction)

        elif self.state == "4":
            #END OF THE GAME, CHECK IF YOU ARE THE WINNER!
            # tutaj gra nie powinna tylko wypisywac i zamykac okno, tylko
            #wypisac na ekran wynik gry i poczekac na wcisniecie klawisza
            if data[1] == "0":
                self.screen.fill((0, 0, 0))
                self.inputText("YOU WON", self.resolution[0], self.resolution[1], 0, 255, 0)
                self.waitAnyKey()
            elif data[1] == "1":
                self.screen.fill((0, 0, 0))
                self.inputText("YOU LOSE", self.resolution[0], self.resolution[1], 255, 0, 0)
                self.waitAnyKey()
            elif data[1] == "2":
                self.screen.fill((0, 0, 0))
                self.inputText("IT'S A DRAW", self.resolution[0], self.resolution[1], 255, 255, 0)
                self.waitAnyKey()
            self.done = True;

    def snakes_update(self, data):
        self.my_segments.append([int(data[2]),int(data[3])])
        self.enemy_segments.append([int(data[5]),int(data[6])])
        if data[1] == '0':
            self.my_segments.pop(0)
        if data[4] == '0':
            self.enemy_segments.pop(0)
        self.apple = [int(data[7]),int(data[8])]

    def draw_segments(self):
        #draw all segments
        # segments = self.x, ymy_segments + self.enemy_segments
        for s in self.my_segments:
            pygame.draw.rect(self.screen, [1,155,64], (s[0]*self.segment_width,s[1]*self.segment_height,self.segment_width, self.segment_height),0)

        for s in self.enemy_segments:
            pygame.draw.rect(self.screen, [155,155,64], (s[0]*self.segment_width,s[1]*self.segment_height,self.segment_width, self.segment_height),0)
        #draw apple if it is possible
        if self.apple:
            pygame.draw.rect(self.screen, [255,0,2], (self.apple[0]*self.segment_width,self.apple[1]*self.segment_height,self.segment_width, self.segment_height),0)

    def draw(self):
        if self.ready:
            self.screen.fill([4,10,100])
            self.draw_segments()
            pygame.display.flip()
        else:
            self.screen.fill((0, 0, 0))

    def display_fps(self):
        caption = "{} - FPS: {:.2f}".format(CAPTION, self.clock.get_fps())
        pygame.display.set_caption(caption)

    def main_loop(self):
        while not self.done:
            self.draw()
            pygame.display.flip()
            self.event_loop()
            self.clock.tick(10)
            self.update()



def get_response(socket):
    #print "Get data"
    data = socket.recv(1024).split()
    return data

def get_config(filename):
    file = open(filename, 'rU')
    try:
		string = file.read()
    finally:
		file.close()

    list = string.split('\n')
    data = [x for x in list if "#" not in x]
    while '' in data:
		data.remove('')

    for i in range(0, len(data)):
		try:
			data[i] = int(data[i])
		except ValueError:
		    continue
    print data
    return data

def main():
    pygame.init()
    config_options = get_config('cfg.txt')
    #network settings
    s = socket.socket()
    host = config_options[0] # Get local machine name
    port = config_options[1]
    print "Try to connect " + host
    s.connect((host, port))

    data = get_response(s)
    print data
    s.send("1")
    #screen options
    gameDisplay = pygame.display.set_mode([config_options[2],config_options[3]])
    pygame.display.set_caption("Snake!")
    #initialize and start game
    Control(s, data, config_options[2], config_options[3]).main_loop()
    #close program
    s.close
    pygame.quit()
    sys.exit()


if __name__ == "__main__":
    main()
