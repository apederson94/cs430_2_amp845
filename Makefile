all: raycaster.c
	gcc raycaster.c -o raycaster

clean:
	rm -rf raycaster *~
