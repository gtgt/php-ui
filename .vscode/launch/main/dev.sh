#!/usr/bin/env bash
C=(
	vphp3.8	-P																			\
			-\; -r		ui																\
			-\; -d		ui\\Window::fs		-\( "$(fs=~/.v/ui && ([[ -d "$fs" ]]||mkdir "$fs") && printf %s "$fs")" -\) \
			-\;			UI\\onShouldQuit	-\( -, 'function(){return true	;}'		-\)	\
			-\; -d		ui\\menu			-\( dev									-\)	\
			-\; -d		ui\\window			-\( dev			-[	640	480	-] -,true	\
				-d		ui\\area			-\(					  500			\
					-d	ui\\cross					-\(			  250		\
													  -[		  0	  .5 -]	\
																0x0000FFFF	-\)	\
					-d	ui\\layer\(\)' -> 'lines	-\([	-[	 25	 50	-]\
															-[	 75	 75	-]\
															-[	300	150	-]]	-\)	\
						-+			-' -> 'stroke	-\(			0xFF0000FF	-\) -\)	-\)	\
			-+	 -' -> 'showAndRun					-\(								-\)	\
#		-v
	)