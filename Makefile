all: raycaster.c
	gcc raycaster.c -o raycaster -lm

clean:
	rm -rf raycaster *~
