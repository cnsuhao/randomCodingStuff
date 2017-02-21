# test
import pygame

from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *

vertices = (
    (1, -1, -1),
    (1, 1, -1),
    (-1, 1, -1),
    (-1, -1, -1),
    (1, -1, 1),
    (1, 1, 1),
    (-1, -1, 1),
    (-1, 1, 1)
)

edges = (
    (0,1),
    (0,3),
    (0,4),
    (2,1),
    (2,3),
    (2,7),
    (6,3),
    (6,4),
    (6,7),
    (5,1),
    (5,4),
    (5,7)
)

surfaces = (
	(0,1,2,3),
	(3,2,7,6),
	(6,7,5,4),
	(4,5,1,0),
	(1,5,7,2),
	(4,0,3,6)
)

colors = (
    (1,0,0),
    (0,1,0),
    (0,0,1),
    (0,1,0),
    (1,1,1),
    (0,1,1),
    (1,0,0),
    (0,1,0),
    (0,0,1),
    (1,0,0),
    (1,1,1),
    (0,1,1),
)




def Cube():
	# define faces
	
	glBegin(GL_QUADS)
	for surface in surfaces:
		x = 0
		for vertex in surface:
			x += 1
			glColor3fv(colors[x])
			glVertex(vertices[vertex])
	glEnd()
	# define edges
	glBegin(GL_LINES)
	for edge in edges:
		for vertex in edge:
			glVertex3fv(vertices[vertex])
	glEnd()


def main():
	pygame.init()

	# initialize window
	display = (600,600)
	pygame.display.set_mode(display, DOUBLEBUF | OPENGL)

	# view to the 3D space
	# 					FOV,       aspectratio   nearclip, farclip
	gluPerspective(45, (display[0] / display[1]), 0.1, 50.0)

	# move view back
	glTranslatef(0.0,0.0, -5)

	while True:
		for event in pygame.event.get():
			# quit game
			if event.type == pygame.QUIT:
				pygame.quit()
				quit()

		# rotate view?
		glRotatef(1, 3, 1, 1)
		# clear buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
		# create/render? cube
		Cube()
		# updates view?
		pygame.display.flip()
		# wait time till next loop
		pygame.time.wait(10)


main()
