OPTS=+w1024 +h768 +GS +P -d +q9 -V -GA -GS -GD
OPTS += +HTN +HNcpu.png
#OPTS += +a0.1 -j +am2 +r5 
#OPTS=+GS +P +q9 +A0.3 DECLARE=CHEAP_MUSIC=0
#OPTS=+P +q9 -A DECLARE=CHEAP_MUSIC=0
#OPTS += DECLARE=NO_SHINY_DRUM_GUTS=1 
OPTS += DECLARE=NO_AREA_LIGHTS=1 
#OPTS += -q7 +SR400 

OPTS += $(if $(findstring area, $(shell hostname)), +sp8 +ep8, )
OPTS += $(if $(findstring area, $(shell hostname)), +d, )

DEFAULT: dsot.png

DALEK=dalek.inc dalekcables.inc dalekplinthcables.inc

MAPS=$(wildcard *map*.png) rack_map_gamma.jpg

ALL=dsot.inc dsot.pov stage.inc vl2c.inc varidisc.inc drums.inc keys.inc \
	ludwig.png remoamb.png guitars.inc $(MAPS) $(DALEK)

dsot.png: $(ALL)

# popup.png: popup.pov stage.inc dalek.inc dalekcables.inc dalekplinthcables.inc

cymbals.png: cymbals.inc dsot.inc

vl2c.png: vl2c.inc dsot.inc

drums.png: drums.inc dsot.inc ludwig.png remoamb.png

dalek.png: $(DALEK) dsot.inc

dalek.avi: $(DALEK) dsot.inc
	povray ${OPTS} +w800 +h600 +KFF60 +q9 -D DECLARE=TEST=1 +i$< +o$(basename $@).png

spot.png: spot.pov varidisc.pov gantry.pov stage.pov vl2c.inc dsot.inc

grille.png: stage.inc dsot.inc
	povray ${OPTS} DECLARE=TEST=1 DECLARE=TEST_GRILLE=1 -q5 +i$< +o$@

lightbox.png: stage.inc dsot.inc
	povray ${OPTS} DECLARE=TEST=1 DECLARE=TEST_LIGHTBOX=1 +i$< +o$@

popup.avi: stage.inc $(DALEK) dsot.inc
	povray ${OPTS} +w640 +h480 +KFF30 +q3 -D DECLARE=TEST=1 DECLARE=TEST_GRILLE=1 +i$< +o$(basename $@).png
	mv $(basename $@)?*.png anim
#	mencoder "mf://anim/$(basename $@)?*.png" -mf fps=30 -o $@ -ovc lavc -lavcopts vcodec=mpeg4

lightbox.avi: stage.inc dsot.inc
	povray ${OPTS} +w640 +h480 +KC +KFF30 +q9 -D DECLARE=TEST=1 DECLARE=TEST_LIGHTBOX=1 +i$< +o$(basename $@).png
	mv $(basename $@)?*.png anim

dsot.avi: $(ALL)
	povray ${OPTS} +KFF100 +q9 +a0.1 +am2 +r3 -D +idsot.pov +o$(basename $@).png
	mv $(basename $@)?*.png anim
#	mencoder "mf://anim/$(basename $@)?*.png" -mf fps=30 -o $@ -ovc lavc -lavcopts vcodec=mpeg4

chopsticks.avi: $(DALEK)
	povray ${OPTS} +w640 +h480 +KC +KFF100 +q9 -D DECLARE=TEST=1 DECLARE=NO_DALEK_CHOPSTICKS=0 +idalek.inc +o$(basename $@).png
	mv $(basename $@)?*.png anim

softly.png: dsot.inc
	povray ${OPTS} DECLARE=TEST_SOFTLY=1 +i$< +o$@

bendylight.png: dsot.inc
	povray ${OPTS} DECLARE=TEST_BENDYLIGHT=1 +i$< +o$@

slopedbox.png: dsot.inc
	povray ${OPTS} DECLARE=TEST_SLOPEDBOX=1 +i$< +o$@

mic.png: dsot.inc stage.inc
	povray ${OPTS} DECLARE=TEST_MICROPHONE=1 +i$< +o$@

guitars.png: guitars.inc dsot.inc $(MAPS)
	povray ${OPTS} DECLARE=TEST=1 +i$< +o$@

guitars.avi: guitars.inc dsot.inc
	povray ${OPTS} +w800 +h600 +KC +KFF100 +a0.1 +q9 -D DECLARE=TEST=1 +i$< +o$(basename $@).png
	mv $(basename $@)?*.png anim

%.png:%.pov; 
	povray ${OPTS} $< +o$@

%.png:%.inc; 
	povray ${OPTS} DECLARE=TEST=1 +i$< +o$@

rack_map_gamma.jpg: rack_map.jpg
	djpeg rack_map.jpg | pnmgamma -ungamma 2.2 | cjpeg > rack_map_gamma.jpg 

clean:
	rm -f spot.png drums.png

push:
	rsync -av --exclude anim/ --exclude "*.png" --exclude "*.tpn" ~/ray webxd:
	rsync -av $(MAPS) webxd:ray/stage/
