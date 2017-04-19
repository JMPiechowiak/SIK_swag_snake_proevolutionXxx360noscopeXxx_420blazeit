# display_width = 800
# display_height = 800
#
# #network settings
# s = socket.socket()
# host = socket.gethostname() # Get local machine name
# port = 12345
#
# s.connect((host, port))
#
# #get width and height of the grid from server
# data = s.recv(1024).rstrip('\x00').split();
#
# segment_width = display_width/int(data[0])
# segment_height = display_height/int(data[1])
# s.send("1")
#
# data = s.recv(1024).rstrip('\x00').split();
# start_x = int(data[0]);
# start_y = int(data[1]);
#
#
# s.send("1")
# s.close
#
#
#
#
#
#
# pygame.init()
#
# gameDisplay = pygame.display.set_mode([display_width,display_height])
# pygame.display.set_caption("Snake!")
#
# all_sprites = pygame.sprite.Group()
#
# snake_segments = []
#
# segment = Segment(start_x, start_y)
# snake_segments.append(segment)
# all_sprites.add(segment)
#
# clock = pygame.time.Clock()
#
# runing = True;
#
# while runing:
#     for event in pygame.event.get():
#         if event.type == pygame.QUIT:
#             runing = False
#

#     clock.tick(5)
# pygame.quit()
import pygame
import socket
import sys

CAPTION = "Snake!"

class Control(object):
    def __init__(self, socket, data):
        #set needed variables
        self.screen = pygame.display.get_surface()
        self.done = False
        self.socket = socket
        self.keys = pygame.key.get_pressed()
        self.clock = pygame.time.Clock()
        #load from rec data
        self.segment_width = 800/int(data[1])
        self.segment_height = 800/int(data[2])
        self.state = data[0]#0-4
        self.direction = data[5]#0-3
        self.my_segments = [[int(data[3]),int(data[4])]]
        #init empty variables needed
        self.enemy_segments = [];
        self.apple = [];

    def event_loop(self):
        for event in pygame.event.get():
            self.keys = pygame.key.get_pressed()
            if event.type == pygame.QUIT  or self.keys[pygame.K_ESCAPE]:
                self.done = True

    def update(self):
        if not status == "0":
            data = get_response(self.socket)
            print data
            self.state = data[0]

        print "status: "+self.state

        if self.state == "0":
            print "WAITING"
            self.socket.send("1")

        elif self.state == "1":
            print "INIT"
            print data
            #enemy and apple init
            #set state to ready

        elif self.state == "2":
            print "READY"
            self.socket.send("2")

        elif self.state == "3":
            print "RUN"
            self.state = "4"

        elif self.state == "4":
            print "END"
            self.done = True;

    def snakes_update(self, data):
            for i in range(2,int(data[1])*2+2,2):
                self.segments.append([int(data[i]),int(data[i+1])])

    def draw_segments(self, segments):
        for s in segments:
            pygame.draw.rect(self.screen, [1,155,64], (s[0]*self.segment_width,s[1]*self.segment_height,self.segment_width, self.segment_height),0)

    def draw(self):
        self.screen.fill([4,10,100])
        self.draw_segments(self.segments)
        pygame.display.flip()

    def display_fps(self):
        caption = "{} - FPS: {:.2f}".format(CAPTION, self.clock.get_fps())
        pygame.display.set_caption(caption)


    def main_loop(self):
        while not self.done:
            self.draw()
            self.event_loop()
            self.update()
            pygame.display.flip()
            self.clock.tick(10)
            self.display_fps()

def get_response(socket):
    print "Get data"
    data = socket.recv(1024).split()
    #socket.send("1")
    return data

def main():
    pygame.init()
    #network settings
    s = socket.socket()
    host = socket.gethostname() # Get local machine name
    port = 12345
    print "Try to connect " + host
    s.connect((host, port))

    data = get_response(s)
    #screen options
    gameDisplay = pygame.display.set_mode([800,800])
    pygame.display.set_caption("Snake!")
    #initialize and start game
    Control(s, data).main_loop()
    #close program
    s.close
    pygame.quit()
    sys.exit()


if __name__ == "__main__":
    main()
